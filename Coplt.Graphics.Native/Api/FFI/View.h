#pragma once

#include "GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FGpuViewable, "b3aeb8a5-1fa6-4866-97ef-1a5fa401e18f", FGpuObject)
    {
    };

    enum class FViewType : u8
    {
        None = 0,
        Buffer = 1,
        Image = 2,
        Sampler = 3,
    };

    struct FView
    {
        FGpuViewable* Viewable{};
        FViewType Type{};
    };
}
