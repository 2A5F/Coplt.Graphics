using System.Runtime.CompilerServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Flags]
public enum LegacyState : uint
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
    public static FLegacyState ToFFI(this LegacyState value) => (FLegacyState)value;

    public static LegacyState FromFFI(this FLegacyState value) => (LegacyState)value;

    public static bool ContainsOnly(this LegacyState value, LegacyState other) => (value & ~other) == 0;
    public static bool Contains(this LegacyState value, LegacyState other) => (value & other) != 0;

    public static bool IsReadOnly(this LegacyState value) => value.ContainsOnly(
        LegacyState.IndexBuffer | LegacyState.VertexBuffer |
        LegacyState.ConstantBuffer | LegacyState.IndirectBuffer | LegacyState.CopySrc |
        LegacyState.ResolveSrc | LegacyState.ShaderResource | LegacyState.DepthRead
    );

    public static bool IsCompatible(this LegacyState value, LegacyState other)
    {
        if (value == LegacyState.Manual || other == LegacyState.Manual) return false;
        if (value == other) return true;
        if (value.IsReadOnly() && other.IsReadOnly()) return true;
        return false;
    }

    public static void ChangeState(this ref LegacyState value, LegacyState state)
    {
        if (value.IsCompatible(state)) value |= state;
        else value = state;
    }

    public static void ChangeState(this ref FLegacyState value, FLegacyState state) =>
        ChangeState(ref Unsafe.As<FLegacyState, LegacyState>(ref value), (LegacyState)state);
}
