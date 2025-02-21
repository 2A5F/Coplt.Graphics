#pragma once

#include <directx/d3d12.h>

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

    inline D3D12_RESOURCE_STATES ToDx(const FResourceState state)
    {
        switch (state)
        {
        case FResourceState::Common:
        case FResourceState::Present:
            return D3D12_RESOURCE_STATE_COMMON;
        case FResourceState::RenderTarget:
            return D3D12_RESOURCE_STATE_RENDER_TARGET;
        case FResourceState::DepthWrite:
            return D3D12_RESOURCE_STATE_DEPTH_WRITE;
        case FResourceState::UnorderedAccess:
            return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
        case FResourceState::CopyDst:
            return D3D12_RESOURCE_STATE_COPY_DEST;
        case FResourceState::ResolveDst:
            return D3D12_RESOURCE_STATE_RESOLVE_DEST;
        case FResourceState::RayTracing:
            return D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
        case FResourceState::ShadingRate:
            return D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
        case FResourceState::StreamOutput:
            return D3D12_RESOURCE_STATE_STREAM_OUT;
        default:
            break;
        }
        D3D12_RESOURCE_STATES r = D3D12_RESOURCE_STATE_COMMON;
        if (HasAnyFlags(state, FResourceState::ConstantBuffer | FResourceState::VertexBuffer))
            r |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        if (HasFlags(state, FResourceState::IndexBuffer)) r |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
        if (HasFlags(state, FResourceState::IndirectBuffer)) r |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
        if (HasFlags(state, FResourceState::DepthRead)) r |= D3D12_RESOURCE_STATE_DEPTH_READ;
        if (HasFlags(state, FResourceState::ShaderResource)) r |= D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
        if (HasFlags(state, FResourceState::CopySrc)) r |= D3D12_RESOURCE_STATE_COPY_SOURCE;
        if (HasFlags(state, FResourceState::ResolveSrc)) r |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
        return r;
    }
}
