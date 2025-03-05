using System.Buffers;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct ShaderBindingSetItem(uint Index, View View);

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBinding
{
    #region Fields

    internal FShaderBinding* m_ptr;
    internal string? m_name;
    internal readonly GpuDevice m_device;
    internal readonly GpuQueue m_queue;
    internal readonly ShaderLayout m_layout;
    internal readonly View[] m_views;

    #endregion

    #region Props

    public FShaderBinding* Ptr => m_ptr;
    public GpuDevice Device => m_device;
    public GpuQueue Queue => m_queue;
    public ShaderLayout Layout => m_layout;
    public ReadOnlySpan<View> Views => m_views;
    internal Span<View> MutViews => m_views;
    public ref readonly View this[int index] => ref m_views[index];

    #endregion

    #region Ctor

    internal ShaderBinding(FShaderBinding* ptr, string? name, GpuDevice device, GpuQueue queue, ShaderLayout layout)
    {
        m_name = name;
        m_ptr = ptr;
        m_device = device;
        m_queue = queue;
        m_layout = layout;

        m_views = new View[m_layout.m_native_defines.Length];
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        ptr->Release();
    }

    #endregion

    #region SetName

    public void SetName(string name)
    {
        m_name = name;
        fixed (char* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        m_name = null;
        fixed (byte* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            m_ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"0x{nameof(ShaderBinding)}({(nuint)m_ptr:X})"
            : $"0x{nameof(ShaderBinding)}({(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
