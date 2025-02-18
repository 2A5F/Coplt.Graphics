#pragma once

#include "GpuObject.h"

namespace Coplt
{
    enum class FResourceViewType : u8
    {
        None = 0,
        ConstantBuffer,
        ShaderResource,
        UnorderedAccess,
        RenderTarget,
        DepthStencil,
        IndexBuffer,
        VertexBuffer,
        StreamOutput,
    };

    COPLT_ENUM_FLAGS(FResourcePurpose, u32)
    {
        None = 0,
        // 资源可以用于 Cbv
        ConstantBuffer = 1 << 0,
        // 资源可以用于 Srv
        ShaderResource = 1 << 1,
        // 资源可以用于 Uav
        UnorderedAccess = 1 << 2,
        // 资源可以用于 Rtv
        RenderTarget = 1 << 3,
        // 资源可以用于 Dsv
        DepthStencil = 1 << 4,
        // 资源可以用于 Ibv
        IndexBuffer = 1 << 5,
        // 资源可以用于 Vbv
        VertexBuffer = 1 << 6,
        // 资源可以用于 Sov
        StreamOutput = 1 << 7,
        // 资源可以用于 RayTracing
        RayTracing = 1 << 8,
        // 资源可以用于 ShadingRate
        ShadingRate = 1 << 9,
        // 资源可以用于 IndirectDrawArgs
        IndirectDrawArgs = 1 << 10,
    };

    enum class FLifeTime : u8
    {
        // 一般资源
        Common,
        // 静态资源，一般比如静态加载的图片、材质的参数
        Static,
        // 瞬态资源，只在帧内使用，帧结束后将丢弃，同时每帧都会具有独立副本，类似交换链
        Transient,
    };

    enum class FCpuAccess : u8
    {
        None = 0,
        Write = 1,
        Read = 2,
    };

    enum class FBufferUsage : u8
    {
        // 未定义用法
        Undefined,
        // 按结构化缓冲区使用
        Structured,
        // 按原始缓冲区使用
        Raw,
    };

    struct FGpuResourceCreateOptions
    {
        Str8or16 Name{};
        FResourcePurpose Purpose{};
        FCpuAccess CpuAccess{};
        FLifeTime LifeTime{};
    };

    COPLT_INTERFACE_DEFINE(FGpuResource, "f99dceec-2f0c-4a28-b666-beb7c35219d6", FGpuObject)
    {
        FResourcePurpose m_purpose{};
        FCpuAccess m_cpu_access{};
        FLifeTime m_life_time{};
    };

    struct FGpuBufferCreateOptions : FGpuResourceCreateOptions
    {
        // 字节大小
        u64 Size{};
        // 默认的 Structured Buffer 元素数量
        u32 Count{};
        // 默认的 Structured Buffer 元素步幅
        u32 Stride{};
        // 指示默认用法
        FBufferUsage Usage{};
    };

    COPLT_INTERFACE_DEFINE(FGpuBuffer, "283740e3-fe96-41d0-830a-0a4c6a725336", FGpuResource)
    {
        // 字节大小
        u64 m_size{};
        // 默认的 Structured Buffer 元素数量
        u32 m_count{};
        // 默认的 Structured Buffer 元素步幅
        u32 m_stride{};
        // 指示默认用法
        FBufferUsage m_usage{};
    };

    union FOptimizedClearColor
    {
        f32 Color[4];

        struct
        {
            f32 Depth;
            u8 Stencil;
        };
    };

    enum class FImageDimension : u8
    {
        // 一维
        One,
        // 二维
        Two,
        // 三维
        Three,
        // Cube
        Cube,
    };

    enum class FImageLayout : u8
    {
        Undefined,
        RowMajor,
        Undefined64kSwizzle,
        Standard64kSwizzle,
    };

    struct FGpuTextureCreateOptions : FGpuResourceCreateOptions
    {
        // 纹理格式
        FGraphicsFormat Format{};
        // 宽度
        u32 Width{};
        // 高度
        u32 Height{};
        // 深度或者数组长度
        u32 DepthOrLength{};
        // MipLevels
        u16 MipLevels{};
        // 优化的清除颜色
        FOptimizedClearColor OptimizedClearValue{};
        // 是否有优化的清除颜色
        b8 HasOptimizedClearValue{};
        // 多重采样数量
        u8 MultisampleCount{};
        // 纹理维度
        FImageDimension Dimension{};
        // 纹理布局
        FImageLayout Layout{};
    };

    COPLT_INTERFACE_DEFINE(FGpuImage, "667efa36-21c7-4561-abad-85780fa4929e", FGpuResource)
    {
        // 纹理格式
        FGraphicsFormat m_format{};
        // 宽度
        u32 m_width{};
        // 高度
        u32 m_height{};
        // 深度或者数组长度
        u32 m_depth_or_length{};
        // MipLevels
        u16 m_mip_levels{};
        // 多重采样数量
        u8 m_multisample_count{};
        // 纹理维度
        FImageDimension m_dimension{};
        // 纹理布局
        FImageLayout m_layout{};
    };
}
