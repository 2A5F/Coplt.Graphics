#pragma once

#include "Object.h"
#include "Shader.h"

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
    };

    enum class FShaderLayoutGroupView : u8
    {
        Cbv,
        Srv,
        Uav,
        Sampler,
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

    enum class FShaderLayoutItemUsage : u8
    {
        // 动态变量，每帧都可能会改变
        Dynamic,
        // 持久变量，例如材质参数，一般很少改变，可以进行一定的静态优化，建议将所有材质绑定放到单独的 space 中区分
        Persist,
        // 即时变量，例如每次绘制调用都会改变, dx 后端将直接在根签名内，类型是 Sampler 时表示是静态采样器，不支持纹理
        Instant,
    };

    enum class FShaderLayoutGroupScope : u8
    {
        Dynamic,
        Persist,
    };

    struct FShaderLayoutItemDefine
    {
        // dx 后端无论什么时候都是 register, vk 后端一般情况是 binding，类型为 Constants 时是 push const 的 offset，为字节偏移
        u32 Slot{};
        u32 Space{};
        // 类型是 Sampler并且 Usage 是 Static 时是静态采样器描述的索引；其他类型表示数量
        u32 CountOrIndex{};
        FGraphicsFormat Format{};
        FShaderStage Stage{};
        FShaderLayoutItemView View{};
        FShaderLayoutItemType Type{};
        FShaderLayoutItemUsage Usage{};
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

    enum class FShaderLayoutItemPlace : u8
    {
        None,
        Const,
        Direct,
        Grouped,
        StaticSampler,
    };

    struct FShaderLayoutItemInfo
    {
        // Const | Direct 时是 Root Index，Grouped 时是 Group 内的 Index
        u32 Index{};
        // 所属哪个绑定组类
        u32 Class{};
        // 所属哪个绑定组
        u32 Group{};
        // 绑定放在哪
        FShaderLayoutItemPlace Place{};
    };

    struct FShaderLayoutGroupInfo
    {
        // Root Index
        u32 Index{};
        // 包含多少个绑定
        u32 Size{};
        FShaderStage Stage{};
        FShaderLayoutGroupView View{};
    };

    struct FShaderLayoutGroupClass
    {
        FShaderLayoutGroupInfo* Infos{};
        u32 Size{};
        FShaderLayoutGroupScope Scope{};
        b8 Sampler{};

        #if FFI_SRC
        std::span<const FShaderLayoutGroupInfo> AsSpan() const noexcept
        {
            return std::span{Infos, static_cast<usize>(Size)};
        }
        #endif
    };

    COPLT_ENUM_FLAGS(FShaderLayoutFlags, u8)
    {
        None = 0,
        // 是否启用无绑定
        BindLess = 1 << 0,
        // 是否启用输入组装
        InputAssembler = 1 << 1,
        // 是否启用流输出
        StreamOutput = 1 << 2,
    };

    struct FShaderLayoutCreateOptions
    {
        FStr8or16 Name{};
        u32 Count{};
        FShaderLayoutItemDefine* Items{};
        // todo 静态采样器

        FShaderLayoutFlags Flags{};
    };

    struct FGetEmptyShaderLayoutOptions
    {
        FShaderLayoutFlags Flags{};
    };

    struct FShaderLayoutData
    {
        FShaderLayoutFlags Flags{};
    };

    COPLT_INTERFACE_DEFINE(FShaderLayout, "552a498e-8f3a-47ff-a335-7af2de0901e8", FGpuObject)
    {
        virtual FShaderLayoutData* ShaderLayoutData() noexcept = 0;
        virtual const FShaderLayoutItemDefine* GetItemDefines(u32* out_count) noexcept = 0;
        virtual const FShaderLayoutItemInfo* GetItemInfos(u32* out_count) noexcept = 0;
        virtual const FShaderLayoutGroupClass* GetGroupClasses(u32* out_count) noexcept = 0;

        #if FFI_SRC
        std::span<const FShaderLayoutItemDefine> GetItemDefines() noexcept
        {
            u32 count{};
            return std::span{GetItemDefines(&count), static_cast<usize>(count)};
        }

        std::span<const FShaderLayoutItemInfo> GetItemInfos() noexcept
        {
            u32 count{};
            return std::span{GetItemInfos(&count), static_cast<usize>(count)};
        }

        std::span<const FShaderLayoutGroupClass> GetGroupClasses() noexcept
        {
            u32 count{};
            return std::span{GetGroupClasses(&count), static_cast<usize>(count)};
        }
        #endif
    };

    struct FShaderLayoutCreateResult
    {
        FShaderLayout* Layout{};
        FShaderLayoutData* Data{};
    };

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
