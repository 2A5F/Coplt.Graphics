#pragma once

#include <ranges>

#include "Device.h"
#include "Layout.h"
#include "../../Api/Include/Object.h"
#include "../../Api/Include/Ptr.h"
#include "../FFI/Binding.h"
#include "../Include/View.h"

namespace Coplt
{
    struct DescriptorHeap;
    struct D3d12GpuQueue;
    struct DescriptorAllocation;

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBinding, "84f2b3e2-bb16-4276-ba3f-5da54eda462d", FD3d12ShaderBinding)
    {

    };

    struct D3d12ShaderBinding final : GpuObject<D3d12ShaderBinding, ID3d12ShaderBinding>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;
    };
}
