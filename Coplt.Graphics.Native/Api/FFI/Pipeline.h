#pragma once

#include "Shader.h"
#include "PipelineState.h"
#include "Layout.h"

namespace Coplt
{
    struct FShaderPipelineCreateOptions
    {
        Str8or16 Name{};
        FShader* Shader{};
    };

    COPLT_INTERFACE_DEFINE(FShaderPipeline, "356a2610-34e3-4c01-9904-22e3c5ce2f4e", FGpuObject)
    {
        virtual FShader* GetShader() noexcept = 0;
        virtual FShaderLayout* GetLayout() noexcept = 0;
        virtual FShaderStageFlags GetStages() noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(FComputeShaderPipeline, "5241c089-2ee2-43ee-add4-0a10c04a56ce", FShaderPipeline)
    {
    };

    struct FGraphicsShaderPipelineCreateOptions : FShaderPipelineCreateOptions
    {
        // 可选
        FMeshLayout* MeshLayout{};
        FGraphicsPipelineState GraphicsState{};
    };

    COPLT_INTERFACE_DEFINE(FGraphicsShaderPipeline, "32a67d44-132c-449b-972d-bad3413783e5", FShaderPipeline)
    {
        virtual const FGraphicsPipelineState* GetGraphicsState() noexcept = 0;
        // 可空
        virtual const FShaderInputLayout* GetInputLayout() noexcept = 0;
        // 可空
        virtual const FMeshLayout* GetMeshLayout() noexcept = 0;
    };
}
