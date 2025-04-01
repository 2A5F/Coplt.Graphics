#pragma once

#include "GpuObject.h"
#include "Context.h"

namespace Coplt
{
    enum class FGpuQueueType : u8
    {
        Direct,
        Compute,
        Copy,
        VideoEncode,
        VideoDecode,
        VideoProcess,
    };

    COPLT_ENUM_FLAGS(FGpuQueueFlags, u8)
    {
        None = 0,
        Direct = 1 << 0,
        Compute = 1 << 1,
        Copy = 1 << 2,
        VideoEncode = 1 << 3,
        VideoDecode = 1 << 4,
        VideoProcess = 1 << 5,
    };

    struct FGpuQueueData
    {
        FGpuQueueType QueueType{};
    };

    COPLT_INTERFACE_DEFINE(FGpuQueue2, "fc94e4d0-5f01-4d4f-894c-6aefaad90499", FGpuObject)
    {
        virtual FGpuQueueData* GpuQueueData() noexcept = 0;
        // d3d12 返回 ID3D12CommandQueue*
        virtual void* GetRawQueue() noexcept = 0;
    };

    struct FGpuQueueCreateResult
    {
        FGpuQueue2* Queue{};
        FGpuQueueData* Data{};
    };
}
