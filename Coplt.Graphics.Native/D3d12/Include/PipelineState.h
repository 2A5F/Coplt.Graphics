#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/PipelineState.h"

namespace Coplt
{
    inline D3D12_BLEND ToDx(const FBlendType value)
    {
        switch (value)
        {
        case FBlendType::Zero:
            return D3D12_BLEND_ZERO;
        case FBlendType::One:
            return D3D12_BLEND_ONE;
        case FBlendType::SrcColor:
            return D3D12_BLEND_SRC_COLOR;
        case FBlendType::InvSrcColor:
            return D3D12_BLEND_INV_SRC_COLOR;
        case FBlendType::SrcAlpha:
            return D3D12_BLEND_SRC_ALPHA;
        case FBlendType::InvSrcAlpha:
            return D3D12_BLEND_INV_SRC_ALPHA;
        case FBlendType::DstAlpha:
            return D3D12_BLEND_DEST_ALPHA;
        case FBlendType::InvDstAlpha:
            return D3D12_BLEND_INV_DEST_ALPHA;
        case FBlendType::DstColor:
            return D3D12_BLEND_DEST_COLOR;
        case FBlendType::InvDstColor:
            return D3D12_BLEND_INV_DEST_COLOR;
        case FBlendType::SrcAlphaSat:
            return D3D12_BLEND_SRC_ALPHA_SAT;
        case FBlendType::BlendFactor:
            return D3D12_BLEND_BLEND_FACTOR;
        case FBlendType::InvBlendFactor:
            return D3D12_BLEND_INV_BLEND_FACTOR;
        case FBlendType::Src1Color:
            return D3D12_BLEND_SRC1_COLOR;
        case FBlendType::InvSrc1Color:
            return D3D12_BLEND_INV_SRC1_COLOR;
        case FBlendType::Src1Alpha:
            return D3D12_BLEND_SRC1_ALPHA;
        case FBlendType::InvSrc1Alpha:
            return D3D12_BLEND_INV_SRC1_ALPHA;
        case FBlendType::AlphaFactor:
            return D3D12_BLEND_ALPHA_FACTOR;
        case FBlendType::InvAlphaFactor:
            return D3D12_BLEND_INV_ALPHA_FACTOR;
        default:
            return D3D12_BLEND_ONE;
        }
    }

    inline D3D12_BLEND_OP ToDx(const FBlendOp value)
    {
        switch (value)
        {
        case FBlendOp::Add:
            return D3D12_BLEND_OP_ADD;
        case FBlendOp::Sub:
            return D3D12_BLEND_OP_SUBTRACT;
        case FBlendOp::RevSub:
            return D3D12_BLEND_OP_REV_SUBTRACT;
        case FBlendOp::Min:
            return D3D12_BLEND_OP_MIN;
        case FBlendOp::Max:
        default:
            return D3D12_BLEND_OP_MAX;
        }
    }

    inline D3D12_LOGIC_OP ToDx(const FLogicOp value)
    {
        switch (value)
        {
        case FLogicOp::Zero:
            return D3D12_LOGIC_OP_CLEAR;
        case FLogicOp::One:
            return D3D12_LOGIC_OP_SET;
        case FLogicOp::Copy:
            return D3D12_LOGIC_OP_COPY;
        case FLogicOp::CopyInv:
            return D3D12_LOGIC_OP_COPY_INVERTED;
        case FLogicOp::Noop:
            return D3D12_LOGIC_OP_NOOP;
        case FLogicOp::Inv:
            return D3D12_LOGIC_OP_INVERT;
        case FLogicOp::And:
            return D3D12_LOGIC_OP_AND;
        case FLogicOp::NAnd:
            return D3D12_LOGIC_OP_NAND;
        case FLogicOp::Or:
            return D3D12_LOGIC_OP_OR;
        case FLogicOp::Nor:
            return D3D12_LOGIC_OP_NOR;
        case FLogicOp::Xor:
            return D3D12_LOGIC_OP_XOR;
        case FLogicOp::Equiv:
            return D3D12_LOGIC_OP_EQUIV;
        case FLogicOp::AndRev:
            return D3D12_LOGIC_OP_AND_REVERSE;
        case FLogicOp::AndInv:
            return D3D12_LOGIC_OP_AND_INVERTED;
        case FLogicOp::OrRev:
            return D3D12_LOGIC_OP_OR_REVERSE;
        case FLogicOp::OrInv:
            return D3D12_LOGIC_OP_OR_INVERTED;
        default:
            return D3D12_LOGIC_OP_NOOP;
        }
    }

