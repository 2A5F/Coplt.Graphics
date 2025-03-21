#pragma once

#include <directx/d3d12.h>
#include <directx/d3dx12.h>

#include "../../Api/Include/Object.h"
#include "../FFI/Resource.h"
#include "Device.h"
#include "Resource.h"
#include "../../Api/Include/GpuObject.h"
#include "../../Api/Include/Ptr.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuBuffer, "6edf3bc5-dfc7-461a-ab6b-0e5f5a9d71e7", FD3d12GpuBuffer)
    {
        virtual NonNull<FGpuBufferData> Data() = 0;
        virtual NonNull<ID3D12Resource> GetResourcePtr() = 0;
    };

    struct D3d12GpuBuffer final : GpuObject<D3d12GpuBuffer, ID3d12GpuBuffer>, FGpuBufferData
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<FString> m_name{};
        ComPtr<D3D12MA::Allocator> m_allocator{};
        Box<ResourcePack> m_resource{};
        CD3DX12_RESOURCE_DESC1 m_desc{};

        explicit D3d12GpuBuffer(Rc<D3d12GpuDevice>&& device, const FGpuBufferCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        ResourceType GetResourceType() noexcept override;
        FGpuViewableData* GpuViewableData() noexcept override;
        FGpuResourceData* GpuResourceData() noexcept override;
        FGpuBufferData* GpuBufferData() noexcept override;
        NonNull<FGpuBufferData> Data() override;

        void* GetRawResourcePtr() noexcept override;
        NonNull<ID3D12Resource> GetResourcePtr() override;

        FResult Map(void** ptr, b8 Discard) noexcept override;
        FResult Unmap(b8 Discard) noexcept override;
    };
}
