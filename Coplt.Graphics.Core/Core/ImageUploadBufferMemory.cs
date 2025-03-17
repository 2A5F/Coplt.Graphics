namespace Coplt.Graphics.Core;

public readonly ref struct ImageUploadBufferMemory(Span<byte> RawSpan, uint RowStride, uint RowCount, uint ImageCount, uint RowsPerImage, UploadLoc Loc)
{
    public readonly Span<byte> RawSpan = RawSpan;
    public readonly uint RowStride = RowStride;
    public readonly uint RowCount = RowCount;
    public readonly uint ImageCount = ImageCount;
    public readonly uint RowsPerImage = RowsPerImage;
    public readonly UploadLoc Loc = Loc;

    public Span<byte> this[uint index]
    {
        get
        {
            if (index >= RowCount) throw new IndexOutOfRangeException();
            var offset = RowStride * index;
            return RawSpan.Slice((int)offset, (int)RowStride);
        }
    }
}
