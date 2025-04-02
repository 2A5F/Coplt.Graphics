#pragma once

#include "Object.h"
#include "Shader.h"
#include "Sampler.h"

#if FFI_SRC
#include <span>
#endif

namespace Coplt
{
    enum class FResourceAccess : u8
    {
        Unknown,
        ReadOnly,
        WriteOnly,
        ReadWrite,
    };

    enum class FShaderLayoutItemView : u8
    {
        Cbv,
        Srv,
        Uav,
        // Type 必须是 Sampler
        Sampler,
        // 将使用 Push Const / Root Const, 忽略 Usage，Type 必须是 ConstantBuffer
        Constants,
        // 静态采样器, Type 必须是 Sampler
        StaticSampler,
    };

    enum class FShaderLayoutItemType : u8
    {
        ConstantBuffer,
        Buffer,
        RawBuffer,
        StructureBuffer,
        StructureBufferWithCounter,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture2DMultisample,
        Texture2DArrayMultisample,
        Texture3D,
        TextureCube,
        TextureCubeArray,
        Sampler,
        RayTracingAccelerationStructure,
    };

    #ifdef FFI_SRC
    inline bool IsBuffer(const FShaderLayoutItemType value)
    {
        switch (value)
        {
        case FShaderLayoutItemType::ConstantBuffer:
        case FShaderLayoutItemType::Buffer:
        case FShaderLayoutItemType::RawBuffer:
        case FShaderLayoutItemType::StructureBuffer:
        case FShaderLayoutItemType::StructureBufferWithCounter:
        case FShaderLayoutItemType::RayTracingAccelerationStructure:
            return true;
        default: ;
        }
        return false;
    }
    #endif

    struct FShaderLayoutItem
    {
        // 绑定点的 Id，建议从名称缓存自增 id, 不同 stage 不同 scope 的 id 可以重复
        u64 Id{};
        // 绑定点所属范围，和 Id 共同组成唯一定位
        u64 Scope{};
        // dx 后端无论什么时候都是 register, vk 后端一般情况是 binding，类型为 Constants 时是 push const 的 offset，为字节偏移
        u32 Slot{};
        // dx 的 space，vk 的 set，vk 建议尽可能多的拆分 set，dx 的 space 写不写都一样 // todo 引入 spv 修改器运行时修改 vk 的 set
        u32 Space{};
        // 数量，View 是 StaticSampler 时必须是 1，其他必须最少是 1, View 是 Constants 是 32 位值的数量，而不是 byte 的数量
        u32 Count{};
        FGraphicsFormat Format{};
        FShaderStage Stage{};
        FShaderLayoutItemView View{};
        FShaderLayoutItemType Type{};
        FResourceAccess UavAccess{};

        #ifdef FFI_SRC
        bool IsAllowBuffer() const
        {
            switch (Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
            case FShaderLayoutItemType::RawBuffer:
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                return true;
            default:
                return false;
            }
        }

        bool IsAllowTexture() const
        {
            switch (Type)
            {
            case FShaderLayoutItemType::Texture1D:
            case FShaderLayoutItemType::Texture1DArray:
            case FShaderLayoutItemType::Texture2D:
            case FShaderLayoutItemType::Texture2DArray:
            case FShaderLayoutItemType::Texture2DMultisample:
            case FShaderLayoutItemType::Texture2DArrayMultisample:
            case FShaderLayoutItemType::Texture3D:
            case FShaderLayoutItemType::TextureCube:
            case FShaderLayoutItemType::TextureCubeArray:
                return true;
            default:
                return false;
            }
        }
        #endif
    };

