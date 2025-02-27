using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum D3dFeatureLevel
{
    Unset = 0,
    _12_1 = 0xc100,
    _12_2 = 0xc200,
}

public enum VulkanVersion
{
    Unset = 0,
    _1_2 = 1002,
    _1_3 = 1003,
    _1_4 = 1004,
}

public enum DeviceTypeRequire : byte
{
    Any,
    Gpu,
    IntegratedGpu,
    DiscreteGpu,
    VirtualGpu,
    Cpu,
}

public enum DeviceType : byte
{
    Other,
    IntegratedGpu,
    DiscreteGpu,
    VirtualGpu,
    Cpu,
}

public enum Backend : byte
{
    None,
    Dx12,
    Vulkan,
};

public record struct DeviceRequires
{
    public D3dFeatureLevel D3dFeatureLevel;
    public VulkanVersion VulkanVersion;
    public DeviceTypeRequire DeviceType;
    public DeviceFeatureRequires FeatureRequires;
}

public enum ShaderModelLevel : uint
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

public record struct DeviceFeatureRequires
{
    /// <summary>
    /// 兼容的 hlsl 着色器模型级别
    /// </summary>
    public ShaderModelLevel ShaderModelLevel;
    /// <summary>
    /// 光追
    /// </summary>
    public bool RayTracing;
    /// <summary>
    /// 网格着色器
    /// </summary>
    public bool MeshShader;
    /// <summary>
    /// 描述符堆，dx 后端忽略，vk 使用 VK_EXT_descriptor_buffer
    /// </summary>
    public bool DescriptorHeap;
    /// <summary>
    /// 增强屏障，vk 后端忽略
    /// </summary>
    public bool EnhancedBarriers;
    /// <summary>
    /// 假 · 无绑定, dx 后端后端忽略
    /// </summary>
    public bool ArrBindless;
    /// <summary>
    /// 真 · 无绑定, dx 使用 dynamic resource，vk 使用 VK_EXT_mutable_descriptor_type
    /// </summary>
    public bool DynBindless;
}

public record struct DeviceFeatures
{
    /// <summary>
    /// 兼容的 hlsl 着色器模型级别
    /// </summary>
    public ShaderModelLevel ShaderModelLevel;
    /// <summary>
    /// 光追
    /// </summary>
    public bool RayTracing;
    /// <summary>
    /// 网格着色器
    /// </summary>
    public bool MeshShader;
    /// <summary>
    /// 描述符堆，dx 后端忽略，vk 使用 VK_EXT_descriptor_buffer
    /// </summary>
    public bool DescriptorHeap;
    /// <summary>
    /// 增强屏障，vk 后端忽略
    /// </summary>
    public bool EnhancedBarriers;
    /// <summary>
    /// 假 · 无绑定, dx 后端后端忽略
    /// </summary>
    public bool ArrBindless;
    /// <summary>
    /// 真 · 无绑定, dx 使用 dynamic resource，vk 使用 VK_EXT_mutable_descriptor_type
    /// </summary>
    public bool DynBindless;

    public DeviceFeatures(in FDeviceFeatures native)
    {
        ShaderModelLevel = (ShaderModelLevel)native.ShaderModelLevel;
        RayTracing = native.RayTracing;
        MeshShader = native.MeshShader;
        DescriptorHeap = native.DescriptorHeap;
        EnhancedBarriers = native.EnhancedBarriers;
        ArrBindless = native.ArrBindless;
        DynBindless = native.DynBindless;
    }
}
