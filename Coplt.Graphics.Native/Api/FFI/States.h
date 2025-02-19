#pragma once

#include "Common.h"
#include "Types.h"

namespace Coplt
{
    COPLT_ENUM_FLAGS(FResourceState, u32)
    {
        // 手动管理资源状态，自动过渡时将忽略
        Manual = 0,
        Common = 1 << 0,
        Present = 1 << 1,
        VertexBuffer = 1 << 2,
        IndexBuffer = 1 << 3,
        ConstantBuffer = 1 << 4,
        IndirectBuffer = 1 << 5,
        RenderTarget = 1 << 6,
        DepthRead = 1 << 7,
        DepthWrite = 1 << 8,
        ShaderResource = 1 << 9,
        UnorderedAccess = 1 << 10,
        CopySrc = 1 << 11,
        CopyDst = 1 << 12,
        ResolveSrc = 1 << 13,
        ResolveDst = 1 << 14,
        RayTracing = 1 << 15,
        ShadingRate = 1 << 16,
        StreamOutput = 1 << 17,

        GenericRead = VertexBuffer | ConstantBuffer | ShaderResource | IndirectBuffer | CopySrc,
    };

#ifdef FFI_SRC
    inline bool ContainsOnly(const FResourceState value, const FResourceState other)
    {
        return (value & ~other) == 0;
    }

    inline bool Contains(const FResourceState value, const FResourceState other)
    {
        return (value & other) != 0;
    }

    inline bool IsReadOnly(const FResourceState value)
    {
        return ContainsOnly(
            value,
            FResourceState::IndexBuffer | FResourceState::VertexBuffer |
            FResourceState::ConstantBuffer | FResourceState::IndirectBuffer | FResourceState::CopySrc |
            FResourceState::ResolveSrc | FResourceState::ShaderResource | FResourceState::DepthRead
        );
    }

    inline bool IsCompatible(const FResourceState value, const FResourceState other)
    {
        if (value == FResourceState::Manual || other == FResourceState::Manual) return false;
        if (value == other) return true;
        if (IsReadOnly(value) && IsReadOnly(other)) return true;
        return false;
    }

    inline FResourceState ChangeState(const FResourceState value, const FResourceState state)
    {
        if (IsCompatible(value, state)) return value | state;
        return state;
    }
#endif
};
