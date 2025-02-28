using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuResource : GpuView
{
    #region Fields

    internal new FGpuResource* m_ptr;

    #endregion

    #region Props

    public new FGpuResource* Ptr => m_ptr;
    public ResState State => m_ptr->m_state.FromFFI();
    public CpuAccess CpuAccess => (CpuAccess)m_ptr->m_cpu_access;

    #endregion

    #region Ctor

    internal GpuResource(FGpuResource* ptr, string? name, GpuQueue queue) : base(&ptr->Base, name, queue)
    {
        m_ptr = ptr;
        m_name = name;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_ptr = null;
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuResource)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuResource)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
