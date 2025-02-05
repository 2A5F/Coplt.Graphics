#pragma once
#include "../../Api/Include/Object.h"
#include "../../Api/FFI/Instance.h"
#include "../../Api/FFI/Device.h"
#include "../../Api/Include/InstanceBase.h"
#include "../FFI/Instance.h"

namespace Coplt
{
    struct D3d12Instance final : InstanceBase<Object<D3d12Instance, FD3d12Instance>>
    {
        FResult CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDevice** out) noexcept override;
    };
}
