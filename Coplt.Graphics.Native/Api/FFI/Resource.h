#pragma once

#include "GpuObject.h"

namespace Coplt
{
    enum class FGpuViewType : u8
    {
        None = 0,
        Cbv,
        Srv,
        Uav,
        Rtv,
        Dsv,
        Ibv,
        Vbv,
        Sov,
    };

    COPLT_INTERFACE_DEFINE(FGpuResource, "f99dceec-2f0c-4a28-b666-beb7c35219d6", FGpuObject)
    {
    };
}
