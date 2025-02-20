#pragma once

#include "../../Api/Include/Object.h"
#include "../FFI/Resource.h"
#include "Device.h"

namespace Coplt
{
    struct ResourcePack final
    {
        ComPtr<D3D12MA::Allocation> m_allocation{};
        ComPtr<ID3D12Resource> m_resource{};

        ResourcePack() = default;

        explicit ResourcePack(
            D3D12MA::Allocator* allocator, FCpuAccess cpu_access, FResourceState& state,
            const D3D12_RESOURCE_DESC* desc, const D3D12_CLEAR_VALUE* clear_value
        );
    };

    struct BufferPack final
    {
        ResourcePack m_resource{};
        u64 m_size{};
        FResourceState m_state{};

        BufferPack() = default;

        explicit BufferPack(
            D3D12MA::Allocator* allocator, FCpuAccess cpu_access, u64 size
        );
    };
}