    inline D3D12_FILL_MODE ToDx(const FFillMode value)
    {
        switch (value)
        {
        case FFillMode::WireFrame:
            return D3D12_FILL_MODE_WIREFRAME;
        case FFillMode::Solid:
        default:
            return D3D12_FILL_MODE_SOLID;
        }
    }

    inline D3D12_CULL_MODE ToDx(const FCullMode value)
    {
        switch (value)
        {
        case FCullMode::Front:
            return D3D12_CULL_MODE_FRONT;
        case FCullMode::Back:
            return D3D12_CULL_MODE_BACK;
        case FCullMode::Off:
        default:
            return D3D12_CULL_MODE_NONE;
        }
    }

    inline D3D12_COMPARISON_FUNC ToDx(const FCmpFunc value)
    {
        switch (value)
        {
        case FCmpFunc::Never:
            return D3D12_COMPARISON_FUNC_NEVER;
        case FCmpFunc::Less:
            return D3D12_COMPARISON_FUNC_LESS;
        case FCmpFunc::Equal:
            return D3D12_COMPARISON_FUNC_EQUAL;
        case FCmpFunc::LessEqual:
            return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case FCmpFunc::Greater:
            return D3D12_COMPARISON_FUNC_GREATER;
        case FCmpFunc::NotEqual:
            return D3D12_COMPARISON_FUNC_NOT_EQUAL;
        case FCmpFunc::GreaterEqual:
            return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        case FCmpFunc::Always:
            return D3D12_COMPARISON_FUNC_ALWAYS;
        case FCmpFunc::Off:
        default:
            return D3D12_COMPARISON_FUNC_NONE;
        }
    }

    inline D3D12_STENCIL_OP ToDx(const FStencilOp value)
    {
        switch (value)
        {
        case FStencilOp::Keep:
            return D3D12_STENCIL_OP_KEEP;
        case FStencilOp::Zero:
            return D3D12_STENCIL_OP_ZERO;
        case FStencilOp::Replace:
            return D3D12_STENCIL_OP_REPLACE;
        case FStencilOp::IncSat:
            return D3D12_STENCIL_OP_INCR_SAT;
        case FStencilOp::DecSat:
            return D3D12_STENCIL_OP_DECR_SAT;
        case FStencilOp::Invert:
            return D3D12_STENCIL_OP_INVERT;
        case FStencilOp::Inc:
            return D3D12_STENCIL_OP_INCR;
        case FStencilOp::Dec:
            return D3D12_STENCIL_OP_DECR;
        default:
            return D3D12_STENCIL_OP_KEEP;
        }
    }

    inline D3D12_PRIMITIVE_TOPOLOGY_TYPE ToDx(const FPrimitiveTopologyType value)
    {
        switch (value)
        {
        case FPrimitiveTopologyType::Triangle:
        case FPrimitiveTopologyType::TriangleStrip:
            return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        case FPrimitiveTopologyType::Point:
            return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        case FPrimitiveTopologyType::Line:
        case FPrimitiveTopologyType::LineStrip:
            return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        default:
            return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        }
    }

    inline D3D12_INPUT_CLASSIFICATION ToDx(const FMeshBufferElementRate value)
    {
        switch (value)
        {
        case FMeshBufferElementRate::Instance:
            return D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
        case FMeshBufferElementRate::Vertex:
        default:
            return D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        }
    }
}
