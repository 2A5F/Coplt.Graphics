#pragma once

#include "Object.h"
#include "Shader.h"

namespace Coplt
{
    enum class FShaderLayoutItemType : u8
    {
        Cbv,
        SrvTexture,
        UavTexture,
        SrvBuffer,
        UavBuffer,
        Sampler,
        // 将使用 Push Const / Root Const, 忽略 Usage
        Constants,
    };

    enum class FShaderLayoutItemUsage : u8
    {
        // 一般改变频率的变量，可能每帧都会改变
        Common,
        // 不太改变的变量，例如材质参数，一般很少改变，可以进行一定的静态优化，建议将所有材质绑定放到单独的 space 中区分
        Material,
        // 经常改变的变量，例如每次绘制调用都会改变, dx 后端将直接在根签名内
        Instant,
        // 仅限采样器
        Static,
    };

    struct FShaderLayoutItemDefine
    {
        // dx 后端无论什么时候都是 register, vk 后端一般情况是 binding，类型为 Constants 时是 push const 的 offset，为字节偏移
        u32 Slot{};
        u32 Space{};
        // 类型为 Constants 时，dx 后端表示有多少个常量，vk 后端表示字节大小；类型是 Sampler并且 Usage 是 Static 时是静态采样器描述的索引；其他类型忽略
        u32 X0{};
        FShaderStageFlags Visibility{};
        FShaderLayoutItemType Type{};
        FShaderLayoutItemUsage Usage{};
    };

    struct FShaderLayoutCreateOptions
    {
        Str8or16 Name{};
        u32 Count{};
        FShaderLayoutItemDefine* Items{};
        // todo 静态采样器
    };

    COPLT_INTERFACE_DEFINE(FShaderLayout, "552a498e-8f3a-47ff-a335-7af2de0901e8", FGpuObject)
    {
        virtual FShaderLayoutItemDefine* GetItemDefines(u32* out_count) noexcept = 0;
    };
}
