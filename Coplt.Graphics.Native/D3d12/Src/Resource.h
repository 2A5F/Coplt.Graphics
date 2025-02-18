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
            D3D12MA::Allocator* allocator, FCpuAccess cpu_access,
            const D3D12_RESOURCE_DESC* desc, const D3D12_CLEAR_VALUE* clear_value
        );
    };
}
