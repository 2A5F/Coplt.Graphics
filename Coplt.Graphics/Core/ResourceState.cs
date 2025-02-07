using Coplt.Graphics.Native;

namespace Coplt.Graphics;

[Flags]
public enum ResourceState : uint
{
    Manual = uint.MaxValue,
    Common = 0,
    Present = 1 << 0,
    VertexBuffer = 1 << 1,
    IndexBuffer = 1 << 2,
    ConstantBuffer = 1 << 3,
    RenderTarget = 1 << 4,
    DepthRead = 1 << 5,
    DepthWrite = 1 << 6,
    ShaderResource = 1 << 7,
    UnorderedAccess = 1 << 8,
    CopySrc = 1 << 9,
    CopyDst = 1 << 10,
    ResolveSrc = 1 << 11,
    ResolveDst = 1 << 12,
    RayTracing = 1 << 13,
    ShadingRate = 1 << 14,
}

public static partial class GraphicsExtensions
{
    public static FResourceState ToFFI(this ResourceState value) => (FResourceState)value;

    public static ResourceState FromFFI(this FResourceState value) => (ResourceState)value;
}
