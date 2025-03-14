using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Union;

namespace Coplt.Graphics.Core;

[Union]
public partial struct OptimizedClearColor
{
    [UnionTemplate]
    private interface Template
    {
        void None();
        void Color(GraphicsFormat Format, Color Color);
        void Depth(GraphicsFormat Format, float Depth, byte Stencil);
    }

    public static OptimizedClearColor None => MakeNone();
}

public record struct GpuImageCreateOptions()
{
    public required ResourcePurpose Purpose;
    public CpuAccess CpuAccess = CpuAccess.None;
    public required GraphicsFormat Format;
    public required uint Width;
    public uint Height = 1;
    public uint DepthOrLength = 1;
    public uint MipLevels = 1;
    public uint MultisampleCount = 1;
    public ImageDimension Dimension = ImageDimension.Two;
    public ImageLayout Layout = ImageLayout.Undefined;
    public OptimizedClearColor OptimizedClearColor = OptimizedClearColor.None;
}

public enum ImageDimension : byte
{
    One,
    Two,
    Three,
    Cube,
}

public enum ImageLayout : byte
{
    Undefined,
    RowMajor,
    Undefined64kSwizzle,
    Standard64kSwizzle,
}

public static partial class GraphicsExtensions
{
    public static FImageDimension ToFFI(this ImageDimension value) => value switch
    {
        ImageDimension.One   => FImageDimension.One,
        ImageDimension.Two   => FImageDimension.Two,
        ImageDimension.Three => FImageDimension.Three,
        ImageDimension.Cube  => FImageDimension.Cube,
        _                    => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
    public static ImageDimension FromFFI(this FImageDimension value) => value switch
    {
        FImageDimension.One   => ImageDimension.One,
        FImageDimension.Two   => ImageDimension.Two,
        FImageDimension.Three => ImageDimension.Three,
        FImageDimension.Cube  => ImageDimension.Cube,
        _                     => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
    public static FImageLayout ToFFI(this ImageLayout value) => value switch
    {
        ImageLayout.Undefined           => FImageLayout.Undefined,
        ImageLayout.RowMajor            => FImageLayout.RowMajor,
        ImageLayout.Undefined64kSwizzle => FImageLayout.Undefined64kSwizzle,
        ImageLayout.Standard64kSwizzle  => FImageLayout.Standard64kSwizzle,
        _                               => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
    public static ImageLayout FromFFI(this FImageLayout value) => value switch
    {
        FImageLayout.Undefined           => ImageLayout.Undefined,
        FImageLayout.RowMajor            => ImageLayout.RowMajor,
        FImageLayout.Undefined64kSwizzle => ImageLayout.Undefined64kSwizzle,
        FImageLayout.Standard64kSwizzle  => ImageLayout.Standard64kSwizzle,
        _                                => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuImage : GpuResource, IGpuResource, ISrv, IUav, IRtv, IDsv
{
    #region Props

    public new FGpuImage* Ptr => (FGpuImage*)m_ptr;
    public new FGpuImageData* Data => (FGpuImageData*)m_data;
    public GraphicsFormat Format => Data->m_format.FromFFI();
    ulong IGpuView.Size => (ulong)Width * Height * DepthOrLength;
    public uint Width => Data->m_width;
    public uint Height => Data->m_height;
    public uint DepthOrLength => Data->m_depth_or_length;
    public uint MipLevels => Data->m_mip_levels;
    public uint Planes => Data->m_planes;
    public ImageDimension Dimension => Data->m_dimension.FromFFI();
    public ImageLayout Layout => Data->m_layout.FromFFI();
    uint IGpuView.Count => 0;
    uint IGpuView.Stride => 0;
    GpuResourceType IGpuResource.Type => GpuResourceType.Image;
    IGpuResource IGpuView.Resource => this;
    FResourceMeta IGpuResource.GetMeta() => new()
    {
        Type = FResourceRefType.Image,
        Image = Ptr,
    };

    #endregion

    #region Ctor

    internal GpuImage(FGpuImage* ptr, string? name, GpuQueue queue) : this(ptr, ptr->GpuImageData(), name, queue) { }
    private GpuImage(FGpuImage* ptr, FGpuImageData* data, string? name, GpuQueue queue)
        : base((FGpuResource*)ptr, (FGpuResourceData*)data, name, queue) { }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuImage)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuImage)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region View

    public bool TryRtv() => Purpose.HasFlags(ResourcePurpose.RenderTarget);
    public bool TryDsv() => Purpose.HasFlags(ResourcePurpose.DepthStencil);
    public bool TryUav() => Purpose.HasFlags(ResourcePurpose.UnorderedAccess);
    public bool TrySrv() => Purpose.HasFlags(ResourcePurpose.ShaderResource);

    #endregion
}
