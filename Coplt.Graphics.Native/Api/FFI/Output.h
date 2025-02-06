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

    using WhenDoneFn = void COPLT_CDECL(void* obj);

    COPLT_INTERFACE_DEFINE(FGpuOutput, "f1c59cb4-7ee6-4ee2-80f4-07cc568920d2", FGpuObject)
    {
        // 设置垂直同步，实现可以选择忽略
        virtual FResult SetVSync(b8 Enable) noexcept = 0;

        virtual FResult Resize(u32 Width, u32 Height) noexcept = 0;

        // 提交命令并等待下帧可用
        virtual FResult Present() noexcept = 0;
        // 提交命令
        virtual FResult PresentNoWait() noexcept = 0;
        // 等待下帧可用
        virtual FResult WaitNextFrame() noexcept = 0;
    };
}
