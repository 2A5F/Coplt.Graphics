#pragma once

#include "GpuObject.h"
#include "States.h"
#include "GraphicsFormat.h"
#include "Resource.h"

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

    enum class FHdrType : u8
    {
        None,
        UNorm10,
        Float16,
    };

    // 输出格式选择器，按选择器选择，不符合将回退，保证成功；指定格式不保证
    struct FGpuOutputFormatSelector
    {
        // 指定格式，忽略选择器
        b8 Specify{};
        // 如果可能，使用 srgb 格式
        b8 Srgb{};
        // 如果可能，使用 hdr 格式，和 srgb 冲突，并且优先级更高
        FHdrType Hdr{};
    };

    struct FGpuOutputFromSwapChainCreateOptions
    {
        b8 VSync{};
    };

    struct FGpuOutputCreateOptions
    {
        Str8or16 Name{};
        u32 Width{};
        u32 Height{};
        FGraphicsFormat Format{};
        // 呈现模式，实现可以选择忽略
        FPresentMode PresentMode{};
        FOutputAlphaMode AlphaMode{};
        b8 VSync{};
        FGpuOutputFormatSelector FormatSelector{};
    };

    using WhenDoneFn = void COPLT_CDECL(void* obj);

    struct FCommandSubmit;

    COPLT_INTERFACE_DEFINE(FGpuOutput, "f1c59cb4-7ee6-4ee2-80f4-07cc568920d2", FGpuObject)
    {
        FResourceState m_state;
        // 外部不允许修改
        FGraphicsFormat m_format;
        // 外部不允许修改
        u32 m_width;
        // 外部不允许修改
        u32 m_height;

        // 设置垂直同步，实现可以选择忽略
        virtual FResult SetVSync(b8 Enable) noexcept = 0;

        virtual FResult Resize(u32 Width, u32 Height) noexcept = 0;

        // 提交命令并等待下帧可用
        virtual FResult Present(/* 可选 */ const FCommandSubmit* submit) noexcept = 0;
        // 提交命令
        virtual FResult PresentNoWait(/* 可选 */ const FCommandSubmit* submit) noexcept = 0;
        // 等待下帧可用
        virtual FResult WaitNextFrame() noexcept = 0;
    };
}
