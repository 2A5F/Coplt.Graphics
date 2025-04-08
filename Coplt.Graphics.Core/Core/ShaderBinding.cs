using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct SetShaderBindGroupItem(uint Index, ShaderBindGroup Group);

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBinding : IsolateChild
{
    #region Fields

    internal FShaderBindingData* m_data;

    #endregion

    #region Props

    public new FShaderBinding* Ptr => (FShaderBinding*)base.Ptr;
    public ref readonly FShaderBindingData Data => ref *m_data;
    public ShaderBindingLayout Layout { get; }

    #endregion

    #region Ctor

    internal ShaderBinding(
        FShaderBindingCreateResult result, string? name, GpuIsolate isolate, ShaderBindingLayout layout
    ) : base((FGpuObject*)result.Binding, name, isolate)
    {
        Layout = layout;
        m_data = result.Data;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion

    #region CheckSet

    internal static void CheckSet(ShaderBindingLayout layout, ReadOnlySpan<SetShaderBindGroupItem> items)
    {
        var count = layout.BindGroupLayouts.Length;
        foreach (ref readonly var item in items)
        {
            if (item.Index >= count) throw new IndexOutOfRangeException();
        }
    }

    #endregion
}
