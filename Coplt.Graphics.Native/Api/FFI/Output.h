#pragma once

#include "GpuObject.h"
#include "TextureFormat.h"

namespace Coplt
{
    enum class FPresentMode : u8
    {
        NoBuffer,
        DoubleBuffer,
        TripleBuffer,
    };

    enum class FOutputAlphaMode : u8
    {
        Opaque,
        PrePremultiplied,
        PostPremultiplied,
    };

    struct FGpuOutputCreateOptions
    {
        Str8or16 Name;
        u32 Width{};
        u32 Height{};
        FTextureFormat Format{};
        // 呈现模式，实现可以选择忽略
        FPresentMode PresentMode{};
        FOutputAlphaMode AlphaMode{};
        b8 VSync{};
    };

    COPLT_INTERFACE_DEFINE(FGpuOutput, "f1c59cb4-7ee6-4ee2-80f4-07cc568920d2", FGpuObject)
    {
        // 设置垂直同步，实现可以选择忽略
        virtual FResult SetVSync(b8 Enable) noexcept = 0;
    };
}
