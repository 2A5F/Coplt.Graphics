using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics;

namespace Coplt.Graphics.Core;

public record struct USize2d
{
    #region Fields

    internal Vector64<uint> m_vector;

    #endregion

    #region Ctor

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public USize2d(uint width, uint height) => m_vector = Vector64.Create(width, height);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public USize2d(Vector64<uint> vector)
    {
        m_vector = vector;
    }

    #endregion

    #region Vector

    public Vector64<uint> Vector
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = value;
    }

    #endregion

    #region Getter

    public uint Width
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(0);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(0, value);
    }

    public uint Height
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(1);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(1, value);
    }

    #endregion

    #region + - * /

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize2d operator +(USize2d a, USize2d b) => new(a.Vector + b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize2d operator -(USize2d a, USize2d b) => new(a.Vector - b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize2d operator *(USize2d a, USize2d b) => new(a.Vector * b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize2d operator /(USize2d a, USize2d b) => new(a.Vector / b.Vector);

    #endregion

    #region Equals

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public readonly bool Equals(USize2d other) => m_vector.Equals(other.m_vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public readonly override int GetHashCode() => m_vector.GetHashCode();

    #endregion
}

public record struct USize3d
{
    #region Fields

    internal Vector128<uint> m_vector;

    #endregion

    #region Ctor

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public USize3d(uint width, uint height, uint depth) => m_vector = Vector128.Create(width, height, depth, 0);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public USize3d(Vector128<uint> vector)
    {
        m_vector = vector.WithElement(3, 0u);
    }

    #endregion

    #region Vector

    public Vector128<uint> Vector
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = value;
    }

    #endregion

    #region Getter

    public uint Width
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(0);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(0, value);
    }

    public uint Height
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(1);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(1, value);
    }

    public uint Depth
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(2, value);
    }

    #endregion

    #region + - * /

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize3d operator +(USize3d a, USize3d b) => new(a.Vector + b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize3d operator -(USize3d a, USize3d b) => new(a.Vector - b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize3d operator *(USize3d a, USize3d b) => new(a.Vector * b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static USize3d operator /(USize3d a, USize3d b) => new(a.Vector / b.Vector.WithElement(3, 1u));

    #endregion

    #region Equals

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public readonly bool Equals(USize3d other) => m_vector.Equals(other.m_vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public readonly override int GetHashCode() => m_vector.GetHashCode();

    #endregion
}
