#pragma once

#include "../../Api/FFI/Output.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FD3d12GpuOutput, "8d068a07-118c-4d1b-9ab0-384576b61897", FGpuOutput)
    {
        //out 是 ID3D12Resource**
        virtual FResult GetCurrentResourcePtr(void* out) const noexcept = 0;
        // out 是 D3D12_CPU_DESCRIPTOR_HANDLE*
        virtual FResult GetCurrentRtv(void* out) noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(FD3d12GpuSwapChainOutput, "5ee90a8d-8c3d-48ad-915e-87f3d28dabe7", FD3d12GpuOutput)
    {
    };
}
