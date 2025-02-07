using Coplt.Graphics.Native;

namespace Coplt.Graphics;

public interface IView
{
    public GpuQueue Queue { get; }
    
    public FResourceMeta GetMeta();

    /// <summary>
    /// 只是用来追踪最终状态的，实际过渡不会使用
    /// </summary>
    public void UnsafeChangeState(FResourceState state);
}

public interface ICbv : IView;
public interface ISrv : IView;
public interface IUav : IView;
public interface IRtv : IView;
public interface IDsv : IView;
public interface IIbv : IView;
public interface IVbv : IView;
public interface ISov : IView;
