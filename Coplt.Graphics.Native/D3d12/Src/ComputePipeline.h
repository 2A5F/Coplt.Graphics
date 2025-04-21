#pragma once

#include <directx/d3d12.h>

#include "../../Api/Include/Object.h"
#include "Device.h"
#include "Layout.h"
#include "Pipeline.h"

namespace Coplt
{
    struct D3d12ComputeShaderPipeline final : GpuObject<D3d12ComputeShaderPipeline, ID3d12ComputeShaderPipeline, ID3d12ShaderPipeline>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<FShader> m_shader{};
        Rc<ID3d12BindingLayout> m_layout{};
        ComPtr<ID3D12PipelineState> m_pipeline{};

        explicit D3d12ComputeShaderPipeline(Rc<D3d12GpuDevice>&& device, const FShaderPipelineCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        const Rc<ID3d12BindingLayout>& Layout() const noexcept override;
        const ComPtr<ID3D12PipelineState>& GetPipelineState() const noexcept override;

        FShader* GetShader() noexcept override;
        FBindingLayout* GetLayout() noexcept override;
        FShaderStageFlags GetStages() noexcept override;
    };
}
