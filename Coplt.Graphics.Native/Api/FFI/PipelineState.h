#pragma once

#include "Common.h"
#include "TextureFormat.h"
#include "String.h"

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
        InvBlendFactor = 15,
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
        Zero,
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
        b8 Enable{true};
        b8 DepthWrite{true};
        FCmpFunc DepthFunc{FCmpFunc::Less};
        b8 StencilEnable{false};
        u8 StencilReadMask{0xFF};
        u8 StencilWriteMask{0xFF};
        FStencilState Front{};
        FStencilState Back{};
    };

    struct FRtBlendState
    {
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
        b8 IndependentBlendEnable{false};
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

    enum class FRotationDirection : u8
    {
        CounterClockWise,
        ClockWise,
    };

    struct FRasterizerState
    {
        FFillMode Fill{FFillMode::Solid};
        FCullMode Cull{FCullMode::Back};
        i32 DepthBias{0};
        f32 DepthBiasClamp{0};
        f32 SlopeScaledDepthBias{0};
        b8 DepthClip{true};
        b8 AlphaAALine{false};
        FRotationDirection FrontFace{FRotationDirection::CounterClockWise};
        b8 ConservativeRaster{false};
    };

    struct FMultiSampleState
    {
        u32 SampleMask{0xFFFFFFFF};
        u8 SampleCount{1};
        b8 AlphaToCoverageEnable{false};
    };

    struct FGraphicsPipelineState
    {
        FRasterizerState RasterizerState{};
        FMultiSampleState MultiSample{};
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
        FBlendState BlendState{};
        FDepthStencilState DepthStencilState{};
        u8 NumRts{1};
        FPrimitiveTopologyType Topology{FPrimitiveTopologyType::Triangle};
    };

    struct FShaderInputLayoutElement
    {
        // dx 后端为语义名称，必须有；其他后端可选
        FString8* SlotName{};
        // 与 FMeshBufferElement 对应的 Id，相同 SlotName 的 SlotId 必须也相同
        u32 SlotId{};
        // 是 Slot 中第几个项，对应 dx 的 SemanticIndex
        u32 SlotIndex{};
        // 仅 vk, 用于指定位置；dx 将忽略; u32::max 将使用 FShaderInputLayoutElement 索引顺序
        u32 Location{COPLT_U32_MAX};
    };

    struct FMeshBufferElement
    {
        // 属于 mesh 中的第几个 buffer
        u32 BufferIndex{};
        // 与 FShaderInputLayoutElement 对应的 Id
        u32 SlotId{};
        // 是 Slot 中第几个项，对应 dx 的 SemanticIndex
        u32 SlotIndex{};
        // 元素格式
        FTextureFormat Format{};
        // 元素在间隔中的偏移
        u32 Offset{};
        // 每次实例数据可重复几次，对应 dx 的 InstanceDataStepRate；对于实例 0 相当于 1；仅 dx 支持
        u32 InstanceRepeat{};
    };

    enum class FMeshBufferElementRate : u8
    {
        Vertex,
        Instance,
    };

    struct FMeshBufferDefine
    {
        // 元素间隔
        u32 Stride{};
        // 元素频率，指示是按顶点还是按实例
        FMeshBufferElementRate Rate{};
    };
}
