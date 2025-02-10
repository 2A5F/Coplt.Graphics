#pragma once

#include "Common.h"

#ifdef _MSC_VER
#define COPLT_UUID_MARK(ID) __declspec( uuid(ID) )
#else
#define COPLT_UUID_MARK(ID)
#endif

#ifdef _MSC_VER
#define COPLT_NOVTABLE __declspec(novtable)
#else
#define COPLT_NOVTABLE
#endif

#ifdef FFI_SRC
#define COPLT_INTERFACE_GUID(ID) constexpr static Guid GUID = Guid(ID);
#define COPLT_INTERFACE_DEFINE(NAME, ID, ...) namespace _internal { struct COPLT_NOVTABLE META_##NAME : __VA_ARGS__ {\
    COPLT_INTERFACE_GUID(ID)\
private:\
    template <Interface... T>\
    friend struct DoQueryInterface;\
    COPLT_FORCE_INLINE constexpr static void* QueryInterfaceForBase(auto* self, Guid id)\
    {\
        return ::Coplt::_internal::DoQueryInterface<__VA_ARGS__>::QueryInterface(self, id);\
    }\
};};\
struct COPLT_UUID_MARK(ID) COPLT_NOVTABLE NAME : _internal::META_##NAME
#else
#define COPLT_INTERFACE_GUID(ID) constexpr static auto s_FFI_GUID = L##ID;
#define COPLT_INTERFACE_DEFINE(NAME, ID, ...) struct COPLT_UUID_MARK(ID) NAME : __VA_ARGS__
#endif

namespace Coplt
{
    struct FObject
    {
        virtual ~FObject() noexcept = default;
        virtual size_t Release() noexcept = 0;
    };

    struct FRcObject : FObject
    {
        virtual size_t AddRef() noexcept = 0;
    };

#ifdef FFI_SRC

    struct FUnknown;

    template <class T>
    concept Interface = requires
    {
        { T::GUID } -> std::same_as<const Guid&>;
        std::is_base_of_v<FUnknown, T>;
    };

    namespace _internal
    {
        template <Interface... Interfaces>
        struct DoQueryInterface
        {
        };

        template <>
        struct DoQueryInterface<>
        {
            COPLT_FORCE_INLINE constexpr static void* QueryInterface(auto* self, const Guid& id)
            {
                return nullptr;
            }
        };

        template <Interface First, Interface... Last>
        struct DoQueryInterface<First, Last...>
        {
            COPLT_FORCE_INLINE constexpr static void* QueryInterface(auto* self, const Guid& id)
            {
                if (First::GUID == id) return static_cast<First*>(self);
                if (void* r = First::QueryInterfaceForBase(static_cast<First*>(self), id)) return r;
                return DoQueryInterface<Last...>::QueryInterface(self, id);
            }
        };
    }

#endif

    struct COPLT_UUID_MARK("00000000-0000-0000-0000-000000000000") COPLT_NOVTABLE FUnknown : FRcObject
    {
        COPLT_INTERFACE_GUID("00000000-0000-0000-0000-000000000000");

        // 返回 null 表示失败，这个 QueryInterface 不会增加引用计数
        virtual void* QueryInterface(const Guid& id) noexcept = 0;

#ifdef FFI_SRC
        template <Interface T>
        COPLT_FORCE_INLINE T* QueryInterface() noexcept
        {
            return static_cast<T*>(QueryInterface(T::GUID));
        }

    private:
        template <Interface... T>
        friend struct _internal::DoQueryInterface;

        COPLT_FORCE_INLINE constexpr static void* QueryInterfaceForBase(auto* self, const Guid& id)
        {
            return nullptr;
        }
#endif
    };
}
