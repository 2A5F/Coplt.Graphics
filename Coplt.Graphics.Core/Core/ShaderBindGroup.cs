using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

/// <param name="BindIndex">绑定槽的索引</param>
/// <param name="ArrayIndex">如果绑定是数组的话，数组中的索引</param>
/// <param name="View">绑定内容</param>
public record struct SetShaderBindItem(uint BindIndex, uint ArrayIndex, View View)
{
    /// <param name="BindIndex">绑定槽的索引</param>
    /// <param name="View">绑定内容</param>
    public SetShaderBindItem(uint BindIndex, View View) : this(BindIndex, 0, View) { }
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBindGroup : IsolateChild
{
    #region Fields

    internal FShaderBindGroupData* m_data;

    #endregion

    #region Props

    public new FShaderBindGroup* Ptr => (FShaderBindGroup*)base.Ptr;
    public ref readonly FShaderBindGroupData Data => ref *m_data;
    public uint CountSlots => Data.CountSlots;
    public ShaderBindGroupLayout Layout { get; }

    #endregion

    #region Ctor

    internal ShaderBindGroup(
        FShaderBindGroupCreateResult result, string? name, GpuIsolate isolate, ShaderBindGroupLayout layout
    ) : base((FGpuObject*)result.BindGroup, name, isolate)
    {
        m_data = result.Data;
        Layout = layout;
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

    internal static void CheckSet(ShaderBindGroupLayout layout, ReadOnlySpan<SetShaderBindItem> items)
    {
        var defs = layout.Items;
        foreach (ref readonly var item in items)
        {
            if (item.BindIndex >= defs.Length) throw new IndexOutOfRangeException();
            ref readonly var def = ref defs[(int)item.BindIndex];
            if (item.ArrayIndex >= def.Count) throw new IndexOutOfRangeException();
        }
    }

    #endregion
}
