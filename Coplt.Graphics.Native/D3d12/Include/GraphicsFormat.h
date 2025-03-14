#pragma once

#include <directx/dxgiformat.h>

#include "../../Api/FFI/GraphicsFormat.h"

namespace Coplt
{
    inline DXGI_FORMAT ToDx(FGraphicsFormat value)
    {
        return static_cast<DXGI_FORMAT>(value);
    }

    inline FGraphicsFormat FromDx(DXGI_FORMAT value)
    {
        return static_cast<FGraphicsFormat>(value);
    }

    inline bool IsDepthStencil(const FGraphicsFormat value)
    {
        switch (value)
        {
        case FGraphicsFormat::D32_Float_S8X24_UInt:
        case FGraphicsFormat::D32_Float:
        case FGraphicsFormat::D24_UNorm_S8_UInt:
        case FGraphicsFormat::D16_UNorm:
            return true;
        default:
            return false;
        }
    }
}
