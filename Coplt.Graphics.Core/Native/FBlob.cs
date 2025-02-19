using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Coplt.Graphics.Native;

public unsafe partial struct FBlob
{
    [UnscopedRef]
    public Span<byte> Span => MemoryMarshal.CreateSpan(ref DataRef, (int)Size);

    public nuint Size => m_size;

    [UnscopedRef]
    public byte* DataPtr => (byte*)Unsafe.AsPointer(ref DataRef);

    [UnscopedRef]
    public ref byte DataRef => ref Unsafe.As<nuint, byte>(ref Unsafe.Add(ref m_size, 1));

    [UnscopedRef]
    public Span<byte>.Enumerator GetEnumerator() => Span.GetEnumerator();
}
