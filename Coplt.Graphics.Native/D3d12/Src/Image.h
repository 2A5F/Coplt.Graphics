#pragma once

#include <directx/d3d12.h>
#include <directx/d3dx12.h>

#include "../../Api/FFI/Resource.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Resource.h"
#include "Device.h"
#include "Resource.h"

namespace Coplt
{
    struct D3d12GpuImage final : Object<D3d12GpuImage, FD3d12FGpuImage>
    {
        u64 m_object_id{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<FString> m_name{};
        ComPtr<D3D12MA::Allocator> m_allocator{};
        Box<ResourcePack> m_resource{};
        CD3DX12_RESOURCE_DESC1 m_desc{};

        explicit D3d12GpuImage(Rc<D3d12GpuDevice>&& device, const FGpuImageCreateOptions& options);

        u64 ObjectId() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;

        FResult GetCurrentResourcePtr(void* out) const noexcept override;
    };
}
