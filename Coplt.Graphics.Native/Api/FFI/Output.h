#pragma once

#include "Executor.h"
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

    struct FGpuOutputFromSwapChainCreateOptions
    {
        b8 VSync{};
    };

    struct FGpuOutputCreateOptions
    {
        FStr8or16 Name{};
        u32 Width{};
        u32 Height{};
        // 是否使用 srgb，实现可以选择忽略
        b8 Srgb{};
        // 是否使用 hdr，实现可以选择忽略
        FHdrType Hdr{};
        // 呈现模式，实现可以选择忽略
        FPresentMode PresentMode{};
        FOutputAlphaMode AlphaMode{};
        b8 VSync{};
    };

    using WhenDoneFn = void COPLT_CDECL(void* obj);

    struct FCommandSubmit;

    COPLT_INTERFACE_DEFINE(FGpuOutput, "f1c59cb4-7ee6-4ee2-80f4-07cc568920d2", FGpuExecutor)
    {
        FResState m_state{};
        // 外部不允许修改
        FGraphicsFormat m_format{};
        // 外部不允许修改
        u32 m_width{};
        // 外部不允许修改
        u32 m_height{};

        // 设置垂直同步，实现可以选择忽略
        virtual FResult SetVSync(b8 Enable) noexcept = 0;

        virtual FResult Resize(u32 Width, u32 Height) noexcept = 0;
    };

    struct FGpuOutputData
    {
        FGraphicsFormat Format{};
        u32 Width{};
        u32 Height{};
        b8 Srgb{};
        FHdrType Hdr{};
        FPresentMode PresentMode{};
    };

    struct FGpuOutput2CreateOptions
    {
        FStr8or16 Name{};
        // 呈现模式，实现可以选择忽略
        FPresentMode PresentMode{};
        b8 Srgb{};
        FHdrType Hdr{};
    };

    COPLT_INTERFACE_DEFINE(FGpuOutput2, "3f82be7f-2cf5-48a9-8ca4-bb2f9cfe58b2", FGpuExecutor)
    {
        virtual const FGpuOutputData* GpuOutputData() noexcept = 0;

        virtual FResult Resize(u32 Width, u32 Height) noexcept = 0;

        virtual FResult Present() noexcept = 0;
        virtual FResult PresentNoWait() noexcept = 0;

        virtual bool IsAvailable() noexcept = 0;
    };

    struct FGpuSwapChainData : FGpuOutputData
    {
        FOutputAlphaMode AlphaMode{};
        b8 VSync{};
        // 是否使用 srgb，实现可以选择忽略
        b8 Srgb{};
        // 是否使用 hdr，实现可以选择忽略
        FHdrType Hdr{};
    };

    struct FGpuSwapChainCreateOptions : FGpuOutput2CreateOptions
    {
        u32 Width{};
        u32 Height{};
        FOutputAlphaMode AlphaMode{};
        b8 VSync{};
    };

    struct FGpuSwapChainFromExistsCreateOptions : FGpuOutput2CreateOptions
    {
        // d3d12 是 IDXGISwapChain3*
        void* ExistsSwapChain{};
        FOutputAlphaMode AlphaMode{};
        b8 VSync{};
    };

    struct FGpuIsolate;

    COPLT_INTERFACE_DEFINE(FGpuSwapChain, "2bc677a1-77fc-4540-89e2-1fb7e3417dc2", FGpuOutput2)
    {
        virtual const FGpuSwapChainData* GpuSwapChainData() noexcept = 0;
        virtual FGpuIsolate* GetIsolate() noexcept = 0;

        // 设置垂直同步，实现可以选择忽略
        virtual FResult SetVSync(b8 Enable) noexcept = 0;
    };

    struct FGpuSwapChainCreateResult
    {
        FGpuSwapChain* SwapChain{};
        FGpuSwapChainData* Data{};
    };
}
