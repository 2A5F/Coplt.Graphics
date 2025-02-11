#pragma once

#include <algorithm>

#include "../FFI/Common.h"
#include "mimalloc.h"

namespace Coplt
{
    template <class T>
    class Memory final
    {
        struct Data
        {
            usize m_size;
            T m_data[];
        };

        Data* m_ptr{};

        constexpr static usize Align()
        {
            return std::max(alignof(Data), alignof(T));
        }

        explicit Memory(Data* data) : m_ptr{data}
        {
        }

    public:
        explicit Memory() = default;

        ~Memory()
        {
            if (m_ptr != nullptr) mi_free_aligned(m_ptr, Align());
        }

        Memory(Memory&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr))
        {
        }

        Memory& operator=(Memory&& other) noexcept
        {
            if (this == &other) return *this;
            Memory(std::move(*this));
            new(this) Memory(std::move(other));
            return *this;
        }

        Memory(const Memory&) = delete;
        Memory& operator=(const Memory&) = delete;

        T* data() const noexcept { return m_ptr == nullptr ? nullptr : m_ptr->m_data; }
        usize size() const noexcept { return m_ptr == nullptr ? 0 : m_ptr->m_size; }

        static Memory CreateCopy(const T* data, const usize size)
        {
            Memory r = Create(size);
            memcpy(r.data(), data, size * sizeof(T));
            return r;
        }

        static Memory Create(const usize size)
        {
            auto ptr = mi_malloc_aligned(sizeof(Data) + size * sizeof(T), Align());
            return Memory(new(ptr) Data{size});
        }

        static Memory CreateZeroed(const usize size)
        {
            auto ptr = mi_zalloc_aligned(sizeof(Data) + size * sizeof(T), Align());
            return Memory(new(ptr) Data{size});
        }

        static Memory CreateInitialized(const usize size)
        {
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                return CreateZeroed(size);
            }
            else
            {
                auto r = Create(size);
                for (usize i = 0; i < r.size(); ++i)
                {
                    auto p = &r[i];
                    new(p) T();
                }
                return r;
            }
        }

        void Resize(const usize new_size)
        {
            if (m_ptr == nullptr)
            {
                *this = Create(new_size);
                return;
            }
            m_ptr = new(mi_realloc_aligned(m_ptr, sizeof(Data) + new_size * sizeof(T), Align())) Data{new_size};
        }

        void ResizeZeroed(const usize new_size)
        {
            if (m_ptr == nullptr)
            {
                *this = CreateZeroed(new_size);
                return;
            }
            m_ptr = new(mi_rezalloc_aligned(m_ptr, sizeof(Data) + new_size * sizeof(T), Align())) Data{new_size};
        }

        void ResizeInitialized(const usize new_size)
        {
            if (m_ptr == nullptr)
            {
                *this = CreateInitialized(new_size);
                return;
            }
            if constexpr (std::is_trivially_copyable_v<T>)
            {
                ResizeZeroed(new_size);
            }
            else
            {
                auto old_size = m_ptr->m_size;
                Resize(new_size);
                for (usize i = old_size; i < size(); ++i)
                {
                    auto p = &this[i];
                    new(p) T();
                }
            }
        }

        bool operator==(std::nullptr_t) const
        {
            return nullptr == m_ptr;
        }

        bool operator!=(std::nullptr_t) const
        {
            return nullptr != m_ptr;
        }

        explicit operator bool() const
        {
            return nullptr == m_ptr;
        }

        T& operator[](const usize index) const
        {
            return data()[index];
        }
    };
}
