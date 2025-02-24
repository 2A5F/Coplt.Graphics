#pragma once

#include "GpuObject.h"
#include "Layout.h"
#include "Resource.h"

namespace Coplt
{
    struct FShaderBindingCreateOptions
    {
        Str8or16 Name{};
        FShaderLayout* Layout{};
    };

    struct FShaderBindingBatchSet
    {
        FGpuView* View{};
        u32 Index{};
    };

    COPLT_INTERFACE_DEFINE(FShaderBinding, "a3ca644a-0e02-4d25-9a18-8835d66600f7", FGpuObject)
    {
        virtual FResult Set(u32 count, const FShaderBindingBatchSet* bindings) noexcept = 0;
    };
}
