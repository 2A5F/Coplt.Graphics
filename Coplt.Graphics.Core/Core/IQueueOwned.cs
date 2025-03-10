namespace Coplt.Graphics.Core;

public interface IQueueOwned
{
    public GpuQueue Queue { get; }
}

public static partial class GraphicsExtensions
{
    public static bool IsSameQueue<T>(this T obj, GpuQueue queue) where T : IQueueOwned => obj.Queue == queue;

    public static void AssertSameQueue<T>(this T obj, GpuQueue queue) where T : IQueueOwned
    {
        if (obj.Queue != queue)
            throw new InvalidOperationException($"{obj} does not belong to {queue}.");
    }
}
