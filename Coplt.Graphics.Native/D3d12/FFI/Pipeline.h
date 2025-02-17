#pragma once

#include "../../Api/FFI/Pipeline.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FD3d12PipelineState, "6d9c7db7-3261-4d79-bd21-1f1bb5e01e4d", FShaderPipeline)
    {
        // 返回 ID3D12PipelineState*
        virtual void* GetPipelineStatePtr() noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(
        FD3d12GraphicsShaderPipeline, "a1c6b8a9-1e4c-4e6c-85ea-2e64ac89afc6", FGraphicsShaderPipeline
    )
    {
    };
}
