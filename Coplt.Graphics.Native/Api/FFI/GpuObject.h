#pragma once

#include "Object.h"
#include "Result.h"
#include "TextureFormat.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FGpuObject, "9fc6890b-e9a2-4f4a-9217-a14a4223a715", FUnknown)
    {
        // 外部不允许修改
        FTextureFormat m_format;
        // 外部不允许修改
        u32 m_width;
        // 外部不允许修改
        u32 m_height;

        virtual FResult SetName(const Str8or16& name) noexcept = 0;
    };
}
