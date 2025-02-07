using System.Runtime.CompilerServices;
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
    PixelShaderResource = 1 << 7,
    NonPixelShaderResource = 1 << 8,
    UnorderedAccess = 1 << 9,
    CopySrc = 1 << 10,
    CopyDst = 1 << 11,
    ResolveSrc = 1 << 12,
    ResolveDst = 1 << 13,
    RayTracing = 1 << 14,
    ShadingRate = 1 << 15,
    IndirectBuffer = 1 << 16,
}

public static partial class GraphicsExtensions
{
    public static FResourceState ToFFI(this ResourceState value) => (FResourceState)value;

    public static ResourceState FromFFI(this FResourceState value) => (ResourceState)value;

    public static bool ContainsOnly(this ResourceState value, ResourceState other) => (value & ~other) == 0;
    public static bool Contains(this ResourceState value, ResourceState other) => (value & other) != 0;

    public static bool IsCompatible(this ResourceState value, ResourceState other)
    {
        if (value == ResourceState.Manual || other == ResourceState.Manual) return false;
        if (value == other) return true;
        if (
            value.ContainsOnly(
                ResourceState.IndexBuffer | ResourceState.VertexBuffer |
                ResourceState.ConstantBuffer | ResourceState.CopySrc | ResourceState.IndirectBuffer
            ) &&
            other.ContainsOnly(
                ResourceState.IndexBuffer | ResourceState.VertexBuffer |
                ResourceState.ConstantBuffer | ResourceState.CopySrc | ResourceState.IndirectBuffer
            )
        ) return true;
        if (
            value.ContainsOnly(
                ResourceState.PixelShaderResource | ResourceState.NonPixelShaderResource |
                ResourceState.DepthRead | ResourceState.CopySrc | ResourceState.ResolveSrc
            ) &&
            other.ContainsOnly(
                ResourceState.PixelShaderResource | ResourceState.NonPixelShaderResource |
                ResourceState.DepthRead | ResourceState.CopySrc | ResourceState.ResolveSrc
            )
        ) return true;
        return false;
    }

    public static void ChangeState(this ref ResourceState value, ResourceState state)
    {
        if (value == ResourceState.RayTracing) return;
        if (value.IsCompatible(state)) value |= state;
        else value = state;
    }

    public static void ChangeState(this ref FResourceState value, FResourceState state) =>
        ChangeState(ref Unsafe.As<FResourceState, ResourceState>(ref value), (ResourceState)state);
}
