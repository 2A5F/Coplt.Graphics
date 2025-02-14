#pragma once

#include <algorithm>
#include <span>
#include <vector>

#include "mimalloc.h"

#include "../FFI/Common.h"

#include "Concepts.h"
#include "Utils.h"
#include "Uninit.h"

namespace Coplt
{
    namespace _HashMap_internal
    {
        struct HashHelpers
        {
            static bool IsPrime(i32 candidate);

            static i32 GetPrime(i32 min);

            static i32 ExpandPrime(i32 old_size);

            static u64 GetFastModMultiplier(u32 divisor);

            static u32 FastMod(usize value, u32 divisor, u64 multiplier);
        };

        enum class InsertionBehavior : u8
        {
            None,
            OverwriteExisting,
        };

        struct Slot
        {
            i32 next{};
            u32 hash0{};
            u32 hash1{};

            usize& hash_code()
            {
                return reinterpret_cast<usize&>(hash0);
            }
        };
    }

    enum class HashMapInsertResult : u8
    {
        // 未添加或者修改
        None,
        // 已添加
        Added,
        // 已更新
        Updated,
    };

    template <class Key, class Value, Hash<Key> Hash = std::hash<Key>, Eq<Key> Eq = std::equal_to<Key>>
    class HashMap final
    {
        using HashHelpers = _HashMap_internal::HashHelpers;
        using InsertionBehavior = _HashMap_internal::InsertionBehavior;
        using Slot = _HashMap_internal::Slot;

        template <class T>
        using UP = Uninit<T>&;
        template <class T>
        using RP = T&;

    public:
        using Entry = std::pair<Key, Value>;

    private:
        i32* m_p_buckets{};
        Slot* m_p_slots{};
        Entry* m_p_entries{};
        u64 m_fast_mod_multiplier{};
        u32 m_cap{};
        i32 m_count{};
        i32 m_free_list{};
        i32 m_free_count{};
        Hash m_hash{};
        Eq m_eq{};

        constexpr static i32 StartOfFreeList = -3;

        std::span<Slot> Slots() const
        {
            return std::span(m_p_slots, m_cap);
        }

        std::span<Entry> Entries() const
        {
            return std::span(m_p_entries, m_cap);
        }

    public:
        HashMap() = default;

        ~HashMap()
        {
            if (m_p_buckets != nullptr)
            {
                mi_free_aligned(m_p_buckets, alignof(i32));
            }
            if (m_p_slots != nullptr)
            {
                mi_free_aligned(m_p_slots, alignof(Slot));
            }
            if (m_p_entries != nullptr)
            {
                if constexpr (!std::is_trivially_destructible<Entry>())
                {
                    auto iter = Iterator();
                    while (auto entry = iter.Next())
                    {
                        entry->~Entry();
                    }
                }
                mi_free_aligned(m_p_entries, alignof(Entry));
            }
        }

        HashMap(HashMap&& other) noexcept
            : m_p_buckets(std::exchange(other.m_p_buckets, nullptr)),
              m_p_slots(std::exchange(other.m_p_slots, nullptr)),
              m_p_entries(std::exchange(other.m_p_entries, nullptr)),
              m_fast_mod_multiplier(std::exchange(other.m_fast_mod_multiplier, 0)),
              m_cap(std::exchange(other.m_cap, 0)),
              m_count(std::exchange(other.m_count, 0)),
              m_free_list(std::exchange(other.m_free_list, 0))
        {
        }

        HashMap& operator=(HashMap&& other) noexcept
        {
            if (this == &other) return *this;
            new HashMap(std::move(*this));
            new(this) HashMap(std::forward<HashMap>(other));
            return *this;
        }

        HashMap(HashMap& other) noexcept = delete;

        HashMap& operator=(HashMap& other) = delete;

        usize Count() const noexcept
        {
            return m_count;
        }

        bool IsEmpty() const noexcept
        {
            return m_count == 0;
        }

    private:
        i32 Initialize(const i32 capacity)
        {
            const auto size = HashHelpers::GetPrime(capacity);

            m_cap = size;
            m_p_buckets = static_cast<i32*>(mi_zalloc_aligned(size * sizeof(i32), alignof(i32)));
            m_p_slots = static_cast<Slot*>(mi_zalloc_aligned(size * sizeof(Slot), alignof(Slot)));
            m_p_entries = static_cast<Entry*>(mi_malloc_aligned(size * sizeof(Entry), alignof(Entry)));

            m_free_list = -1;
            m_fast_mod_multiplier = HashHelpers::GetFastModMultiplier(static_cast<u32>(size));

            return size;
        }

