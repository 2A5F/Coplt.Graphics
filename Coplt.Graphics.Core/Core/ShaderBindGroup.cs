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
            if (item.Slot >= defs.Length) throw new IndexOutOfRangeException();
            ref readonly var def = ref defs[(int)item.Slot];
            if (item.Index >= def.Count) throw new IndexOutOfRangeException();
        }
    }

    #endregion
}
