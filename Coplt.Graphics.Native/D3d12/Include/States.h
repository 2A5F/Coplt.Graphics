#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/Queue.h"

namespace Coplt
{
    constexpr D3D12_RESOURCE_STATES AllReadStates =
        D3D12_RESOURCE_STATE_GENERIC_READ | D3D12_RESOURCE_STATE_DEPTH_READ |
        D3D12_RESOURCE_STATE_RESOLVE_SOURCE | D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;

    inline bool ContainsOnly(const D3D12_RESOURCE_STATES value, const D3D12_RESOURCE_STATES other)
    {
        return (value & ~other) == 0;
    }

    inline bool Contains(const D3D12_RESOURCE_STATES value, const D3D12_RESOURCE_STATES other)
    {
        return (value & other) != 0;
    }

    inline bool IsReadOnly(const D3D12_RESOURCE_STATES value)
    {
        return ContainsOnly(value, AllReadStates);
    }

    inline bool IsCompatible(const D3D12_RESOURCE_STATES value, const D3D12_RESOURCE_STATES other)
    {
        if (value == other) return true;
        if (IsReadOnly(value) && IsReadOnly(other)) return true;
        return false;
    }

    inline D3D12_RESOURCE_STATES ChangeState(const D3D12_RESOURCE_STATES value, const D3D12_RESOURCE_STATES state)
    {
        if (IsCompatible(value, state)) return value | state;
        return state;
    }

    inline D3D12_RESOURCE_STATES ToDx(const FLegacyState state)
    {
        switch (state)
        {
        case FLegacyState::Common:
        case FLegacyState::Present:
            return D3D12_RESOURCE_STATE_COMMON;
        case FLegacyState::RenderTarget:
            return D3D12_RESOURCE_STATE_RENDER_TARGET;
        case FLegacyState::DepthWrite:
            return D3D12_RESOURCE_STATE_DEPTH_WRITE;
        case FLegacyState::UnorderedAccess:
            return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
        case FLegacyState::CopyDst:
            return D3D12_RESOURCE_STATE_COPY_DEST;
        case FLegacyState::ResolveDst:
            return D3D12_RESOURCE_STATE_RESOLVE_DEST;
        case FLegacyState::RayTracing:
            return D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
        case FLegacyState::ShadingRate:
            return D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
        case FLegacyState::StreamOutput:
            return D3D12_RESOURCE_STATE_STREAM_OUT;
        default:
            break;
        }
        D3D12_RESOURCE_STATES r = D3D12_RESOURCE_STATE_COMMON;
        if (HasAnyFlags(state, FLegacyState::ConstantBuffer | FLegacyState::VertexBuffer))
            r |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        if (HasFlags(state, FLegacyState::IndexBuffer)) r |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
        if (HasFlags(state, FLegacyState::IndirectBuffer)) r |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
        if (HasFlags(state, FLegacyState::DepthRead)) r |= D3D12_RESOURCE_STATE_DEPTH_READ;
        if (HasFlags(state, FLegacyState::ShaderResource)) r |= D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
        if (HasFlags(state, FLegacyState::CopySrc)) r |= D3D12_RESOURCE_STATE_COPY_SOURCE;
        if (HasFlags(state, FLegacyState::ResolveSrc)) r |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
        return r;
    }

