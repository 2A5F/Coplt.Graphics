#pragma once

#include "GpuObject.h"
#include "Output.h"

namespace Coplt
{
    struct FMainQueueCreateOptions
    {
        // 可选
        Str8or16 Name{};
    };

    enum class FGpuQueueType : u8
    {
        Direct,
        Compute,
        Copy,
    };

    COPLT_INTERFACE_DEFINE(FGpuQueue, "95e60e28-e387-4055-9b33-2d23af901f8a", FGpuObject)
    {
        // 不安全的内存暴露，外部不能修改
        FGpuQueueType m_queue_type{};

        // d3d12 返回 ID3D12CommandQueue*
        virtual void* GetRawQueue() noexcept = 0;

        // 在非 windows 平台调用会报错
        virtual FResult CreateOutputForHwnd(
            const FGpuOutputCreateOptions& options,
            void* hwnd,
            FGpuOutput** out
        ) noexcept = 0;
    };
}
