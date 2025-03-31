#pragma once

#include <atomic>
#include <utility>

#include "../FFI/Object.h"

namespace Coplt
{
    struct UniqueObject : FObject
    {
        size_t Release() noexcept override;
    };

    template <class T>
    class Weak;

    template <class T>
    class Rc final
    {
        T* m_ptr;

        template <class U>
        friend class Rc;

        template <class U>
        friend class Weak;

        struct clone_t
        {
        };

        struct upgrade_t
        {
        };

        // clone
        explicit Rc(T* ptr, clone_t) : m_ptr(ptr)
        {
            static_assert(std::is_base_of_v<FRcObject, T>);

            if (auto p = get())
            {
                p->AddRef();
            }
        }

        // upgrade
        explicit Rc(T* ptr, upgrade_t) : m_ptr(ptr && ptr->TryUpgrade() ? ptr : nullptr)
        {
            static_assert(std::is_base_of_v<FRcObject, T>);
        }

    public:
        using value_t = T;

        ~Rc()
        {
            if (auto p = m_ptr)
            {
                p->Release();
            }
        }

        void reset()
        {
            if (auto p = std::exchange(m_ptr, nullptr))
            {
                p->Release();
            }
        }

        explicit operator bool() const
        {
            return get() != nullptr;
        }

        T* get() const
        {
            return m_ptr;
        }

        T& operator*() const;

        T* operator->() const;

        T* operator&() const
        {
            return get();
        }

        // null
        Rc() : m_ptr(nullptr)
        {
        }

        // null
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Rc(std::nullptr_t) : m_ptr(nullptr) // NOLINT(*-explicit-constructor)
        {
        }

        // create
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Rc(T* ptr) : m_ptr(ptr) // NOLINT(*-explicit-constructor)
        {
            static_assert(std::is_base_of_v<FRcObject, T>);
        }

        // copy
        Rc(const Rc& r) : Rc(r.get(), clone_t())
        {
        }

        // copy conv
        template <typename U> requires std::convertible_to<U*, T*>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Rc(const Rc<U>& r): Rc(r.get(), clone_t()) // NOLINT(*-explicit-constructor)
        {
        }

        // move
        Rc(Rc&& r) noexcept: m_ptr(std::exchange(r.m_ptr, nullptr))
        {
        }

        // move conv
        template <typename U> requires std::convertible_to<U*, T*>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Rc(Rc<U>&& r): m_ptr(std::exchange(r.m_ptr, nullptr)) // NOLINT(*-explicit-constructor)
        {
        }

        Rc& operator=(T* p)
        {
            Rc(p).swap(*this);
            return *this;
        }

        // copy ass
        Rc& operator=(Rc const& r) noexcept
        {
            if (m_ptr != r.m_ptr) Rc(r).swap(*this);
            return *this;
        }

        // move ass
        Rc& operator=(Rc&& r) noexcept
        {
            if (&m_ptr != &r.m_ptr) Rc(std::forward<Rc>(r)).swap(*this);
            return *this;
        }

        // copy ass conv
        template <typename U> requires std::convertible_to<U*, T*>
        Rc& operator=(Rc<U> const& r) noexcept
        {
            Rc(r).swap(*this);
            return *this;
        }

        // move ass conv
        template <typename U> requires std::convertible_to<U*, T*>
        Rc& operator=(Rc<U>&& r) noexcept
        {
            Rc(std::forward<Rc<U>>(r)).swap(*this);
            return *this;
        }

        void swap(Rc& r) noexcept
        {
            std::swap(m_ptr, r.m_ptr);
        }

        void Swap(Rc& r) noexcept
        {
            swap(r);
        }

        Rc clone() const
        {
            return Rc(get(), clone_t());
        }

        Rc Clone() const
        {
            return clone();
        }

        static Rc UnsafeClone(T* ptr)
        {
            return Rc(ptr, clone_t());
        }

