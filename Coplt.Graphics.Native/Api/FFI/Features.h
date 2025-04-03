#pragma once

#include "Common.h"

namespace Coplt
{
    enum class FShaderModelLevel : u32
    {
        Any = 0,
        SM_5_1 = 0x51,
        SM_6_0 = 0x60,
        SM_6_1 = 0x61,
        SM_6_2 = 0x62,
        SM_6_3 = 0x63,
        SM_6_4 = 0x64,
        SM_6_5 = 0x65,
        SM_6_6 = 0x66,
        SM_6_7 = 0x67,
        SM_6_8 = 0x68,
        SM_6_9 = 0x69,
    };

    struct FDeviceFeatureRequires
    {
        // 兼容的 hlsl 着色器模型级别
        FShaderModelLevel ShaderModelLevel{};
        // 光追
        b8 RayTracing{};
        // 网格着色器
        b8 MeshShader{};
        // 描述符堆，dx 后端忽略，vk 使用 VK_EXT_descriptor_buffer
        b8 DescriptorHeap{};
        // 增强屏障，vk 后端忽略
        b8 EnhancedBarriers{};
        // 假 · 无绑定, dx 后端后端忽略
        b8 ArrBindless{};
        // 真 · 无绑定, dx 使用 dynamic resource，vk 使用 VK_EXT_mutable_descriptor_type
        b8 DynBindless{};
        // 仅动态无绑定模式，将无法使用着色器绑定，着色器布局中只能具有根常量/常量推送，此模式具有设备全局描述符堆
        b8 DynBindlessOnly{};
    };

    struct FDeviceFeatures
    {
        // 兼容的 hlsl 着色器模型级别
        FShaderModelLevel ShaderModelLevel{};
        // 光追
        b8 RayTracing{};
        // 网格着色器
        b8 MeshShader{};
        // 描述符堆，dx 后端永远为 true，vk 使用 VK_EXT_descriptor_buffer
        b8 DescriptorHeap{};
        // 增强屏障，vk 后端永远为 true
        b8 EnhancedBarriers{};
        // 假 · 无绑定, dx 后端永远为 true
        b8 ArrBindless{};
        // 真 · 无绑定, dx 使用 dynamic resource，vk 使用 VK_EXT_mutable_descriptor_type
        b8 DynBindless{};
        // 仅动态无绑定模式，将无法使用着色器绑定，着色器布局中只能具有根常量/常量推送，此模式具有设备全局描述符堆
        b8 DynBindlessOnly{};
        // 可以任意创建隔离，dx12 永远为 true，vk 永远为 false
        b8 ExtraIsolate{};
        // 是否是缓存一致的 UMA 架构
        b8 UMA{};
        // 是否是缓存一致的 UMA 架构
        b8 CacheCoherentUMA{};
    };
}
