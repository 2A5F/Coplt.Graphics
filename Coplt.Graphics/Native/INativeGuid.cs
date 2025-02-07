namespace Coplt.Graphics.Native;

public interface INativeGuid
{
    public static abstract unsafe Guid* NativeGuid { get; }
}
