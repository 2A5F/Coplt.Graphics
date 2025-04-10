using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public abstract unsafe class ShaderPipeline : DeviceChild
{
    #region Fields

    internal readonly Shader m_shader;
    internal readonly ShaderBindingLayout m_binding_layout;

    #endregion

    #region Props

    public new FShaderPipeline* Ptr => (FShaderPipeline*)m_ptr;
    public Shader Shader => m_shader;
    public ShaderBindingLayout BindingLayout => m_binding_layout;

    #endregion

    #region Ctor

    internal ShaderPipeline(FShaderPipeline* ptr, string? name, Shader shader, ShaderBindingLayout binding_layout) : base((FGpuObject*)ptr, name, shader.Device)
    {
        m_shader = shader;
        m_binding_layout = binding_layout;
    }

    #endregion
}
