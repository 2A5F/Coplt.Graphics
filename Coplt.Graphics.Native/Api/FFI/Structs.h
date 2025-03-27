#pragma once

#include "Common.h"

namespace Coplt
{
    struct FRect
    {
        u32 Left;
        u32 Top;
        u32 Right;
        u32 Bottom;
    };

    struct FViewport
    {
        f32 X;
        f32 Y;
        f32 Width;
        f32 Height;
        f32 MinDepth;
        f32 MaxDepth;
    };

    union FOrRectViewport
    {
        FRect Rect;
        FViewport Viewport;
    };

    COPLT_ENUM_FLAGS(FDepthStencilClearFlags, u8)
    {
        None = 0,
        Depth = 1,
        Stencil = 2,
    };

    enum class FLoadOp : u8
    {
        Load,
        Clear,
        Discard,
        NoAccess,
    };

    enum class FStoreOp : u8
    {
        Store,
        Discard,
        Resolve,
        NoAccess,
    };

    enum class FResolveMode : u8
    {
        Decompress,
        Min,
        Max,
        Average,
    };
}
