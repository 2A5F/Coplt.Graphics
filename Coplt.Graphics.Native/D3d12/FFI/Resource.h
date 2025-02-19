#pragma once

#include "../../Api/FFI/Resource.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FD3d12GpuBuffer, "1a65cfb3-07ee-4a04-a9b6-170d92ea956b", FGpuBuffer)
    {
        //out 是 ID3D12Resource**
        virtual FResult GetCurrentResourcePtr(void* out) const noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(FD3d12FGpuImage, "d10a7427-8798-4732-81ff-9c2deb938637", FGpuImage)
    {
        //out 是 ID3D12Resource**
        virtual FResult GetCurrentResourcePtr(void* out) const noexcept = 0;
    };
}
