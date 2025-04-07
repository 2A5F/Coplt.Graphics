namespace Coplt.Graphics.Core;

public class UnrecoverableException : Exception
{
    public UnrecoverableException() { }
    public UnrecoverableException(string message) : base(message) { }
    public UnrecoverableException(string message, Exception inner) : base(message, inner) { }
    public UnrecoverableException(Exception inner) : base("The memory state is corrupted and continuing execution may result in undefined behavior", inner) { }
}
