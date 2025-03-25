#include "../Include/ResState.h"

using namespace Coplt;

ResState::ResState(const ResAccess Access, const ResUsage Usage, const ResLayout Layout) : Access(Access), Usage(Usage), Layout(Layout)
{
}

bool ResState::IsCompatible(const ResState& New, const bool Enhanced) const
{
    if (Enhanced && Layout != New.Layout) return false;
    return Coplt::IsCompatible(Access, New.Access);
}

ResState ResState::Merge(const ResState& New) const
{
    return ResState(Access | New.Access, Usage | New.Usage, New.Layout);
}

ResState ResState::Split() const
{
    return ResState(Access, ResUsage::Common, Layout);
}

bool Coplt::IsValid(const ResAccess access)
{
    switch (access)
    {
    case ResAccess::None:
    case ResAccess::Common:
        return true;
    case ResAccess::UnorderedAccessRead:
    case ResAccess::UnorderedAccessWrite:
    case ResAccess::UnorderedAccessRead | ResAccess::UnorderedAccessWrite:
        return HasFlagsOnly(access, ResAccess::UnorderedAccessRead | ResAccess::UnorderedAccessWrite);
    case ResAccess::RayTracingAccelerationStructureRead | ResAccess::RayTracingAccelerationStructureWrite:
        return false;
    case ResAccess::RayTracingAccelerationStructureRead:
    case ResAccess::RayTracingAccelerationStructureWrite:
        return HasFlagsOnly(access, ResAccess::RayTracingAccelerationStructureRead | ResAccess::RayTracingAccelerationStructureWrite);
    case ResAccess::RenderTargetWrite:
    case ResAccess::DepthStencilWrite:
    case ResAccess::StreamOutputWrite:
    case ResAccess::CopyDestWrite:
    case ResAccess::ResolveDestWrite:
    case ResAccess::VideoEncodeWrite:
    case ResAccess::VideoDecodeWrite:
    case ResAccess::VideoProcessWrite:
        return true;
    default:
        break;
    }
    if (HasFlagsOnly(
        access,
        ResAccess::VertexBufferRead | ResAccess::ConstantBufferRead | ResAccess::IndexBufferRead |
        ResAccess::DepthStencilRead | ResAccess::ShaderResourceRead | ResAccess::IndirectArgumentRead |
        ResAccess::CopySourceRead | ResAccess::ResolveSourceRead | ResAccess::ShadingRateSourceRead |
        ResAccess::VideoEncodeRead | ResAccess::VideoDecodeRead | ResAccess::VideoProcessRead
    ))
        return true;
    return false;
}

ResUseType Coplt::GetUseType(const ResAccess access)
{
    auto usage = ResUseType::None;
    if (access == ResAccess::Common) return ResUseType::ReadWrite;
    if (HasAnyFlags(
        access,
        ResAccess::VertexBufferRead | ResAccess::ConstantBufferRead | ResAccess::IndexBufferRead | ResAccess::UnorderedAccessRead |
        ResAccess::DepthStencilRead | ResAccess::ShaderResourceRead | ResAccess::IndirectArgumentRead | ResAccess::CopySourceRead |
        ResAccess::ResolveSourceRead | ResAccess::RayTracingAccelerationStructureRead | ResAccess::ShadingRateSourceRead |
        ResAccess::VideoEncodeRead | ResAccess::VideoDecodeRead | ResAccess::VideoProcessRead
    ))
        usage = ResUseType::Read;
    if (HasAnyFlags(
        access,
        ResAccess::RenderTargetWrite | ResAccess::UnorderedAccessWrite | ResAccess::DepthStencilWrite | ResAccess::StreamOutputWrite |
        ResAccess::CopyDestWrite | ResAccess::ResolveDestWrite | ResAccess::RayTracingAccelerationStructureWrite |
        ResAccess::VideoEncodeWrite | ResAccess::VideoDecodeWrite | ResAccess::VideoProcessWrite
    ))
        usage = ResUseType::Write;
    return usage;
}

bool Coplt::IsReadOnly(const ResAccess access)
{
    return HasFlagsOnly(
        access,
        ResAccess::VertexBufferRead | ResAccess::ConstantBufferRead | ResAccess::IndexBufferRead | ResAccess::UnorderedAccessRead |
        ResAccess::DepthStencilRead | ResAccess::ShaderResourceRead | ResAccess::IndirectArgumentRead | ResAccess::CopySourceRead |
        ResAccess::ResolveSourceRead | ResAccess::RayTracingAccelerationStructureRead | ResAccess::ShadingRateSourceRead |
        ResAccess::VideoEncodeRead | ResAccess::VideoDecodeRead | ResAccess::VideoProcessRead
    );
}

