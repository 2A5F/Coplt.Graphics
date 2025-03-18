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

public abstract unsafe class IsolateChild : DeviceChild
{
    #region Props

    public GpuIsolate Isolate { get; }

    #endregion

    #region Ctor

    internal IsolateChild(FGpuObject* ptr, string? name, GpuIsolate isolate) : base(ptr, name, isolate.Device)
    {
        Isolate = isolate;
    }

    #endregion
}
