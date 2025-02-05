#include "Instance.h"

#include "Device.h"
#include "../../Api/Include/Error.h"

using namespace Coplt;

FResult D3d12Instance::CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDevice** out) noexcept
{
    return feb([&]
    {
        Rc r = D3d12GpuDevice::Create(options);
        *out = r.leak();
    });
}
