#pragma once

#include "Object.h"
#include "Shader.h"

#if FFI_SRC
#include <span>
#endif

namespace Coplt
{
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

    enum class FShaderLayoutItemType : u8
    {
        ConstantBuffer,
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

    enum class FShaderLayoutItemUsage : u8
    {
        // 一般改变频率的变量，可能每帧都会改变
        Common,
        // 不太改变的变量，例如材质参数，一般很少改变，可以进行一定的静态优化，建议将所有材质绑定放到单独的 space 中区分
        Material,
        // 经常改变的变量，例如每次绘制调用都会改变, dx 后端将直接在根签名内，类型是 Sampler 时表示是静态采样器
        Instant,
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
        Str8or16 Name{};
        u32 Count{};
        FShaderLayoutItemDefine* Items{};
        // todo 静态采样器

        FShaderLayoutFlags Flags{};
    };

    COPLT_INTERFACE_DEFINE(FShaderLayout, "552a498e-8f3a-47ff-a335-7af2de0901e8", FGpuObject)
    {
        FShaderLayoutFlags Flags{};

        virtual const FShaderLayoutItemDefine* GetItemDefines(u32* out_count) noexcept = 0;

#if FFI_SRC
        std::span<const FShaderLayoutItemDefine> GetItemDefines() noexcept
        {
            u32 count{};
            return std::span{GetItemDefines(&count), static_cast<usize>(count)};
        }
#endif
    };

    struct FShaderInputLayoutCreateOptions
    {
        Str8or16 Name{};
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
        Str8or16 Name{};
        // Buffers 按顺序定义到 InputSlot，不允许随机隔空的 InputSlot
        FMeshBufferDefine* Buffers{};
        FMeshBufferElement* Elements{};
        u32 BufferCount{};
        u32 ElementCount{};
    };

    COPLT_INTERFACE_DEFINE(FMeshLayout, "8fe5121f-c2ce-46f5-aa14-f28595f35361", FGpuObject)
    {
        virtual const FMeshBufferDefine* GetBuffers(u32* out_count) const  noexcept = 0;
        virtual const FMeshBufferElement* GetElements(u32* out_count) const  noexcept = 0;

        virtual const FMeshBufferElement* TryGetElement(u32 SlotId, u32 SlotIndex) const  noexcept = 0;

#if FFI_SRC
        std::span<const FMeshBufferDefine> GetBuffers() const noexcept
        {
            u32 count{};
            return std::span{GetBuffers(&count), static_cast<usize>(count)};
        }

        std::span<const FMeshBufferElement> GetElements() const  noexcept
        {
            u32 count{};
            return std::span{GetElements(&count), static_cast<usize>(count)};
        }
#endif
    };
}
