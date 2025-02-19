using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Core;
using Coplt.Graphics.Native;
using Coplt.Graphics.States;

namespace Coplt.Graphics.States
{
    public enum PrimitiveTopologyType : byte
    {
        Triangle,
        TriangleStrip,
        Point,
        Line,
        LineStrip,
    }

    public enum FillMode : byte
    {
        Solid,
        WireFrame,
    }

    public enum CullMode : byte
    {
        Off,
        Back,
        Front,
    }

    public enum BlendType : byte
    {
        One,
        Zero,
        SrcColor,
        InvSrcColor,
        SrcAlpha,
        InvSrcAlpha,
        DstAlpha,
        InvDstAlpha,
        DstColor,
        InvDstColor,
        SrcAlphaSat,
        BlendFactor,
        InvBlendFactor,
        Src1Color,
        InvSrc1Color,
        Src1Alpha,
        InvSrc1Alpha,
        AlphaFactor,
        InvAlphaFactor,
    }

    public enum LogicOp : byte
    {
        None,
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
    }

    public enum BlendOp : byte
    {
        None,
        Add,
        Sub,
        RevSub,
        Min,
        Max,
    }

    public enum CmpFunc : byte
    {
        Off,
        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,
    }

    public enum StencilOp : byte
    {
        Keep,
        Zero,
        Replace,
        IncSat,
        DecSat,
        Invert,
        Inc,
        Dec,
    }

    public enum DepthWriteMask : byte
    {
        Zero,
        All = 255,
    }

    [Flags]
    public enum ColorWriteMask : byte
    {
        None = 0,
        R = 1,
        G = 2,
        B = 4,
        A = 8,
        RG = R | G,
        RB = R | B,
        RA = R | A,
        GB = G | B,
        GA = G | A,
        BA = B | A,
        RGB = R | G | B,
        GBA = G | B | A,
        RGBA = R | G | B | A,
    }

    public enum RotationDirection : byte
    {
        CounterClockWise,
        ClockWise,
    }

    public record struct StencilState()
    {
        public StencilOp Fail = StencilOp.Keep;
        public StencilOp DepthFail = StencilOp.Keep;
        public StencilOp Pass = StencilOp.Keep;
        public CmpFunc Func = CmpFunc.Always;
    }

    public record struct DepthStencilState()
    {
        public bool Enable = true;
        public bool DepthWrite = true;
        public CmpFunc DepthFunc = CmpFunc.Less;
        public bool StencilEnable = false;
        public byte StencilReadMask = 0xFF;
        public byte StencilWriteMask = 0xFF;
        public StencilState Front = new();
        public StencilState Back = new();

        public StencilState Stencil
        {
            set => Front = Back = value;
        }
    }

    public record struct RtBlendState()
    {
        public BlendType Src = BlendType.One;
        public BlendType Dst = BlendType.Zero;
        public BlendOp Op = BlendOp.Add;
        public BlendType AlphaSrc = BlendType.One;
        public BlendType AlphaDst = BlendType.One;
        public BlendOp AlphaOp = BlendOp.Max;
        public LogicOp LogicOp = LogicOp.None;
        public ColorWriteMask WriteMask = ColorWriteMask.RGB;
    }

    public record struct BlendState()
    {
        public bool IndependentBlendEnable = false;
        public RtBlendState Rt0 = new();
        public RtBlendState Rt1 = new();
        public RtBlendState Rt2 = new();
        public RtBlendState Rt3 = new();
        public RtBlendState Rt4 = new();
        public RtBlendState Rt5 = new();
        public RtBlendState Rt6 = new();
        public RtBlendState Rt7 = new();

        #region Props

        [UnscopedRef]
        public ref RtBlendState Rt => ref Rt0;

        [UnscopedRef]
        public readonly ref readonly RtBlendState RoRt => ref Rt0;

        [UnscopedRef]
        public Span<RtBlendState> Rts
        {
            get => MemoryMarshal.CreateSpan(ref Rt0, 8);
            set => value.CopyTo(Rts);
        }

        [UnscopedRef]
        public ReadOnlySpan<RtBlendState> RoRts
        {
            readonly get => MemoryMarshal.CreateReadOnlySpan(in Rt0, 8);
            set => value.CopyTo(Rts);
        }

        #endregion
    }

    public record struct RasterizerState()
    {
        public FillMode Fill = FillMode.Solid;
        public CullMode Cull = CullMode.Back;
        public int DepthBias = 0;
        public float DepthBiasClamp = 0;
        public float SlopeScaledDepthBias = 0;
        public bool DepthClip = true;
        public bool AlphaAALine = false;
        public RotationDirection FrontFace = RotationDirection.CounterClockWise;
        public bool ConservativeRaster = false;
    }

    public record struct MultiSampleState()
    {
        public uint SampleMask = 0xFFFFFFFF;
        public byte SampleCount = 1;
        public bool AlphaToCoverageEnable = false;
    }