        i32* GetBucket(const usize hash_code) const
        {
            const auto index = HashHelpers::FastMod(hash_code, m_cap, m_fast_mod_multiplier);
            return &m_p_buckets[index];
        }

        void Resize()
        {
            Resize(HashHelpers::ExpandPrime(m_count));
        }

        void Resize(i32 new_size)
        {
            m_cap = new_size;
            mi_free_aligned(m_p_buckets, alignof(i32));
            m_p_buckets = static_cast<i32*>(mi_zalloc_aligned(new_size * sizeof(i32), alignof(i32)));
            m_p_slots = static_cast<Slot*>(mi_rezalloc_aligned(m_p_slots, new_size * sizeof(Slot), alignof(Slot)));
            m_p_entries = static_cast<Entry*>(
                mi_realloc_aligned(m_p_entries, new_size * sizeof(Entry), alignof(Entry))
            );
            m_fast_mod_multiplier = HashHelpers::GetFastModMultiplier(static_cast<u32>(new_size));

            const auto count = m_count;
            for (int i = 0; i < count; i++)
            {
                if (auto& slot = m_p_slots[i]; slot.next >= -1)
                {
                    const auto bucket = GetBucket(slot.hash_code());
                    slot.next = *bucket - 1; // Value in _buckets is 1-based
                    *bucket = i + 1;
                }
            }
        }

    public:
        template <
            Fn<const Key&> QueryKey, Fn<void, UP<Key>> CreateKey,
            Fn<void, RP<Value>> UpdateValue, Fn<void, UP<Value>> CreateValue
        >
        HashMapInsertResult TryInsert(
            QueryKey query_key, CreateKey create_key, UpdateValue update_value, CreateValue create_value,
            const InsertionBehavior behavior, Entry** out_entry
        )
        {
            if (m_p_buckets == nullptr) Initialize(0);

            const Key& key = query_key();
            const usize hash_code = m_hash(key);

            u32 collision_count = 0;
            auto bucket = GetBucket(hash_code);
            auto i = *bucket - 1;

            while (static_cast<u32>(i) < m_cap)
            {
                auto& slot = m_p_slots[i];
                auto& entry = m_p_entries[i];
                if (slot.hash_code() == hash_code && m_eq(entry.first, key))
                {
                    if (out_entry) *out_entry = std::addressof(entry);

                    if (behavior == InsertionBehavior::OverwriteExisting)
                    {
                        update_value(RP<Value>(entry.second));
                        return HashMapInsertResult::Updated;
                    }

                    return HashMapInsertResult::None;
                }

                i = slot.next;

                collision_count++;
                if (collision_count >= m_cap)
                {
                    throw std::out_of_range("HashMap does not support concurrent operations");
                }
            }

            i32 index{};
            if (m_free_count > 0)
            {
                index = m_free_list;
                m_free_list = StartOfFreeList - m_p_slots[m_free_list].next;
                m_free_count--;
            }
            else
            {
                const auto count = m_count;
                if (count == m_cap)
                {
                    Resize();
                    bucket = GetBucket(hash_code);
                }
                index = count;
                m_count = count + 1;
            }

            auto& slot = m_p_slots[index];
            auto& entry = m_p_entries[index];
            slot.hash_code() = hash_code;
            slot.next = *bucket - 1;
            Uninit<Key> u_key(entry.first);
            Uninit<Value> u_value(entry.second);
            create_key(u_key);
            create_value(u_value);
            *bucket = index + 1;
            if (out_entry) *out_entry = std::addressof(entry);

            return HashMapInsertResult::Added;
        }

