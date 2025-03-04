using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum ResLayout : uint
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
}

[Flags]
public enum ResAccess : uint
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
}

public record struct ResState
{
    public ResLayout Layout;
    public ResAccess Access;
    public ShaderStageFlags Stages;
    public LegacyState Legacy;
    public bool CrossQueue;
}

public static partial class GraphicsExtensions
{
    public static FResAccess ToFFI(this ResAccess value) => (FResAccess)value;

    public static ResAccess FromFFI(this FResAccess value) => (ResAccess)value;

    public static FResLayout ToFFI(this ResLayout value) => value switch
    {
        ResLayout.None              => FResLayout.None,
        ResLayout.Common            => FResLayout.Common,
        ResLayout.GenericRead       => FResLayout.GenericRead,
        ResLayout.RenderTarget      => FResLayout.RenderTarget,
        ResLayout.UnorderedAccess   => FResLayout.UnorderedAccess,
        ResLayout.DepthStencilRead  => FResLayout.DepthStencilRead,
        ResLayout.DepthStencilWrite => FResLayout.DepthStencilWrite,
        ResLayout.ShaderResource    => FResLayout.ShaderResource,
        ResLayout.CopySrc           => FResLayout.CopySrc,
        ResLayout.CopyDst           => FResLayout.CopyDst,
        ResLayout.ResolveSrc        => FResLayout.ResolveSrc,
        ResLayout.ResolveDst        => FResLayout.ResolveDst,
        ResLayout.ShadingRate       => FResLayout.ShadingRate,
        ResLayout.VideoDecodeRead   => FResLayout.VideoDecodeRead,
        ResLayout.VideoDecodeWrite  => FResLayout.VideoDecodeWrite,
        ResLayout.VideoProcessRead  => FResLayout.VideoProcessRead,
        ResLayout.VideoProcessWrite => FResLayout.VideoProcessWrite,
        ResLayout.VideoEncodeRead   => FResLayout.VideoEncodeRead,
        ResLayout.VideoEncodeWrite  => FResLayout.VideoEncodeWrite,
        _                           => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };

    public static ResLayout FromFFI(this FResLayout value) => value switch
    {
        FResLayout.None              => ResLayout.None,
        FResLayout.Common            => ResLayout.Common,
        FResLayout.GenericRead       => ResLayout.GenericRead,
        FResLayout.RenderTarget      => ResLayout.RenderTarget,
        FResLayout.UnorderedAccess   => ResLayout.UnorderedAccess,
        FResLayout.DepthStencilRead  => ResLayout.DepthStencilRead,
        FResLayout.DepthStencilWrite => ResLayout.DepthStencilWrite,
        FResLayout.ShaderResource    => ResLayout.ShaderResource,
        FResLayout.CopySrc           => ResLayout.CopySrc,
        FResLayout.CopyDst           => ResLayout.CopyDst,
        FResLayout.ResolveSrc        => ResLayout.ResolveSrc,
        FResLayout.ResolveDst        => ResLayout.ResolveDst,
        FResLayout.ShadingRate       => ResLayout.ShadingRate,
        FResLayout.VideoDecodeRead   => ResLayout.VideoDecodeRead,
        FResLayout.VideoDecodeWrite  => ResLayout.VideoDecodeWrite,
        FResLayout.VideoProcessRead  => ResLayout.VideoProcessRead,
        FResLayout.VideoProcessWrite => ResLayout.VideoProcessWrite,
        FResLayout.VideoEncodeRead   => ResLayout.VideoEncodeRead,
        FResLayout.VideoEncodeWrite  => ResLayout.VideoEncodeWrite,
        _                            => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };

    public static FResState ToFFI(this in ResState value) => new()
    {
        Layout = value.Layout.ToFFI(),
        Access = value.Access.ToFFI(),
        Stages = value.Stages.ToFFI(),
        Legacy = value.Legacy.ToFFI(),
        CrossQueue = value.CrossQueue,
    };

    public static ResState FromFFI(this in FResState value) => new()
    {
        Layout = value.Layout.FromFFI(),
        Access = value.Access.FromFFI(),
        Stages = value.Stages.FromFFI(),
        Legacy = value.Legacy.FromFFI(),
        CrossQueue = value.CrossQueue,
    };
}
