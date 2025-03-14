#pragma once

#include <directx/d3d12.h>
#include <directx/d3dx12.h>

#include "../../Api/FFI/Resource.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Resource.h"
#include "Device.h"
#include "Resource.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    struct D3d12GpuImage final : GpuObject<D3d12GpuImage, FD3d12GpuImage>, FGpuImageData
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<FString> m_name{};
        ComPtr<D3D12MA::Allocator> m_allocator{};
        Box<ResourcePack> m_resource{};
        CD3DX12_RESOURCE_DESC1 m_desc{};

        explicit D3d12GpuImage(Rc<D3d12GpuDevice>&& device, const FGpuImageCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;
        ResourceType GetResourceType() noexcept override;
        FGpuViewableData* GpuViewableData() noexcept override;
        FGpuResourceData* GpuResourceData() noexcept override;
        FGpuImageData* GpuImageData() noexcept override;

        FResult GetCurrentResourcePtr(void* out) const noexcept override;
    };
}
