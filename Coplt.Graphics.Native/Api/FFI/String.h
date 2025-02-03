#pragma once

#include "Object.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FString8, "63c7fc71-d775-42bb-891a-69dbb26e75c5")
    {
        const Char8* m_data{};
        usize m_size{};
    };

    COPLT_INTERFACE_DEFINE(FString16, "669355d5-9443-487c-8486-b0b0c00d2367")
    {
        const Char16* m_data{};
        usize m_size{};
    };
}
