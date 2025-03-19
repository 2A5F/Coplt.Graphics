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
}
