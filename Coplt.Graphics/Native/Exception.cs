namespace Coplt.Graphics.Native;

public class NativeException : Exception
{
    public FResultCode Code { get; }
    public NativeException(FResultCode code)
    {
        Code = code;
    }
    public NativeException(FResultCode code, string message) : base(message)
    {
        Code = code;
    }
    public NativeException(FResultCode code, string message, Exception inner) : base(message, inner)
    {
        Code = code;
    }
}
