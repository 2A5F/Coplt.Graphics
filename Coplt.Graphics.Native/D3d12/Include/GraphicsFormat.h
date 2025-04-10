#pragma once

#include <directx/dxgiformat.h>

#include "../../Api/FFI/GraphicsFormat.h"

namespace Coplt
{
    inline DXGI_FORMAT ToDx(FGraphicsFormat value)
    {
        switch (value)
        {
        case FGraphicsFormat::Unknown:
        case FGraphicsFormat::R32G32B32A32_TypeLess:
        case FGraphicsFormat::R32G32B32A32_Float:
        case FGraphicsFormat::R32G32B32A32_UInt:
        case FGraphicsFormat::R32G32B32A32_SInt:
        case FGraphicsFormat::R32G32B32_TypeLess:
        case FGraphicsFormat::R32G32B32_Float:
        case FGraphicsFormat::R32G32B32_UInt:
        case FGraphicsFormat::R32G32B32_SInt:
        case FGraphicsFormat::R16G16B16A16_TypeLess:
        case FGraphicsFormat::R16G16B16A16_Float:
        case FGraphicsFormat::R16G16B16A16_UNorm:
        case FGraphicsFormat::R16G16B16A16_UInt:
        case FGraphicsFormat::R16G16B16A16_SNorm:
        case FGraphicsFormat::R16G16B16A16_SInt:
        case FGraphicsFormat::R32G32_TypeLess:
        case FGraphicsFormat::R32G32_Float:
        case FGraphicsFormat::R32G32_UInt:
        case FGraphicsFormat::R32G32_SInt:
        case FGraphicsFormat::R32G8X24_TypeLess:
        case FGraphicsFormat::D32_Float_S8X24_UInt:
        case FGraphicsFormat::R32_Float_X8X24_TypeLess:
        case FGraphicsFormat::X32_TypeLess_G8X24_Float:
        case FGraphicsFormat::R10G10B10A2_TypeLess:
        case FGraphicsFormat::R10G10B10A2_UNorm:
        case FGraphicsFormat::R10G10B10A2_UInt:
        case FGraphicsFormat::R11G11B10_Float:
        case FGraphicsFormat::R8G8B8A8_TypeLess:
        case FGraphicsFormat::R8G8B8A8_UNorm:
        case FGraphicsFormat::R8G8B8A8_UNorm_sRGB:
        case FGraphicsFormat::R8G8B8A8_UInt:
        case FGraphicsFormat::R8G8B8A8_SNorm:
        case FGraphicsFormat::R8G8B8A8_SInt:
        case FGraphicsFormat::R16G16_TypeLess:
        case FGraphicsFormat::R16G16_Float:
        case FGraphicsFormat::R16G16_UNorm:
        case FGraphicsFormat::R16G16_UInt:
        case FGraphicsFormat::R16G16_SNorm:
        case FGraphicsFormat::R16G16_SInt:
        case FGraphicsFormat::R32_TypeLess:
        case FGraphicsFormat::D32_Float:
        case FGraphicsFormat::R32_Float:
        case FGraphicsFormat::R32_UInt:
        case FGraphicsFormat::R32_SInt:
        case FGraphicsFormat::R24G8_TypeLess:
        case FGraphicsFormat::D24_UNorm_S8_UInt:
        case FGraphicsFormat::R24_UNorm_X8_TypeLess:
        case FGraphicsFormat::X24_TypeLess_G8_UInt:
        case FGraphicsFormat::R8G8_TypeLess:
        case FGraphicsFormat::R8G8_UNorm:
        case FGraphicsFormat::R8G8_UInt:
        case FGraphicsFormat::R8G8_SNorm:
        case FGraphicsFormat::R8G8_SInt:
        case FGraphicsFormat::R16_TypeLess:
        case FGraphicsFormat::R16_Float:
        case FGraphicsFormat::D16_UNorm:
        case FGraphicsFormat::R16_UNorm:
        case FGraphicsFormat::R16_UInt:
        case FGraphicsFormat::R16_SNorm:
        case FGraphicsFormat::R16_SInt:
        case FGraphicsFormat::R8_TypeLess:
        case FGraphicsFormat::R8_UNorm:
        case FGraphicsFormat::R8_UInt:
        case FGraphicsFormat::R8_SNorm:
        case FGraphicsFormat::R8_SInt:
        case FGraphicsFormat::A8_UNorm:
        case FGraphicsFormat::R1_UNorm:
        case FGraphicsFormat::R9G9B9E5_SharedExp:
        case FGraphicsFormat::R8G8_B8G8_UNorm:
        case FGraphicsFormat::G8R8_G8B8_UNorm:
        case FGraphicsFormat::BC1_TypeLess:
        case FGraphicsFormat::BC1_UNorm:
        case FGraphicsFormat::BC1_UNorm_sRGB:
        case FGraphicsFormat::BC2_TypeLess:
        case FGraphicsFormat::BC2_UNorm:
        case FGraphicsFormat::BC2_UNorm_sRGB:
        case FGraphicsFormat::BC3_TypeLess:
        case FGraphicsFormat::BC3_UNorm:
        case FGraphicsFormat::BC3_UNorm_sRGB:
        case FGraphicsFormat::BC4_TypeLess:
        case FGraphicsFormat::BC4_UNorm:
        case FGraphicsFormat::BC4_SNorm:
        case FGraphicsFormat::BC5_TypeLess:
        case FGraphicsFormat::BC5_UNorm:
        case FGraphicsFormat::BC5_SNorm:
        case FGraphicsFormat::B5G6R5_UNorm:
        case FGraphicsFormat::B5G5R5A1_UNorm:
        case FGraphicsFormat::B8G8R8A8_UNorm:
        case FGraphicsFormat::B8G8R8X8_UNorm:
        case FGraphicsFormat::R10G10B10_XR_Bias_A2_UNorm:
        case FGraphicsFormat::B8G8R8A8_TypeLess:
        case FGraphicsFormat::B8G8R8A8_UNorm_sRGB:
        case FGraphicsFormat::B8G8R8X8_TypeLess:
        case FGraphicsFormat::B8G8R8X8_UNorm_sRGB:
        case FGraphicsFormat::BC6H_TypeLess:
        case FGraphicsFormat::BC6H_UF16:
        case FGraphicsFormat::BC6H_SF16:
        case FGraphicsFormat::BC7_TypeLess:
        case FGraphicsFormat::BC7_UNorm:
        case FGraphicsFormat::BC7_UNorm_sRGB:
        case FGraphicsFormat::AYUV:
        case FGraphicsFormat::Y410:
        case FGraphicsFormat::Y416:
        case FGraphicsFormat::NV12:
        case FGraphicsFormat::P010:
        case FGraphicsFormat::P016:
        case FGraphicsFormat::Opaque_420:
        case FGraphicsFormat::YUY2:
        case FGraphicsFormat::Y210:
        case FGraphicsFormat::Y216:
        case FGraphicsFormat::NV11:
        case FGraphicsFormat::AI44:
        case FGraphicsFormat::IA44:
        case FGraphicsFormat::P8:
        case FGraphicsFormat::A8P8:
        case FGraphicsFormat::B4G4R4A4_UNorm:
        case FGraphicsFormat::P208:
        case FGraphicsFormat::V208:
        case FGraphicsFormat::V408:
        case FGraphicsFormat::A4B4G4R4_UNorm:
            return static_cast<DXGI_FORMAT>(value);

        case FGraphicsFormat::S8_UInt:
            return DXGI_FORMAT_R8_UINT;

        case FGraphicsFormat::Astc_4x4_UNorm:
        case FGraphicsFormat::Astc_4x4_sRGB:
        case FGraphicsFormat::Astc_5x4_UNorm:
        case FGraphicsFormat::Astc_5x4_sRGB:
        case FGraphicsFormat::Astc_5x5_UNorm:
        case FGraphicsFormat::Astc_5x5_sRGB:
        case FGraphicsFormat::Astc_6x5_UNorm:
        case FGraphicsFormat::Astc_6x5_sRGB:
        case FGraphicsFormat::Astc_6x6_UNorm:
        case FGraphicsFormat::Astc_6x6_sRGB:
        case FGraphicsFormat::Astc_8x5_UNorm:
        case FGraphicsFormat::Astc_8x5_sRGB:
        case FGraphicsFormat::Astc_8x6_UNorm:
        case FGraphicsFormat::Astc_8x6_sRGB:
        case FGraphicsFormat::Astc_8x8_UNorm:
        case FGraphicsFormat::Astc_8x8_sRGB:
        case FGraphicsFormat::Astc_10x5_UNorm:
        case FGraphicsFormat::Astc_10x5_sRGB:
        case FGraphicsFormat::Astc_10x6_UNorm:
        case FGraphicsFormat::Astc_10x6_sRGB:
        case FGraphicsFormat::Astc_10x8_UNorm:
        case FGraphicsFormat::Astc_10x8_sRGB:
        case FGraphicsFormat::Astc_10x10_UNorm:
        case FGraphicsFormat::Astc_10x10_sRGB:
        case FGraphicsFormat::Astc_12x10_UNorm:
        case FGraphicsFormat::Astc_12x10_sRGB:
        case FGraphicsFormat::Astc_12x12_UNorm:
        case FGraphicsFormat::Astc_12x12_sRGB:
        case FGraphicsFormat::Etc2_R8G8B8_UNorm:
        case FGraphicsFormat::Etc2_R8G8B8_sRGB:
        case FGraphicsFormat::Etc2_R8G8B8A1_UNorm:
        case FGraphicsFormat::Etc2_R8G8B8A1_sRGB:
        case FGraphicsFormat::Etc2_R8G8B8A8_UNorm:
        case FGraphicsFormat::Etc2_R8G8B8A8_sRGB:
        case FGraphicsFormat::Eac_R11_UNorm:
        case FGraphicsFormat::Eac_R11_SNorm:
        case FGraphicsFormat::Eac_R11G11_UNorm:
        case FGraphicsFormat::Eac_R11G11_SNorm:
            return DXGI_FORMAT_UNKNOWN;
        }

        return static_cast<DXGI_FORMAT>(value);
    }

    inline FGraphicsFormat FromDx(DXGI_FORMAT value)
    {
        return static_cast<FGraphicsFormat>(value);
    }

    inline DXGI_FORMAT GetBufferImageCopyFormat(FGraphicsFormat format, FImagePlane plane, bool IsDsOrYCbCr)
    {
        if (!IsDsOrYCbCr) return ToDx(format);
        if (plane == FImagePlane::Depth)
        {
            if (format == FGraphicsFormat::D16_UNorm)
                return DXGI_FORMAT_R16_UNORM;
            if (format == FGraphicsFormat::D32_Float || format == FGraphicsFormat::D32_Float_S8X24_UInt)
                return DXGI_FORMAT_R32_FLOAT;
            // todo yuv
        }
        if (plane == FImagePlane::Stencil)
        {
            if (format == FGraphicsFormat::S8_UInt)
                return DXGI_FORMAT_R8_UINT;
            if (format == FGraphicsFormat::D24_UNorm_S8_UInt || format == FGraphicsFormat::D32_Float_S8X24_UInt)
                return DXGI_FORMAT_R8_UINT;
            // todo yuv
        }
        return ToDx(format);
    }
}