    inline D3D12_BARRIER_LAYOUT ToDx(const FResLayout value, const std::optional<FGpuQueueType> queue)
    {
        if (queue.has_value())
        {
            switch (queue.value())
            {
            case FGpuQueueType::Direct:
                switch (value)
                {
                case FResLayout::Common:
                    return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COMMON;
                case FResLayout::GenericRead:
                    return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_GENERIC_READ;
                case FResLayout::UnorderedAccess:
                    return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_UNORDERED_ACCESS;
                case FResLayout::ShaderResource:
                    return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_SHADER_RESOURCE;
                case FResLayout::CopySrc:
                    return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_SOURCE;
                case FResLayout::CopyDst:
                    return D3D12_BARRIER_LAYOUT_DIRECT_QUEUE_COPY_DEST;
                default:
                    break;
                }
                break;
            case FGpuQueueType::Compute:
                switch (value)
                {
                case FResLayout::Common:
                    return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COMMON;
                case FResLayout::GenericRead:
                    return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_GENERIC_READ;
                case FResLayout::UnorderedAccess:
                    return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_UNORDERED_ACCESS;
                case FResLayout::ShaderResource:
                    return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_SHADER_RESOURCE;
                case FResLayout::CopySrc:
                    return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_SOURCE;
                case FResLayout::CopyDst:
                    return D3D12_BARRIER_LAYOUT_COMPUTE_QUEUE_COPY_DEST;
                default:
                    break;
                }
                break;
            case FGpuQueueType::Copy:
                break;
            case FGpuQueueType::Video:
                switch (value)
                {
                case FResLayout::Common:
                    return D3D12_BARRIER_LAYOUT_VIDEO_QUEUE_COMMON;
                default:
                    break;
                }
                break;
            }
        }
        switch (value)
        {
        case FResLayout::None:
            return D3D12_BARRIER_LAYOUT_UNDEFINED;
        case FResLayout::Common:
            return D3D12_BARRIER_LAYOUT_COMMON;
        case FResLayout::GenericRead:
            return D3D12_BARRIER_LAYOUT_GENERIC_READ;
        case FResLayout::RenderTarget:
            return D3D12_BARRIER_LAYOUT_RENDER_TARGET;
        case FResLayout::UnorderedAccess:
            return D3D12_BARRIER_LAYOUT_UNORDERED_ACCESS;
        case FResLayout::DepthStencilRead:
            return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_READ;
        case FResLayout::DepthStencilWrite:
            return D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE;
        case FResLayout::ShaderResource:
            return D3D12_BARRIER_LAYOUT_SHADER_RESOURCE;
        case FResLayout::CopySrc:
            return D3D12_BARRIER_LAYOUT_COPY_SOURCE;
        case FResLayout::CopyDst:
            return D3D12_BARRIER_LAYOUT_COPY_DEST;
        case FResLayout::ResolveSrc:
            return D3D12_BARRIER_LAYOUT_RESOLVE_SOURCE;
        case FResLayout::ResolveDst:
            return D3D12_BARRIER_LAYOUT_RESOLVE_DEST;
        case FResLayout::ShadingRate:
            return D3D12_BARRIER_LAYOUT_SHADING_RATE_SOURCE;
        case FResLayout::VideoDecodeRead:
            return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_READ;
        case FResLayout::VideoDecodeWrite:
            return D3D12_BARRIER_LAYOUT_VIDEO_DECODE_WRITE;
        case FResLayout::VideoProcessRead:
            return D3D12_BARRIER_LAYOUT_VIDEO_PROCESS_READ;
        case FResLayout::VideoProcessWrite:
            return D3D12_BARRIER_LAYOUT_VIDEO_PROCESS_WRITE;
        case FResLayout::VideoEncodeRead:
            return D3D12_BARRIER_LAYOUT_VIDEO_ENCODE_READ;
        case FResLayout::VideoEncodeWrite:
            return D3D12_BARRIER_LAYOUT_VIDEO_ENCODE_WRITE;
        }
        return D3D12_BARRIER_LAYOUT_UNDEFINED;
    }