    public record struct GraphicsPipelineState()
    {
        public RasterizerState RasterizerState = new();
        public MultiSampleState MultiSample = new();
        public GraphicsFormat DsvFormat = GraphicsFormat.D24_UNorm_S8_UInt;
        public GraphicsFormat Rtv0Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv1Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv2Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv3Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv4Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv5Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv6Format = GraphicsFormat.R8G8B8A8_UNorm;
        public GraphicsFormat Rtv7Format = GraphicsFormat.R8G8B8A8_UNorm;
        public BlendState BlendState = new();
        public DepthStencilState DepthStencilState = new();
        public byte NumRts = 1;
        public PrimitiveTopologyType Topology = PrimitiveTopologyType.Triangle;

        #region Props

        public Span<GraphicsFormat> RtvFormats
        {
            get => MemoryMarshal.CreateSpan(ref Rtv0Format, 8);
            set
            {
                value.CopyTo(RtvFormats);
                NumRts = Math.Min((byte)value.Length, (byte)8);
            }
        }

        public ReadOnlySpan<GraphicsFormat> RoRtvFormats
        {
            readonly get => MemoryMarshal.CreateReadOnlySpan(in Rtv0Format, 8);
            set
            {
                value.CopyTo(RtvFormats);
                NumRts = Math.Min((byte)value.Length, (byte)8);
            }
        }

        #endregion
    }
}

