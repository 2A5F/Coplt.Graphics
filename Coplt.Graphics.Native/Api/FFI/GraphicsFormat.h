#pragma once

#ifdef FFI_SRC
#include "../Include/Option.h"
#endif

namespace Coplt
{
    enum class FImagePlane : u8
    {
        Color = 0,
        Depth = 0,
        Stencil = 1,
        Y = 0,
        CbCr = 1,
    };

    enum class FGraphicsFormat
    {
        Unknown = 0,
        R32G32B32A32_TypeLess = 1,
        R32G32B32A32_Float = 2,
        R32G32B32A32_UInt = 3,
        R32G32B32A32_SInt = 4,
        R32G32B32_TypeLess = 5,
        R32G32B32_Float = 6,
        R32G32B32_UInt = 7,
        R32G32B32_SInt = 8,
        R16G16B16A16_TypeLess = 9,
        R16G16B16A16_Float = 10,
        R16G16B16A16_UNorm = 11,
        R16G16B16A16_UInt = 12,
        R16G16B16A16_SNorm = 13,
        R16G16B16A16_SInt = 14,
        R32G32_TypeLess = 15,
        R32G32_Float = 16,
        R32G32_UInt = 17,
        R32G32_SInt = 18,
        R32G8X24_TypeLess = 19,
        D32_Float_S8X24_UInt = 20,
        R32_Float_X8X24_TypeLess = 21,
        X32_TypeLess_G8X24_Float = 22,
        R10G10B10A2_TypeLess = 23,
        R10G10B10A2_UNorm = 24,
        R10G10B10A2_UInt = 25,
        R11G11B10_Float = 26,
        R8G8B8A8_TypeLess = 27,
        R8G8B8A8_UNorm = 28,
        R8G8B8A8_UNorm_sRGB = 29,
        R8G8B8A8_UInt = 30,
        R8G8B8A8_SNorm = 31,
        R8G8B8A8_SInt = 32,
        R16G16_TypeLess = 33,
        R16G16_Float = 34,
        R16G16_UNorm = 35,
        R16G16_UInt = 36,
        R16G16_SNorm = 37,
        R16G16_SInt = 38,
        R32_TypeLess = 39,
        D32_Float = 40,
        R32_Float = 41,
        R32_UInt = 42,
        R32_SInt = 43,
        R24G8_TypeLess = 44,
        D24_UNorm_S8_UInt = 45,
        R24_UNorm_X8_TypeLess = 46,
        X24_TypeLess_G8_UInt = 47,
        R8G8_TypeLess = 48,
        R8G8_UNorm = 49,
        R8G8_UInt = 50,
        R8G8_SNorm = 51,
        R8G8_SInt = 52,
        R16_TypeLess = 53,
        R16_Float = 54,
        D16_UNorm = 55,
        R16_UNorm = 56,
        R16_UInt = 57,
        R16_SNorm = 58,
        R16_SInt = 59,
        R8_TypeLess = 60,
        R8_UNorm = 61,
        R8_UInt = 62,
        R8_SNorm = 63,
        R8_SInt = 64,
        A8_UNorm = 65,
        R1_UNorm = 66,
        R9G9B9E5_SharedExp = 67,
        R8G8_B8G8_UNorm = 68,
        G8R8_G8B8_UNorm = 69,
        BC1_TypeLess = 70,
        BC1_UNorm = 71,
        BC1_UNorm_sRGB = 72,
        BC2_TypeLess = 73,
        BC2_UNorm = 74,
        BC2_UNorm_sRGB = 75,
        BC3_TypeLess = 76,
        BC3_UNorm = 77,
        BC3_UNorm_sRGB = 78,
        BC4_TypeLess = 79,
        BC4_UNorm = 80,
        BC4_SNorm = 81,
        BC5_TypeLess = 82,
        BC5_UNorm = 83,
        BC5_SNorm = 84,
        B5G6R5_UNorm = 85,
        B5G5R5A1_UNorm = 86,
        B8G8R8A8_UNorm = 87,
        B8G8R8X8_UNorm = 88,
        R10G10B10_XR_Bias_A2_UNorm = 89,
        B8G8R8A8_TypeLess = 90,
        B8G8R8A8_UNorm_sRGB = 91,
        B8G8R8X8_TypeLess = 92,
        B8G8R8X8_UNorm_sRGB = 93,
        BC6H_TypeLess = 94,
        BC6H_UF16 = 95,
        BC6H_SF16 = 96,
        BC7_TypeLess = 97,
        BC7_UNorm = 98,
        BC7_UNorm_sRGB = 99,
        AYUV = 100,
        Y410 = 101,
        Y416 = 102,
        NV12 = 103,
        P010 = 104,
        P016 = 105,
        Opaque_420 = 106,
        YUY2 = 107,
        Y210 = 108,
        Y216 = 109,
        NV11 = 110,
        AI44 = 111,
        IA44 = 112,
        P8 = 113,
        A8P8 = 114,
        B4G4R4A4_UNorm = 115,
        P208 = 130,
        V208 = 131,
        V408 = 132,
        A4B4G4R4_UNorm = 191,