        HashMapInsertResult TryInsert(Key&& key, Value&& value, const InsertionBehavior behavior, Entry** out_entry)
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(std::move(key)); },
                [&](RP<Value> p) -> void { p = std::move(value); },
                [&](UP<Value> p) -> void { new(p.unsafe_put()) Value(std::move(value)); },
                behavior, out_entry
            );
        }

        HashMapInsertResult TryInsert(
            const Key& key, Value&& value, const InsertionBehavior behavior, Entry** out_entry
        )
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(key); },
                [&](RP<Value> p) -> void { p = std::move(value); },
                [&](UP<Value> p) -> void { new(p.unsafe_put()) Value(std::move(value)); },
                behavior, out_entry
            );
        }

        HashMapInsertResult TryInsert(
            Key&& key, const Value& value, const InsertionBehavior behavior, Entry** out_entry
        )
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(std::move(key)); },
                [&](RP<Value> p) -> void { p = value; },
                [&](UP<Value> p) -> void { new(p.unsafe_put()) Value(value); },
                behavior, out_entry
            );
        }

        HashMapInsertResult TryInsert(
            const Key& key, const Value& value, const InsertionBehavior behavior, Entry** out_entry
        )
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(key); },
                [&](RP<Value> p) -> void { p = value; },
                [&](UP<Value> p) -> void { new(p.unsafe_put()) Value(value); },
                behavior, out_entry
            );
        }

        template <Fn<void, RP<Value>> UpdateValue, Fn<void, UP<Value>> CreateValue>
        HashMapInsertResult TryInsert(
            Key&& key, UpdateValue update_value, CreateValue create_value,
            const InsertionBehavior behavior, Entry** out_entry
        )
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(std::move(key)); },
                fove(update_value),
                fove(create_value),
                behavior, out_entry
            );
        }

        template <Fn<void, RP<Value>> UpdateValue, Fn<void, UP<Value>> CreateValue>
        HashMapInsertResult TryInsert(
            const Key& key, UpdateValue update_value, CreateValue create_value,
            const InsertionBehavior behavior, Entry** out_entry
        )
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(key); },
                fove(update_value),
                fove(create_value),
                behavior, out_entry
            );
        }

        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        Entry* FindEntry(const QKey& query_key, QHash hasher, QEq eq) const
        {
            Slot* slot = nullptr;
            Entry* entry = nullptr;
            if (m_p_buckets == nullptr) return entry;

            auto hash_code = hasher(query_key);
            auto i = *GetBucket(hash_code);
            u32 collision_count = 0;

            --i;
            // Value in _buckets is 1-based; subtract 1 from i. We do it here so it fuses with the following conditional.
            do
            {
                if (static_cast<u32>(i) >= m_cap)
                {
                    return nullptr;
                }

                slot = std::addressof(m_p_slots[i]);
                entry = std::addressof(m_p_entries[i]);
                if (slot->hash_code() == hash_code && eq(entry->first, query_key))
                {
                    return entry;
                }

                i = slot->next;

                collision_count++;
            }
            while (collision_count <= m_cap);

            // The chain of entries forms a loop; which means a concurrent update has happened.
            // Break out of the loop and throw, rather than looping forever.
            throw std::out_of_range("HashMap does not support concurrent operations");
        }

        Entry* FindEntry(const Key& key) const
        {
            return FindEntry(key, m_hash, m_eq);
        }

        bool Contains(const Key& key) const
        {
            return FindEntry(key) != nullptr;
        }

        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        bool Contains(const QKey& query_key, QHash hasher, QEq eq) const
        {
            return FindEntry(query_key, fove(hasher), fove(eq)) != nullptr;
        }

        Value* TryGet(const Key& key) const
        {
            auto entry = FindEntry(key);
            if (entry) return std::addressof(entry->value);
            return nullptr;
        }

        Value& GetOr(const Key& key, Value& Or) const
        {
            if (auto entry = FindEntry(key))
                return entry->second;
            return Or;
        }

        const Value& GetOr(const Key& key, const Value& Or) const
        {
            if (auto entry = FindEntry(key))
                return entry->second;
            return Or;
        }

        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        Value* TryGet(const QKey& query_key, QHash hasher, QEq eq) const
        {
            auto entry = FindEntry(query_key, fove(hasher), fove(eq));
            if (entry) return std::addressof(entry->second);
            return nullptr;
        }

        // 返回是否删除，如果删除并且 out_entry 不为空将写入 out_entry
        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        bool Remove(const QKey& query_key, QHash hasher, QEq eq, Entry* out_entry)
        {
            if (m_p_buckets == nullptr) return false;
            u32 collision_count = 0;
            auto hash_code = hasher(query_key);
            auto bucket = GetBucket(hash_code);
            i32 last = -1;
            i32 i = *bucket - 1; // Value in buckets is 1-based
            while (i >= 0)
            {
                auto& slot = m_p_slots[i];
                auto& entry = m_p_entries[i];

                if (slot.hash_code() == hash_code && eq(entry->first, query_key))
                {
                    if (last < 0)
                    {
                        *bucket = slot.next + 1; // Value in buckets is 1-based
                    }
                    else
                    {
                        m_p_slots[last].next = slot.next;
                    }

                    slot.next = StartOfFreeList - m_free_list;

                    if (out_entry != nullptr)
                    {
                        *out_entry = std::move(entry);
                    }
                    if constexpr (std::is_trivially_destructible<Entry>())
                    {
                        entry.~Entry();
                    }

                    m_free_list = i;
                    m_free_count++;
                    return true;
                }

                last = i;
                i = slot.next;

                collision_count++;
                if (collision_count > m_cap)
                {
                    throw std::out_of_range("HashMap does not support concurrent operations");
                }
            }
            return false;
        }

        // 返回是否删除
        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        bool Remove(const QKey& query_key, QHash hasher, QEq eq)
        {
            return Remove(query_key, fove(hasher), fove(eq), nullptr);
        }

        // 返回是否删除，如果删除并且 out_entry 不为空将写入 out_entry
        bool Remove(const Key& key, Entry* out_entry)
        {
            return Remove(key, m_hash, m_eq, out_entry);
        }

        // 返回是否删除
        bool Remove(const Key& key)
        {
            return Remove(key, m_hash, m_eq, nullptr);
        }

        // 返回是否添加
        bool TryAdd(Key&& key, Value&& value)
        {
            return TryInsert(std::forward<Key>(key), std::forward<Value>(value), InsertionBehavior::None, nullptr)
                == HashMapInsertResult::Added;
        }

        // 返回是否添加
        bool TryAdd(const Key& key, Value&& value)
        {
            return TryInsert(key, std::forward<Value>(value), InsertionBehavior::None, nullptr)
                == HashMapInsertResult::Added;
        }

        // 返回是否添加
        bool TryAdd(Key&& key, const Value& value)
        {
            return TryInsert(std::forward<Key>(key), value, InsertionBehavior::None, nullptr)
                == HashMapInsertResult::Added;
        }

        // 返回是否添加
        bool TryAdd(const Key& key, const Value& value)
        {
            return TryInsert(key, value, InsertionBehavior::None, nullptr)
                == HashMapInsertResult::Added;
        }

        // 返回是否添加
        template <Fn<void, UP<Value>> CreateValue>
        bool TryAdd(Key&& key, CreateValue create_value)
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(std::move(key)); },
                [&](RP<Value> p)
                {
                    // 不需要更新
                },
                fove(create_value),
                InsertionBehavior::None, nullptr
            ) == HashMapInsertResult::Added;
        }

        // 返回是否添加
        template <Fn<void, Value*> CreateValue>
        bool TryAdd(const Key& key, CreateValue create_value)
        {
            return TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(key); },
                [&](RP<Value> p)
                {
                    // 不需要更新
                },
                fove(create_value),
                InsertionBehavior::None, nullptr
            ) == HashMapInsertResult::Added;
        }

        HashMapInsertResult AddOrReplace(Key&& key, Value&& value)
        {
            return TryInsert(
                std::forward<Key>(key), std::forward<Value>(value),
                InsertionBehavior::OverwriteExisting, nullptr
            );
        }

        HashMapInsertResult AddOrReplace(const Key& key, Value&& value)
        {
            return TryInsert(
                key, std::forward<Value>(value),
                InsertionBehavior::OverwriteExisting, nullptr
            );
        }

        HashMapInsertResult AddOrReplace(Key&& key, const Value& value)
        {
            return TryInsert(
                std::forward<Key>(key), value,
                InsertionBehavior::OverwriteExisting, nullptr
            );
        }

        HashMapInsertResult AddOrReplace(const Key& key, const Value& value)
        {
            return TryInsert(
                key, value,
                InsertionBehavior::OverwriteExisting, nullptr
            );
        }

        template <Fn<void, Value&> UpdateValue, Fn<void, Value*> CreateValue>
        HashMapInsertResult AddOrUpdate(Key&& key, UpdateValue update_value, CreateValue create_value)
        {
            return TryInsert(
                std::forward<Key>(key),
                fove(update_value),
                fove(create_value),
                InsertionBehavior::None, nullptr
            );
        }

        template <Fn<void, Value&> UpdateValue, Fn<void, Value*> CreateValue>
        HashMapInsertResult AddOrUpdate(const Key& key, UpdateValue update_value, CreateValue create_value)
        {
            return TryInsert(
                key,
                fove(update_value),
                fove(create_value),
                InsertionBehavior::None, nullptr
            );
        }

        template <Fn<void, UP<Value>> CreateValue>
        Entry& GetOrAddEntry(Key&& key, bool& already_exist, CreateValue create_value)
        {
            Entry* entry{};
            already_exist = TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(std::move(key)); },
                [&](RP<Value> p) -> void
                {
                    // 不需要更新
                },
                fove(create_value),
                InsertionBehavior::None, &entry
            ) == HashMapInsertResult::None;
            return *entry;
        }

        template <Fn<void, UP<Value>> CreateValue>
        Entry& GetOrAddEntry(const Key& key, bool& already_exist, CreateValue create_value)
        {
            Entry* entry{};
            already_exist = TryInsert(
                [&] -> const Key& { return key; },
                [&](UP<Key> p) -> void { new(p.unsafe_put()) Key(key); },
                [&](RP<Value> p) -> void
                {
                    // 不需要更新
                },
                fove(create_value),
                InsertionBehavior::None, std::addressof(entry)
            ) == HashMapInsertResult::None;
            return *entry;
        }

        template <Fn<void, UP<Value>> CreateValue>
        Value& GetOrAdd(Key&& key, bool& already_exist, CreateValue create_value)
        {
            return GetOrAddEntry(
                std::forward<Key>(key), already_exist, fove(create_value)
            ).second;
        }

        template <Fn<void, UP<Value>> CreateValue>
        Value& GetOrAdd(Key&& key, CreateValue create_value)
        {
            bool already_exist{};
            return GetOrAddEntry(
                std::forward<Key>(key), already_exist, fove(create_value)
            ).second;
        }

        template <Fn<void, UP<Value>> CreateValue>
        Value& GetOrAdd(const Key& key, bool& already_exist, CreateValue create_value)
        {
            return GetOrAddEntry(key, already_exist, fove(create_value)).second;
        }

        template <Fn<void, UP<Value>> CreateValue>
        Value& GetOrAdd(const Key& key, CreateValue create_value)
        {
            bool already_exist{};
            return GetOrAddEntry(key, already_exist, fove(create_value)).second;
        }

        Value& GetOrAddDefault(Key&& key, bool& already_exist)
        {
            return GetOrAdd(std::forward<Key>(key), already_exist, [&](UP<Value> p) { new(p.unsafe_put()) Value(); });
        }

        Value& GetOrAddDefault(Key&& key)
        {
            bool already_exist{};
            return GetOrAddDefault(std::forward<Key>(key), already_exist);
        }

        Value& GetOrAddDefault(const Key& key, bool& already_exist)
        {
            return GetOrAdd(key, already_exist, [&](UP<Value> p) { new(p.unsafe_put()) Value(); });
        }

        Value& GetOrAddDefault(const Key& key)
        {
            return GetOrAddDefault(key);
        }

        Entry& GetOrAddDefaultEntry(Key&& key, bool& already_exist)
        {
            return GetOrAddEntry(
                std::forward<Key>(key), already_exist,
                [&](UP<Value> p) { new(p.unsafe_put()) Value(); }
            );
        }

        Entry& GetOrAddDefaultEntry(Key&& key)
        {
            bool already_exist{};
            return GetOrAddDefaultEntry(std::forward<Key>(key), already_exist);
        }

        Entry& GetOrAddDefaultEntry(const Key& key, bool& already_exist)
        {
            return GetOrAddEntry(key, already_exist, [&](UP<Value> p) { new(p.unsafe_put()) Value(); });
        }

        Entry& GetOrAddDefaultEntry(const Key& key)
        {
            bool already_exist{};
            return GetOrAddDefaultEntry(key, already_exist);
        }

        void Clear()
        {
            if (m_count > 0)
            {
                if constexpr (!std::is_trivially_destructible<Entry>())
                {
                    auto iter = Iterator();
                    while (auto entry = iter.Next())
                    {
                        entry->~Entry();
                    }
                }

                std::fill_n(m_p_buckets, m_cap, 0);
                std::fill_n(m_p_slots, m_cap, Slot{});

                m_count = 0;
                m_free_list = -1;
                m_free_count = 0;
            }
        }

        class MutIterator
        {
            HashMap* m_self{};
            i32 m_index{0};

        public:
            explicit MutIterator(HashMap* self) : m_self(self)
            {
            }

            // 尝试获取下个项，如果成功获取将不是 null
            Entry* Next()
            {
                while (static_cast<u32>(m_index) < static_cast<u32>(m_self->m_count))
                {
                    auto i = m_index++;
                    auto& slot = m_self->m_p_slots[i];
                    auto& entry = m_self->m_p_entries[i];

                    if (slot.next >= -1)
                    {
                        return std::addressof(entry);
                    }
                }

                m_index = m_self->m_count + 1;
                return nullptr;
            }
        };

        MutIterator Iterator()
        {
            return MutIterator(this);
        }

        class ImmIterator
        {
            const HashMap* m_self{};
            i32 m_index{0};

        public:
            explicit ImmIterator(const HashMap* self) : m_self(self)
            {
            }

            // 尝试获取下个项，如果成功获取将不是 null
            const Entry* Next()
            {
                while (static_cast<u32>(m_index) < static_cast<u32>(m_self->m_count))
                {
                    auto i = m_index++;
                    auto& slot = m_self->m_p_slots[i];
                    auto& entry = m_self->m_p_entries[i];

                    if (slot.next >= -1)
                    {
                        return std::addressof(entry);
                    }
                }

                m_index = m_self->m_count + 1;
                return nullptr;
            }
        };

        ImmIterator Iterator() const
        {
            return ImmIterator(this);
        }

        class StdIterator
        {
            HashMap* m_self{};
            i32 m_index{0};

            void AdvanceToValid()
            {
                while (m_self && m_index < m_self->m_count)
                {
                    auto& slot = m_self->m_p_slots[m_index];
                    if (slot.next >= -1)
                    {
                        break;
                    }
                    ++m_index;
                }
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using iterator_concept = std::forward_iterator_tag;
            using value_type = Entry;
            using difference_type = std::ptrdiff_t;;
            using pointer = Entry*;
            using reference = Entry&;

            explicit StdIterator(HashMap* self, const i32 index) : m_self(self), m_index(index)
            {
                if (m_self) AdvanceToValid();
            }

            explicit StdIterator(HashMap* self) : StdIterator(self, 0)
            {
            }

            Entry& operator*() const
            {
                return m_self->m_p_entries[m_index];
            }

            Entry* operator->() const
            {
                return &m_self->m_p_entries[m_index];
            }

            // 前缀增量
            StdIterator& operator++()
            {
                ++m_index;
                AdvanceToValid();
                return *this;
            }

            // 后缀增量
            StdIterator operator++(int)
            {
                StdIterator temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const StdIterator& other) const
            {
                return m_self == other.m_self && m_index == other.m_index;
            }

            bool operator!=(const StdIterator& other) const
            {
                return !(*this == other);
            }
        };

        using iterator = StdIterator;

        class ConstIterator
        {
            const HashMap* m_self{};
            i32 m_index{0};

            void AdvanceToValid()
            {
                while (m_self && m_index < m_self->m_count)
                {
                    auto& slot = m_self->m_p_slots[m_index];
                    if (slot.next >= -1)
                    {
                        break;
                    }
                    ++m_index;
                }
            }

        public:
            using iterator_category = std::forward_iterator_tag;
            using iterator_concept = std::forward_iterator_tag;
            using value_type = Entry;
            using difference_type = std::ptrdiff_t;;
            using pointer = const Entry*;
            using reference = const Entry&;

            explicit ConstIterator(const HashMap* self) : ConstIterator(self, 0)
            {
            }

            explicit ConstIterator(const HashMap* self, const i32 index) : m_self(self), m_index(index)
            {
                if (m_self) AdvanceToValid();
            }

            const Entry& operator*() const
            {
                return m_self->m_p_entries[m_index];
            }

            const Entry* operator->() const
            {
                return &m_self->m_p_entries[m_index];
            }

            // 前缀增量
            ConstIterator& operator++()
            {
                ++m_index;
                AdvanceToValid();
                return *this;
            }

            // 后缀增量
            ConstIterator operator++(int)
            {
                ConstIterator temp = *this;
                ++(*this);
                return temp;
            }

            bool operator==(const ConstIterator& other) const
            {
                return m_self == other.m_self && m_index == other.m_index;
            }

            bool operator!=(const ConstIterator& other) const
            {
                return !(*this == other);
            }
        };

        iterator begin()
        {
            return iterator(this);
        }

        iterator end()
        {
            return iterator(this, m_count);
        }

        ConstIterator begin() const
        {
            return ConstIterator(this);
        }

        ConstIterator end() const
        {
            return ConstIterator(this, m_count);
        }

        std::vector<Entry> IntoVector() &&
        {
            std::vector<Entry> entries{};
            entries.reserve(m_count);
            auto iter = Iterator();
            while (auto entry = iter.Next())
            {
                entries.push_back(std::move(*entry));
            }
            new HashMap(std::move(*this));
            return entries;
        }
    };
}
