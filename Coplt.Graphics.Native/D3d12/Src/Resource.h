#pragma once

#include "../FFI/Resource.h"
#include "../Include/ResState.h"
#include "Device.h"

namespace Coplt
{
    struct ResourcePack final
    {
        ComPtr<D3D12MA::Allocation> m_allocation{};
        ComPtr<ID3D12Resource> m_resource{};

        ResourcePack() = default;

        explicit ResourcePack(
            D3d12GpuDevice* device, D3D12MA::Allocator* allocator, FCpuAccess cpu_access, ResLayout& layout,
            const D3D12_RESOURCE_DESC1* desc, const D3D12_CLEAR_VALUE* clear_value, bool image
        );
    };

    struct BufferPack final
    {
        ResourcePack m_resource{};
        u64 m_size{};

        BufferPack() = default;

        explicit BufferPack(
            D3d12GpuDevice* device, D3D12MA::Allocator* allocator, FCpuAccess cpu_access,
            u64 size, D3D12_RESOURCE_FLAGS Flags = D3D12_RESOURCE_FLAG_NONE
        );
    };
}
