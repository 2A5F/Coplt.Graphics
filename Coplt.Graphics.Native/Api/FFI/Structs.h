#pragma once

#include "Common.h"
#include "GraphicsFormat.h"

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

    struct FBufferCopyRange
    {
        // Size 为 u64::max 时复制整个
        u64 Size{};
        u64 DstOffset{};
        u64 SrcOffset{};
    };

    struct FBufferImageCopyRange
    {
        u64 BufferOffset{};
        // 必须是 256 的倍数
        u32 BytesPerRow{};
        u32 RowsPerImage{};
        u32 ImageOffset[3]{};
        u32 ImageExtent[3]{};
        u32 ImageIndex{};
        u32 ImageCount{};
        u16 MipLevel{};
        FImagePlane Plane{};
    };

    struct FUploadLoc
    {
        u32 Index{};
    };
}
