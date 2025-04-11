#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/Pipeline.h"
#include "../Include/Utils.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12ShaderPipeline, "c35a7e1e-278a-4655-bcab-eb3f7dff0124", FShaderPipeline)
    {
        virtual const Rc<ID3d12BindingLayout>& Layout() const noexcept = 0;
        virtual const ComPtr<ID3D12PipelineState>& GetPipelineState() const noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(ID3d12ComputeShaderPipeline, "85989313-9f53-4d01-8ea9-5a2d3decd508", FComputeShaderPipeline)
    {
    };

    COPLT_INTERFACE_DEFINE(ID3d12GraphicsShaderPipeline, "b27e1754-365f-4f44-ac9d-50a44b877226", FGraphicsShaderPipeline)
    {
    };
}
