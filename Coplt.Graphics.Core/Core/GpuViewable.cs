using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public abstract unsafe class GpuViewable : DeviceChild
{
    #region Props

    public new FGpuViewable* Ptr => (FGpuViewable*)m_ptr;

    #endregion

    #region Ctor

    internal GpuViewable(FGpuViewable* ptr, string? name, GpuDevice device) : base((FGpuObject*)ptr, name, device) { }

    #endregion
}
