using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderModule
{
    #region Fields

    internal FShaderModule* m_ptr;
    internal string? m_name;
    internal readonly String8? m_entry_point;

    #endregion

    #region Props

    public FShaderModule* Ptr => m_ptr;
    public ShaderStage Stage => m_ptr->Stage.FromFFI();
    public String8? EntryPoint => m_entry_point;

    #endregion

    #region Ctor

    internal ShaderModule(FShaderModule* ptr, string? name, String8? entry_point)
    {
        m_ptr = ptr;
        m_name = name;
        m_entry_point = entry_point;
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
            ? $"{nameof(ShaderModule)}(0x{(nuint)m_ptr:X}) {{ Stage = {Stage} }}"
            : $"{nameof(ShaderModule)}(0x{(nuint)m_ptr:X} \"{m_name}\") {{ Stage = {Stage} }}";

    #endregion
}
