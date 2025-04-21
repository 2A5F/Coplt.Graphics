using Coplt.Graphics.Native;
using Coplt.Union;
using Coplt.Union.Misc;

namespace Coplt.Graphics
{
    public enum GraphicsFormat
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
    }
}

namespace Coplt.Graphics.Core
{
    public static partial class GraphicsExtensions
    {
        public static FGraphicsFormat ToFFI(this GraphicsFormat value) => (FGraphicsFormat)value;

        public static GraphicsFormat FromFFI(this FGraphicsFormat value) => (GraphicsFormat)value;

        public static bool IsSrgb(this GraphicsFormat value) => value switch
        {
            GraphicsFormat.B8G8R8A8_UNorm_sRGB or GraphicsFormat.B8G8R8X8_UNorm_sRGB or GraphicsFormat.R8G8B8A8_UNorm_sRGB
                or GraphicsFormat.BC1_UNorm_sRGB or GraphicsFormat.BC2_UNorm_sRGB or GraphicsFormat.BC3_UNorm_sRGB or GraphicsFormat.BC7_UNorm_sRGB
                => true,
            GraphicsFormat.Etc2_R8G8B8_sRGB or
                GraphicsFormat.Etc2_R8G8B8A1_sRGB or
                GraphicsFormat.Etc2_R8G8B8A8_sRGB
                => true,
            GraphicsFormat.Astc_4x4_sRGB or
                GraphicsFormat.Astc_5x4_sRGB or
                GraphicsFormat.Astc_5x5_sRGB or
                GraphicsFormat.Astc_6x5_sRGB or
                GraphicsFormat.Astc_6x6_sRGB or
                GraphicsFormat.Astc_8x5_sRGB or
                GraphicsFormat.Astc_8x6_sRGB or
                GraphicsFormat.Astc_8x8_sRGB or
                GraphicsFormat.Astc_10x5_sRGB or
                GraphicsFormat.Astc_10x6_sRGB or
                GraphicsFormat.Astc_10x8_sRGB or
                GraphicsFormat.Astc_10x10_sRGB or
                GraphicsFormat.Astc_12x10_sRGB or
                GraphicsFormat.Astc_12x12_sRGB
                => true,
            _ => false,
        };

        public static bool IsDepthStencil(this GraphicsFormat value) => value switch
        {
            GraphicsFormat.D32_Float_S8X24_UInt or GraphicsFormat.D32_Float
                or GraphicsFormat.D24_UNorm_S8_UInt or GraphicsFormat.D16_UNorm
                or GraphicsFormat.S8_UInt
                => true,
            _ => false,
        };

        public static bool HasDepth(this GraphicsFormat value) => value switch
        {
            GraphicsFormat.D32_Float_S8X24_UInt or GraphicsFormat.D32_Float
                or GraphicsFormat.D24_UNorm_S8_UInt or GraphicsFormat.D16_UNorm
                => true,
            _ => false,
        };

        public static bool HasStencil(this GraphicsFormat value) => value switch
        {
            GraphicsFormat.D32_Float_S8X24_UInt
                or GraphicsFormat.D24_UNorm_S8_UInt
                or GraphicsFormat.S8_UInt
                => true,
            _ => false,
        };

        public static bool IsYCbCr(this GraphicsFormat value) => value switch
        {
            GraphicsFormat.AYUV or GraphicsFormat.Y410 or GraphicsFormat.Y416 or GraphicsFormat.NV12 or GraphicsFormat.P010 or GraphicsFormat.P016
                or GraphicsFormat.Opaque_420 or GraphicsFormat.YUY2 or GraphicsFormat.Y210 or GraphicsFormat.Y216 or GraphicsFormat.NV11 or GraphicsFormat.AI44
                or GraphicsFormat.IA44 or GraphicsFormat.P8 or GraphicsFormat.A8P8 or GraphicsFormat.P208 or GraphicsFormat.V208 or GraphicsFormat.V408
                => true,
            _ => false
        };
    }
}
