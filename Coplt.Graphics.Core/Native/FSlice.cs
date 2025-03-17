using System.Runtime.CompilerServices;

namespace Coplt.Graphics.Native;

public readonly unsafe struct FSlice<T>(T* Data, nuint Size) : IEquatable<FSlice<T>>
{
    #region Fields

    public readonly T* Data = Data;
    public readonly nuint Size = Size;

    #endregion

    #region Props

    public bool IsEmpty => Size == 0;
    public int Length => (int)(uint)Size;

    public Span<T> Span => new(Data, Length);

    public ref T this[int index] => ref Span[index];

    #endregion

    #region Convert

    public static implicit operator Span<T>(FSlice<T> Slice) => new(Slice.Data, Slice.Length);

    public static implicit operator ReadOnlySpan<T>(FSlice<T> Slice) => new(Slice.Data, Slice.Length);

    public static implicit operator FRoSlice<T>(FSlice<T> Slice) => new(Slice.Data, Slice.Size);

    #endregion

    #region Equals

    public bool Equals(FSlice<T> other) => Data == other.Data && Size == other.Size;
    public override bool Equals(object? obj) => obj is FSlice<T> other && Equals(other);
    public override int GetHashCode() => HashCode.Combine(unchecked((int)(long)Data), Size);
    public static bool operator ==(FSlice<T> left, FSlice<T> right) => left.Equals(right);
    public static bool operator !=(FSlice<T> left, FSlice<T> right) => !left.Equals(right);

    #endregion

    #region ToString

    public override string ToString()
    {
        if (typeof(T) == typeof(char)) return Span.ToString();
        return $"FSlice<{typeof(T).Name}>(0x{(nuint)Data:X})[{Size}]";
    }

    #endregion

    #region Slice

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public FSlice<T> Slice(int start)
    {
        ArgumentOutOfRangeException.ThrowIfGreaterThan((uint)start, (ulong)Size);
        return new(Data + (nuint)(uint)start, Size - (uint)start);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public FSlice<T> Slice(int start, int length)
    {
        ArgumentOutOfRangeException.ThrowIfGreaterThan((uint)start + (uint)length, (ulong)Size);
        return new(Data + (nuint)(uint)start, (uint)length);
    }

    #endregion

    #region Slice nuint

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public FSlice<T> Slice(nuint start)
    {
        ArgumentOutOfRangeException.ThrowIfGreaterThan(start, Size);
        return new(Data + start, Size - start);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public FSlice<T> Slice(nuint start, nuint length)
    {
        ArgumentOutOfRangeException.ThrowIfGreaterThan(start + length, Size);
        return new(Data + start, length);
    }

    #endregion

    #region Enumerator

    public Span<T>.Enumerator GetEnumerator() => Span.GetEnumerator();

    #endregion
}

public readonly unsafe struct FRoSlice<T>(T* Data, nuint Size) : IEquatable<FRoSlice<T>>
{
    #region Fields

    public readonly T* Data = Data;
    public readonly nuint Size = Size;

    #endregion

    #region Props

    public bool IsEmpty => Size == 0;
    public int Length => (int)(uint)Size;

    public ReadOnlySpan<T> Span => new(Data, Length);

    public ref readonly T this[int index] => ref Span[index];

    #endregion

    #region Convert

    public static explicit operator Span<T>(FRoSlice<T> Slice) => new(Slice.Data, Slice.Length);

    public static implicit operator ReadOnlySpan<T>(FRoSlice<T> Slice) => new(Slice.Data, Slice.Length);

    public static explicit operator FSlice<T>(FRoSlice<T> Slice) => new(Slice.Data, Slice.Size);

    #endregion

    #region Equals

    public bool Equals(FRoSlice<T> other) => Data == other.Data && Size == other.Size;
    public override bool Equals(object? obj) => obj is FRoSlice<T> other && Equals(other);
    public override int GetHashCode() => HashCode.Combine(unchecked((int)(long)Data), Size);
    public static bool operator ==(FRoSlice<T> left, FRoSlice<T> right) => left.Equals(right);
    public static bool operator !=(FRoSlice<T> left, FRoSlice<T> right) => !left.Equals(right);

    #endregion

    #region ToString

    public override string ToString()
    {
        if (typeof(T) == typeof(char)) return Span.ToString();
        return $"FRoSlice<{typeof(T).Name}>(0x{(nuint)Data:X})[{Size}]";
    }

    #endregion

    #region Slice

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public FRoSlice<T> Slice(int start)
    {
        ArgumentOutOfRangeException.ThrowIfGreaterThan((uint)start, (ulong)Size);
        return new(Data + (nuint)(uint)start, Size - (uint)start);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public FRoSlice<T> Slice(int start, int length)
    {
        ArgumentOutOfRangeException.ThrowIfGreaterThan((uint)start + (uint)length, (ulong)Size);
        return new(Data + (nuint)(uint)start, (uint)length);
    }

    #endregion

    #region Enumerator

    public ReadOnlySpan<T>.Enumerator GetEnumerator() => Span.GetEnumerator();

    #endregion
}
