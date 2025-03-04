using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public interface IGpuView
{
    /// <summary>
    /// 视图所属资源
    /// </summary>
    public IGpuResource Resource { get; }
    /// <summary>
    /// 资源格式
    /// </summary>
    public GraphicsFormat Format { get; }
    /// <summary>
    /// 缓冲区大小
    /// </summary>
    public ulong Size { get; }
    /// <summary>
    /// 纹理 2d 大小
    /// </summary>
    public uint2 Size2d => new(Width, Height);
    /// <summary>
    /// 纹理 3d 大小
    /// </summary>
    public uint3 Size3d => new(Width, Height, DepthOrLength);
    /// <summary>
    /// 纹理宽度
    /// </summary>
    public uint Width { get; }
    /// <summary>
    /// 纹理高度
    /// </summary>
    public uint Height { get; }
    /// <summary>
    /// 纹理深度或数组长度
    /// </summary>
    public uint DepthOrLength { get; }
    /// <summary>
    /// Mip级数量
    /// </summary>
    public uint MipLevels { get; }
    /// <summary>
    /// 平面数量
    /// </summary>
    public uint Planes { get; }
    /// <summary>
    /// 结构缓冲区中元素数量
    /// </summary>
    public uint Count { get; }
    /// <summary>
    /// 结构缓冲区中元素步幅
    /// </summary>
    public uint Stride { get; }
}

public enum GpuResourceType
{
    Unknown,
    Buffer,
    Image,
}

public interface IGpuResource : IGpuView
{
    /// <summary>
    /// 资源所属队列
    /// </summary>
    public GpuQueue Queue { get; }
    
    public GpuResourceType Type { get; }

    public ResState State { get; set; }
    public ref FResState NativeState { get; }

    public FResourceMeta GetMeta();
}

public interface ICbv : IGpuView
{
    public bool TryCbv();
}

public interface ISrv : IGpuView
{
    public bool TrySrv();
}

public interface IUav : IGpuView
{
    public bool TryUav();
}

public interface IRtv : IGpuView
{
    public bool TryRtv();
}

public interface IDsv : IGpuView
{
    public bool TryDsv();
}

public interface IIbv : IGpuView
{
    public bool TryIbv();
}

public interface IVbv : IGpuView
{
    public bool TryVbv();
}

public interface ISov : IGpuView
{
    public bool TrySov();
}
