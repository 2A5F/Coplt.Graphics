using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum ImagePlane : byte
{
    All = 0,
    Depth = 0,
    Stencil = 1,
    Y = 0,
    CbCr = 1,
}

public static partial class GraphicsExtensions
{
    public static FImagePlane ToFFI(this ImagePlane value) => value switch {
        ImagePlane.Depth   => FImagePlane.Depth,
        ImagePlane.Stencil => FImagePlane.Stencil,
        _                  => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
    public static ImagePlane FromFFI(this FImagePlane value) => value switch {
        FImagePlane.Depth   => ImagePlane.Depth,
        FImagePlane.Stencil => ImagePlane.Stencil,
        _                  => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
}
