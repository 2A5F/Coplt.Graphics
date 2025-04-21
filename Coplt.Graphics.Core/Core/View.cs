using Coplt.Graphics.Native;
using Coplt.Union;

namespace Coplt.Graphics.Core;

[Union]
public unsafe partial struct View
{
    [UnionTemplate]
    private interface Template
    {
        void None();
        Sampler Sampler();
        void Buffer(GpuBuffer Buffer, ulong Offset = 0, int Size = -1, int Stride = -1, GraphicsFormat Format = 0);
        void UploadBuffer(nuint Index, ulong Offset, uint Size);
        /// <summary>Uav 忽略 NumMips</summary>
        void Image1D(GpuImage Image, byte Mip = 0, sbyte NumMips = -1, GraphicsFormat Format = 0);
        /// <summary>Uav 忽略 NumMips</summary>
        void Image1DArray(GpuImage Image, uint Index = 0, int Size = -1, byte Mip = 0, sbyte NumMips = -1, GraphicsFormat Format = 0);
        /// <summary>Uav 忽略 NumMips</summary>
        void Image2D(GpuImage Image, byte Mip = 0, sbyte NumMips = -1, byte Plane = 0, GraphicsFormat Format = 0);
        /// <summary>Uav 忽略 NumMips</summary>
        void Image2DArray(GpuImage Image, uint Index = 0, int Size = -1, byte Mip = 0, sbyte NumMips = -1, byte Plane = 0, GraphicsFormat Format = 0);
        void Image2DMs(GpuImage Image, GraphicsFormat Format = 0);
        void Image2DMsArray(GpuImage Image, uint Index = 0, int Size = -1, GraphicsFormat Format = 0);
        /// <summary>Uav 忽略 NumMips</summary>
        void Image3D(GpuImage Image, uint Z = 0, int Depth = -1, byte Mip = 0, sbyte NumMips = -1, GraphicsFormat Format = 0);
        /// <summary>Uav 忽略 NumMips</summary>
        void ImageCube(GpuImage Image, byte Mip = 0, sbyte NumMips = -1, GraphicsFormat Format = 0);
        /// <summary>Uav 忽略 NumMips</summary>
        void ImageCubeArray(GpuImage Image, uint Index = 0, int Size = -1, byte Mip = 0, sbyte NumMips = -1, GraphicsFormat Format = 0);
    }

    public static View None => default;

    public static View MakeUploadBuffer(UploadLoc loc) =>
        MakeUploadBuffer(loc.Index, loc.Offset, loc.Size);

    public static implicit operator View(GpuBuffer buffer) => MakeBuffer(buffer);
    public static implicit operator View(UploadLoc buffer) => MakeUploadBuffer(buffer);
    public static implicit operator View(GpuImage image) => image.Dimension switch
    {
        ImageDimension.One   => image.DepthOrLength > 1 ? MakeImage1DArray(image) : MakeImage1D(image),
        ImageDimension.Two   => image.DepthOrLength > 1 ? MakeImage2DArray(image) : MakeImage2D(image),
        ImageDimension.Three => MakeImage3D(image),
        ImageDimension.Cube  => image.DepthOrLength > 1 ? MakeImageCubeArray(image) : MakeImageCube(image),
        _                    => throw new ArgumentOutOfRangeException()
    };
    public static implicit operator View(Sampler sampler) => MakeSampler(sampler);

    public FView ToFFI() => this switch
    {
        { Tag: Tags.None } =>
            new() { Data = { Type = FViewType.None } },
        { Tag: Tags.Buffer, Buffer: var (Buffer, Offset, Size, Stride, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Buffer).Ptr,
                Data =
                {
                    Buffer = new()
                    {
                        Type = FViewType.Buffer,
                        Offset = Offset, Size = Size < 0
                            ? (Stride < 0 && Buffer.Stride > 0) || Stride > 0 ? Buffer.Count : (uint)Buffer.Size
                            : (uint)Size,
                        Stride = Stride < 0 ? Buffer.Stride : (uint)Stride,
                        Format = Format.ToFFI(),
                    },
                },
            },
        { Tag: Tags.UploadBuffer, UploadBuffer: var (Index, Offset, Size) } =>
            new() { Data = { UploadBuffer = new() { Type = FViewType.UploadBuffer, Size = Size, Index = Index, Offset = Offset } } },
        { Tag: Tags.Sampler, Sampler: var Sampler } =>
            new() { Viewable = ((GpuViewable)Sampler).Ptr, Data = { Type = FViewType.Sampler } },
        { Tag: Tags.Image1D, Image1D: var (Image, Mip, NumMips, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.Image1D,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips, Format = Format.ToFFI()
                    },
                },
            },
        { Tag: Tags.Image1DArray, Image1DArray: var (Image, Index, Size, Mip, NumMips, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.Image1DArray,
                        Index = Index, Size = Size < 0 ? Image.DepthOrLength : (uint)Size,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips,
                        Format = Format.ToFFI(),
                    }
                },
            },
        { Tag: Tags.Image2D, Image2D: var (Image, Mip, NumMips, Plane, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.Image2D,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips, Plane = Plane, Format = Format.ToFFI(),
                    },
                },
            },
        { Tag: Tags.Image2DArray, Image2DArray: var (Image, Index, Size, Mip, NumMips, Plane, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.Image2DArray,
                        Index = Index, Size = Size < 0 ? Image.DepthOrLength : (uint)Size,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips,
                        Plane = Plane, Format = Format.ToFFI()
                    },
                },
            },
        { Tag: Tags.Image2DMs, Image2DMs: var (Image, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data = { Image = new() { Type = FViewType.Image2DMs, Format = Format.ToFFI() } },
            },
        { Tag: Tags.Image2DMsArray, Image2DMsArray: var (Image, Index, Size, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.Image2DMsArray,
                        Index = Index, Size = Size < 0 ? Image.DepthOrLength : (uint)Size, Format = Format.ToFFI(),
                    },
                },
            },
        { Tag: Tags.Image3D, Image3D : var (Image, Z, Depth, Mip, NumMips, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.Image3D,
                        Z = Z, Depth = Depth < 0 ? Image.DepthOrLength : (uint)Depth,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips,
                        Format = Format.ToFFI(),
                    },
                },
            },
        { Tag: Tags.ImageCube, ImageCube: var (Image, Mip, NumMips, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.ImageCube,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips, Format = Format.ToFFI(),
                    },
                },
            },
        { Tag: Tags.ImageCubeArray, ImageCubeArray: var (Image, Index, Size, Mip, NumMips, Format) } =>
            new()
            {
                Viewable = ((GpuViewable)Image).Ptr,
                Data =
                {
                    Image = new()
                    {
                        Type = FViewType.ImageCubeArray,
                        Index = Index, Size = Size < 0 ? (Image.DepthOrLength + 5) / 6 : (uint)Size,
                        Mip = Mip, NumMips = NumMips < 0 ? (byte)Image.MipLevels : (byte)NumMips,
                        Format = Format.ToFFI(),
                    },
                },
            },
        _ => throw new ArgumentOutOfRangeException()
    };
}
