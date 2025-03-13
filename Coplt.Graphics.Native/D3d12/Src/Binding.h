#pragma once

#include <ranges>

#include "Device.h"
#include "Layout.h"
#include "../../Api/Include/Object.h"
#include "../../Api/Include/Ptr.h"
#include "../FFI/Binding.h"
#include "../FFI/Layout.h"
#include "../Include/View.h"

namespace Coplt
{
    struct DescriptorHeap;
    struct D3d12GpuQueue;
    struct DescriptorAllocation;

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBinding, "84f2b3e2-bb16-4276-ba3f-5da54eda462d", FD3d12ShaderBinding)
    {
        virtual const Rc<ID3d12ShaderLayout>& Layout() noexcept = 0;
        virtual std::span<View> Views() noexcept = 0;
        virtual bool Changed() noexcept = 0;
        virtual const HashSet<u64>& ChangedGroups() noexcept = 0;
        virtual std::span<const std::vector<Rc<DescriptorHeap>>> DescHeaps() noexcept = 0;
        // virtual std::span<std::vector<DescriptorAllocation>> Allocations() noexcept = 0;

        virtual void Set(std::span<const FBindItem> bindings) = 0;

        // 更新更改过的表，但是不会消除更新
        virtual void Update(NonNull<D3d12GpuQueue> queue) = 0;
        // 实际消除更新
        virtual void ApplyChange() = 0;

        auto IterChangedGroups() noexcept
        {
            return ChangedGroups() | std::ranges::views::transform([](const u64 cg)
            {
                return std::pair{static_cast<u32>(cg >> 32), static_cast<u32>(cg & 0xFFFFFFFF)};
            });
        }
    };

    struct D3d12ShaderBinding final : Object<D3d12ShaderBinding, ID3d12ShaderBinding>
    {
        u64 m_object_id{};
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        Rc<ID3d12ShaderLayout> m_layout{};
        std::vector<View> m_views{};
        std::vector<std::vector<std::vector<u32>>> m_item_indexes{};
        std::vector<std::vector<Rc<DescriptorHeap>>> m_desc_heaps{};
        // std::vector<std::vector<DescriptorAllocation>> m_allocations{};
        HashSet<u64> m_changed_groups{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);

        u64 ObjectId() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;

        void Set(std::span<const FBindItem> bindings) override;

        const Rc<ID3d12ShaderLayout>& Layout() noexcept override;
        std::span<View> Views() noexcept override;
        bool Changed() noexcept override;
        const HashSet<u64>& ChangedGroups() noexcept override;
        std::span<const std::vector<Rc<DescriptorHeap>>> DescHeaps() noexcept override;
        // std::span<std::vector<DescriptorAllocation>> Allocations() noexcept override;

        void Update(NonNull<D3d12GpuQueue> queue) override;
        void ApplyChange() override;
    };
}
