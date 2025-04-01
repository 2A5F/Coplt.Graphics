#pragma once

#include "Binding.h"
#include "Features.h"
#include "GpuObject.h"
#include "Isolate.h"
#include "Layout.h"
#include "Queue.h"
#include "Shader.h"
#include "Pipeline.h"
#include "Resource.h"
#include "Sampler.h"

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

    enum class FDeviceTypeRequire : u8
    {
        Any,
        Gpu,
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    };

    enum class FDeviceType : u8
    {
        Other,
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    };

    struct FDeviceRequires
    {
        // 仅 dx 后端时可用
        FD3dFeatureLevel D3dFeatureLevel{};
        // 仅 vk 后端时可用
        FVulkanVersion VulkanVersion{};
        FDeviceTypeRequire DeviceType{};
        FDeviceFeatureRequires FeatureRequires{};
    };

    struct FGpuDeviceCreateOptions
    {
        // 可选
        FStr8or16 Name{};
    };

    struct FGpuAutoSelectDeviceCreateOptions : FGpuDeviceCreateOptions
    {
        FDeviceRequires Requires{};
    };

    struct FGpuAdapter;

    COPLT_INTERFACE_DEFINE(FGpuDevice, "557f032d-ed50-403a-adc5-214fddbe6c6b", FGpuObject)
    {
        virtual FGpuAdapter* GetAdapter() noexcept = 0;

        // d3d12 返回 ID3D12Device*
        virtual void* GetRawDevice() noexcept = 0;

        virtual FResult CreateIsolate(const FGpuIsolateCreateOptions& options, FMainQueueCreateResult& out) noexcept = 0;

        virtual FResult CreateShaderModule(const FShaderModuleCreateOptions& options, FShaderModule** out) noexcept = 0;
        virtual FResult CreateShaderLayout(const FShaderLayoutCreateOptions& options, FShaderLayout** out) noexcept = 0;
        virtual FResult GetEmptyShaderLayout(const FGetEmptyShaderLayoutOptions& options, FShaderLayout** out) noexcept = 0;
        virtual FResult CreateShaderInputLayout(const FShaderInputLayoutCreateOptions& options, FShaderInputLayout** out) noexcept = 0;
        virtual FResult CreateShader(const FShaderCreateOptions& options, FShader** out) noexcept = 0;
        virtual FResult CreateShaderBinding(const FShaderBindingCreateOptions& options, FShaderBinding** out) noexcept = 0;

        virtual FResult CreateMeshLayout(const FMeshLayoutCreateOptions& options, FMeshLayout** out) noexcept = 0;

        virtual FResult CreateGraphicsPipeline(const FGraphicsShaderPipelineCreateOptions& options, FGraphicsShaderPipeline** out) noexcept = 0;

        virtual FResult CreateBuffer(const FGpuBufferCreateOptions& options, FGpuBuffer** out) noexcept = 0;
        virtual FResult CreateImage(const FGpuImageCreateOptions& options, FGpuImage** out) noexcept = 0;
        virtual FResult CreateSampler(const FGpuSamplerCreateOptions& options, FGpuSampler** out) noexcept = 0;
    };
}
