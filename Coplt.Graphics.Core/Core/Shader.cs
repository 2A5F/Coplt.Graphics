using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class Shader
{
    #region Fields

    internal FShader* m_ptr;
    internal string? m_name;
    internal readonly ShaderLayout? m_layout;
    internal readonly ShaderInputLayout? m_input_layout;
    internal ShaderModuleArray m_modules;

    [InlineArray((int)FShader.MaxShaderModuleCount)]
    internal struct ShaderModuleArray
    {
        private ShaderModule _;
    }

    #endregion

    #region Props

    public FShader* Ptr => m_ptr;
    public ShaderLayout? Layout => m_layout;
    public ShaderInputLayout? InputLayout => m_input_layout;
    public ReadOnlySpan<ShaderModule> Modules => m_modules;
    public ShaderStageFlags Stages => m_ptr->Stages.FromFFI();

    public ShaderModule? Compute => (Stages & ShaderStageFlags.Compute) != 0 ? m_modules[0] : null;
    public ShaderModule? Pixel => (Stages & ShaderStageFlags.Pixel) != 0 ? m_modules[0] : null;
    public ShaderModule? Vertex => (Stages & ShaderStageFlags.Vertex) != 0 ? m_modules[1] : null;
    public ShaderModule? Mesh => (Stages & ShaderStageFlags.Mesh) != 0 ? m_modules[1] : null;
    public ShaderModule? Task => (Stages & ShaderStageFlags.Task) != 0 ? m_modules[2] : null;

    #endregion

    #region Ctor

    internal Shader(
        FShader* ptr, ShaderModuleArray modules, ShaderLayout? layout, ShaderInputLayout? input_layout, string? name
    )
    {
        m_ptr = ptr;
        m_modules = modules;
        m_layout = layout;
        m_input_layout = input_layout;
        m_name = name;
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
            ? $"{nameof(Shader)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(Shader)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