bool Coplt::IsCompatible(const ResAccess Old, const ResAccess New)
{
    switch (Old)
    {
    case ResAccess::None:
        return true;
    case ResAccess::UnorderedAccessRead:
    case ResAccess::RayTracingAccelerationStructureRead:
        return false;
    default:
        if (IsReadOnly(Old) && IsReadOnly(New)) return true;
        return false;
    }
}

D3D12_RESOURCE_STATES Coplt::GetResourceState(const ResAccess access)
{
    D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
    switch (access)
    {
    case ResAccess::None:
    case ResAccess::Common:
        return D3D12_RESOURCE_STATE_COMMON;
    case ResAccess::RenderTargetWrite:
        return D3D12_RESOURCE_STATE_RENDER_TARGET;
    case ResAccess::DepthStencilWrite:
        return D3D12_RESOURCE_STATE_DEPTH_WRITE;
    case ResAccess::UnorderedAccessRead | ResAccess::UnorderedAccessWrite:
    case ResAccess::UnorderedAccessRead:
    case ResAccess::UnorderedAccessWrite:
        return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    case ResAccess::StreamOutputWrite:
        return D3D12_RESOURCE_STATE_STREAM_OUT;
    case ResAccess::CopyDestWrite:
        return D3D12_RESOURCE_STATE_COPY_DEST;
    case ResAccess::ResolveDestWrite:
        return D3D12_RESOURCE_STATE_RESOLVE_DEST;
    case ResAccess::RayTracingAccelerationStructureRead:
    case ResAccess::RayTracingAccelerationStructureWrite:
        return D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
    case ResAccess::VideoEncodeWrite:
        return D3D12_RESOURCE_STATE_VIDEO_ENCODE_WRITE;
    case ResAccess::VideoDecodeWrite:
        return D3D12_RESOURCE_STATE_VIDEO_DECODE_WRITE;
    case ResAccess::VideoProcessWrite:
        return D3D12_RESOURCE_STATE_VIDEO_PROCESS_WRITE;
    default:
        break;
    }
    if (HasAnyFlags(access, ResAccess::VertexBufferRead | ResAccess::ConstantBufferRead))
        state |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if (HasAnyFlags(access, ResAccess::IndexBufferRead))
        state |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
    if (HasAnyFlags(access, ResAccess::DepthStencilRead))
        state |= D3D12_RESOURCE_STATE_DEPTH_READ;
    if (HasAnyFlags(access, ResAccess::ShaderResourceRead))
        state |= D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
    if (HasAnyFlags(access, ResAccess::IndirectArgumentRead))
        state |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
    if (HasAnyFlags(access, ResAccess::CopySourceRead))
        state |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if (HasAnyFlags(access, ResAccess::ResolveSourceRead))
        state |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
    if (HasAnyFlags(access, ResAccess::ShadingRateSourceRead))
        state |= D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
    if (HasAnyFlags(access, ResAccess::VideoEncodeRead))
        state |= D3D12_RESOURCE_STATE_VIDEO_ENCODE_READ;
    if (HasAnyFlags(access, ResAccess::VideoDecodeRead))
        state |= D3D12_RESOURCE_STATE_VIDEO_DECODE_READ;
    if (HasAnyFlags(access, ResAccess::VideoProcessRead))
        state |= D3D12_RESOURCE_STATE_VIDEO_PROCESS_READ;
    return state;
}