        static Rc UnsafeCreate(T* ptr)
        {
            Rc r = ptr;
            return r;
        }

        static void UnsafeDrop(T* ptr)
        {
            Rc _ = ptr;
        }

        // Direct leakage, out of RAII management
        T* leak() noexcept
        {
            return std::exchange(m_ptr, nullptr);
        }

        T* Leak() noexcept
        {
            return leak();
        }

        Weak<T> downgrade() const
        {
            return Weak(get(), Weak<T>::downgrade_t());
        }

        bool operator==(std::nullptr_t) const
        {
            return get() == nullptr;
        }

        bool operator==(T* ptr) const
        {
            return get() == ptr;
        }

        bool operator==(const T& ref) const
        {
            return *get() == ref;
        }
    };

    template <class T>
    class Weak final
    {
        T* m_ptr;

        template <class U>
        friend class Weak;

        template <class U>
        friend class Rc;

        struct clone_t
        {
        };

        struct downgrade_t
        {
        };

        // clone
        explicit Weak(T* ptr, clone_t) : m_ptr(ptr)
        {
            static_assert(std::is_base_of_v<FRcObject, T>);

            if (auto p = get())
            {
                p->AddRefWeak();
            }
        }

        // downgrade
        explicit Weak(T* ptr, downgrade_t) : m_ptr(ptr && ptr->TryDowngrade() ? ptr : nullptr)
        {
            static_assert(std::is_base_of_v<FRcObject, T>);
        }

        T* get() const
        {
            return m_ptr;
        }

    public:
        using value_t = T;

        inline static Weak None{};

        ~Weak()
        {
            if (auto p = m_ptr)
            {
                p->ReleaseWeak();
            }
        }

        void reset()
        {
            if (auto p = std::exchange(m_ptr, nullptr))
            {
                p->ReleaseWeak();
            }
        }

        explicit operator bool() const
        {
            return get() != nullptr;
        }

        // null
        Weak() : m_ptr(nullptr)
        {
        }

        // null
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Weak(std::nullptr_t) : m_ptr(nullptr) // NOLINT(*-explicit-constructor)
        {
        }

        // copy
        Weak(const Weak& r) : Weak(r.get(), clone_t())
        {
        }

        // downgrade
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Weak(const Rc<T>& r) : Weak(r.get(), downgrade_t()) // NOLINT(*-explicit-constructor)
        {
        }

        // copy conv
        template <typename U> requires std::convertible_to<U*, T*>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Weak(const Weak<U>& r): Weak(r.get(), clone_t()) // NOLINT(*-explicit-constructor)
        {
        }

        // downgrade conv
        template <typename U> requires std::convertible_to<U*, T*>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Weak(const Rc<U>& r): Weak(r.get(), downgrade_t()) // NOLINT(*-explicit-constructor)
        {
        }

        // move
        Weak(Weak&& r) noexcept: m_ptr(std::exchange(r.m_ptr, nullptr))
        {
        }

        // move conv
        template <typename U> requires std::convertible_to<U*, T*>
        // ReSharper disable once CppNonExplicitConvertingConstructor
        Weak(Weak<U>&& r): m_ptr(std::exchange(r.ptr, nullptr)) // NOLINT(*-explicit-constructor)
        {
        }

        // copy ass
        Weak& operator=(Weak const& r) noexcept
        {
            if (m_ptr != r.m_ptr) Weak(r).swap(*this);
            return *this;
        }

        // move ass
        Weak& operator=(Weak&& r) noexcept
        {
            if (&m_ptr != &r.m_ptr) Weak(std::forward<Weak>(r)).swap(*this);
            return *this;
        }

        // copy ass conv
        template <typename U> requires std::convertible_to<U*, T*>
        Weak& operator=(Weak<U> const& r) noexcept
        {
            Weak(r).swap(*this);
            return *this;
        }

