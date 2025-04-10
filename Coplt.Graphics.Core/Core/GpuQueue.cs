using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

public enum GpuQueueType : byte
{
    Direct,
    Compute,
    Copy,
    VideoEncode,
    VideoDecode,
    VideoProcess,
}

public static partial class GraphicsExtensions
{
    public static FGpuQueueType ToFFI(this GpuQueueType value) => (FGpuQueueType)value;

    public static GpuQueueType FromFFI(this FGpuQueueType value) => (GpuQueueType)value;

    public static ReadOnlySpan<byte> ToUtf8String(this GpuQueueType value) => value switch
    {
        GpuQueueType.Direct       => "Direct"u8,
        GpuQueueType.Compute      => "Compute"u8,
        GpuQueueType.Copy         => "Copy"u8,
        GpuQueueType.VideoEncode  => "VideoEncode"u8,
        GpuQueueType.VideoDecode  => "VideoDecode"u8,
        GpuQueueType.VideoProcess => "VideoProcess"u8,
        _                         => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
}