        S8_UInt = 127,

        Etc2_R8G8B8_UNorm = 147,
        Etc2_R8G8B8_sRGB = 148,
        Etc2_R8G8B8A1_UNorm = 149,
        Etc2_R8G8B8A1_sRGB = 150,
        Etc2_R8G8B8A8_UNorm = 151,
        Etc2_R8G8B8A8_sRGB = 152,
        Eac_R11_UNorm = 153,
        Eac_R11_SNorm = 154,
        Eac_R11G11_UNorm = 155,
        Eac_R11G11_SNorm = 156,

        Astc_4x4_UNorm = 157,
        Astc_4x4_sRGB = 158,
        Astc_5x4_UNorm = 159,
        Astc_5x4_sRGB = 160,
        Astc_5x5_UNorm = 161,
        Astc_5x5_sRGB = 162,
        Astc_6x5_UNorm = 163,
        Astc_6x5_sRGB = 164,
        Astc_6x6_UNorm = 165,
        Astc_6x6_sRGB = 166,
        Astc_8x5_UNorm = 167,
        Astc_8x5_sRGB = 168,
        Astc_8x6_UNorm = 169,
        Astc_8x6_sRGB = 170,
        Astc_8x8_UNorm = 171,
        Astc_8x8_sRGB = 172,
        Astc_10x5_UNorm = 173,
        Astc_10x5_sRGB = 174,
        Astc_10x6_UNorm = 175,
        Astc_10x6_sRGB = 176,
        Astc_10x8_UNorm = 177,
        Astc_10x8_sRGB = 178,
        Astc_10x10_UNorm = 179,
        Astc_10x10_sRGB = 180,
        Astc_12x10_UNorm = 181,
        Astc_12x10_sRGB = 182,
        Astc_12x12_UNorm = 183,
        Astc_12x12_sRGB = 184,
    };

    #ifdef FFI_SRC

