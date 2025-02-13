#pragma once

#include "GpuObject.h"
#include "Layout.h"
#include "Queue.h"
#include "Shader.h"
#include "Pipeline.h"

namespace Coplt
{
    enum class FVulkanVersion : i32
    {
        Unset = 0,
        _1_2 = 1002,
        _1_3 = 1003,
        _1_4 = 1004,
    };

    enum class FD3dFeatureLevel : i32
    {
        Unset = 0,
        _12_1 = 0xc100,
        _12_2 = 0xc200,
    };

    enum class FGpuPreference : u8
    {
        HighPerformance = 0,
        MinimumPower = 1,
        Any = 255,
    };

    struct FGpuDeviceCreateOptions
    {
        // 可选
        Str8or16 Name{};
        // 仅 dx 后端时可用
        FD3dFeatureLevel D3dFeatureLevel{};
        // 仅 vk 后端时可用
        FVulkanVersion VulkanVersion{};
        FGpuPreference Preference{};
        b8 Debug{};
    };

    COPLT_INTERFACE_DEFINE(FGpuDevice, "557f032d-ed50-403a-adc5-214fddbe6c6b", FGpuObject)
    {
        // d3d12 返回 ID3D12Device*
        virtual void* GetRawDevice() noexcept = 0;

        virtual FResult CreateMainQueue(const FMainQueueCreateOptions& options, FGpuQueue** out) noexcept = 0;

        virtual FResult CreateShaderModule(const FShaderModuleCreateOptions& options, FShaderModule** out) noexcept = 0;
        virtual FResult CreateShaderLayout(const FShaderLayoutCreateOptions& options, FShaderLayout** out) noexcept = 0;
        virtual FResult CreateShaderInputLayout(
            const FShaderInputLayoutCreateOptions& options, FShaderInputLayout** out
        ) noexcept = 0;
        virtual FResult CreateShader(const FShaderCreateOptions& options, FShader** out) noexcept = 0;

        virtual FResult CreateMeshLayout(const FMeshLayoutCreateOptions& options, FMeshLayout** out) noexcept = 0;

        virtual FResult CreateGraphicsPipeline(
            const FGraphicsShaderPipelineCreateOptions& options, FGraphicsShaderPipeline** out
        ) noexcept = 0;
    };
}
