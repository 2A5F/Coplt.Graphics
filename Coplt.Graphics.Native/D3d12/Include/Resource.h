#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/Resource.h"

namespace Coplt
{
    inline D3D12_TEXTURE_LAYOUT ToDx(const FImageLayout value)
    {
        switch (value)
        {
        case FImageLayout::Undefined:
            return D3D12_TEXTURE_LAYOUT_UNKNOWN;
        case FImageLayout::RowMajor:
            return D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        case FImageLayout::Undefined64kSwizzle:
            return D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;
        case FImageLayout::Standard64kSwizzle:
            return D3D12_TEXTURE_LAYOUT_64KB_STANDARD_SWIZZLE;
        default:
            return D3D12_TEXTURE_LAYOUT_UNKNOWN;
        }
    }
}
