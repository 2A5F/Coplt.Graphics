using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public abstract unsafe class ShaderPipeline : DeviceChild
{
    #region Fields

    internal readonly Shader m_shader;
    internal readonly BindingLayout m_binding_layout;

    #endregion

    #region Props

    public new FShaderPipeline* Ptr => (FShaderPipeline*)m_ptr;
    public Shader Shader => m_shader;
    public BindingLayout BindingLayout => m_binding_layout;

    #endregion

    #region Ctor

    internal ShaderPipeline(FShaderPipeline* ptr, string? name, Shader shader, BindingLayout binding_layout) : base((FGpuObject*)ptr, name, shader.Device)
    {
        m_shader = shader;
        m_binding_layout = binding_layout;
    }

    #endregion
}
