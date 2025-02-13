#pragma once

#include "GpuObject.h"
#include "PipelineState.h"

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
        None = 0,
        Compute = 1 << 0,
        Pixel = 1 << 1,
        Vertex = 1 << 2,
        Mesh = 1 << 3,
        Task = 1 << 4,
    };

    struct FShaderModuleCreateOptions
    {
        Str8or16 Name{};
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

    struct FShaderLayout;
    struct FShaderInputLayout;

    struct FShaderCreateOptions
    {
        Str8or16 Name{};
        // 此处是借用，不传递所有权
        FShaderLayout* Layout{};
        // 此处是借用，不传递所有权
        FShaderInputLayout* InputLayout{};
        // 此处是借用，不传递所有权
        FShaderModule** Modules{};
        u8 Count{};
    };

    COPLT_INTERFACE_DEFINE(FShader, "de1352d5-023d-42b0-beac-122b3b296c9c", FGpuObject)
    {
        constexpr static u32 MaxShaderModuleCount = 3;

        FShaderStageFlags Stages{};

        virtual FShaderLayout* Layout() noexcept = 0;

        virtual FShaderInputLayout* InputLayout() noexcept = 0;

        // 没有返回 null
        virtual FShaderModule* Compute() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Pixel() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Vertex() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Mesh() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Task() noexcept = 0;
    };
}
