#pragma once

#include "GpuObject.h"
#include "Layout.h"
#include "Resource.h"

namespace Coplt
{
    struct FBindItem
    {
        FView View{};
        u32 Index{};
    };

    struct FShaderBindGroupCreateOptions
    {
        FStr8or16 Name{};
        FBindGroupLayout* Layout{};
    };

    COPLT_INTERFACE_DEFINE(FShaderBindGroup, "ae54efe5-c372-4291-b995-55298758d2b2", FGpuObject)
    {
    };

    struct FShaderBindingCreateOptions
    {
        FStr8or16 Name{};
        FBindingLayout* Layout{};
    };

    COPLT_INTERFACE_DEFINE(FShaderBinding, "a3ca644a-0e02-4d25-9a18-8835d66600f7", FGpuObject)
    {
    };
}
