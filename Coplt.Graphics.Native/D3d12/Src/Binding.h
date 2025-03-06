#pragma once

#include "Device.h"
#include "Layout.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Binding.h"
#include "../FFI/Layout.h"
#include "../Include/View.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12ShaderBinding, "84f2b3e2-bb16-4276-ba3f-5da54eda462d", FD3d12ShaderBinding)
    {
        virtual const Rc<ID3d12ShaderLayout>& Layout() noexcept = 0;
        virtual std::span<View> Views() noexcept = 0;
        virtual bool Changed() noexcept = 0;
        virtual const HashSet<u32>& ChangedItems() noexcept = 0;
        virtual const HashSet<u32>& ChangedGroups() noexcept = 0;

        virtual void Set(std::span<const FBindItem> bindings) = 0;

        virtual void ApplyChange() = 0;
    };

    struct D3d12ShaderBinding final : Object<D3d12ShaderBinding, ID3d12ShaderBinding>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        Rc<ID3d12ShaderLayout> m_layout{};
        std::vector<View> m_views{};
        HashSet<u32> m_changed_items{};
        HashSet<u32> m_changed_groups{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        void Set(std::span<const FBindItem> bindings) override;

        const Rc<ID3d12ShaderLayout>& Layout() noexcept override;
        std::span<View> Views() noexcept override;
        bool Changed() noexcept override;
        const HashSet<u32>& ChangedItems() noexcept override;
        const HashSet<u32>& ChangedGroups() noexcept override;

        void ApplyChange() override;
    };
}
