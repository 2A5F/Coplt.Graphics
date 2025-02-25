#pragma once

#if defined(FFI_SRC) && defined(_WINDOWS)
#include <atlconv.h>
#endif

#include "Object.h"

namespace Coplt
{
    struct FStr8or16;

    COPLT_INTERFACE_DEFINE(FString, "5a478800-e7da-4a6b-b428-1e3fda55997f", FUnknown)
    {
        virtual FStr8or16 GetStr() const noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(FString8, "63c7fc71-d775-42bb-891a-69dbb26e75c5", FString)
    {
        const Char8* m_data{};
        usize m_size{};
    };

    COPLT_INTERFACE_DEFINE(FString16, "669355d5-9443-487c-8486-b0b0c00d2367", FString)
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

    struct FStr8
    {
        const char* str;
        i32 len;
    };

    struct FStr16
    {
        const Char16* str;
        i32 len;
    };

    enum class FStrType : u8
    {
        Str16,
        Str8,
    };

    struct FStr8or16
    {
        union
        {
            const Char16* str16;
            const char* str8;
        };

        i32 len;
        FStrType type;

#ifdef FFI_SRC
        FStr8or16() = default;

        explicit FStr8or16(const char* ptr, const usize len)
            : str8(ptr), len(static_cast<i32>(static_cast<u32>(len))), type(FStrType::Str8)
        {
        }

        explicit FStr8or16(const Char8* ptr, const usize len)
            : str8(reinterpret_cast<const char*>(ptr)),
              len(static_cast<i32>(static_cast<u32>(len))), type(FStrType::Str8)
        {
        }

        explicit FStr8or16(const Char16* ptr, const usize len)
            : str16(ptr), len(static_cast<i32>(static_cast<u32>(len))), type(FStrType::Str16)
        {
        }

        explicit FStr8or16(const std::string& str)
            : str8(str.c_str()), len(static_cast<i32>(static_cast<u32>(str.length()))), type(FStrType::Str8)
        {
        }

        template <usize N>
        explicit FStr8or16(const char (&str)[N])
            : str8(str), len(static_cast<i32>(static_cast<u32>(N))), type(FStrType::Str8)
        {
        }

#ifdef _WINDOWS

        explicit FStr8or16(const wchar_t* ptr, const usize len)
            : str16(reinterpret_cast<const Char16*>(ptr)),
              len(static_cast<i32>(static_cast<u32>(len))), type(FStrType::Str16)
        {
        }

        explicit FStr8or16(const std::wstring& str)
            : str16(reinterpret_cast<const Char16*>(str.c_str())),
              len(static_cast<i32>(static_cast<u32>(str.length()))), type(FStrType::Str16)
        {
        }

        template <usize N>
        explicit FStr8or16(const wchar_t (&str)[N]) :
            str16(reinterpret_cast<const Char16*>(str)),
            len(static_cast<i32>(static_cast<u32>(N))), type(FStrType::Str16)
        {
        }

#endif

        bool is8() const
        {
            return type == FStrType::Str8;
        }

        bool is16() const
        {
            return type == FStrType::Str16;
        }

        bool is_null() const
        {
            return str16 == nullptr;
        }

        FString* ToString() const;
#endif
    };

    struct FCStr8or16
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
