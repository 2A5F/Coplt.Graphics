using Coplt.Dropping;
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

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBindGroup : IsolateChild
{
    #region Fields

    internal FShaderBindGroupData* m_data;
    internal View[] m_views;

    #endregion

    #region Props

    public new FShaderBindGroup* Ptr => (FShaderBindGroup*)base.Ptr;
    public ref readonly FShaderBindGroupData Data => ref *m_data;
    public ReadOnlySpan<View> Views => m_views;
    public ReadOnlySpan<uint> ItemIndexes => new(m_data->ItemIndexes, (int)m_data->NumItemIndexes);
    public ShaderBindGroupLayout Layout { get; }

    #endregion

    #region Ctor

    internal ShaderBindGroup(
        FShaderBindGroupCreateResult result, string? name, GpuIsolate isolate, ShaderBindGroupLayout layout, ReadOnlySpan<SetShaderBindItem> items
    ) : base((FGpuObject*)result.BindGroup, name, isolate)
    {
        m_data = result.Data;
        Layout = layout;
        m_views = new View[Data.CountSlots];
        Set(items);
    }

    private void Set(ReadOnlySpan<SetShaderBindItem> items)
    {
        var defs = Layout.Items;
        var item_indexes = ItemIndexes;
        foreach (ref readonly var item in items)
        {
            if (item.Slot >= defs.Length) throw new IndexOutOfRangeException();
            ref readonly var def = ref defs[(int)item.Slot];
            if (item.Index >= def.Count) throw new IndexOutOfRangeException();
            ref var slot = ref m_views[item_indexes[(int)item.Slot] + item.Index];
            slot = item.View;
        }
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
        m_views = null!;
    }

    #endregion
}
