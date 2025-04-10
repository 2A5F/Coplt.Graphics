using Coplt.Graphics.Native;
using Coplt.Graphics.States;

namespace Coplt.Graphics.Core;

public sealed unsafe class GraphicsShaderPipeline : ShaderPipeline
{
    #region Fields

    internal readonly MeshLayout? m_mesh_layout;
    internal readonly GraphicsPipelineState m_pipeline_state;

    #endregion

    #region Props

    public new FGraphicsShaderPipeline* Ptr => (FGraphicsShaderPipeline*)m_ptr;
    public MeshLayout? MeshLayout => m_mesh_layout;
    public ref readonly GraphicsPipelineState PipelineState => ref m_pipeline_state;

    #endregion

    #region Ctor

    internal GraphicsShaderPipeline(
        FGraphicsShaderPipeline* ptr, string? name,
        Shader shader, GraphicsPipelineState pipeline_state, ShaderBindingLayout binding_layout, MeshLayout? mesh_layout
    ) : base((FShaderPipeline*)ptr, name, shader, binding_layout)
    {
        m_mesh_layout = mesh_layout;
        m_pipeline_state = pipeline_state;
    }

    #endregion
}
