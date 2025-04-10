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

    COPLT_INTERFACE_DEFINE(ID3d12GraphicsShaderPipeline, "345aeb23-3dc9-4365-babc-dfc7badf4ff8", FGraphicsShaderPipeline)
    {
    };
}