namespace Coplt.Graphics
{
    public static partial class GraphicsExtensions
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FPrimitiveTopologyType ToFFI(this PrimitiveTopologyType value) => value switch
        {
            PrimitiveTopologyType.Triangle      => FPrimitiveTopologyType.Triangle,
            PrimitiveTopologyType.TriangleStrip => FPrimitiveTopologyType.TriangleStrip,
            PrimitiveTopologyType.Point         => FPrimitiveTopologyType.Point,
            PrimitiveTopologyType.Line          => FPrimitiveTopologyType.Line,
            PrimitiveTopologyType.LineStrip     => FPrimitiveTopologyType.LineStrip,
            _                                   => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FFillMode ToFFI(this FillMode value) => value switch
        {
            FillMode.Solid     => FFillMode.Solid,
            FillMode.WireFrame => FFillMode.WireFrame,
            _                  => throw new ArgumentOutOfRangeException(nameof(value), value, null),
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FCullMode ToFFI(this CullMode value) => value switch
        {
            CullMode.Off   => FCullMode.Off,
            CullMode.Back  => FCullMode.Back,
            CullMode.Front => FCullMode.Front,
            _              => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FBlendType ToFFI(this BlendType value) => value switch
        {
            BlendType.One            => FBlendType.One,
            BlendType.Zero           => FBlendType.Zero,
            BlendType.SrcColor       => FBlendType.SrcColor,
            BlendType.InvSrcColor    => FBlendType.InvSrcColor,
            BlendType.SrcAlpha       => FBlendType.SrcAlpha,
            BlendType.InvSrcAlpha    => FBlendType.InvSrcAlpha,
            BlendType.DstAlpha       => FBlendType.DstAlpha,
            BlendType.InvDstAlpha    => FBlendType.InvDstAlpha,
            BlendType.DstColor       => FBlendType.DstColor,
            BlendType.InvDstColor    => FBlendType.InvDstColor,
            BlendType.SrcAlphaSat    => FBlendType.SrcAlphaSat,
            BlendType.BlendFactor    => FBlendType.BlendFactor,
            BlendType.InvBlendFactor => FBlendType.InvBlendFactor,
            BlendType.Src1Color      => FBlendType.Src1Color,
            BlendType.InvSrc1Color   => FBlendType.InvSrc1Color,
            BlendType.Src1Alpha      => FBlendType.Src1Alpha,
            BlendType.InvSrc1Alpha   => FBlendType.InvSrc1Alpha,
            BlendType.AlphaFactor    => FBlendType.AlphaFactor,
            BlendType.InvAlphaFactor => FBlendType.InvAlphaFactor,
            _                        => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FLogicOp ToFFI(this LogicOp value) => value switch
        {
            LogicOp.None    => FLogicOp.None,
            LogicOp.Zero    => FLogicOp.Zero,
            LogicOp.One     => FLogicOp.One,
            LogicOp.Copy    => FLogicOp.Copy,
            LogicOp.CopyInv => FLogicOp.CopyInv,
            LogicOp.Noop    => FLogicOp.Noop,
            LogicOp.Inv     => FLogicOp.Inv,
            LogicOp.And     => FLogicOp.And,
            LogicOp.NAnd    => FLogicOp.NAnd,
            LogicOp.Or      => FLogicOp.Or,
            LogicOp.Nor     => FLogicOp.Nor,
            LogicOp.Xor     => FLogicOp.Xor,
            LogicOp.Equiv   => FLogicOp.Equiv,
            LogicOp.AndRev  => FLogicOp.AndRev,
            LogicOp.AndInv  => FLogicOp.AndInv,
            LogicOp.OrRev   => FLogicOp.OrRev,
            LogicOp.OrInv   => FLogicOp.OrInv,
            _               => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FBlendOp ToFFI(this BlendOp value) => value switch
        {
            BlendOp.None   => FBlendOp.None,
            BlendOp.Add    => FBlendOp.Add,
            BlendOp.Sub    => FBlendOp.Sub,
            BlendOp.RevSub => FBlendOp.RevSub,
            BlendOp.Min    => FBlendOp.Min,
            BlendOp.Max    => FBlendOp.Max,
            _              => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FCmpFunc ToFFI(this CmpFunc value) => value switch
        {
            CmpFunc.Off          => FCmpFunc.Off,
            CmpFunc.Never        => FCmpFunc.Never,
            CmpFunc.Less         => FCmpFunc.Less,
            CmpFunc.Equal        => FCmpFunc.Equal,
            CmpFunc.LessEqual    => FCmpFunc.LessEqual,
            CmpFunc.Greater      => FCmpFunc.Greater,
            CmpFunc.NotEqual     => FCmpFunc.NotEqual,
            CmpFunc.GreaterEqual => FCmpFunc.GreaterEqual,
            CmpFunc.Always       => FCmpFunc.Always,
            _                    => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FStencilOp ToFFI(this StencilOp value) => value switch
        {
            StencilOp.Keep    => FStencilOp.Keep,
            StencilOp.Zero    => FStencilOp.Zero,
            StencilOp.Replace => FStencilOp.Replace,
            StencilOp.IncSat  => FStencilOp.IncSat,
            StencilOp.DecSat  => FStencilOp.DecSat,
            StencilOp.Invert  => FStencilOp.Invert,
            StencilOp.Inc     => FStencilOp.Inc,
            StencilOp.Dec     => FStencilOp.Dec,
            _                 => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FDepthWriteMask ToFFI(this DepthWriteMask value) => value switch
        {
            DepthWriteMask.Zero => FDepthWriteMask.Zero,
            DepthWriteMask.All  => FDepthWriteMask.All,
            _                   => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FRotationDirection ToFFI(this RotationDirection value) => value switch
        {
            RotationDirection.CounterClockWise => FRotationDirection.CounterClockWise,
            RotationDirection.ClockWise        => FRotationDirection.ClockWise,
            _                                  => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FRasterizerState ToFFI(this in RasterizerState value) => new()
        {
            Fill = value.Fill.ToFFI(),
            Cull = value.Cull.ToFFI(),
            DepthBias = value.DepthBias,
            DepthBiasClamp = value.DepthBiasClamp,
            SlopeScaledDepthBias = value.SlopeScaledDepthBias,
            DepthClip = value.DepthClip,
            AlphaAALine = value.AlphaAALine,
            FrontFace = value.FrontFace.ToFFI(),
            ConservativeRaster = value.ConservativeRaster,
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FMultiSampleState ToFFI(this in MultiSampleState value) => new()
        {
            SampleMask = value.SampleMask,
            SampleCount = value.SampleCount,
            AlphaToCoverageEnable = value.AlphaToCoverageEnable,
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FRtBlendState ToFFI(this in RtBlendState value) => new()
        {
            Src = value.Src.ToFFI(),
            Dst = value.Dst.ToFFI(),
            Op = value.Op.ToFFI(),
            AlphaSrc = value.AlphaSrc.ToFFI(),
            AlphaDst = value.AlphaDst.ToFFI(),
            AlphaOp = value.AlphaOp.ToFFI(),
            LogicOp = value.LogicOp.ToFFI(),
            WriteMask = (byte)value.WriteMask,
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FBlendState ToFFI(this in BlendState value)
        {
            FBlendState r = new()
            {
                IndependentBlendEnable = value.IndependentBlendEnable,
            };
            var rts = value.RoRts;
            for (var i = 0; i < rts.Length; i++)
            {
                r.Rts[i] = rts[i].ToFFI();
            }
            return r;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FStencilState ToFFI(this in StencilState value) => new()
        {
            Fail = value.Fail.ToFFI(),
            DepthFail = value.DepthFail.ToFFI(),
            Pass = value.Pass.ToFFI(),
            Func = value.Func.ToFFI(),
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FDepthStencilState ToFFI(this DepthStencilState value) => new()
        {
            Enable = value.Enable,
            DepthWrite = value.DepthWrite,
            DepthFunc = value.DepthFunc.ToFFI(),
            StencilEnable = value.StencilEnable,
            StencilReadMask = value.StencilReadMask,
            StencilWriteMask = value.StencilWriteMask,
            Front = value.Front.ToFFI(),
            Back = value.Back.ToFFI(),
        };

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static FGraphicsPipelineState ToFFI(this in GraphicsPipelineState value)
        {
            FGraphicsPipelineState r = new()
            {
                RasterizerState = value.RasterizerState.ToFFI(),
                MultiSample = value.MultiSample.ToFFI(),
                DsvFormat = value.DsvFormat.ToFFI(),
                BlendState = value.BlendState.ToFFI(),
                DepthStencilState = value.DepthStencilState.ToFFI(),
                NumRts = value.NumRts,
                Topology = value.Topology.ToFFI(),
            };
            value.RoRtvFormats[..value.NumRts].CopyTo(MemoryMarshal.Cast<FGraphicsFormat, GraphicsFormat>(r.RtvFormats));
            return r;
        }
    }
}
