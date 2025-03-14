#pragma once

#include <utility>
#include "Ptr.h"

namespace Coplt
{
    struct OptionBase
    {
        template <class Self>
        operator bool(this const Self& self)
        {
            return self.HasValue();
        }

        template <class Self>
        bool operator!=(this const Self& self, std::nullptr_t)
        {
            return self.HasValue();
        }

        template <class Self>
        bool operator!=(this const Self& self, std::nullopt_t)
        {
            return self.HasValue();
        }
    };

    template <class T>
    class Option : public OptionBase
    {
        T m_value;
        bool m_has;

    public:
        ~Option()
        {
            if (std::exchange(m_has, false))
            {
                m_value.~T();
            }
        }

        Option(std::nullopt_t) : Option()
        {
        }

        Option(std::nullptr_t) : Option()
        {
        }

        Option(): m_has(false)
        {
        }

        Option(T&& value): m_value(std::forward<T>(value)), m_has(true)
        {
        }

        Option(const T& value): m_value(value), m_has(true)
        {
        }

        bool HasValue() const
        {
            return m_has;
        }

        T& get()
        {
            if (!m_has)
                COPLT_THROW("Null Option");
            return m_value;
        }

        const T& get() const
        {
            if (!m_has)
                COPLT_THROW("Null Option");
            return m_value;
        }

        T& operator*()
        {
            if (!m_has)
                COPLT_THROW("Null Option");
            return m_value;
        }

        T* operator->()
        {
            if (!m_has)
                COPLT_THROW("Null Option");
            return std::addressof(m_value);
        }

        const T& operator*() const
        {
            if (!m_has)
                COPLT_THROW("Null Option");
            return m_value;
        }

        const T* operator->() const
        {
            if (!m_has)
                COPLT_THROW("Null Option");
            return std::addressof(m_value);
        }
    };

    template <class T>
    class Option<NonNull<T>> : public OptionBase
    {
        T* m_ptr;

    public:
        Option(std::nullopt_t) : Option()
        {
        }

        Option(std::nullptr_t) : Option()
        {
        }

        Option(): m_ptr(nullptr)
        {
        }

        Option(const NonNull<T>& ptr) : m_ptr(ptr.m_ptr)
        {
        }

        Option(NonNull<T>&& ptr) : m_ptr(ptr.m_ptr)
        {
        }

        bool HasValue() const
        {
            return m_ptr != nullptr;
        }

        NonNull<T> get() const
        {
            if (!HasValue())
                COPLT_THROW("Null Option");
            return NonNull<T>::Unchecked(m_ptr);
        }

        T& operator*() const
        {
            if (!HasValue())
                COPLT_THROW("Null Option");
            return *m_ptr;
        }

        T* operator->() const
        {
            if (!HasValue())
                COPLT_THROW("Null Option");
            return m_ptr;
        }
    };

    template <class T>
    Option<T> Some(const T& value)
    {
        return Option(value);
    }

    template <class T>
    Option<T> Some(T&& value)
    {
        return Option(std::forward<T>(value));
    }
}
