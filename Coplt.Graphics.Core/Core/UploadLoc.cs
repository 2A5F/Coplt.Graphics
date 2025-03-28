namespace Coplt.Graphics.Core;

public record struct UploadLoc(nuint Index, ulong Offset, uint Size, SubmitId SubmitId);
public record struct UploadLoc2(ulong RecordId, ulong RecordVersion, nuint Index, ulong Offset, uint Size);
