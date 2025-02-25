#pragma once

#include <directx/d3d12.h>
#include "D3D12MemAlloc.h"
#include "Instance.h"
#include "../../Api/Include/HashMap.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Device.h"

namespace Coplt
{
    struct DescriptorManager;
    struct D3d12ShaderLayout;
    struct D3d12MeshLayout;

    struct D3d12GpuDevice final : Object<D3d12GpuDevice, FD3d12GpuDevice>
    {
        using EmptyLayouts = HashMap<FShaderLayoutFlags, Rc<D3d12ShaderLayout>>;

        Rc<D3d12Instance> m_instance{};
        ComPtr<ID3D12Debug> m_debug_controller{};
        ComPtr<IDXGIFactory4> m_factory{};
        ComPtr<IDXGIAdapter1> m_adapter{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12Device> m_device0{};
        ComPtr<ID3D12InfoQueue1> m_info_queue{};
        ComPtr<D3D12MA::Allocator> m_gpu_allocator{};
        Box<DescriptorManager> m_descriptor_manager{};
        Box<EmptyLayouts> m_empty_layouts{};
        Rc<D3d12MeshLayout> m_empty_mesh_layout{};
        DWORD m_callback_cookie{};

        explicit D3d12GpuDevice(
            Rc<D3d12Instance> instance,
            const FGpuDeviceCreateOptions& options
        );

        ~D3d12GpuDevice() override;

        const Logger& Logger() const noexcept
        {
            return m_instance->Logger();
        }

        bool Debug() const noexcept
        {
            return m_debug_controller != nullptr;
        }

        FResult SetName(const FStr8or16& name) noexcept override;

        void* GetRawDevice() noexcept override;

        const Rc<D3d12ShaderLayout>& GetEmptyLayout(FShaderLayoutFlags flags);
        const Rc<D3d12MeshLayout>& GetEmptyMeshLayout();

        FResult CreateMainQueue(const FMainQueueCreateOptions& options, FGpuQueue** out) noexcept override;

        FResult CreateShaderModule(const FShaderModuleCreateOptions& options, FShaderModule** out) noexcept override;
        FResult CreateShaderLayout(const FShaderLayoutCreateOptions& options, FShaderLayout** out) noexcept override;
        FResult GetEmptyShaderLayout(
            const FGetEmptyShaderLayoutOptions& options, FShaderLayout** out
        ) noexcept override;
        FResult CreateShaderInputLayout(
            const FShaderInputLayoutCreateOptions& options, FShaderInputLayout** out
        ) noexcept override;
        FResult CreateShader(const FShaderCreateOptions& options, FShader** out) noexcept override;
        FResult CreateShaderBinding(const FShaderBindingCreateOptions& options, FShaderBinding** out) noexcept override;

        FResult CreateMeshLayout(const FMeshLayoutCreateOptions& options, FMeshLayout** out) noexcept override;

        FResult CreateGraphicsPipeline(
            const FGraphicsShaderPipelineCreateOptions& options, FGraphicsShaderPipeline** out
        ) noexcept override;

        FResult CreateBuffer(const FGpuBufferCreateOptions& options, FGpuBuffer** out) noexcept override;
    };
}