    inline D3D12_BARRIER_ACCESS ToDx(const FResAccess value)
    {
        if (HasFlags(value, FResAccess::NoAccess)) return D3D12_BARRIER_ACCESS_NO_ACCESS;
        D3D12_BARRIER_ACCESS r = D3D12_BARRIER_ACCESS_COMMON;
        if (HasFlags(value, FResAccess::ConstantBuffer)) r |= D3D12_BARRIER_ACCESS_CONSTANT_BUFFER;
        if (HasFlags(value, FResAccess::VertexBuffer)) r |= D3D12_BARRIER_ACCESS_VERTEX_BUFFER;
        if (HasFlags(value, FResAccess::IndexBuffer)) r |= D3D12_BARRIER_ACCESS_INDEX_BUFFER;
        if (HasFlags(value, FResAccess::RenderTarget)) r |= D3D12_BARRIER_ACCESS_RENDER_TARGET;
        if (HasFlags(value, FResAccess::UnorderedAccess)) r |= D3D12_BARRIER_ACCESS_UNORDERED_ACCESS;
        if (HasFlags(value, FResAccess::DepthStencilRead)) r |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_READ;
        if (HasFlags(value, FResAccess::DepthStencilWrite)) r |= D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE;
        if (HasFlags(value, FResAccess::ShaderResource)) r |= D3D12_BARRIER_ACCESS_SHADER_RESOURCE;
        if (HasFlags(value, FResAccess::StreamOutput)) r |= D3D12_BARRIER_ACCESS_STREAM_OUTPUT;
        if (HasFlags(value, FResAccess::IndirectOrPredicationBuffer)) r |= D3D12_BARRIER_ACCESS_INDIRECT_ARGUMENT;
        if (HasFlags(value, FResAccess::CopySrc)) r |= D3D12_BARRIER_ACCESS_COPY_SOURCE;
        if (HasFlags(value, FResAccess::CopyDst)) r |= D3D12_BARRIER_ACCESS_COPY_DEST;
        if (HasFlags(value, FResAccess::ResolveSrc)) r |= D3D12_BARRIER_ACCESS_RESOLVE_SOURCE;
        if (HasFlags(value, FResAccess::ResolveDst)) r |= D3D12_BARRIER_ACCESS_RESOLVE_DEST;
        if (HasFlags(value, FResAccess::RayTracingAccelerationStructureRead)) r |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_READ;
        if (HasFlags(value, FResAccess::RayTracingAccelerationStructureWrite)) r |= D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE;
        if (HasFlags(value, FResAccess::ShadingRate)) r |= D3D12_BARRIER_ACCESS_SHADING_RATE_SOURCE;
        if (HasFlags(value, FResAccess::VideoDecodeRead)) r |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_READ;
        if (HasFlags(value, FResAccess::VideoDecodeWrite)) r |= D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE;
        if (HasFlags(value, FResAccess::VideoProcessRead)) r |= D3D12_BARRIER_ACCESS_VIDEO_PROCESS_READ;
        if (HasFlags(value, FResAccess::VideoProcessWrite)) r |= D3D12_BARRIER_ACCESS_VIDEO_PROCESS_WRITE;
        if (HasFlags(value, FResAccess::VideoEncodeRead)) r |= D3D12_BARRIER_ACCESS_VIDEO_ENCODE_READ;
        if (HasFlags(value, FResAccess::VideoEncodeWrite)) r |= D3D12_BARRIER_ACCESS_VIDEO_ENCODE_WRITE;
        return r;
    }

    inline bool Diff(const FResAccess self, const FResAccess other, const FResAccess cmp)
    {
        return (self & ~other & cmp) != 0;
    }

