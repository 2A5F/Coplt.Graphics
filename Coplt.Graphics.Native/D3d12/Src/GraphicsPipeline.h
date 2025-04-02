#pragma once

#include <directx/d3d12.h>

#include "Device.h"
#include "Layout.h"
#include "../../Api/Include/Object.h"
#include "../Include/Utils.h"
#include "../FFI/Pipeline.h"

namespace Coplt
{
    struct D3d12GraphicsShaderPipeline final : GpuObject<D3d12GraphicsShaderPipeline, FD3d12GraphicsShaderPipeline, FD3d12PipelineState>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<FShader> m_shader{};
        Rc<ID3d12BindingLayout> m_layout{};
        // 可选
        Rc<FShaderInputLayout> m_input_layout{};
        // 可选
        Rc<FMeshLayout> m_mesh_layout{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12PipelineState> m_pipeline{};
        std::vector<u32> m_input_slots{};
        FGraphicsPipelineState m_graphics_state{};

        explicit D3d12GraphicsShaderPipeline(
            Rc<D3d12GpuDevice>&& device, const FGraphicsShaderPipelineCreateOptions& options
        );

        FResult SetName(const FStr8or16& name) noexcept override;

        void* GetPipelineStatePtr() noexcept override;

        FShader* GetShader() noexcept override;
        FBindingLayout* GetLayout() noexcept override;
        FShaderStageFlags GetStages() noexcept override;

        const FGraphicsPipelineState* GetGraphicsState() noexcept override;
        const FShaderInputLayout* GetInputLayout() noexcept override;
        const FMeshLayout* GetMeshLayout() noexcept override;
    };
}
