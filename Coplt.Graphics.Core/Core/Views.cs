using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public interface IGpuView
{
    public ulong LongSize => Size.x;
    public uint3 Size { get; }
    public GpuQueue Queue { get; }
}

public interface IGpuResource : IGpuView
{
    public FResourceMeta GetMeta();

    /// <summary>
    /// 只是用来追踪最终状态的，实际过渡不会使用
    /// </summary>
    public void UnsafeChangeState(FResourceState state);
}

public interface ICbv : IGpuView
{
    public bool TryCbv();
}

public interface ICbvRes : ICbv, IGpuResource;

public interface ISrv : IGpuView
{
    public bool TrySrv();
}

public interface ISrvRes : ISrv, IGpuResource;

public interface IUav : IGpuView
{
    public bool TryUav();
}

public interface IUavRes : IUav, IGpuResource;

public interface IRtv : IGpuView
{
    public bool TryRtv();
}

public interface IRtvRes : IRtv, IGpuResource;

public interface IDsv : IGpuView
{
    public bool TryDsv();
}

public interface IDsvRes : IDsv, IGpuResource;

public interface IIbv : IGpuView
{
    public bool TryIbv();
}

public interface IIbvRes : IIbv, IGpuResource;

public interface IVbv : IGpuView
{
    public bool TryVbv();
}

public interface IVbvRes : IVbv, IGpuResource;

public interface ISov : IGpuView
{
    public bool TrySov();
}

public interface ISovRes : ISov, IGpuResource;
