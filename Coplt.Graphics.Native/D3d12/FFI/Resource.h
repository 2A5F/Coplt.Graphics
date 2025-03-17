#pragma once

#include "../../Api/FFI/Resource.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FD3d12GpuBuffer, "1a65cfb3-07ee-4a04-a9b6-170d92ea956b", FGpuBuffer)
    {
        // 返回 ID3D12Resource*
        virtual void* GetRawResourcePtr() noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(FD3d12GpuImage, "d10a7427-8798-4732-81ff-9c2deb938637", FGpuImage)
    {
        // 返回 ID3D12Resource*
        virtual void* GetRawResourcePtr() noexcept = 0;
    };
}
