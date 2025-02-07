#pragma once

#include "Object.h"
#include "Instance.h"

#if FFI_SRC
#include <utility>
#endif

namespace Coplt
{
    template <typename T> requires std::is_trivially_copyable_v<T>
    struct FList
    {
        FAllocator* m_allocator{};
        T* m_ptr{};
        size_t m_len{};
        size_t m_cap{};

#ifdef FFI_SRC

        constexpr size_t InitCapacity = 4;

        explicit FList(FAllocator* allocator) : m_allocator(allocator), m_ptr(nullptr)
        {
        }

        FList(FList&& other) noexcept
            : m_allocator(std::exchange(other.m_allocator, nullptr)),
              m_ptr(std::exchange(other.m_ptr, nullptr)),
              m_len(other.m_len), m_cap(other.m_cap)
        {
        }

        FList& operator=(FList&& other) noexcept
        {
            if (this != &other) return *this;
            FList _(std::move(this));
            new(this) FList(std::move(other));
            return *this;
        }

        FList(FList& other) = delete;

        FList& operator=(FList& other) = delete;

        ~FList()
        {
            if (m_allocator)
            {
                if (m_ptr)
                {
                    m_allocator->MemoryFree(m_ptr, alignof(T));
                    m_ptr = nullptr;
                }
                m_allocator->Release();
                m_allocator = nullptr;
            }
        }

        T& operator[](size_t index)
        {
            if (index >= m_len) throw std::out_of_range("FList::operator[]");
            return m_ptr[index];
        }

        T* data() const
        {
            return m_ptr;
        }

        size_t size() const
        {
            return m_len;
        }

    private:
        void Grow()
        {
            if (m_ptr == nullptr)
            {
                m_cap = InitCapacity;
                m_ptr = static_cast<T*>(m_allocator->MemoryAlloc(m_cap * sizeof(T), alignof(T)));
            }
            else
            {
                m_cap *= 2;
                m_ptr = static_cast<T*>(m_allocator->MemoryReAlloc(m_ptr, m_cap * sizeof(T), alignof(T)));
            }
        }

    public:
        T& UnsafeAdd()
        {
            if (m_len >= m_cap) Grow();
            auto& slot = m_ptr[m_len];
            m_len++;
            return slot;
        }

        void Add(T& item) // 不使用 move，因为 trivially_copyable
        {
            UnsafeAdd() = item;
        }

        void RemoveAt(const size_t index)
        {
            if (index >= m_len) throw std::out_of_range("FList::operator[]");
            m_len--;
            if (index < m_len)
            {
                const auto size = (m_len - index) * sizeof(T);
                memmove(&m_ptr[m_len], &m_ptr[m_len + 1], size);
            }
        }

        void Clear()
        {
            m_len = 0;
        }
#endif
    };

    // struct Foo
    // {
    //     FList<i32> a;
    // };
}
