#pragma once

#include "Common.h"
#include "Types.h"

namespace Coplt
{
    enum class FResourceState : u32
    {
        // 手动管理资源状态，自动过渡时将忽略
        Manual = COPLT_U32_MAX,
        Common = 0,
        Present = 1 << 0,
        VertexBuffer = 1 << 1,
        IndexBuffer = 1 << 2,
        ConstantBuffer = 1 << 3,
        RenderTarget = 1 << 4,
        DepthRead = 1 << 5,
        DepthWrite = 1 << 6,
        PixelShaderResource = 1 << 7,
        NonPixelShaderResource = 1 << 8,
        UnorderedAccess = 1 << 9,
        CopySrc = 1 << 10,
        CopyDst = 1 << 11,
        ResolveSrc = 1 << 12,
        ResolveDst = 1 << 13,
        RayTracing = 1 << 14,
        ShadingRate = 1 << 15,
        IndirectBuffer = 1 << 16,
    };

    struct FSamplerState
    {
        u32 Count{1};
        u32 Quality{0};
    };
};