    COPLT_ENUM_FLAGS(FShaderLayoutFlags, u8)
    {
        None = 0,
        // 是否启用动态无绑定
        DynBindLess = 1 << 0,
        // 是否启用输入组装
        InputAssembler = 1 << 1,
        // 是否启用流输出
        StreamOutput = 1 << 2,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FShaderLayoutCreateOptions
    {
        FStr8or16 Name{};
        FShaderLayoutItem* Items{};
        u32 NumItems{};

        FShaderLayoutFlags Flags{};
    };

    struct FGetEmptyShaderLayoutOptions
    {
        FShaderLayoutFlags Flags{};
    };

    struct FShaderLayoutData
    {
        const FShaderLayoutItem* Items{};
        u32 NumItems{};
        FShaderLayoutFlags Flags{};
    };

    COPLT_INTERFACE_DEFINE(FShaderLayout, "552a498e-8f3a-47ff-a335-7af2de0901e8", FGpuObject)
    {
        virtual FShaderLayoutData* ShaderLayoutData() noexcept = 0;

        #if FFI_SRC
        std::span<const FShaderLayoutItem> GetItems() noexcept
        {
            const auto data = ShaderLayoutData();
            return std::span{data->Items, static_cast<usize>(data->NumItems)};
        }
        #endif
    };

    struct FShaderLayoutCreateResult
    {
        FShaderLayout* Layout{};
        FShaderLayoutData* Data{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FBindGroupItem
    {
        // 绑定点的 Id，建议从名称缓存自增 id, 不同 stage 不同 scope 的 id 可以重复
        u64 Id{};
        // 绑定点所属范围，和 Id 共同组成唯一定位
        u64 Scope{};
        // 数量
        u32 Count{};
        // StaticSamplers 中的 Index
        u32 StaticSamplerIndex{};
        FGraphicsFormat Format{};
        FShaderStageFlags Stages{};
        FShaderLayoutItemView View{};
        FShaderLayoutItemType Type{};
        FResourceAccess UavAccess{};
    };

    enum class FBindGroupUsage : u8
    {
        // 一般组
        Common = 0,
        // 提示组的使用是频繁更改的，d3d12 将会尽可能使用根描述符
        Dynamic = 1,
    };

    struct FBindGroupLayoutCreateOptions
    {
        FStr8or16 Name{};
        FBindGroupItem* Items{};
        FStaticSamplerInfo* StaticSamplers{};
        u32 NumItems{};
        u32 NumStaticSamplers{};
        FBindGroupUsage Usage{};
    };

    struct FBindGroupLayoutData
    {
        const FBindGroupItem* Items{};
        const FStaticSamplerInfo* StaticSamplers{};
        u32 NumItems{};
        u32 NumStaticSamplers{};
        FBindGroupUsage Usage{};
    };

    COPLT_INTERFACE_DEFINE(FBindGroupLayout, "312c75eb-30f5-40b3-b79e-acb5498ca9dc", FGpuObject)
    {
        virtual FBindGroupLayoutData* BindGroupLayoutData() noexcept = 0;

        #if FFI_SRC
        std::span<const FBindGroupItem> GetItems() noexcept
        {
            const auto data = BindGroupLayoutData();
            return std::span{data->Items, static_cast<usize>(data->NumItems)};
        }

        std::span<const FStaticSamplerInfo> GetStaticSamplers() noexcept
        {
            const auto data = BindGroupLayoutData();
            return std::span{data->StaticSamplers, static_cast<usize>(data->NumStaticSamplers)};
        }

        FBindGroupLayoutData& Data() noexcept
        {
            return *BindGroupLayoutData();
        }
        #endif
    };

    struct FBindGroupLayoutCreateResult
    {
        FBindGroupLayout* Layout{};
        FBindGroupLayoutData* Data{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FBindingLayoutCreateOptions
    {
        FStr8or16 Name{};
        FShaderLayout* ShaderLayout{};
        FBindGroupLayout** Groups{};
        u32 NumGroups{};
    };

    struct FGetEmptyBindingLayoutOptions
    {
        FShaderLayoutFlags Flags{};
    };

    COPLT_INTERFACE_DEFINE(FBindingLayout, "bc0b662e-2918-4769-9d0c-1fee25b32c5d", FGpuObject)
    {
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FShaderInputLayoutCreateOptions
    {
        FStr8or16 Name{};
        FShaderInputLayoutElement* Element{};
        u32 Count{};
    };

    COPLT_INTERFACE_DEFINE(FShaderInputLayout, "70229c9a-fb3d-46b4-b534-72fdb167d807", FGpuObject)
    {
        virtual const FShaderInputLayoutElement* GetElements(u32* out_count) noexcept = 0;

        #if FFI_SRC
        std::span<const FShaderInputLayoutElement> GetElements() noexcept
        {
            u32 count{};
            return std::span{GetElements(&count), static_cast<usize>(count)};
        }
        #endif
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FMeshLayoutCreateOptions
    {
        FStr8or16 Name{};
        // Buffers 按顺序定义到 InputSlot，不允许随机隔空的 InputSlot
        FMeshBufferDefine* Buffers{};
        FMeshBufferElement* Elements{};
        u32 BufferCount{};
        u32 ElementCount{};
    };

    COPLT_INTERFACE_DEFINE(FMeshLayout, "8fe5121f-c2ce-46f5-aa14-f28595f35361", FGpuObject)
    {
        virtual const FMeshBufferDefine* GetBuffers(u32* out_count) const noexcept = 0;
        virtual const FMeshBufferElement* GetElements(u32* out_count) const noexcept = 0;

        virtual const FMeshBufferElement* TryGetElement(u32 SlotId, u32 SlotIndex) const noexcept = 0;

        #if FFI_SRC
        std::span<const FMeshBufferDefine> GetBuffers() const noexcept
        {
            u32 count{};
            return std::span{GetBuffers(&count), static_cast<usize>(count)};
        }

        std::span<const FMeshBufferElement> GetElements() const noexcept
        {
            u32 count{};
            return std::span{GetElements(&count), static_cast<usize>(count)};
        }
        #endif
    };
}
