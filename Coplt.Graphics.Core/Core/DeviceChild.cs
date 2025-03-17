using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public abstract unsafe class DeviceChild : GpuObject
{
    #region Props

    public GpuDevice Device { get; }

    #endregion

    #region Ctor

    internal DeviceChild(FGpuObject* ptr, string? name, GpuDevice device) : base(ptr, name)
    {
        Device = device;
    }

    #endregion
}
