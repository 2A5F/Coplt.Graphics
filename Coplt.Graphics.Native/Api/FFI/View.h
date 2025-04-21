#pragma once

#include "GpuObject.h"
#include "GraphicsFormat.h"

namespace Coplt
{
    struct FGpuBuffer;
    struct FGpuImage;

    COPLT_INTERFACE_DEFINE(FGpuViewable, "b3aeb8a5-1fa6-4866-97ef-1a5fa401e18f", FGpuObject)
    {
    };

    enum class FViewType : u8
    {
        None = 0,
        Sampler = 1,
        Buffer = 2,
        Image1D = 3,
        Image1DArray = 4,
        Image2D = 5,
        Image2DArray = 6,
        Image2DMs = 7,
        Image2DMsArray = 8,
        Image3D = 9,
        ImageCube = 10,
        ImageCubeArray = 11,
        UploadBuffer = 12,
    };

    struct FViewData
    {
        union
        {
            FViewType Type{};

            struct
            {
                FViewType Type{};
                FGraphicsFormat Format;
                u64 Offset;
                u32 Size;
                u32 Stride;
            } Buffer;

            struct
            {
                FViewType Type{};
                u32 Size;
                u64 Index; // todo 32
                u64 Offset;
            } UploadBuffer;

            struct
            {
                FViewType Type{};
                u8 Mip;
                u8 NumMips;
                u8 Plane;
                FGraphicsFormat Format{};

                union
                {
                    u32 Index;
                    u32 Z;
                };

                union
                {
                    u32 Size{};
                    u32 Depth;
                };
            } Image;
        };
    };

    struct FView
    {
        FGpuViewable* Viewable{};
        FViewData Data{};
    };
}
