using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuResource : GpuViewable, IIsolateChild
{
    #region Fields

    internal FGpuResourceData* m_data;

    #endregion

    #region Props
    
    public GpuIsolate Isolate { get; }
    public new FGpuResource* Ptr => (FGpuResource*)m_ptr;
    internal ref readonly FGpuResourceData Data => ref *m_data;
    public ResourcePurpose Purpose => (ResourcePurpose)Data.m_purpose;
    public CpuAccess CpuAccess => (CpuAccess)Data.m_cpu_access;

    #endregion

    #region Ctor

    internal GpuResource(FGpuResource* ptr, FGpuResourceData* data, string? name, GpuIsolate isolate) : base((FGpuViewable*)ptr, name, isolate.Device)
    {
        Isolate = isolate;
        m_data = data;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion
}
