#pragma once

#include "GpuObject.h"
#include "Layout.h"
#include "Resource.h"

namespace Coplt
{
    struct FSetBindItem
    {
        // 绑定内容
        FView View{};
        // 绑定槽的索引
        u32 Slot{};
        // 如果绑定是数组的话，数组中的索引
        u32 Index{};
    };

    struct FShaderBindGroupCreateOptions
    {
        FStr8or16 Name{};
        FBindGroupLayout* Layout{};
        FSetBindItem* Bindings{};
        u32 NumBindings{};
    };

    struct FShaderBindGroupData
    {
        u32* ItemIndexes{};
        u32 NumItemIndexes{};
        u32 CountSlots{};
    };

    COPLT_INTERFACE_DEFINE(FShaderBindGroup, "ae54efe5-c372-4291-b995-55298758d2b2", FGpuObject)
    {
        virtual FShaderBindGroupData* ShaderBindGroupData() noexcept = 0;
    };

    struct FShaderBindGroupCreateResult
    {
        FShaderBindGroup* BindGroup{};
        FShaderBindGroupData* Data{};
    };

    struct FShaderBindingCreateOptions
    {
        FStr8or16 Name{};
        FBindingLayout* Layout{};
        FShaderBindGroup** BindGroups{};
    };

    COPLT_INTERFACE_DEFINE(FShaderBinding, "a3ca644a-0e02-4d25-9a18-8835d66600f7", FGpuObject)
    {
    };
}
