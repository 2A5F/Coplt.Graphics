#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/Sampler.h"

namespace Coplt
{
    inline D3D12_TEXTURE_ADDRESS_MODE ToDx(const FAddressMode value)
    {
        switch (value)
        {
        case FAddressMode::Repeat:
            return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        case FAddressMode::Mirror:
            return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
        case FAddressMode::Clamp:
            return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        case FAddressMode::Border:
            return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        case FAddressMode::MirrorOnce:
            return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
        }
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }

    inline D3D12_FILTER operator |(const D3D12_FILTER a, const D3D12_FILTER b)
    {
        return static_cast<D3D12_FILTER>(static_cast<u32>(a) | static_cast<u32>(b));
    }

    inline D3D12_FILTER& operator |=(D3D12_FILTER& a, const D3D12_FILTER b)
    {
        a = a | b;
        return a;
    }
}
