using Coplt.Graphics.Native;
using Coplt.Graphics.States;

namespace Coplt.Graphics.Core;

public sealed unsafe class ComputeShaderPipeline : ShaderPipeline
{
    #region Props

    public new FComputeShaderPipeline* Ptr => (FComputeShaderPipeline*)m_ptr;

    #endregion

    #region Ctor

    internal ComputeShaderPipeline(
        FComputeShaderPipeline* ptr, string? name,
        Shader shader, ShaderBindingLayout binding_layout
    ) : base((FShaderPipeline*)ptr, name, shader, binding_layout) { }

    #endregion
}
