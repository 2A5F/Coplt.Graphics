#pragma once
#include <utility>

namespace Coplt
{
    template <class T>
    class Uninit
    {
        T* m_ptr{};
        bool m_initialized{};

    public:
        Uninit() = default;

        explicit Uninit(T* ptr) : m_ptr(ptr)
        {
        }

        explicit Uninit(T& ref) : m_ptr(std::addressof(ref))
        {
        }

        Uninit(Uninit&& r) = delete;
        Uninit(const Uninit& r) = delete;

        ~Uninit()
        {
            if (!m_initialized) throw std::exception("Uninitialized");
        }

        T* unsafe_put()
        {
            m_initialized = true;
            return m_ptr;
        }

        void put(T&& value)
        {
            if (m_initialized) *m_ptr = std::forward<T>(value);
            else
            {
                new(m_ptr) T(std::forward<T>(value));
                m_initialized = true;
            }
        }

        template <typename U> requires std::convertible_to<U*, T*>
        void put(U&& value)
        {
            if (m_initialized) *m_ptr = std::forward<U>(value);
            else
            {
                new(m_ptr) T(std::forward<U>(value));
                m_initialized = true;
            }
        }

        T& operator=(T&& p) noexcept(std::is_nothrow_move_constructible_v<T>)
        {
            if (m_initialized) *m_ptr = std::forward<T>(p);
            else
            {
                new(m_ptr) T(std::forward<T>(p));
                m_initialized = true;
            }
            return *m_ptr;
        }

        template <typename U> requires std::convertible_to<U*, T*>
        T& operator=(U&& p) noexcept(std::is_nothrow_move_constructible_v<T>)
        {
            if (m_initialized) *m_ptr = std::forward<U>(p);
            else
            {
                new(m_ptr) T(std::forward<U>(p));
                m_initialized = true;
            }
            return *m_ptr;
        }

        T& operator=(const T& p) noexcept(std::is_nothrow_copy_constructible_v<T>)
        {
            if (m_initialized) *m_ptr = p;
            else
            {
                new(m_ptr) T(p);
                m_initialized = true;
            }
            return *m_ptr;
        }

        template <typename U> requires std::convertible_to<U*, T*>
        T& operator=(const U& p) noexcept(std::is_nothrow_copy_constructible_v<T>)
        {
            if (m_initialized) *m_ptr = p;
            else
            {
                new(m_ptr) T(p);
                m_initialized = true;
            }
            return *m_ptr;
        }

        T& operator<<(T&& p) noexcept(std::is_nothrow_move_constructible_v<T>)
        {
            return operator=(std::forward<T>(p));
        }

        template <typename U> requires std::convertible_to<U*, T*>
        T& operator<<(U&& p) noexcept(std::is_nothrow_move_constructible_v<T>)
        {
            return operator=(std::forward<U>(p));
        }

        T& operator<<(const T& p) noexcept(std::is_nothrow_copy_constructible_v<T>)
        {
            return operator=(p);
        }

        template <typename U> requires std::convertible_to<U*, T*>
        T& operator<<(const U& p) noexcept(std::is_nothrow_copy_constructible_v<T>)
        {
            return operator=(p);
        }
    };
}
