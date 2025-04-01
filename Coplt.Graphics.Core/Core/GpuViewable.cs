using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuViewable : IsolateChild
{
    #region Fields

    internal FGpuViewableData* m_data;

    #endregion

    #region Props

    public new FGpuViewable* Ptr => (FGpuViewable*)m_ptr;
    internal ref readonly FGpuViewableData Data => ref *m_data;
    public ResourcePurpose Purpose => (ResourcePurpose)Data.m_purpose;

    #endregion

    #region Ctor

    internal GpuViewable(FGpuViewable* ptr, FGpuViewableData* data, string? name, GpuIsolate isolate) : base((FGpuObject*)ptr, name, isolate)
    {
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
