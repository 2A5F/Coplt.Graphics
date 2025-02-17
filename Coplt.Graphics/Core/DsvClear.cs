namespace Coplt.Graphics;

[Flags]
public enum DsvClear : byte
{
    None = 0,
    Depth = 1,
    Stencil = 2,
    All = Depth | Stencil,
}
