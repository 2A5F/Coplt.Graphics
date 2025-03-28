using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public readonly struct ImageUploadBufferMemory(FSlice<byte> Slice, uint RowStride, uint RowCount, uint ImageCount, uint RowsPerImage, UploadLoc Loc)
{
    public readonly FSlice<byte> Slice = Slice;
    public readonly uint RowStride = RowStride;
    public readonly uint RowCount = RowCount;
    public readonly uint ImageCount = ImageCount;
    public readonly uint RowsPerImage = RowsPerImage;
    public readonly UploadLoc Loc = Loc;

    public FSlice<byte> this[uint index]
    {
        get
        {
            if (index >= RowCount) throw new IndexOutOfRangeException();
            var offset = RowStride * index;
            return Slice.Slice(offset, RowStride);
        }
    }
}

public readonly struct ImageUploadBufferMemory2(FSlice<byte> Slice, uint RowStride, uint RowCount, uint ImageCount, uint RowsPerImage, UploadLoc2 Loc)
{
    public readonly FSlice<byte> Slice = Slice;
    public readonly uint RowStride = RowStride;
    public readonly uint RowCount = RowCount;
    public readonly uint ImageCount = ImageCount;
    public readonly uint RowsPerImage = RowsPerImage;
    public readonly UploadLoc2 Loc = Loc;

    public FSlice<byte> this[uint index]
    {
        get
        {
            if (index >= RowCount) throw new IndexOutOfRangeException();
            var offset = RowStride * index;
            return Slice.Slice(offset, RowStride);
        }
    }
}
