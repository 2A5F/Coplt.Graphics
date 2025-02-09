#pragma once

#include "GpuObject.h"

namespace Coplt
{
    enum class FShaderStage : u8
    {
        Compute = 0,
        Pixel = 1,
        Vertex = 2,
        Mesh = 3,
        Task = 4,
    };

    COPLT_ENUM_FLAGS(FShaderStageFlags, u8)
    {
        Compute = 1 << 0,
        Pixel = 1 << 1,
        Vertex = 1 << 2,
        Mesh = 1 << 3,
        Task = 1 << 4,
    };

    struct FShaderModuleCreateOptions
    {
        // 需要自己保证数据内容符合当前使用的后端，比如 dx 使用的 dxil，vk 使用的 spv
        void* Data{};
        usize Size{};
        FShaderStage Stage{};
    };

    COPLT_INTERFACE_DEFINE(FShaderModule, "5c0e1fdb-2acd-4fce-b985-09e12a7a1aad", FGpuObject)
    {
        // 外部只读
        u8* Data{};
        // 外部只读
        usize Size{};
        // 外部只读
        FShaderStage Stage{};
    };

    struct FShaderCreateOptions
    {
        // 此处是借用，不传递所有权
        FShaderModule** Modules{};
        u8 Count{};
    };

    COPLT_INTERFACE_DEFINE(FShader, "de1352d5-023d-42b0-beac-122b3b296c9c", FGpuObject)
    {
        FShaderStageFlags Stages{};
    };

    COPLT_INTERFACE_DEFINE(FShaderPipeline, "356a2610-34e3-4c01-9904-22e3c5ce2f4e", FGpuObject)
    {
        FShaderStageFlags Stages{};
    };
}
