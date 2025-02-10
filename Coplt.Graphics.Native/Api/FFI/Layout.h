#pragma once

#include "Object.h"
#include "Shader.h"

namespace Coplt
{
    enum class FShaderLayoutItemType : u8
    {
        Buffer,
        Texture,
        Sampler,
        // 将使用 Push Const, 忽略 Usage
        Constants,
    };

    enum class FShaderLayoutItemUsage : u8
    {
        Common,
        // 经常改变的变量，例如每次调用改变
        Dynamic,
    };

    struct FShaderLayoutItemDefine
    {
        u32 Slot{};
        u32 Space{};
        // 类型为 Constants 描述有多少个常量
        u32 Count{};
        FShaderStageFlags Visibility{};
        FShaderLayoutItemType Type{};
        FShaderLayoutItemUsage Usage{};
    };

    struct FShaderLayoutCreateOptions
    {
        Str8or16 Name{};
        u32 Count{};
        FShaderLayoutItemDefine* Items{};
        // todo static samplers
    };

    COPLT_INTERFACE_DEFINE(FShaderLayout, "552a498e-8f3a-47ff-a335-7af2de0901e8", FGpuObject)
    {
    };
}
