#pragma once

#include <directx/d3d12.h>

#include "Device.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Layout.h"
#include "../Include/Utils.h"
#include "../FFI/Pipeline.h"

namespace Coplt
{
    struct D3d12GraphicsShaderPipeline final : Object<D3d12GraphicsShaderPipeline, FD3d12GraphicsShaderPipeline>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<FShader> m_shader{};
        Rc<FD3d12ShaderLayout> m_layout{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12PipelineState> m_pipeline{};

        explicit D3d12GraphicsShaderPipeline(
            Rc<D3d12GpuDevice>&& device, const FGraphicsShaderPipelineCreateOptions& options
        );

        FResult SetName(const Str8or16& name) noexcept override;
    };
}