D3D12_BARRIER_ACCESS Coplt::GetBarrierAccess(const ResAccess access)
{
    D3D12_BARRIER_ACCESS ba = D3D12_BARRIER_ACCESS_COMMON;
    switch (access)
    {
    case ResAccess::None:
        return D3D12_BARRIER_ACCESS_NO_ACCESS;
    case ResAccess::Common:
        return D3D12_BARRIER_ACCESS_COMMON;
    case ResAccess::RenderTargetWrite:
        return D3D12_BARRIER_ACCESS_RENDER_TARGET;
    case ResAccess::DepthStencilWrite:
        return D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
    case ResAccess::UnorderedAccessRead | ResAccess::UnorderedAccessWrite:
    case ResAccess::UnorderedAccessRead:
    case ResAccess::UnorderedAccessWrite:
        return D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
    case ResAccess::StreamOutputWrite:
        return D3D12_BARRIER_ACCESS_STREAM_OUTPUT;
    case ResAccess::CopyDestWrite:
        return D3D12_BARRIER_ACCESS_COPY_DEST;
    case ResAccess::ResolveDestWrite:
        return D3D12_BARRIER_ACCESS_RESOLVE_DEST;
    case ResAccess::RayTracingAccelerationStructureRead:
        return D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
    case ResAccess::RayTracingAccelerationStructureWrite:
        return D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
    case ResAccess::VideoEncodeWrite:
        return D3D12_BARRIER_ACCESS_VIDEO_ENCODE_WRITE;
    case ResAccess::VideoDecodeWrite:
        return D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE;
    case ResAccess::VideoProcessWrite:
        return D3D12_BARRIER_ACCESS_VIDEO_PROCESS_WRITE;
    default:
        break;
    }
    if (HasAnyFlags(access, ResAccess::VertexBufferRead))
        ba |= D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
    if (HasAnyFlags(access, ResAccess::ConstantBufferRead))
        ba |= D3D12_BARRIER_ACCESS_CONSTANT_BUFFER;
    if (HasAnyFlags(access, ResAccess::IndexBufferRead))
        ba |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
    if (HasAnyFlags(access, ResAccess::DepthStencilRead))
        ba |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
    if (HasAnyFlags(access, ResAccess::ShaderResourceRead))
        ba |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
    if (HasAnyFlags(access, ResAccess::IndirectArgumentRead))
        ba |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
    if (HasAnyFlags(access, ResAccess::CopySourceRead))
        ba |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
    if (HasAnyFlags(access, ResAccess::ResolveSourceRead))
        ba |= D3D12_BARRIER_ACCESS_RESOLVE_SOURCE;
    if (HasAnyFlags(access, ResAccess::ShadingRateSourceRead))
        ba |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
    if (HasAnyFlags(access, ResAccess::VideoEncodeRead))
        ba |= D3D12_BARRIER_ACCESS_VIDEO_ENCODE_READ;
    if (HasAnyFlags(access, ResAccess::VideoDecodeRead))
        ba |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_READ;
    if (HasAnyFlags(access, ResAccess::VideoProcessRead))
        ba |= D3D12_BARRIER_ACCESS_VIDEO_PROCESS_READ;
    return ba;
}

D3D12_BARRIER_LAYOUT Coplt::GetBarrierLayout(const ResLayout layout, const FGpuQueueType queue, const bool cross_queue)
{
    switch (layout)
    {
    case ResLayout::Undefined:
        return D3D12_BARRIER_LAYOUT_UNDEFINED;
    case ResLayout::RenderTarget:
        return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
    case ResLayout::DepthStencilWrite:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
    case ResLayout::DepthStencilRead:
        return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
    case ResLayout::ResolveSource:
        return D3D12_BARRIER_LAYOUT_RESOLVE_SOURCE;
    case ResLayout::ResolveDest:
        return D3D12_BARRIER_LAYOUT_RESOLVE_DEST;
    case ResLayout::ShaderRateSource:
        return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
    case ResLayout::VideoEncodeRead:
        return D3D12_BARRIER_LAYOUT_VIDEO_ENCODE_READ;
    case ResLayout::VideoEncodeWrite:
        return D3D12_BARRIER_LAYOUT_VIDEO_ENCODE_WRITE;
    case ResLayout::VideoDecodeRead:
        return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_READ;
    case ResLayout::VideoDecodeWrite:
        return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_WRITE;
    case ResLayout::VideoProcessRead:
        return D3D12_BARRIER_LAYOUT_VIDEO_PROCESS_READ;
    case ResLayout::VideoProcessWrite:
        return D3D12_BARRIER_LAYOUT_VIDEO_PROCESS_WRITE;
    case ResLayout::GenericRead:
        if (!cross_queue)
        {
            switch (queue)
            {
            case FGpuQueueType::Direct:
                return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_GENERIC_READ;
            case FGpuQueueType::Compute:
                return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_GENERIC_READ;
            default:
                break;
            }
        }
        return D3D12_BARRIER_LAYOUT_GENERIC_READ;
    case ResLayout::UnorderedAccess:
        if (!cross_queue)
        {
            switch (queue)
            {
            case FGpuQueueType::Direct:
                return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_UNORDERED_ACCESS;
            case FGpuQueueType::Compute:
                return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_UNORDERED_ACCESS;
            default:
                break;
            }
        }
        return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
    case ResLayout::ShaderResource:
        if (!cross_queue)
        {
            switch (queue)
            {
            case FGpuQueueType::Direct:
                return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_SHADER_RESOURCE;
            case FGpuQueueType::Compute:
                return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_SHADER_RESOURCE;
            default:
                break;
            }
        }
        return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
    case ResLayout::CopySource:
        if (!cross_queue)
        {
            switch (queue)
            {
            case FGpuQueueType::Direct:
                return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_SOURCE;
            case FGpuQueueType::Compute:
                return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_SOURCE;
            default:
                break;
            }
        }
        return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
    case ResLayout::CopyDest:
        if (!cross_queue)
        {
            switch (queue)
            {
            case FGpuQueueType::Direct:
                return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_DEST;
            case FGpuQueueType::Compute:
                return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_DEST;
            default:
                break;
            }
        }
        return D3D12_BARRIER_LAYOUT_COPY_DEST;
    case ResLayout::Common:
    default:
        if (!cross_queue)
        {
            switch (queue)
            {
            case FGpuQueueType::Direct:
                return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COMMON;
            case FGpuQueueType::Compute:
                return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COMMON;
            case FGpuQueueType::VideoEncode:
            case FGpuQueueType::VideoDecode:
            case FGpuQueueType::VideoProcess:
                return D3D12_BARRIER_LAYOUT_VIDEO_QUEUE_COMMON;
            default:
                break;
            }
        }
        return D3D12_BARRIER_LAYOUT_COMMON;
    }
}

