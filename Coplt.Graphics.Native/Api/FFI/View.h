#pragma once

#include "GpuObject.h"
#include "GraphicsFormat.h"

namespace Coplt
{
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
    };

    struct FViewData
    {
        union
        {
            u64 Offset{};
            struct
            {
                union
                {
                    u32 Index;
                    u32 Z;
                };
                union
                {
                    struct
                    {
                        u8 Mip;
                        u8 NumMips;
                        u8 Plane;
                    };
                    u32 Stride;
                };
            };
        };
        union
        {
            u32 Size{};
            u32 Depth;
        };
        FGraphicsFormat Format{};
    };

    struct FView
    {
        FGpuViewable* Viewable{};
        FViewData Data{};
        FViewType Type{};
    };
}
