using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public sealed unsafe class BindingLayout : DeviceChild
{
    #region Fields

    internal readonly ShaderLayout m_shader_layout;
    internal readonly BindGroupLayout[] m_bind_group_layouts;

    #endregion

    #region Props

    public new FBindingLayout* Ptr => (FBindingLayout*)m_ptr;
    public ShaderLayout ShaderLayout => m_shader_layout;
    public ReadOnlySpan<BindGroupLayout> BindGroupLayouts => m_bind_group_layouts;

    #endregion

    #region Ctor

    internal BindingLayout(
        FBindingLayout* ptr, string? name, GpuDevice device,
        ShaderLayout shader_layout, BindGroupLayout[] bind_group_layouts
    ) : base((FGpuObject*)ptr, name, device)
    {
        m_shader_layout = shader_layout;
        m_bind_group_layouts = bind_group_layouts;
    }

    #endregion
}
