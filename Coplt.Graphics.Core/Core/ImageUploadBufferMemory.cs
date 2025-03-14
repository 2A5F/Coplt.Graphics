namespace Coplt.Graphics.Core;

public readonly ref struct ImageUploadBufferMemory(Span<byte> RawSpan, uint RowStride, uint RowCount)
{
    public readonly Span<byte> RawSpan = RawSpan;
    public readonly uint RowStride = RowStride;
    public readonly uint RowCount = RowCount;

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
