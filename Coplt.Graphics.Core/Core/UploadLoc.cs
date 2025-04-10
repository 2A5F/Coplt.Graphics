namespace Coplt.Graphics.Core;

public record struct UploadLoc(ulong RecordId, ulong RecordVersion, nuint Index, ulong Offset, uint Size);
