using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GraphicsShaderPipeline : ShaderPipeline
{
    #region Fields

    internal new FGraphicsShaderPipeline* m_ptr;
    internal readonly MeshLayout? m_mesh_layout;
    internal readonly GraphicsPipelineState m_pipeline_state;

    #endregion

    #region Props

    public new FGraphicsShaderPipeline* Ptr => m_ptr;
    public MeshLayout? MeshLayout => m_mesh_layout;
    public ref readonly GraphicsPipelineState PipelineState => ref m_pipeline_state;

    #endregion

    #region Ctor

    internal GraphicsShaderPipeline(
        FGraphicsShaderPipeline* ptr, string? name,
        Shader shader, GraphicsPipelineState pipeline_state, MeshLayout? mesh_layout
    ) : base(ptr == null ? null : &ptr->Base, name, shader)
    {
        m_ptr = ptr;
        m_mesh_layout = mesh_layout;
        m_pipeline_state = pipeline_state;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop() => m_ptr = null;

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GraphicsShaderPipeline)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GraphicsShaderPipeline)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
