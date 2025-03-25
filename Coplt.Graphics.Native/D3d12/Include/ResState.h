#pragma once

#include <directx/d3d12.h>
#include "../../Api/FFI/Common.h"
#include "../../Api/FFI/Queue.h"

namespace Coplt
{
    COPLT_ENUM_FLAGS(ResUseType, u8)
    {
        None,
        Read = 1 << 0,
        Write = 1 << 1,
        ReadWrite = Read | Write,
    };

    COPLT_ENUM_FLAGS(ResAccess, u32)
    {
        None,
        Common = COPLT_U32_MAX,
        VertexBufferRead = 1 << 0,
        ConstantBufferRead = 1 << 1,
        IndexBufferRead = 1 << 2,
        RenderTargetWrite = 1 << 3,
        UnorderedAccessRead = 1 << 4,
        UnorderedAccessWrite = 1 << 5,
        DepthStencilRead = 1 << 6,
        DepthStencilWrite = 1 << 7,
        ShaderResourceRead = 1 << 8,
        StreamOutputWrite = 1 << 9,
        IndirectArgumentRead = 1 << 10,
        CopySourceRead = 1 << 11,
        CopyDestWrite = 1 << 12,
        ResolveSourceRead = 1 << 13,
        ResolveDestWrite = 1 << 14,
        RayTracingAccelerationStructureRead = 1 << 15,
        RayTracingAccelerationStructureWrite = 1 << 16,
        ShadingRateSourceRead = 1 << 17,
        VideoEncodeRead = 1 << 18,
        VideoEncodeWrite = 1 << 19,
        VideoDecodeRead = 1 << 20,
        VideoDecodeWrite = 1 << 21,
        VideoProcessRead = 1 << 22,
        VideoProcessWrite = 1 << 23,
    };

    COPLT_ENUM_FLAGS(ResUsage, u16)
    {
        Common = 0,
        VertexOrMesh = 1 << 0,
        Pixel = 1 << 1,
        Compute = 1 << 2,
        RayTracing = 1 << 3,
        BuildRayTracingAccelerationStructure = 1 << 4,
        CopyRayTracingAccelerationStructure = 1 << 5,
        EmitRayTracingAccelerationStructurePostBuildInfo = 1 << 6,
        ClearUav = 1 << 7,
    };

    enum class ResLayout : u16
    {
        Undefined = COPLT_U32_MAX,
        Common = 0,
        GenericRead,
        RenderTarget,
        UnorderedAccess,
        DepthStencilWrite,
        DepthStencilRead,
        ShaderResource,
        CopySource,
        CopyDest,
        ResolveSource,
        ResolveDest,
        ShaderRateSource,
        VideoEncodeRead,
        VideoEncodeWrite,
        VideoDecodeRead,
        VideoDecodeWrite,
        VideoProcessRead,
        VideoProcessWrite,
    };

    struct ResState
    {
        ResAccess Access{};
        ResUsage Usage{};
        ResLayout Layout{};

        ResState() = default;
        explicit ResState(ResAccess Access, ResUsage Usage, ResLayout Layout);

        bool IsCompatible(const ResState& New, bool Enhanced) const;
        ResState Merge(const ResState& New) const;
        ResState Split() const;
    };

    struct ResourceState
    {
        ResState State{};
        FGpuQueueType QueueType{};
        bool CrossQueue{};
    };

    bool IsValid(ResAccess access);

    ResUseType GetUseType(ResAccess access);

    bool IsReadOnly(ResAccess access);

    bool IsCompatible(ResAccess Old, ResAccess New);

    D3D12_RESOURCE_STATES GetResourceState(ResAccess access);

    D3D12_BARRIER_ACCESS GetBarrierAccess(ResAccess access);

    D3D12_BARRIER_LAYOUT GetBarrierLayout(ResLayout layout, FGpuQueueType queue, bool cross_queue);

    D3D12_BARRIER_SYNC GetBarrierSync(ResAccess access, ResUsage usage);
}
