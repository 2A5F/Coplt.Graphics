#pragma once

#if defined(FFI_SRC) && defined(_WINDOWS)
#include <atlconv.h>
#endif

#include "Object.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FString8, "63c7fc71-d775-42bb-891a-69dbb26e75c5", FUnknown)
    {
        const Char8* m_data{};
        usize m_size{};
    };

    COPLT_INTERFACE_DEFINE(FString16, "669355d5-9443-487c-8486-b0b0c00d2367", FUnknown)
    {
        const Char16* m_data{};
        usize m_size{};
    };

    enum class FMessageType : u8
    {
        CStr8,
        CStr16,
        Slice8,
        Slice16,
        String8,
        String16,
    };

    union FMessageUnion
    {
        const char* str;
        const wchar_t* wstr;
        const Char8* str8;
        const Char16* str16;
        FString8* string8;
        FString16* string16;
    };

    struct FMessage
    {
        FMessageUnion msg;
        i32 len;
        FMessageType type;
    };

    struct Str8
    {
        const char* str;
        i32 len;
    };

    struct Str16
    {
        const Char16* str;
        i32 len;
    };

    struct Str8or16
    {
        const char* str8;
        const Char16* str16;
        i32 len;

#ifdef FFI_SRC
        bool has8() const
        {
            return str8 != nullptr;
        }

        bool has16() const
        {
            return str16 != nullptr;
        }

        bool is_null() const
        {
            return str8 == nullptr && str16 == nullptr;
        }
#endif
    };

    struct CStr8or16
    {
        const char* str8;
        const Char16* str16;

#ifdef FFI_SRC
        bool has8() const
        {
            return str8 != nullptr;
        }

        bool has16() const
        {
            return str16 != nullptr;
        }

        bool is_null() const
        {
            return str8 == nullptr && str16 == nullptr;
        }
#endif
    };

#if defined(FFI_SRC) && defined(_WINDOWS)


    inline std::wstring to_wstring(const char* str)
    {
        const ATL::CA2W ca2_w(str);
        return std::wstring(ca2_w);
    }

#endif

}
