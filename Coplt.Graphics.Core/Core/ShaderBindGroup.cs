using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

/// <param name="Slot">绑定槽的索引</param>
/// <param name="Index">如果绑定是数组的话，数组中的索引</param>
/// <param name="View">绑定内容</param>
public record struct SetShaderBindItem(uint Slot, uint Index, View View)
{
    /// <param name="Slot">绑定槽的索引</param>
    /// <param name="View">绑定内容</param>
    public SetShaderBindItem(uint Slot, View View) : this(Slot, 0, View) { }
}

public sealed unsafe class ShaderBindGroup : IsolateChild
{
    #region Props

    public new FShaderBindGroup* Ptr => (FShaderBindGroup*)base.Ptr;
    public ShaderBindGroupLayout Layout { get; }

    #endregion

    #region Ctor

    internal ShaderBindGroup(
        FShaderBindGroup* ptr, string? name, GpuIsolate isolate, ShaderBindGroupLayout layout
    ) : base((FGpuObject*)ptr, name, isolate)
    {
        Layout = layout;
    }

    #endregion
}
