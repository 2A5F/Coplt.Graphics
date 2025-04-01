using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public abstract unsafe class ShaderPipeline : DeviceChild
{
    #region Fields

    internal readonly Shader m_shader;

    #endregion

    #region Props

    public new FShaderPipeline* Ptr => (FShaderPipeline*)m_ptr;
    public Shader Shader => m_shader;

    #endregion

    #region Ctor

    internal ShaderPipeline(FShaderPipeline* ptr, string? name, Shader shader) : base((FGpuObject*)ptr, name, shader.Device)
    {
        m_shader = shader;
    }

    #endregion
}
