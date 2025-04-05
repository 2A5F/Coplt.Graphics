using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct SetShaderBindGroupItem(uint Index, ShaderBindGroup Group);

public sealed unsafe class ShaderBinding : IsolateChild
{
    #region Props

    public new FShaderBinding* Ptr => (FShaderBinding*)base.Ptr;
    public ShaderBindingLayout Layout { get; }

    #endregion

    #region Ctor

    internal ShaderBinding(
        FShaderBinding* ptr, string? name, GpuIsolate isolate, ShaderBindingLayout layout
    ) : base((FGpuObject*)ptr, name, isolate)
    {
        Layout = layout;
    }

    #endregion
}
