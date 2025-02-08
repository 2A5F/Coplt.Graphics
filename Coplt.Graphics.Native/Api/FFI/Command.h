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
        // 仅内部使用
        Present,
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
            FGpuOutput* Output{};
            // todo other
        };

        FResourceState CurrentState{};
        FResourceRefType Type{};

#ifdef FFI_SRC
        FUnknown* GetObjectPtr() const
        {
            switch (Type)
            {
            case FResourceRefType::Texture:
                return nullptr; // todo
            case FResourceRefType::Output:
                return Output;
            }
            return nullptr; // todo
        }
#endif
    };

    struct FResourceSrc
    {
        u32 ResourceIndex{};

#ifdef FFI_SRC
        FResourceMeta& Get(const FCommandSubmit& submit) const;
#endif
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

    struct FCommandPresent
    {
        FResourceSrc Image{};
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
            FCommandPresent Present;
            FCommandClearColor ClearColor;
        };
    };

    struct FCommandSubmit
    {
        // 提交操作可能会修改此内存，提交后内存将无效
        FResourceMeta* Resources{};
        // 命令项里面的对象制造需要保证生命周期 >= FCommandSubmit 的生命周期，提交操作可能会修改此内存，提交后内存将无效
        FCommandItem* Items{};
        // 提交操作可能会修改此内存，提交后内存将无效
        u8* Payload{};
        // 有多少命令
        u32 Count{};
    };

#ifdef FFI_SRC

    inline FResourceMeta& FResourceSrc::Get(const FCommandSubmit& submit) const
    {
        return submit.Resources[ResourceIndex];
    }

#endif
}
