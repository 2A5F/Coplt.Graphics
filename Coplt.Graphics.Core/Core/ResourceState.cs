using System.Runtime.CompilerServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Flags]
public enum ResourceState : uint
{
    Manual = uint.MaxValue,
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
}

public static partial class GraphicsExtensions
{
    public static FResourceState ToFFI(this ResourceState value) => (FResourceState)value;

    public static ResourceState FromFFI(this FResourceState value) => (ResourceState)value;

    public static bool ContainsOnly(this ResourceState value, ResourceState other) => (value & ~other) == 0;
    public static bool Contains(this ResourceState value, ResourceState other) => (value & other) != 0;

    public static bool IsReadOnly(this ResourceState value) => value.ContainsOnly(
        ResourceState.IndexBuffer | ResourceState.VertexBuffer |
        ResourceState.ConstantBuffer | ResourceState.IndirectBuffer | ResourceState.CopySrc |
        ResourceState.ResolveSrc | ResourceState.ShaderResource | ResourceState.DepthRead
    );

    public static bool IsCompatible(this ResourceState value, ResourceState other)
    {
        if (value == ResourceState.Manual || other == ResourceState.Manual) return false;
        if (value == other) return true;
        if (value.IsReadOnly() && other.IsReadOnly()) return true;
        return false;
    }

    public static void ChangeState(this ref ResourceState value, ResourceState state)
    {
        if (value.IsCompatible(state)) value |= state;
        else value = state;
    }

    public static void ChangeState(this ref FResourceState value, FResourceState state) =>
        ChangeState(ref Unsafe.As<FResourceState, ResourceState>(ref value), (ResourceState)state);
}