    inline u32 GetBlockCopySize(const FGraphicsFormat format, Option<FImagePlane> plane)
    {
        switch (format)
        {
        case FGraphicsFormat::Unknown:
            return 0;
        case FGraphicsFormat::R32G32B32A32_TypeLess:
        case FGraphicsFormat::R32G32B32A32_Float:
        case FGraphicsFormat::R32G32B32A32_UInt:
        case FGraphicsFormat::R32G32B32A32_SInt:
            return 16;
        case FGraphicsFormat::R32G32B32_TypeLess:
        case FGraphicsFormat::R32G32B32_Float:
        case FGraphicsFormat::R32G32B32_UInt:
        case FGraphicsFormat::R32G32B32_SInt:
            return 12;
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
        case FGraphicsFormat::R32_Float_X8X24_TypeLess:
        case FGraphicsFormat::X32_TypeLess_G8X24_Float:
            return 8;
        case FGraphicsFormat::D32_Float_S8X24_UInt:
            if (plane)
            {
                if (plane.get() == FImagePlane::Depth) return 4;
                if (plane.get() == FImagePlane::Stencil) return 1;
            }
            return 8;
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
        case FGraphicsFormat::R24_UNorm_X8_TypeLess:
        case FGraphicsFormat::X24_TypeLess_G8_UInt:
            return 4;
        case FGraphicsFormat::D24_UNorm_S8_UInt:
            if (plane)
            {
                if (plane.get() == FImagePlane::Depth) return 4;
                if (plane.get() == FImagePlane::Stencil) return 1;
            }
            return 4;
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
            return 2;
        case FGraphicsFormat::R8_TypeLess:
        case FGraphicsFormat::R8_UNorm:
        case FGraphicsFormat::R8_UInt:
        case FGraphicsFormat::R8_SNorm:
        case FGraphicsFormat::R8_SInt:
        case FGraphicsFormat::A8_UNorm:
        case FGraphicsFormat::R1_UNorm:
        case FGraphicsFormat::S8_UInt:
            return 1;
        case FGraphicsFormat::R9G9B9E5_SharedExp:
        case FGraphicsFormat::R8G8_B8G8_UNorm:
        case FGraphicsFormat::G8R8_G8B8_UNorm:
            return 4;
        case FGraphicsFormat::B5G6R5_UNorm:
        case FGraphicsFormat::B5G5R5A1_UNorm:
            return 2;
        case FGraphicsFormat::B8G8R8A8_UNorm:
        case FGraphicsFormat::B8G8R8X8_UNorm:
        case FGraphicsFormat::R10G10B10_XR_Bias_A2_UNorm:
        case FGraphicsFormat::B8G8R8A8_TypeLess:
        case FGraphicsFormat::B8G8R8A8_UNorm_sRGB:
        case FGraphicsFormat::B8G8R8X8_TypeLess:
        case FGraphicsFormat::B8G8R8X8_UNorm_sRGB:
            return 4;
        case FGraphicsFormat::AYUV:
        case FGraphicsFormat::Y410:
            return 4;
        case FGraphicsFormat::Y416:
            return 8;
        case FGraphicsFormat::NV12:
            if (plane)
            {
                if (plane.get() == FImagePlane::Y) return 1;
                if (plane.get() == FImagePlane::CbCr) return 2;
            }
            return 2;
        case FGraphicsFormat::P010:
        case FGraphicsFormat::P016:
            return 2;
        case FGraphicsFormat::Opaque_420:
            return 0;
        case FGraphicsFormat::YUY2:
            return 4;
        case FGraphicsFormat::Y210:
        case FGraphicsFormat::Y216:
            return 8;
        case FGraphicsFormat::NV11:
            return 2;
        case FGraphicsFormat::AI44:
        case FGraphicsFormat::IA44:
        case FGraphicsFormat::P8:
            return 1;
        case FGraphicsFormat::A8P8:
            return 2;
        case FGraphicsFormat::B4G4R4A4_UNorm:
            return 2;
        case FGraphicsFormat::P208:
        case FGraphicsFormat::V208:
        case FGraphicsFormat::V408:
            return 1;
        case FGraphicsFormat::A4B4G4R4_UNorm:
            return 2;

        case FGraphicsFormat::BC1_TypeLess:
        case FGraphicsFormat::BC1_UNorm:
        case FGraphicsFormat::BC1_UNorm_sRGB:
            return 8;
        case FGraphicsFormat::BC2_TypeLess:
        case FGraphicsFormat::BC2_UNorm:
        case FGraphicsFormat::BC2_UNorm_sRGB:
        case FGraphicsFormat::BC3_TypeLess:
        case FGraphicsFormat::BC3_UNorm:
        case FGraphicsFormat::BC3_UNorm_sRGB:
            return 16;
        case FGraphicsFormat::BC4_TypeLess:
        case FGraphicsFormat::BC4_UNorm:
        case FGraphicsFormat::BC4_SNorm:
            return 8;
        case FGraphicsFormat::BC5_TypeLess:
        case FGraphicsFormat::BC5_UNorm:
        case FGraphicsFormat::BC5_SNorm:
            return 16;
        case FGraphicsFormat::BC6H_TypeLess:
        case FGraphicsFormat::BC6H_UF16:
        case FGraphicsFormat::BC6H_SF16:
        case FGraphicsFormat::BC7_TypeLess:
        case FGraphicsFormat::BC7_UNorm:
        case FGraphicsFormat::BC7_UNorm_sRGB:
            return 16;

        case FGraphicsFormat::Etc2_R8G8B8_UNorm:
        case FGraphicsFormat::Etc2_R8G8B8_sRGB:
        case FGraphicsFormat::Etc2_R8G8B8A1_UNorm:
        case FGraphicsFormat::Etc2_R8G8B8A1_sRGB:
        case FGraphicsFormat::Eac_R11_UNorm:
        case FGraphicsFormat::Eac_R11_SNorm:
            return 8;
        case FGraphicsFormat::Etc2_R8G8B8A8_UNorm:
        case FGraphicsFormat::Etc2_R8G8B8A8_sRGB:
        case FGraphicsFormat::Eac_R11G11_UNorm:
        case FGraphicsFormat::Eac_R11G11_SNorm:
            return 16;

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
            return 16;
        }
        return 0;
    }

