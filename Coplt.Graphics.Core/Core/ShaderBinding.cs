using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct SetShaderBindGroupItem(uint Index, ShaderBindGroup Group);

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBinding : IsolateChild
{
    #region Fields

    internal ShaderBindGroup[] m_groups;

    #endregion

    #region Props

    public new FShaderBinding* Ptr => (FShaderBinding*)base.Ptr;
    public ShaderBindingLayout Layout { get; }

    #endregion

    #region Ctor

    internal ShaderBinding(
        FShaderBinding* ptr, string? name, GpuIsolate isolate, ShaderBindingLayout layout, ReadOnlySpan<SetShaderBindGroupItem> items
    ) : base((FGpuObject*)ptr, name, isolate)
    {
        Layout = layout;
        m_groups = new ShaderBindGroup[layout.BindGroupLayouts.Length];
        Set(items);
    }

    private void Set(ReadOnlySpan<SetShaderBindGroupItem> items)
    {
        foreach (var item in items)
        {
            m_groups[item.Index] = item.Group;
        }
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_groups = null!;
    }

    #endregion
}
