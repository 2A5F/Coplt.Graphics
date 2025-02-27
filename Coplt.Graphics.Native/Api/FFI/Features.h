#pragma once

#include "Common.h"

namespace Coplt
{
    struct FDeviceFeatureRequires
    {
        // 光追
        b8 RayTracing{};
        // 网格着色器
        b8 MeshShader{};
        // 描述符堆，dx 后端忽略，vk 使用 VK_EXT_descriptor_buffer
        b8 DescriptorHeap{};
        // 增强屏障，vk 后端忽略
        b8 EnhancedBarriers{};
        // 假 · 无绑定
        b8 ArrBindless{};
        // 真 · 无绑定, dx 使用 dynamic resource，vk 使用 VK_EXT_mutable_descriptor_type
        b8 DynBindless{};
    };

    struct FDeviceFeatures
    {
        // 光追
        b8 RayTracing{};
        // 网格着色器
        b8 MeshShader{};
        // 描述符堆，dx 后端永远为 true，vk 使用 VK_EXT_descriptor_buffer
        b8 DescriptorHeap{};
        // 增强屏障，vk 后端永远为 true
        b8 EnhancedBarriers{};
        // 假 · 无绑定
        b8 ArrBindless{};
        // 真 · 无绑定, dx 使用 dynamic resource，vk 使用 VK_EXT_mutable_descriptor_type
        b8 DynBindless{};
    };
}
