#pragma once

#include <algorithm>
#include <span>

#include "mimalloc.h"

#include "../FFI/Common.h"

#include "Concepts.h"

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

    public:
        using Entry = std::pair<Key, Value>;

    private:
        i32* m_p_buckets{};
        Slot* m_p_slots{};
        Entry* m_p_entries{};
        u32 m_cap{};
        i32 m_capacity{};
        i32 m_count{};
        i32 m_free_list{};
        i32 m_free_count{};
        u64 m_fast_mod_multiplier{};
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
        explicit HashMap() = default;

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
            m_p_buckets = static_cast<i32*>(mi_zalloc_aligned(size, alignof(i32)));
            m_p_slots = static_cast<Slot*>(mi_zalloc_aligned(size, alignof(Slot)));
            m_p_entries = static_cast<Entry*>(mi_malloc_aligned(size, alignof(Entry)));

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
            m_p_buckets = static_cast<i32*>(mi_zalloc_aligned(new_size, alignof(i32)));
            m_p_slots = static_cast<Slot*>(mi_rezalloc_aligned(m_p_slots, new_size, alignof(Slot)));
            m_p_entries = static_cast<Entry*>(mi_realloc_aligned(m_p_entries, new_size, alignof(Entry)));
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
            Fn<Key&> QueryKey, Fn<void, Key*> CreateKey, Fn<void, Value&> UpdateValue, Fn<void, Value*> CreateValue
        >
        HashMapInsertResult TryInsert(
            QueryKey&& query_key, CreateKey&& create_key, UpdateValue&& update_value, CreateValue&& create_value,
            const InsertionBehavior behavior, Entry** out_entry
        )
        {
            if (m_p_buckets == nullptr) Initialize(0);

            auto key = query_key();
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
                    if (behavior == InsertionBehavior::OverwriteExisting)
                    {
                        update_value(entry.second);
                        if (out_entry) *out_entry = &entry;
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
            create_key(&entry.first);
            create_value(&entry.second);
            *bucket = index + 1;
            if (out_entry) *out_entry = &entry;

            return HashMapInsertResult::Added;
        }

        HashMapInsertResult TryInsert(Key&& key, Value&& value, const InsertionBehavior behavior, Entry** out_entry)
        {
            return TryInsert(
                [&]-> Key& { return key; },
                [&](Key* p) -> void { new(p) Key(std::forward<Key>(key)); },
                [&](Value& p) -> void { p = std::forward<Value>(value); },
                [&](Value* p) -> void { new(p) Value(std::forward<Value>(value)); },
                behavior, out_entry
            );
        }

        template <Fn<void, Value&> UpdateValue, Fn<void, Value*> CreateValue>
        HashMapInsertResult TryInsert(
            Key&& key, UpdateValue&& update_value, CreateValue&& create_value,
            const InsertionBehavior behavior, Entry** out_entry
        )
        {
            return TryInsert(
                [&]-> Key& { return key; },
                [&](Key* p) -> void { new(p) Key(std::forward<Key>(key)); },
                std::forward<UpdateValue>(update_value),
                std::forward<CreateValue>(create_value),
                behavior, out_entry
            );
        }

        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        Entry* FindEntry(QKey& query_key, QHash& hasher, QEq& eq)
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

                slot = &m_p_slots[i];
                entry = &m_p_entries[i];
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

        Entry* FindEntry(Key& key)
        {
            return FindEntry(key, m_hash, m_eq);
        }

        bool Contains(Key& key)
        {
            return FindEntry(key) != nullptr;
        }

        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        bool Contains(QKey& query_key, QHash& hasher, QEq& eq)
        {
            return FindEntry(query_key, hasher, eq) != nullptr;
        }

        // 返回是否删除，如果删除并且 out_entry 不为空将写入 out_entry
        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        bool Remove(QKey& query_key, QHash& hasher, QEq& eq, Entry* out_entry)
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
        bool Remove(QKey& query_key, QHash& hasher, QEq& eq)
        {
            return Remove(query_key, hasher, eq, nullptr);
        }

        // 返回是否删除，如果删除并且 out_entry 不为空将写入 out_entry
        bool Remove(Key& key, Entry* out_entry)
        {
            return Remove(key, m_hash, m_eq, out_entry);
        }

        // 返回是否删除
        bool Remove(Key& key)
        {
            return Remove(key, m_hash, m_eq, nullptr);
        }

        Value* TryGet(Key& key)
        {
            auto entry = FindEntry(key);
            if (entry) return &entry->second;
            return nullptr;
        }

        template <class QKey, Coplt::Hash<QKey> QHash = std::hash<QKey>, Coplt::Eq<Key, QKey> QEq = std::equal_to<Key>>
        Value* TryGet(QKey& query_key, QHash& hasher, QEq& eq)
        {
            auto entry = FindEntry(query_key, hasher, eq);
            if (entry) return &entry->second;
            return nullptr;
        }

        // 返回是否添加
        bool TryAdd(Key&& key, Value&& value)
        {
            return TryInsert(std::forward<Key>(key), std::forward<Value>(value), InsertionBehavior::None, nullptr)
                == HashMapInsertResult::Added;
        }

        // 返回是否添加
        template <Fn<void, Value*> CreateValue>
        bool TryAdd(Key&& key, CreateValue&& create_value)
        {
            return TryInsert(
                [&]-> Key& { return key; },
                [&](Key* p) -> void { new(p) Key(std::forward<Key>(key)); },
                [&](Value& p)
                {
                    // 不需要更新
                },
                std::forward<CreateValue>(create_value),
                InsertionBehavior::None, nullptr
            ) == HashMapInsertResult::Added;
        }

        HashMapInsertResult AddOrReplace(Key&& key, Value&& value)
        {
            return TryInsert(std::forward<Key>(key), std::forward<Value>(value), InsertionBehavior::OverwriteExisting,
                             nullptr);
        }

        template <Fn<void, Value&> UpdateValue, Fn<void, Value*> CreateValue>
        HashMapInsertResult AddOrUpdate(Key&& key, UpdateValue&& update_value, CreateValue&& create_value)
        {
            return TryInsert(
                std::forward<Key>(key),
                std::forward<UpdateValue>(update_value),
                std::forward<CreateValue>(create_value),
                InsertionBehavior::None, nullptr
            );
        }

        template <Fn<void, Value*> CreateValue>
        Entry& GetOrAddEntry(Key&& key, bool& already_exist, CreateValue&& create_value)
        {
            Entry* entry{};
            already_exist = TryInsert(
                [&]-> Key& { return key; },
                [&](Key* p) -> void { new(p) Key(std::forward<Key>(key)); },
                [&](Value& p) -> void
                {
                    // 不需要更新
                },
                std::forward<CreateValue>(create_value),
                InsertionBehavior::None, entry
            ) == HashMapInsertResult::None;
            return *entry;
        }

        template <Fn<void, Value*> CreateValue>
        Value& GetOrAdd(Key&& key, bool& already_exist, CreateValue&& create_value)
        {
            return GetOrAddEntry(
                std::forward<Key>(key), already_exist, std::forward<CreateValue>(create_value)
            ).second;
        }

        Value& GetOrAddDefault(Key&& key, bool& already_exist)
        {
            return GetOrAdd(std::forward<Key>(key), already_exist, [&](Value* p) { new(p) Value(); });
        }

        Entry& GetOrAddDefaultEntry(Key&& key, bool& already_exist)
        {
            return GetOrAddEntry(std::forward<Key>(key), already_exist, [&](Value* p) { new(p) Value(); });
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

        class Iterator
        {
            HashMap* m_self{};
            i32 m_index{0};

        public:
            explicit Iterator(HashMap* self) : m_self(self)
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
                        return &entry;
                    }
                }

                m_index = m_self->m_count + 1;
                return nullptr;
            }
        };

        Iterator Iterator() const
        {
            return Iterator(this);
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

            explicit StdIterator(HashMap* self) : m_self(self)
            {
                if (m_self) AdvanceToValid();
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

            explicit ConstIterator(const HashMap* self) : m_self(self)
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
    };
}
