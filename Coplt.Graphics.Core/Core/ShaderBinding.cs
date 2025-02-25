using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBinding
{
    #region Fields

    internal FShaderBinding* m_ptr;
    internal string? m_name;
    internal readonly GpuDevice m_device;
    internal readonly ShaderLayout m_layout;

    #endregion

    #region Props

    public FShaderBinding* Ptr => m_ptr;
    public GpuDevice Device => m_device;
    public ShaderLayout Layout => m_layout;

    #endregion

    #region Ctor

    internal ShaderBinding(FShaderBinding* ptr, string? name, GpuDevice device, ShaderLayout layout)
    {
        m_name = name;
        m_ptr = ptr;
        m_device = device;
        m_layout = layout;
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
