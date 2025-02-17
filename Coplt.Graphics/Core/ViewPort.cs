namespace Coplt.Graphics.Core;

public record struct UViewPort()
{
    public float X;
    public float Y;
    public float Width;
    public float Height;
    public float MinDepth = 0;
    public float MaxDepth = 1;
}
