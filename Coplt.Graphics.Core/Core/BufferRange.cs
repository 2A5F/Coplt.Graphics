﻿namespace Coplt.Graphics.Core;

public record struct BufferRange<T>(T Buffer, uint Offset = 0, uint Size = uint.MaxValue);
public record struct VertexBufferRange(uint Index, IVbvRes Buffer, uint Offset = 0, uint Size = uint.MaxValue);
