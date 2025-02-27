#pragma once

#include "Common.h"
#include "Layout.h"
#include "Types.h"

namespace Coplt
{
    #pragma region LegacyState

    COPLT_ENUM_FLAGS(FLegacyState, u32)
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

    inline bool ContainsOnly(const FLegacyState value, const FLegacyState other)
    {
        return (value & ~other) == 0;
    }

    inline bool Contains(const FLegacyState value, const FLegacyState other)
    {
        return (value & other) != 0;
    }

    inline bool IsReadOnly(const FLegacyState value)
    {
        return ContainsOnly(
            value,
            FLegacyState::IndexBuffer | FLegacyState::VertexBuffer |
            FLegacyState::ConstantBuffer | FLegacyState::IndirectBuffer | FLegacyState::CopySrc |
            FLegacyState::ResolveSrc | FLegacyState::ShaderResource | FLegacyState::DepthRead
        );
    }

    inline bool IsCompatible(const FLegacyState value, const FLegacyState other)
    {
        if (value == FLegacyState::Manual || other == FLegacyState::Manual)
            return false;
        if (value == other)
            return true;
        if (IsReadOnly(value) && IsReadOnly(other))
            return true;
        return false;
    }

    inline FLegacyState ChangeState(const FLegacyState value, const FLegacyState state)
    {
        if (IsCompatible(value, state))
            return value | state;
        return state;
    }
    #endif

    #pragma endregion

    enum FResLayout: u32
    {
        None = 0,
        Common,
        Present = Common,
        GenericRead,
        RenderTarget,
        UnorderedAccess,
        DepthStencilRead,
        DepthStencilWrite,
        ShaderResource,
        CopySrc,
        CopyDst,
        ResolveSrc,
        ResolveDst,
        ShadingRate,
        VideoDecodeRead,
        VideoDecodeWrite,
        VideoProcessRead,
        VideoProcessWrite,
        VideoEncodeRead,
        VideoEncodeWrite,
    };

    COPLT_ENUM_FLAGS(FResAccess, u32)
    {
        NoAccess = 1u << 31,
        Common = 0,
        ConstantBuffer = 1 << 0,
        VertexBuffer = 1 << 1,
        IndexBuffer = 1 << 2,
        RenderTarget = 1 << 3,
        UnorderedAccess = 1 << 4,
        DepthStencilRead = 1 << 5,
        DepthStencilWrite = 1 << 6,
        ShaderResource = 1 << 7,
        StreamOutput = 1 << 8,
        IndirectOrPredicationBuffer = 1 << 9,
        CopySrc = 1 << 10,
        CopyDst = 1 << 11,
        ResolveSrc = 1 << 12,
        ResolveDst = 1 << 13,
        RayTracingAccelerationStructureRead = 1 << 14,
        RayTracingAccelerationStructureWrite = 1 << 15,
        ShadingRate = 1 << 16,
        VideoDecodeRead = 1 << 17,
        VideoDecodeWrite = 1 << 18,
        VideoProcessRead = 1 << 19,
        VideoProcessWrite = 1 << 20,
        VideoEncodeRead = 1 << 21,
        VideoEncodeWrite = 1 << 22,
    };

    struct FResState
    {
        FResLayout Layout{};
        FResAccess Access{};
        FShaderStageFlags Stages{};
    };
}
