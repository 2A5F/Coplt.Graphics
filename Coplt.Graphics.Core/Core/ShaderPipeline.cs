using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class ShaderPipeline
{
    #region Fields

    internal FShaderPipeline* m_ptr;
    internal string? m_name;
    internal readonly Shader m_shader;

    #endregion

    #region Props

    public FShaderPipeline* Ptr => m_ptr;
    public Shader Shader => m_shader;

    #endregion

    #region Ctor

    internal ShaderPipeline(FShaderPipeline* ptr, string? name, Shader shader)
    {
        m_ptr = ptr;
        m_name = name;
        m_shader = shader;
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
            ? $"{nameof(ShaderPipeline)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(ShaderPipeline)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