    inline D3D12_BARRIER_SYNC CalcSync(
        const FShaderStageFlags stages,
        const FResAccess access,
        const FResAccess access2
    )
    {
        if (access == FResAccess::Common) return D3D12_BARRIER_SYNC_ALL;
        if (HasFlags(access, FResAccess::NoAccess)) return D3D12_BARRIER_SYNC_NONE;

        D3D12_BARRIER_SYNC r = D3D12_BARRIER_SYNC_NONE;

        constexpr auto ib_acc = FResAccess::IndexBuffer;
        if (Diff(access, access2, ib_acc))
            r |= D3D12_BARRIER_SYNC_INDEX_INPUT;

        constexpr auto vs_acc = FResAccess::VertexBuffer | FResAccess::ConstantBuffer
            | FResAccess::UnorderedAccess | FResAccess::ShaderResource | FResAccess::StreamOutput;
        if (
            Diff(access, access2, vs_acc)
            && HasAnyFlags(stages, FShaderStageFlags::Vertex | FShaderStageFlags::Mesh | FShaderStageFlags::Task)
        )
            r |= D3D12_BARRIER_SYNC_VERTEX_SHADING;

        constexpr auto ps_acc = FResAccess::ConstantBuffer | FResAccess::UnorderedAccess
            | FResAccess::ShadingRate | FResAccess::ShaderResource;
        if (
            Diff(access, access2, ps_acc)
            && HasAnyFlags(stages, FShaderStageFlags::Pixel)
        )
            r |= D3D12_BARRIER_SYNC_PIXEL_SHADING;

        constexpr auto ds_acc = FResAccess::DepthStencilRead | FResAccess::DepthStencilWrite;
        if (Diff(access, access2, ds_acc))
            r |= D3D12_BARRIER_SYNC_DEPTH_STENCIL;

        constexpr auto rt_acc = FResAccess::RenderTarget;
        if (Diff(access, access2, rt_acc))
            r |= D3D12_BARRIER_SYNC_RENDER_TARGET;

        constexpr auto cs_acc = FResAccess::RenderTarget;
        if (
            Diff(access, access2, cs_acc)
            && HasAnyFlags(stages, FShaderStageFlags::Compute)
        )
            r |= D3D12_BARRIER_SYNC_COMPUTE_SHADING;

        constexpr auto ray_acc = FResAccess::RayTracingAccelerationStructureRead | FResAccess::RayTracingAccelerationStructureWrite;
        if (Diff(access, access2, ray_acc))
            r |= D3D12_BARRIER_SYNC_RAYTRACING;

        constexpr auto copy_acc = FResAccess::CopySrc | FResAccess::CopyDst;
        if (Diff(access, access2, copy_acc))
            r |= D3D12_BARRIER_SYNC_COPY;

        constexpr auto resolve_acc = FResAccess::ResolveSrc | FResAccess::ResolveDst;
        if (Diff(access, access2, resolve_acc))
            r |= D3D12_BARRIER_SYNC_RESOLVE;

        constexpr auto ind_acc = FResAccess::IndirectOrPredicationBuffer;
        if (Diff(access, access2, ind_acc))
            r |= D3D12_BARRIER_SYNC_EXECUTE_INDIRECT;

        constexpr auto vd_acc = FResAccess::VideoDecodeRead | FResAccess::VideoDecodeWrite;
        if (Diff(access, access2, vd_acc))
            r |= D3D12_BARRIER_SYNC_VIDEO_DECODE;

        constexpr auto vp_acc = FResAccess::VideoProcessRead | FResAccess::VideoProcessWrite;
        if (Diff(access, access2, vp_acc))
            r |= D3D12_BARRIER_SYNC_VIDEO_PROCESS;

        constexpr auto ve_acc = FResAccess::VideoEncodeRead | FResAccess::VideoEncodeWrite;
        if (Diff(access, access2, ve_acc))
            r |= D3D12_BARRIER_SYNC_VIDEO_ENCODE;

        constexpr auto ray_build_acc = FResAccess::RayTracingAccelerationStructureRead;
        if (Diff(access, access2, ray_build_acc))
            r |= D3D12_BARRIER_SYNC_BUILD_RAYTRACING_ACCELERATION_STRUCTURE | D3D12_BARRIER_SYNC_COPY_RAYTRACING_ACCELERATION_STRUCTURE;

        constexpr auto clear_uav_acc = FResAccess::UnorderedAccess;
        if (Diff(access, access2, clear_uav_acc))
            r |= D3D12_BARRIER_SYNC_CLEAR_UNORDERED_ACCESS_VIEW;

        return r;
    }
}