        // move ass conv
        template <typename U> requires std::convertible_to<U*, T*>
        Weak& operator=(Weak<U>&& r) noexcept
        {
            Weak(std::forward<Weak<U>>(r)).swap(*this);
            return *this;
        }

        void swap(Weak& r) noexcept
        {
            std::swap(m_ptr, r.m_ptr);
        }

        void Swap(Weak& r) noexcept
        {
            swap(r);
        }

        Rc<T> upgrade() const
        {
            return Rc(get(), Rc<T>::upgrade_t());
        }

        bool operator==(std::nullptr_t) const
        {
            return get() == nullptr;
        }
    };

    namespace _internal
    {
        template <Interface...>
        struct MergeInterface
        {
        };

        template <Interface First>
        struct MergeInterface<First>
        {
            using Output = First;
        };

        template <Interface First, Interface... Last>
        struct MergeInterface<First, Last...> : First, MergeInterface<Last...>::Output
        {
            using Output = MergeInterface;
        };

        template <>
        struct MergeInterface<>
        {
            using Output = FUnknown;
        };
    }

    struct IsObject
    {
    };

    template <class T>
    concept ObjectLike = requires
    {
        std::is_base_of_v<IsObject, T>;
    };

    struct ObjectRcPartialImpl
    {
        mutable std::atomic_size_t m_strong{1};
        mutable std::atomic_size_t m_weak{1};

        COPLT_NO_INLINE void DropSlow(void* self, void* object_start, void (*p_free)(void*, void*)) const noexcept
        {
            this->~ObjectRcPartialImpl();
            ReleaseWeak(self, object_start, p_free);
        }

        size_t AddRef() const noexcept
        {
            return m_strong.fetch_add(1, std::memory_order_relaxed);
        }

        size_t Release(void* self, void* object_start, void (*p_free)(void*, void*)) const noexcept
        {
            const size_t r = m_strong.fetch_sub(1, std::memory_order_release);
            if (r != 1) return r;
            DropSlow(self, object_start, p_free);
            return r;
        }

        size_t AddRefWeak() const noexcept
        {
            return m_weak.fetch_add(1, std::memory_order_relaxed);
        }

        size_t ReleaseWeak(void* self, void* object_start, void (*p_free)(void*, void*)) const noexcept
        {
            const size_t r = m_weak.fetch_sub(1, std::memory_order_release);
            if (r != 1) return r;
            p_free(self, object_start);
            return r;
        }

        b8 TryDowngrade() const noexcept
        {
            size_t cur = m_weak.load(std::memory_order_relaxed);
        re_try:
            if (cur == 0) return false;
            if (m_weak.compare_exchange_weak(cur, cur + 1, std::memory_order_acquire, std::memory_order_relaxed))
            {
                return true;
            }
            goto re_try;
        }

        b8 TryUpgrade() const noexcept
        {
            size_t cur = m_strong.load(std::memory_order_relaxed);
        re_try:
            if (cur == 0) return false;
            if (m_strong.compare_exchange_weak(cur, cur + 1, std::memory_order_acquire, std::memory_order_relaxed))
            {
                return true;
            }
            goto re_try;
        }
    };

    // 对象不允许多继承，只允许具有多个接口
    template <class This, class Base, Interface... Ts>
    struct ObjectImpl
    {
        // ReSharper disable once CppStaticAssertFailure
        static_assert(false, "不应该存在此类型");
    };

