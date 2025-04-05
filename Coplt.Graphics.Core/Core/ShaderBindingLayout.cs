using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public sealed unsafe class ShaderBindingLayout : DeviceChild
{
    #region Fields

    internal readonly ShaderLayout m_shader_layout;
    internal readonly ShaderBindGroupLayout[] m_bind_group_layouts;

    #endregion

    #region Props

    public new FBindingLayout* Ptr => (FBindingLayout*)m_ptr;
    public ShaderLayout ShaderLayout => m_shader_layout;
    public ReadOnlySpan<ShaderBindGroupLayout> BindGroupLayouts => m_bind_group_layouts;

    #endregion

    #region Ctor

    internal ShaderBindingLayout(
        FBindingLayout* ptr, string? name, GpuDevice device,
        ShaderLayout shader_layout, ShaderBindGroupLayout[] bind_group_layouts
    ) : base((FGpuObject*)ptr, name, device)
    {
        m_shader_layout = shader_layout;
        m_bind_group_layouts = bind_group_layouts;
    }

    #endregion
}
