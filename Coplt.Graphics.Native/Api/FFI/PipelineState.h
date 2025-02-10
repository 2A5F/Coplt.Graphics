#pragma once

#include "Common.h"
#include "TextureFormat.h"

namespace Coplt
{
    enum class FPrimitiveTopologyType : u8
    {
        Triangle,
        TriangleStrip,
        Point,
        Line,
        LineStrip,
    };

    enum class FFillMode : u8
    {
        WireFrame = 2,
        Solid = 3,
    };

    enum class FCullMode : u8
    {
        Off = 1,
        Front = 2,
        Back = 3,
    };

    enum class FBlendType : u8
    {
        Zero = 1,
        One = 2,
        SrcColor = 3,
        InvSrcColor = 4,
        SrcAlpha = 5,
        InvSrcAlpha = 6,
        DstAlpha = 7,
        InvDstAlpha = 8,
        DstColor = 9,
        InvDstColor = 10,
        SrcAlphaSat = 11,
        BlendFactor = 14,
        BlendInvBlendFactor = 15,
        Src1Color = 16,
        InvSrc1Color = 17,
        Src1Alpha = 18,
        InvSrc1Alpha = 19,
        AlphaFactor = 20,
        InvAlphaFactor = 21,
    };

    enum class FLogicOp : u8
    {
        None = 0,
        Clear,
        One,
        Copy,
        CopyInv,
        Noop,
        Inv,
        And,
        NAnd,
        Or,
        Nor,
        Xor,
        Equiv,
        AndRev,
        AndInv,
        OrRev,
        OrInv,
    };

    enum class FBlendOp : u8
    {
        None = 0,
        Add = 1,
        Sub = 2,
        RevSub = 3,
        Min = 4,
        Max = 5,
    };

    enum class FCmpFunc : u8
    {
        Off = 0,
        Never = 1,
        Less = 2,
        Equal = 3,
        LessEqual = 4,
        Greater = 5,
        NotEqual = 6,
        GreaterEqual = 7,
        Always = 8,
    };

    enum class FStencilOp : u8
    {
        Keep = 1,
        Zero = 2,
        Replace = 3,
        IncSat = 4,
        DecSat = 5,
        Invert = 6,
        Inc = 7,
        Dec = 8,
    };

    enum class FDepthWriteMask : u8
    {
        Zero = 0,
        All = 1,
    };

    struct FStencilState
    {
        FStencilOp Fail{FStencilOp::Keep};
        FStencilOp DepthFail{FStencilOp::Keep};
        FStencilOp Pass{FStencilOp::Keep};
        FCmpFunc Func{FCmpFunc::Always};
    };

    struct FDepthStencilState
    {
        u8 Enable{true};
        u8 DepthWrite{true};
        FCmpFunc DepthFunc{FCmpFunc::Less};
        u8 StencilEnable{false};
        u8 StencilReadMask{0xFF};
        u8 StencilWriteMask{0xFF};
        FStencilState Front{};
        FStencilState Back{};
    };

    struct FRtBlendState
    {
        u8 Enable{false};
        FBlendType Src{FBlendType::One};
        FBlendType Dst{FBlendType::Zero};
        FBlendOp Op{FBlendOp::Add};
        FBlendType AlphaSrc{FBlendType::One};
        FBlendType AlphaDst{FBlendType::One};
        FBlendOp AlphaOp{FBlendOp::Max};
        FLogicOp LogicOp{FLogicOp::None};
        u8 WriteMask{0b1110};
    };

    struct FBlendState
    {
        u8 AlphaToCoverageEnable{false};
        u8 IndependentBlendEnable{false};
        FRtBlendState Rts[8]
        {
            FRtBlendState{},
            FRtBlendState{},
            FRtBlendState{},
            FRtBlendState{},
            FRtBlendState{},
            FRtBlendState{},
            FRtBlendState{},
            FRtBlendState{},
        };
    };

    struct FRasterizerState
    {
        FFillMode Fill{FFillMode::Solid};
        FCullMode Cull{FCullMode::Back};
        i32 DepthBias{0};
        f32 DepthBiasClamp{0};
        f32 SlopeScaledDepthBias{0};
        u8 DepthClip{true};
        u8 AALine{false};
        u8 FrontCounterClockwise{false};
    };

    struct FSampleState
    {
        u8 Count{1};
        i8 Quality{0};
    };

    struct FGraphicsPipelineState
    {
        FRasterizerState RasterizerState{};
        FTextureFormat DsvFormat{FTextureFormat::D24_UNorm_S8_UInt};
        FTextureFormat RtvFormats[8]
        {
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
            FTextureFormat::R8G8B8A8_UNorm,
        };
        u32 SampleMask{0xFFFFFFFF};
        FSampleState SampleState{};
        FBlendState BlendState{};
        FDepthStencilState DepthStencilState{};
        u8 NumRts{1};
        FPrimitiveTopologyType Topology{FPrimitiveTopologyType::Triangle};
    };

    enum class FInputElementValueType : u8
    {
        Undefined = 0,
        Int8,
        Int16,
        Int32,
        UInt8,
        UInt16,
        UInt32,
        Float16,
        Float32,
        Float64,
    };

    enum class FInputElementFrequency : u8
    {
        Undefined = 0,
        Vertex,
        Instance,
    };

    enum class FInputElementNumComponents : u8
    {
        X1 = 1,
        X2 = 2,
        X3 = 3,
        X4 = 4,
    };

    struct FInputLayoutElement
    {
        u32 SlotNameAt{0};
        u32 SemanticIndex{0};
        u32 BufferSlot{0};
        u32 RelativeOffset{COPLT_U32_MAX};
        u32 Stride{COPLT_U32_MAX};
        u32 InstanceDataStepRate{1};
        FInputElementNumComponents NumComponents{0};
        FInputElementValueType ValueType{FInputElementValueType::Float32};
        u8 IsNormalized{true};
        FInputElementFrequency Frequency{FInputElementFrequency::Vertex};
    };
}
