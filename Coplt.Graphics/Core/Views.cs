using Coplt.Graphics.Native;

namespace Coplt.Graphics;

public interface IView
{
    public FResourceMeta GetMeta();
}

public interface ICbv : IView;
public interface ISrv : IView;
public interface IUav : IView;
public interface IRtv : IView;
public interface IDsv : IView;
public interface IIbv : IView;
public interface IVbv : IView;
public interface ISov : IView;
