#pragma once

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
}
