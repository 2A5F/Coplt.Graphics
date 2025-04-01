using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class Shader : DeviceChild
{
    #region Fields

    internal FShaderData* m_data;
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

    public new FShader* Ptr => (FShader*)m_ptr;
    public ref readonly FShaderData Data => ref *m_data;
    public ShaderLayout? Layout => m_layout;
    public ShaderInputLayout? InputLayout => m_input_layout;
    public ReadOnlySpan<ShaderModule> Modules => m_modules;
    public ShaderStageFlags Stages => Data.Stages.FromFFI();

    public ShaderModule? Compute => (Stages & ShaderStageFlags.Compute) != 0 ? m_modules[0] : null;
    public ShaderModule? Pixel => (Stages & ShaderStageFlags.Pixel) != 0 ? m_modules[0] : null;
    public ShaderModule? Vertex => (Stages & ShaderStageFlags.Vertex) != 0 ? m_modules[1] : null;
    public ShaderModule? Mesh => (Stages & ShaderStageFlags.Mesh) != 0 ? m_modules[1] : null;
    public ShaderModule? Task => (Stages & ShaderStageFlags.Task) != 0 ? m_modules[2] : null;

    #endregion

    #region Ctor

    internal Shader(
        FShaderCreateResult result, ShaderModuleArray modules, ShaderLayout? layout, ShaderInputLayout? input_layout, string? name, GpuDevice device
    ) : base((FGpuObject*)result.Shader, name, device)
    {
        m_data = result.Data;
        m_modules = modules;
        m_layout = layout;
        m_input_layout = input_layout;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion
}
