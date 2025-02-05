#pragma once
#include "D3D12MemAlloc.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Device.h"

namespace Coplt
{
    struct D3d12GpuDevice final : Object<D3d12GpuDevice, FD3d12GpuDevice>
    {
        static Rc<D3d12GpuDevice> Create(const FGpuDeviceCreateOptions& options);
    };
}
