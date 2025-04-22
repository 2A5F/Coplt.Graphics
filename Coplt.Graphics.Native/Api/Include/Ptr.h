#pragma once

#include <utility>
#include "Error.h"

namespace Coplt
{
    template <class T>
    using Pointer = T*;

    template <class T>
    class Ptr;
    template <class T>
    class NonNull;

    template <class T>
    class Ptr
    {
        T* m_ptr;

        template <class A>
        friend class Ptr;
        template <class A>
        friend class NonNull;

    public:
        Ptr() noexcept : m_ptr(nullptr)
        {
        }

        explicit Ptr(T& ref) noexcept : m_ptr(std::addressof(ref))
        {
        }

        Ptr(T* ptr) noexcept : m_ptr(ptr)
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        Ptr(U* ptr) noexcept : m_ptr(ptr)
        {
        }

        Ptr(const Rc<T>& rc) noexcept : m_ptr(rc.get())
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        Ptr(const Rc<U>& rc) noexcept : m_ptr(rc.get())
        {
        }

        Ptr(Ptr&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr))
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        Ptr(Ptr<U>&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr))
        {
        }

        Ptr(const Ptr& other) noexcept : m_ptr(other.m_ptr)
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        Ptr(const Ptr<U>& other) noexcept : m_ptr(other.m_ptr)
        {
        }

        Ptr& swap(Ptr& other) noexcept
        {
            std::swap(m_ptr, other.m_ptr);
            return *this;
        }

        Ptr& operator=(Ptr&& other) noexcept
        {
            Ptr(std::forward<Ptr>(other)).swap(*this);
            return *this;
        }

        template <class U> requires std::convertible_to<U*, T*>
        Ptr& operator=(Ptr<U>&& other) noexcept
        {
            Ptr(std::forward<Ptr<U>>(other)).swap(*this);
            return *this;
        }

        Ptr& operator=(const Ptr& other) noexcept
        {
            Ptr(other).swap(*this);
            return *this;
        }

        template <class U> requires std::convertible_to<U*, T*>
        Ptr& operator=(const Ptr<U>& other) noexcept
        {
            Ptr(other).swap(*this);
            return *this;
        }

        operator T*() const noexcept
        {
            return m_ptr;
        }

        template <class U> requires std::convertible_to<T*, U*>
        operator U*() const noexcept
        {
            return m_ptr;
        }

        T** operator&() noexcept
        {
            return &m_ptr;
        }

        T* const* operator&() const noexcept
        {
            return &m_ptr;
        }

        T* operator->() const
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
            return m_ptr;
        }

        T& operator*() const
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
            return *m_ptr;
        }

        operator bool() const noexcept
        {
            return m_ptr != nullptr;
        }

        bool operator==(std::nullptr_t) const noexcept
        {
            return m_ptr == nullptr;
        }

        T* get() const noexcept
        {
            return m_ptr;
        }

        bool operator==(const Pointer<T>& ptr) const noexcept
        {
            return m_ptr == ptr;
        }

        template <class U> requires std::convertible_to<T*, U*> || std::convertible_to<U*, U*>
        bool operator==(const Pointer<U>& ptr) const noexcept
        {
            return m_ptr == ptr;
        }

        explicit operator usize() const noexcept
        {
            return reinterpret_cast<usize>(m_ptr);
        }

        T& operator[](usize offset) const noexcept
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
            return m_ptr[offset];
        }

        Ptr operator+(usize offset) const noexcept
        {
            return Ptr(m_ptr + offset);
        }

        Ptr operator-(usize offset) const noexcept
        {
            return Ptr(m_ptr - offset);
        }
    };

    template <class T>
    class NonNull
    {
        T* m_ptr;

        template <class A>
        friend class NonNull;
        template <class A>
        friend class Ptr;

        struct Unchecked_t
        {
        };

        NonNull(T* ptr, Unchecked_t) : m_ptr(ptr)
        {
        }

    public:
        NonNull() = delete;

        static NonNull Unchecked(T* ptr) noexcept
        {
            return NonNull(ptr, Unchecked_t());
        }

        explicit NonNull(T& ref) noexcept : m_ptr(std::addressof(ref))
        {
        }

        NonNull(T* ptr) : m_ptr(ptr)
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull(U* ptr) : m_ptr(ptr)
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
        }

        NonNull(const Rc<T>& rc) : NonNull(rc.get())
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull(const Rc<U>& rc) : NonNull(rc.get())
        {
        }

        NonNull(Ptr<T> ptr) : m_ptr(ptr.m_ptr)
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull(Ptr<U> ptr) : m_ptr(ptr.m_ptr)
        {
            #ifdef COPLT_NULL_CHECK
            if (m_ptr == nullptr) [[unlikely]]
                COPLT_THROW("Null Pointer");
            #endif
        }

        NonNull(NonNull&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr))
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull(NonNull<U>&& other) noexcept : m_ptr(std::exchange(other.m_ptr, nullptr))
        {
        }

        NonNull(const NonNull& other) noexcept : m_ptr(other.m_ptr)
        {
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull(const NonNull<U>& other) noexcept : m_ptr(other.m_ptr)
        {
        }

        NonNull& swap(NonNull& other) noexcept
        {
            std::swap(m_ptr, other.m_ptr);
            return *this;
        }

        NonNull& operator=(NonNull&& other) noexcept
        {
            NonNull(std::forward<NonNull>(other)).swap(*this);
            return *this;
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull& operator=(NonNull<U>&& other) noexcept
        {
            NonNull(std::forward<NonNull<U>>(other)).swap(*this);
            return *this;
        }

        NonNull& operator=(const NonNull& other) noexcept
        {
            NonNull(other).swap(*this);
            return *this;
        }

        template <class U> requires std::convertible_to<U*, T*>
        NonNull& operator=(const NonNull<U>& other) noexcept
        {
            NonNull(other).swap(*this);
            return *this;
        }

        operator T*() const noexcept
        {
            return m_ptr;
        }

        template <class U> requires std::convertible_to<T*, U*>
        operator U*() const noexcept
        {
            return m_ptr;
        }

        T** operator&() noexcept
        {
            return &m_ptr;
        }

        T* const* operator&() const noexcept
        {
            return &m_ptr;
        }

        T* operator->() const noexcept
        {
            return m_ptr;
        }

        T& operator*() const noexcept
        {
            return *m_ptr;
        }

        operator bool() const noexcept
        {
            return true;
        }

        bool operator==(std::nullptr_t) const noexcept
        {
            return false;
        }

        T* get() const noexcept
        {
            return m_ptr;
        }

        bool operator==(const Pointer<T>& ptr) const noexcept
        {
            return m_ptr == ptr;
        }

        template <class U> requires std::convertible_to<T*, U*> || std::convertible_to<U*, U*>
        bool operator==(const Pointer<U>& ptr) const noexcept
        {
            return m_ptr == ptr;
        }

        bool operator==(const Ptr<T>& ptr) const noexcept
        {
            return m_ptr == ptr.m_ptr;
        }

        template <class U> requires std::convertible_to<T*, U*> || std::convertible_to<U*, U*>
        bool operator==(const Ptr<U>& ptr) const noexcept
        {
            return m_ptr == ptr.m_ptr;
        }

        explicit operator usize() const noexcept
        {
            return reinterpret_cast<usize>(m_ptr);
        }

        NonNull operator[](usize offset) const noexcept
        {
            return NonNull(m_ptr + offset);
        }

        NonNull operator+(usize offset) const noexcept
        {
            return NonNull(m_ptr + offset);
        }

        NonNull operator-(usize offset) const noexcept
        {
            return NonNull(m_ptr - offset);
        }
    };
}
