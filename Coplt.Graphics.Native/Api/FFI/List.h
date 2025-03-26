#pragma once

#include "Object.h"
#include "Instance.h"

#if FFI_SRC
#include <utility>
#include <span>
#endif

namespace Coplt
{
    template <typename T>
        COPLT_REQUIRES COPLT_POD(T)
    struct FList COPLT_FINAL
    {
        FAllocator* m_allocator{};
        T* m_ptr{};
        size_t m_len{};
        size_t m_cap{};

        #ifdef FFI_SRC

        constexpr static size_t InitCapacity = 4;

        FList() = default;

        explicit FList(FAllocator* allocator) : m_allocator(allocator), m_ptr(nullptr)
        {
        }

        void Swap(FList& other)
        {
            std::swap(m_allocator, other.m_allocator);
            std::swap(m_ptr, other.m_ptr);
            std::swap(m_len, other.m_len);
            std::swap(m_cap, other.m_cap);
        }

        FList(FList&& other) noexcept
            : m_allocator(std::exchange(other.m_allocator, nullptr)),
              m_ptr(std::exchange(other.m_ptr, nullptr)),
              m_len(other.m_len), m_cap(other.m_cap)
        {
        }

        FList& operator=(FList&& other) noexcept
        {
            if (&m_ptr == &other.m_ptr) return *this;
            FList(std::forward<FList>(other)).Swap(*this);
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
                if (m_allocator == nullptr) throw std::bad_alloc();
                m_cap = InitCapacity;
                m_ptr = static_cast<T*>(m_allocator->MemoryAlloc(m_cap * sizeof(T), alignof(T)));
            }
            else
            {
                if (m_allocator == nullptr) throw std::bad_alloc();
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

        void Add(const T& item) // 不使用 move，因为 trivially_copyable
        {
            UnsafeAdd() = item;
        }

        void Add(T&& item) // 不使用 move，因为 trivially_copyable
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

        std::span<T> AsSpan()
        {
            return std::span(data(), size());
        }

        std::span<const T> AsSpan() const
        {
            return std::span(data(), size());
        }
        #endif
    };
}
