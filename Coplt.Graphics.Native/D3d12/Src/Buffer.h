#pragma once
#include "../../Api/Include/Object.h"
#include "../FFI/Resource.h"
#include "Device.h"
#include "Resource.h"

namespace Coplt
{
    struct D3d12GpuBuffer final : Object<D3d12GpuBuffer, FD3d12GpuBuffer>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<FString> m_name{};
        ComPtr<D3D12MA::Allocator> m_allocator{};
        Box<ResourcePack> m_resource{};
        D3D12_RESOURCE_DESC m_desc{};

        explicit D3d12GpuBuffer(Rc<D3d12GpuDevice>&& device, const FGpuBufferCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        FResult GetCurrentResourcePtr(void* out) const noexcept override;
    };
}