D3D12_BARRIER_SYNC Coplt::GetBarrierSync(const ResAccess access, const ResUsage usage)
{
    D3D12_BARRIER_SYNC sync = D3D12_BARRIER_SYNC_NONE;
    switch (access)
    {
    case ResAccess::None:
        return D3D12_BARRIER_SYNC_NONE;
    case ResAccess::Common:
        return D3D12_BARRIER_SYNC_ALL;
    default:
        break;
    }

    if (HasAnyFlags(access, ResAccess::IndexBufferRead))
        sync |= D3D12_BARRIER_SYNC_INDEX_INPUT;

    if (HasAnyFlags(usage, ResUsage::VertexOrMesh))
        sync |= D3D12_BARRIER_SYNC_VERTEX_SHADING;

    if (HasAnyFlags(usage, ResUsage::Pixel))
        sync |= D3D12_BARRIER_SYNC_PIXEL_SHADING;

    if (HasAnyFlags(access, ResAccess::RenderTargetWrite))
        sync |= D3D12_BARRIER_SYNC_RENDER_TARGET;

    if (HasAnyFlags(access, ResAccess::DepthStencilRead | ResAccess::DepthStencilWrite))
        sync |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;

    if (HasAnyFlags(usage, ResUsage::Compute))
        sync |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;

    if (HasAnyFlags(usage, ResUsage::RayTracing))
        sync |= D3D12_BARRIER_SYNC_RAYTRACING;

    if (HasAnyFlags(access, ResAccess::CopySourceRead | ResAccess::CopyDestWrite))
        sync |= D3D12_BARRIER_SYNC_COPY;

    if (HasAnyFlags(access, ResAccess::ResolveSourceRead | ResAccess::ResolveDestWrite))
        sync |= D3D12_BARRIER_SYNC_RESOLVE;

    if (HasAnyFlags(access, ResAccess::IndirectArgumentRead))
        sync |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;

    if (HasAnyFlags(access, ResAccess::VideoEncodeRead | ResAccess::VideoEncodeWrite))
        sync |= D3D12_BARRIER_SYNC_VIDEO_ENCODE;

    if (HasAnyFlags(access, ResAccess::VideoDecodeRead | ResAccess::VideoDecodeWrite))
        sync |= D3D12_BARRIER_SYNC_VIDEO_DECODE;

    if (HasAnyFlags(access, ResAccess::VideoProcessRead | ResAccess::VideoProcessWrite))
        sync |= D3D12_BARRIER_SYNC_VIDEO_PROCESS;

    if (HasAnyFlags(usage, ResUsage::BuildRayTracingAccelerationStructure))
        sync |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE;

    if (HasAnyFlags(usage, ResUsage::CopyRayTracingAccelerationStructure))
        sync |= D3D12_BARRIER_SYNC_COPY_RAYTRACING_ACCELERATION_STRUCTURE;

    if (HasAnyFlags(usage, ResUsage::EmitRayTracingAccelerationStructurePostBuildInfo))
        sync |= D3D12_BARRIER_SYNC_EMIT_RAYTRACING_ACCELERATION_STRUCTURE_POSTBUILD_INFO;

    if (HasAnyFlags(usage, ResUsage::ClearUav))
        sync |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;

    return sync;
}
