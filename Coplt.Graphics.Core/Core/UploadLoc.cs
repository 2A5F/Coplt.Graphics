﻿namespace Coplt.Graphics.Core;

public record struct UploadLoc(nuint Index, ulong Offset, uint Size, SubmitId SubmitId);
