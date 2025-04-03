#pragma once

#include <directx/d3d12.h>
#include "D3D12MemAlloc.h"

#include "Instance.h"
#include "../../Api/Include/GpuObject.h"
#include "../../Api/Include/HashMap.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Device.h"
#include "DescriptorManager.h"

namespace Coplt
{
    struct D3d12GpuAdapter;
    struct D3d12ShaderLayout;
    struct D3d12BindGroupLayout;
    struct D3d12BindingLayout;
    struct D3d12MeshLayout;

    struct D3d12GpuDevice final : GpuObject<D3d12GpuDevice, FD3d12GpuDevice>
    {
        using EmptyShaderLayouts = HashMap<FShaderLayoutFlags, Rc<D3d12ShaderLayout>>;
        using EmptyBindingLayouts = HashMap<FShaderLayoutFlags, Rc<D3d12BindingLayout>>;

        Rc<D3d12Instance> m_instance{};
        Rc<D3d12GpuAdapter> m_adapter{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12Device> m_device0{};
        ComPtr<ID3D12InfoQueue1> m_info_queue{};
        ComPtr<D3D12MA::Allocator> m_gpu_allocator{};
        ComPtr<D3D12MA::Pool> m_uma_pool{};
        Box<EmptyShaderLayouts> m_empty_layouts{};
        Rc<D3d12BindGroupLayout> m_empty_bind_group_layout{};
        Box<EmptyBindingLayouts> m_empty_binding_layout{};
        Rc<D3d12MeshLayout> m_empty_mesh_layout{};
        DWORD m_callback_cookie{};

        explicit D3d12GpuDevice(Rc<D3d12GpuAdapter>&& adapter, const FGpuDeviceCreateOptions& options, FGpuDeviceCreateResult& out);

        ~D3d12GpuDevice() override;

        const Logger& Logger() const noexcept;

        bool Debug() const noexcept;

        FResult SetName(const FStr8or16& name) noexcept override;

        FGpuAdapter* GetAdapter() noexcept override;

        void* GetRawDevice() noexcept override;

        const Rc<D3d12ShaderLayout>& GetEmptyShaderLayout(FShaderLayoutFlags flags);
        const Rc<D3d12BindGroupLayout>& GetEmptyBindGroupLayout();
        const Rc<D3d12MeshLayout>& GetEmptyMeshLayout();
        const Rc<D3d12BindingLayout>& GetEmptyBindingLayout(FShaderLayoutFlags flags);

        FResult CreateIsolate(const FGpuIsolateCreateOptions& options, FGpuIsolateCreateResult& out) noexcept override;
        FGpuIsolateCreateResult CreateIsolate(const FGpuIsolateCreateOptions& options);

        FResult CreateShaderLayout(const FShaderLayoutCreateOptions& options, FShaderLayoutCreateResult* out) noexcept override;
        FResult CreateBindGroupLayout(const FBindGroupLayoutCreateOptions& options, FBindGroupLayoutCreateResult* out) noexcept override;
        FResult CreateBindingLayout(const FBindingLayoutCreateOptions& options, FBindingLayout** out) noexcept override;
        FResult GetEmptyShaderLayout(const FGetEmptyShaderLayoutOptions& options, FShaderLayoutCreateResult* out) noexcept override;
        FResult GetEmptyBindGroupLayout(FBindGroupLayoutCreateResult* out) noexcept override;
        FResult GetEmptyBindingLayout(const FGetEmptyBindingLayoutOptions& options, FBindingLayout** out) noexcept override;
        FResult CreateShaderInputLayout(const FShaderInputLayoutCreateOptions& options, FShaderInputLayout** out) noexcept override;
        FResult CreateMeshLayout(const FMeshLayoutCreateOptions& options, FMeshLayout** out) noexcept override;

        FResult CreateShaderModule(const FShaderModuleCreateOptions& options, FShaderModuleCreateResult* out) noexcept override;
        FResult CreateShader(const FShaderCreateOptions& options, FShaderCreateResult* out) noexcept override;

        FResult CreateBindGroup(const FShaderBindGroupCreateOptions& options, FShaderBindGroup** out) noexcept override;
        FResult CreateBinding(const FShaderBindingCreateOptions& options, FShaderBinding** out) noexcept override;

        FResult CreateGraphicsPipeline(const FGraphicsShaderPipelineCreateOptions& options, FGraphicsShaderPipeline** out) noexcept override;

        FResult CreateBuffer(const FGpuBufferCreateOptions& options, FGpuBuffer** out) noexcept override;
        FResult CreateImage(const FGpuImageCreateOptions& options, FGpuImage** out) noexcept override;
        FResult CreateSampler(const FGpuSamplerCreateOptions& options, FGpuSampler** out) noexcept override;
    };
}
