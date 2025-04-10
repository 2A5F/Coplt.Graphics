using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public interface IDeviceChild
{
    public GpuDevice Device { get; }
}

public abstract unsafe class DeviceChild : GpuObject, IDeviceChild
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

public interface IIsolateChild : IDeviceChild
{
    public GpuIsolate Isolate { get; }
}

public abstract unsafe class IsolateChild : DeviceChild, IIsolateChild
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

public static partial class GraphicsExtensions
{
    public static bool IsSameIsolate<T>(this T obj, GpuIsolate isolate) where T : IIsolateChild => obj.Isolate == isolate;

    public static void AssertSameIsolate<T>(this T obj, GpuIsolate isolate) where T : IIsolateChild
    {
        if (obj.Isolate != isolate)
            throw new InvalidOperationException($"{obj} does not belong to {isolate}.");
    }
}
