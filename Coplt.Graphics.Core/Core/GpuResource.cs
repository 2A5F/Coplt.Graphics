using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuResource : GpuViewable
{
    #region Props

    public new FGpuResource* Ptr => (FGpuResource*)m_ptr;
    public new FGpuResourceData* Data => (FGpuResourceData*)m_data;
    public ResState State
    {
        get => NativeState.FromFFI();
        set => NativeState = value.ToFFI();
    }
    public ref FResState NativeState => ref Data->m_state;
    public CpuAccess CpuAccess => (CpuAccess)Data->m_cpu_access;

    #endregion

    #region Ctor

    internal GpuResource(FGpuResource* ptr, FGpuResourceData* data, string? name, GpuQueue queue)
        : base((FGpuViewable*)ptr, (FGpuViewableData*)data, name, queue) { }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuResource)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuResource)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
