using Coplt.Graphics.Native;

namespace Coplt.Graphics;

public enum GpuQueueType : byte
{
    Direct,
    Compute,
    Copy,
}

public static partial class GraphicsExtensions
{
    public static FGpuQueueType ToFFI(this GpuQueueType value) => (FGpuQueueType)value;

    public static GpuQueueType FromFFI(this FGpuQueueType value) => (GpuQueueType)value;
}