    inline std::pair<u32, u32> GetBlockDimensions(const FGraphicsFormat format)
    {
        switch (format)
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
        case FGraphicsFormat::B5G6R5_UNorm:
        case FGraphicsFormat::B5G5R5A1_UNorm:
        case FGraphicsFormat::B8G8R8A8_UNorm:
        case FGraphicsFormat::B8G8R8X8_UNorm:
        case FGraphicsFormat::R10G10B10_XR_Bias_A2_UNorm:
        case FGraphicsFormat::B8G8R8A8_TypeLess:
        case FGraphicsFormat::B8G8R8A8_UNorm_sRGB:
        case FGraphicsFormat::B8G8R8X8_TypeLess:
        case FGraphicsFormat::B8G8R8X8_UNorm_sRGB:
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
        case FGraphicsFormat::S8_UInt:
            return {1, 1};

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
        case FGraphicsFormat::BC6H_TypeLess:
        case FGraphicsFormat::BC6H_UF16:
        case FGraphicsFormat::BC6H_SF16:
        case FGraphicsFormat::BC7_TypeLess:
        case FGraphicsFormat::BC7_UNorm:
        case FGraphicsFormat::BC7_UNorm_sRGB:
            return {4, 4};

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
            return {4, 4};

        case FGraphicsFormat::Astc_4x4_UNorm:
        case FGraphicsFormat::Astc_4x4_sRGB:
            return {4, 4};
        case FGraphicsFormat::Astc_5x4_UNorm:
        case FGraphicsFormat::Astc_5x4_sRGB:
            return {5, 4};
        case FGraphicsFormat::Astc_5x5_UNorm:
        case FGraphicsFormat::Astc_5x5_sRGB:
            return {5, 5};
        case FGraphicsFormat::Astc_6x5_UNorm:
        case FGraphicsFormat::Astc_6x5_sRGB:
            return {6, 5};
        case FGraphicsFormat::Astc_6x6_UNorm:
        case FGraphicsFormat::Astc_6x6_sRGB:
            return {6, 6};
        case FGraphicsFormat::Astc_8x5_UNorm:
        case FGraphicsFormat::Astc_8x5_sRGB:
            return {8, 5};
        case FGraphicsFormat::Astc_8x6_UNorm:
        case FGraphicsFormat::Astc_8x6_sRGB:
            return {8, 6};
        case FGraphicsFormat::Astc_8x8_UNorm:
        case FGraphicsFormat::Astc_8x8_sRGB:
            return {8, 8};
        case FGraphicsFormat::Astc_10x5_UNorm:
        case FGraphicsFormat::Astc_10x5_sRGB:
            return {10, 5};
        case FGraphicsFormat::Astc_10x6_UNorm:
        case FGraphicsFormat::Astc_10x6_sRGB:
            return {10, 6};
        case FGraphicsFormat::Astc_10x8_UNorm:
        case FGraphicsFormat::Astc_10x8_sRGB:
            return {10, 8};
        case FGraphicsFormat::Astc_10x10_UNorm:
        case FGraphicsFormat::Astc_10x10_sRGB:
            return {10, 10};
        case FGraphicsFormat::Astc_12x10_UNorm:
        case FGraphicsFormat::Astc_12x10_sRGB:
            return {12, 10};
        case FGraphicsFormat::Astc_12x12_UNorm:
        case FGraphicsFormat::Astc_12x12_sRGB:
            return {12, 12};
        }
        return {1, 1};
    }

    inline bool IsSrgb(const FGraphicsFormat value)
    {
        switch (value)
        {
        case FGraphicsFormat::BC1_UNorm_sRGB:
        case FGraphicsFormat::BC2_UNorm_sRGB:
        case FGraphicsFormat::BC3_UNorm_sRGB:
        case FGraphicsFormat::BC7_UNorm_sRGB:
        case FGraphicsFormat::B8G8R8A8_UNorm_sRGB:
        case FGraphicsFormat::B8G8R8X8_UNorm_sRGB:
        case FGraphicsFormat::R8G8B8A8_UNorm_sRGB:
            return true;
        case FGraphicsFormat::Etc2_R8G8B8_sRGB:
        case FGraphicsFormat::Etc2_R8G8B8A1_sRGB:
        case FGraphicsFormat::Etc2_R8G8B8A8_sRGB:
            return true;
        case FGraphicsFormat::Astc_4x4_sRGB:
        case FGraphicsFormat::Astc_5x4_sRGB:
        case FGraphicsFormat::Astc_5x5_sRGB:
        case FGraphicsFormat::Astc_6x5_sRGB:
        case FGraphicsFormat::Astc_6x6_sRGB:
        case FGraphicsFormat::Astc_8x5_sRGB:
        case FGraphicsFormat::Astc_8x6_sRGB:
        case FGraphicsFormat::Astc_8x8_sRGB:
        case FGraphicsFormat::Astc_10x5_sRGB:
        case FGraphicsFormat::Astc_10x6_sRGB:
        case FGraphicsFormat::Astc_10x8_sRGB:
        case FGraphicsFormat::Astc_10x10_sRGB:
        case FGraphicsFormat::Astc_12x10_sRGB:
        case FGraphicsFormat::Astc_12x12_sRGB:
            return true;
        default:
            return false;
        }
    }

    inline bool IsDepthStencil(const FGraphicsFormat value)
    {
        switch (value)
        {
        case FGraphicsFormat::S8_UInt:
        case FGraphicsFormat::D32_Float_S8X24_UInt:
        case FGraphicsFormat::D32_Float:
        case FGraphicsFormat::D24_UNorm_S8_UInt:
        case FGraphicsFormat::D16_UNorm:
            return true;
        default:
            return false;
        }
    }

    inline bool HasDepth(const FGraphicsFormat value)
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

    inline bool HasStencil(const FGraphicsFormat value)
    {
        switch (value)
        {
        case FGraphicsFormat::S8_UInt:
        case FGraphicsFormat::D32_Float_S8X24_UInt:
        case FGraphicsFormat::D24_UNorm_S8_UInt:
            return true;
        default:
            return false;
        }
    }

    inline bool IsYCbCr(const FGraphicsFormat value)
    {
        switch (value)
        {
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
        case FGraphicsFormat::P208:
        case FGraphicsFormat::V208:
        case FGraphicsFormat::V408:
            return true;
        default:
            return false;
        }
    }

    #endif
}