    // 对象不允许多继承，只允许具有多个接口
    template <class This, Interface... Interfaces>
    struct ObjectImpl<This, void, Interfaces...>
        : _internal::MergeInterface<Interfaces...>::Output, ObjectRcPartialImpl, IsObject
    {
        virtual void* ObjectStart() noexcept
        {
            This* self = static_cast<This*>(this);
            return self;
        }

    private:
        static void DoFree(void* self, void* p)
        {
            static_cast<ObjectImpl*>(self)->Free(p);
        }

    protected:
        virtual void Free(void* self) noexcept { operator delete(self); }

    public:
        void* QueryInterface(const Guid& id) noexcept override
        {
            return _internal::DoQueryInterface<Interfaces...>::QueryInterface(this, id);
        }

        template <class Self>
        Self&& Move(this Self&& self)
        {
            return std::forward<Self>(self);
        }

        template <class Self>
        Rc<Self> CloneThis(this Self& self)
        {
            return Rc<Self>::UnsafeClone(&self);
        }

        size_t AddRef() noexcept override
        {
            return ObjectRcPartialImpl::AddRef();
        }

        size_t Release() noexcept override
        {
            return ObjectRcPartialImpl::Release(this, ObjectStart(), DoFree);
        }

        virtual size_t AddRefWeak() noexcept
        {
            return ObjectRcPartialImpl::AddRefWeak();
        }

        virtual size_t ReleaseWeak() noexcept
        {
            return ObjectRcPartialImpl::ReleaseWeak(this, ObjectStart(), DoFree);
        }

        virtual b8 TryDowngrade() noexcept
        {
            return ObjectRcPartialImpl::TryDowngrade();
        }

        virtual b8 TryUpgrade() noexcept
        {
            return ObjectRcPartialImpl::TryUpgrade();
        }
    };

    // 对象不允许多继承，只允许具有多个接口
    template <class This, ObjectLike TBase, Interface... Interfaces>
    struct ObjectImpl<This, TBase, Interfaces...> : TBase, _internal::MergeInterface<Interfaces...>::Output
    {
        using Base = ObjectImpl;

        void* ObjectStart() noexcept override
        {
            This* self = static_cast<This*>(this);
            return self;
        }

        size_t Release() noexcept override
        {
            return TBase::Release();
        }

        size_t AddRef() noexcept override
        {
            return TBase::AddRef();
        }

        void* QueryInterface(const Guid& id) noexcept override
        {
            return TBase::QueryInterface(id);
        }
    };

    template <class This, class Base>
    struct SubObject : Base
    {
        void* ObjectStart() noexcept override
        {
            This* self = static_cast<This*>(this);
            return self;
        }
    };

    template <class This, class Base, Interface... Interfaces>
    using Object = std::conditional_t<
        std::is_base_of_v<IsObject, Base>,
        ObjectImpl<This, Base, Interfaces...>,
        ObjectImpl<This, void, Base, Interfaces...>
    >;

    template <Interface T>
    struct TryQueryInterface_t
    {
    };

    template <Interface T>
    TryQueryInterface_t<T> TryQueryInterface()
    {
        return {};
    }

    template <Interface T,Interface U>
    U* operator >>(T* ptr, TryQueryInterface_t<U>)
    {
        if (ptr == nullptr) return nullptr;
        return ptr->template QueryInterface<U>();
    }

    template <Interface T,Interface U>
    U* operator >>(Rc<T>& ptr, TryQueryInterface_t<U>)
    {
        if (ptr == nullptr) return nullptr;
        return ptr->template QueryInterface<U>();
    }

    template <Interface T,Interface U>
    U* operator >>(Weak<T>& ptr, TryQueryInterface_t<U>)
    {
        if (ptr == nullptr) return nullptr;
        return ptr->template QueryInterface<U>();
    }
}

#include "Error.h"

namespace Coplt
{
    template <class T>
    T& Rc<T>::operator*() const
    {
        const auto ptr = get();
        #ifdef COPLT_NULL_CHECK
        if (ptr == nullptr) [[unlikely]]
            COPLT_THROW("Null Pointer");
        #endif
        return *ptr;
    }

    template <class T>
    T* Rc<T>::operator->() const
    {
        const auto ptr = get();
        #ifdef COPLT_NULL_CHECK
        if (ptr == nullptr) [[unlikely]]
            COPLT_THROW("Null Pointer");
        #endif
        return ptr;
    }
}
