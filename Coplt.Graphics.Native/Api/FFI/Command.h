#pragma once

#include "Output.h"
#include "States.h"

namespace Coplt
{
    enum class FCommandType : u32
    {
        None,
        // 手动资源过渡
        Transition,
        ClearColor,
    };

    enum class FCommandFlags : u32
    {
        None = 0,
        // 不要自动计算资源过渡
        DontTransition = 1 << 0,
    };

    enum class FResourceRefType : u8
    {
        Texture,
        Output,
        // todo other
    };

    struct FResourceMeta
    {
        union
        {
            FGpuOutput* Output;
            // todo other
        };

        FResourceState CurrentState{};
        FResourceRefType Type{};
    };

    struct FResourceSrc
    {
        u32 ResourceIndex{};
    };

    struct FRect
    {
        u32 left;
        u32 top;
        u32 right;
        u32 bottom;
    };

    struct FCommandTransition
    {
        FResourceSrc Resource{};
        FResourceState SrcState{};
        FResourceState DstState{};
    };

    struct FCommandClearColor
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 内的索引
        u32 RectIndex{};
        FResourceSrc Image{};
        f32 Color[4]{};
    };

    struct FCommandItem
    {
        FCommandType Type{};
        FCommandFlags Flags{};

        union
        {
            FCommandTransition Transition;
            FCommandClearColor ClearColor;
        };
    };

    struct FCommandSubmit
    {
        FResourceMeta* Resources{};
        // 命令项里面的对象制造需要保证生命周期 >= FCommandSubmit 的生命周期
        FCommandItem* Items{};
        u8* Payload{};
        // 有多少命令
        u32 Count{};
    };
}
