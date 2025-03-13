#pragma once

#include <directx/d3d12.h>

#include "Device.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Layout.h"
#include "../Include/Utils.h"
#include "../FFI/Pipeline.h"

namespace Coplt
{
    struct D3d12GraphicsShaderPipeline final : Object<D3d12GraphicsShaderPipeline, FD3d12GraphicsShaderPipeline, FD3d12PipelineState>
    {
        u64 m_object_id{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<FShader> m_shader{};
        Rc<FD3d12ShaderLayout> m_layout{};
        // 可选
        Rc<FD3d12ShaderInputLayout> m_input_layout{};
        // 可选
        Rc<FD3d12MeshLayout> m_mesh_layout{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12PipelineState> m_pipeline{};
        std::vector<u32> m_input_slots{};
        FGraphicsPipelineState m_graphics_state{};

        explicit D3d12GraphicsShaderPipeline(
            Rc<D3d12GpuDevice>&& device, const FGraphicsShaderPipelineCreateOptions& options
        );

        u64 ObjectId() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;

        void* GetPipelineStatePtr() noexcept override;

        FShader* GetShader() noexcept override;
        FShaderLayout* GetLayout() noexcept override;
        FShaderStageFlags GetStages() noexcept override;

        const FGraphicsPipelineState* GetGraphicsState() noexcept override;
        const FShaderInputLayout* GetInputLayout() noexcept override;
        const FMeshLayout* GetMeshLayout() noexcept override;
    };
}
