using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public interface IView
{
    public ulong LongSize => Size.x;
    public uint3 Size { get; }
    public GpuQueue Queue { get; }

    public FResourceMeta GetMeta();

    /// <summary>
    /// 只是用来追踪最终状态的，实际过渡不会使用
    /// </summary>
    public void UnsafeChangeState(FResourceState state);
}

public interface ICbv : IView
{
    public bool TryCbv();
}

public interface ISrv : IView
{
    public bool TrySrv();
}

public interface IUav : IView
{
    public bool TryUav();
}

public interface IRtv : IView
{
    public bool TryRtv();
}

public interface IDsv : IView
{
    public bool TryDsv();
}

public interface IIbv : IView
{
    public bool TryIbv();
}

public interface IVbv : IView
{
    public bool TryVbv();
}

public interface ISov : IView
{
    public bool TrySov();
}
