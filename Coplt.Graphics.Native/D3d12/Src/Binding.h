#pragma once

#include <ranges>

#include "../../Api/FFI/Binding.h"
#include "../../Api/Include/Object.h"
#include "../../Api/Include/Ptr.h"
#include "../Include/View.h"
#include "Device.h"
#include "Layout.h"

namespace Coplt
{
    struct DescriptorHeap;
    struct D3d12GpuQueue;
    struct DescriptorAllocation;

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBindGroup, "5bb948b9-ad31-4eb8-b881-98017e048259", FShaderBindGroup)
    {
        virtual const Rc<FBindGroupLayout>& Layout() const noexcept = 0;
    };

    struct D3d12ShaderBindGroup final : GpuObject<D3d12ShaderBindGroup, ID3d12ShaderBindGroup>
    {
        Rc<FString> m_name{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<FBindGroupLayout> m_layout{};
        std::vector<View> m_views{};
        std::vector<u32> m_item_indexes{};

        explicit D3d12ShaderBindGroup(Rc<D3d12GpuDevice>&& device, const FShaderBindGroupCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;

        const Rc<FBindGroupLayout>& Layout() const noexcept override;

        void Set(std::span<const FSetBindItem> items);
    };

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBinding, "5073f785-cfb7-414d-9a84-1602d3bf378d", FShaderBinding)
    {
        virtual const Rc<ID3d12BindingLayout>& Layout() const noexcept = 0;
    };

    struct D3d12ShaderBinding final : GpuObject<D3d12ShaderBinding, ID3d12ShaderBinding>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<ID3d12BindingLayout> m_layout{};
        std::vector<Rc<ID3d12ShaderBindGroup>> m_groups{};
        HashSet<u32> m_changed_groups{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;

        const Rc<ID3d12BindingLayout>& Layout() const noexcept override;

        void Set(std::span<FSetBindGroupItem> items);
    };
}
