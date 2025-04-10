using System.Numerics;
using System.Runtime.CompilerServices;
using System.Runtime.Intrinsics;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics;

public struct Color
    : IEquatable<Color>, IComparable<Color>, IComparable,
        IAdditionOperators<Color, Color, Color>,
        ISubtractionOperators<Color, Color, Color>,
        IMultiplyOperators<Color, Color, Color>,
        IDivisionOperators<Color, Color, Color>,
        IModulusOperators<Color, Color, Color>
{
    #region Fields

    internal Vector128<float> m_vector;

    #endregion

    #region Ctor

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color(float r, float g, float b, float a = 1)
    {
        m_vector = Vector128.Create(r, g, b, a);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color(Vector128<float> vector)
    {
        m_vector = vector;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color From255(float r, float g, float b, float a = 1) => new Color255(r, g, b, a).To01;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color From255(byte r, byte g, byte b, float a = 1) => new Color255(r, g, b, a).To01;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color FromHsv(float h, float s, float v, float a = 1) => new ColorHsv(h, s, v, a).Rgb;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color FromHsl(float h, float s, float l, float a = 1) => new ColorHsv(h, s, l, a).Rgb;

    #endregion

    #region Consts

    internal const float Epsilon = 1e-10f;

    public static readonly Color Transparent = new(0f, 0f, 0f, 0f);
    public static readonly Color Black = new(0f, 0f, 0f, 1f);
    public static readonly Color White = new(1f, 1f, 1f, 1f);

    public static readonly Color Red = new(1f, 0f, 0f, 1f);
    public static readonly Color Green = new(0f, 1f, 0f, 1f);
    public static readonly Color Blue = new(0f, 0f, 1f, 1f);

    #endregion

    #region Vector

    public Vector128<float> Vector
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = value;
    }

    #endregion

    #region Getter

    public float R
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(0);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(0, value);
    }
    public float G
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(1);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(1, value);
    }
    public float B
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(2, value);
    }
    public float A
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(3);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(3, value);
    }

    #endregion

    #region + - * /

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color operator +(Color a, Color b) => new(a.Vector + b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color operator -(Color a, Color b) => new(a.Vector - b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color operator *(Color a, Color b) => new(a.Vector * b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color operator /(Color a, Color b) => new(a.Vector / b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color operator %(Color a, Color b) => new(Utils.Rem(a.Vector, b.Vector));

    #endregion

    #region Equals

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool Equals(Color other) => m_vector.Equals(other.m_vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override bool Equals(object? obj) => obj is Color other && Equals(other);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override int GetHashCode() => m_vector.GetHashCode();
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator ==(Color left, Color right) => left.Equals(right);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator !=(Color left, Color right) => !left.Equals(right);

    #endregion

    #region CompareTo

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(Color other)
    {
        var c = R.CompareTo(other.R);
        if (c != 0) return c;
        c = G.CompareTo(other.G);
        if (c != 0) return c;
        c = B.CompareTo(other.B);
        if (c != 0) return c;
        c = A.CompareTo(other.A);
        if (c != 0) return c;
        return 0;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(object? obj)
    {
        if (obj is null) return 1;
        return obj is Color other
            ? CompareTo(other)
            : throw new ArgumentException($"Object must be of type {nameof(Color)}");
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <(Color left, Color right) => left.CompareTo(right) < 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >(Color left, Color right) => left.CompareTo(right) > 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <=(Color left, Color right) => left.CompareTo(right) <= 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >=(Color left, Color right) => left.CompareTo(right) >= 0;

    #endregion

    #region ToString

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override string ToString() => $"{nameof(Color)}({R}, {G}, {B}, {A})";

    #endregion

    #region Convert

    public Color255 To255
    {
        [MethodImpl(256 | 512)]
        get
        {
            var rgb = m_vector * Vector128.Create(255f);
            return new(
                Vector128.ConditionalSelect(
                    Vector128.Create(-1, -1, -1, 0).AsSingle(),
                    rgb,
                    m_vector
                )
            );
        }
        [MethodImpl(256 | 512)]
        set => this = value.To01;
    }

    /// <returns>w is invalid</returns>
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    internal (Vector128<float> H, Vector128<float> C, Vector128<float> V) ToHcv()
    {
        var R = Vector128.Shuffle(m_vector, Vector128.Create(0, 0, 0, 0));
        var G = Vector128.Shuffle(m_vector, Vector128.Create(1, 1, 1, 1));
        var B = Vector128.Shuffle(m_vector, Vector128.Create(2, 2, 2, 2));
        var GB = Vector128.Shuffle(m_vector, Vector128.Create(1, 2, 1, 2));
        var BG = Vector128.Shuffle(m_vector, Vector128.Create(2, 1, 2, 1));
        var P = Vector128.ConditionalSelect(
            Vector128.LessThan(G, B),
            Vector128.Create(
                BG.GetLower(),
                Vector64.Create(-1f, 2f / 3f)
            ),
            Vector128.Create(
                GB.GetLower(),
                Vector64.Create(0f, -1f / 3f)
            )
        );
        var PX = Vector128.Shuffle(P, Vector128.Create(0, 0, 0, 0));
        var P_XYW = Vector128.Shuffle(P, Vector128.Create(0, 1, 3, 2));
        var P_YZX = Vector128.Shuffle(P, Vector128.Create(1, 2, 0, 3));
        var Q = Vector128.ConditionalSelect(
            Vector128.LessThan(R, PX),
            Vector128.ConditionalSelect(
                Vector128.Create(-1, -1, -1, 0).AsSingle(),
                P_XYW,
                R
            ),
            Vector128.ConditionalSelect(
                Vector128.Create(-1, 0, 0, 0).AsSingle(),
                R,
                P_YZX
            )
        );
        var QX = Vector128.Shuffle(Q, Vector128.Create(0, 0, 0, 0));
        var QY = Vector128.Shuffle(Q, Vector128.Create(1, 1, 1, 1));
        var QZ = Vector128.Shuffle(Q, Vector128.Create(2, 2, 2, 2));
        var QW = Vector128.Shuffle(Q, Vector128.Create(3, 3, 3, 3));
        var C = QX - Vector128.Min(QW, QY);
        var H = Vector128.Abs((QW - QY) / Utils.Fma(Vector128.Create(6f), C, Vector128.Create(Epsilon)) + QZ);
        return (H, C, QX);
    }

    public ColorHsv Hsv
    {
        [MethodImpl(256 | 512)]
        get
        {
            var (H, C, V) = ToHcv();
            var S = C / (V + Vector128.Create(Epsilon));
            var v = Vector128.ConditionalSelect(
                Vector128.Create(-1, -1, 0, 0).AsSingle(),
                Vector128.ConditionalSelect(
                    Vector128.Create(-1, 0, 0, 0).AsSingle(),
                    H,
                    S
                ),
                Vector128.ConditionalSelect(
                    Vector128.Create(-1, -1, -1, 0).AsSingle(),
                    V,
                    m_vector
                )
            );
            return new(v);
        }
        [MethodImpl(256 | 512)]
        set => this = value.Rgb;
    }

    public ColorHsl Hsl
    {
        [MethodImpl(256 | 512)]
        get
        {
            var (H, C, V) = ToHcv();
            var L = Utils.Fnma(C, Vector128.Create(0.5f), V);
            var S = C / (
                Vector128<float>.One - Vector128.Abs(Utils.Fma(L, Vector128.Create(2f), Vector128.Create(-1f)))
                + Vector128.Create(Epsilon)
            );
            var v = Vector128.ConditionalSelect(
                Vector128.Create(-1, -1, 0, 0).AsSingle(),
                Vector128.ConditionalSelect(
                    Vector128.Create(-1, 0, 0, 0).AsSingle(),
                    H,
                    S
                ),
                Vector128.ConditionalSelect(
                    Vector128.Create(-1, -1, -1, 0).AsSingle(),
                    L,
                    m_vector
                )
            );
            return new(v);
        }
        [MethodImpl(256 | 512)]
        set => this = value.Rgb;
    }

    #endregion

    #region Shuffle

    public Color RgbaToBgra
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(2, 1, 0, 3));
            return new(r);
        }
    }

    public Color BgraToRgba
    {
        [MethodImpl(256 | 512)]
        get => RgbaToBgra;
    }

    public Color RgbaToArgb
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(3, 0, 1, 2));
            return new(r);
        }
    }

    public Color ArgbToRgba
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(1, 2, 3, 0));
            return new(r);
        }
    }

    public Color RgbaToAbgr
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(3, 2, 1, 0));
            return new(r);
        }
    }

    public Color AbgrToRgba
    {
        [MethodImpl(256 | 512)]
        get => RgbaToAbgr;
    }

    #endregion

    #region Linear

    public readonly Color ToLinear
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var c = Vector128.LessThanOrEqual(vector, Vector128.Create(0.04045f));
            var low = vector * Vector128.Create(1f / 12.92f);
            var high = Utils.Pow((vector + Vector128.Create(0.055f)) * Vector128.Create(1f / 1.055f), Vector128.Create(2.4f));
            var r = Vector128.ConditionalSelect(c, low, high);
            return new(Vector128.ConditionalSelect(Vector128.Create(-1, -1, -1, 0).AsSingle(), r, vector));
        }
    }

    public readonly Color ToSrgb
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var c = Vector128.LessThanOrEqual(vector, Vector128.Create(0.0031308f));
            var low = vector * Vector128.Create(12.92f);
            var high = Utils.Fma(Vector128.Create(1.055f), Utils.Pow(vector, Vector128.Create(1f / 2.4f)), Vector128.Create(-0.055f));
            var r = Vector128.ConditionalSelect(c, low, high);
            return new(Vector128.ConditionalSelect(Vector128.Create(-1, -1, -1, 0).AsSingle(), r, vector));
        }
    }

    #endregion
}

public struct Color255
    : IEquatable<Color255>, IComparable<Color255>, IComparable,
        IAdditionOperators<Color255, Color255, Color255>,
        ISubtractionOperators<Color255, Color255, Color255>,
        IMultiplyOperators<Color255, Color255, Color255>,
        IDivisionOperators<Color255, Color255, Color255>,
        IModulusOperators<Color255, Color255, Color255>
{
    #region Fields

    internal Vector128<float> m_vector;

    #endregion

    #region Ctor

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color255(float r, float g, float b, float a)
    {
        m_vector = Vector128.Create(r, g, b, a);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color255(byte r, byte g, byte b, float a)
    {
        m_vector = Vector128.Create(r, g, b, a);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color255(Vector128<float> vector)
    {
        m_vector = vector;
    }

    #endregion

    #region Consts

    internal const float Epsilon = 1e-10f;

    public static readonly Color255 Transparent = new(0f, 0f, 0f, 0f);
    public static readonly Color255 Black = new(0f, 0f, 0f, 1f);
    public static readonly Color255 White = new(255f, 255f, 255f, 1f);
    public static readonly Color255 One = new(1f, 1f, 1f, 1f);

    public static readonly Color255 Red = new(255f, 0f, 0f, 1f);
    public static readonly Color255 Green = new(0f, 255f, 0f, 1f);
    public static readonly Color255 Blue = new(0f, 255f, 0f, 1f);

    #endregion

    #region Vector

    public Vector128<float> Vector
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = value;
    }

    #endregion

    #region Getter

    public float R
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(0);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(0, value);
    }
    public float G
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(1);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(1, value);
    }
    public float B
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(2, value);
    }
    public float A
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(3);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(3, value);
    }

    #endregion

    #region + - * /

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color255 operator +(Color255 a, Color255 b) => new(a.Vector + b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color255 operator -(Color255 a, Color255 b) => new(a.Vector - b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color255 operator *(Color255 a, Color255 b) => new(a.Vector * b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color255 operator /(Color255 a, Color255 b) => new(a.Vector / b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Color255 operator %(Color255 a, Color255 b) => new(Utils.Rem(a.Vector, b.Vector));

    #endregion

    #region Equals

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool Equals(Color255 other) => m_vector.Equals(other.m_vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override bool Equals(object? obj) => obj is Color255 other && Equals(other);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override int GetHashCode() => m_vector.GetHashCode();
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator ==(Color255 left, Color255 right) => left.Equals(right);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator !=(Color255 left, Color255 right) => !left.Equals(right);

    #endregion

    #region CompareTo

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(Color255 other)
    {
        var c = R.CompareTo(other.R);
        if (c != 0) return c;
        c = G.CompareTo(other.G);
        if (c != 0) return c;
        c = B.CompareTo(other.B);
        if (c != 0) return c;
        c = A.CompareTo(other.A);
        if (c != 0) return c;
        return 0;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(object? obj)
    {
        if (obj is null) return 1;
        return obj is Color255 other
            ? CompareTo(other)
            : throw new ArgumentException($"Object must be of type {nameof(Color255)}");
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <(Color255 left, Color255 right) => left.CompareTo(right) < 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >(Color255 left, Color255 right) => left.CompareTo(right) > 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <=(Color255 left, Color255 right) => left.CompareTo(right) <= 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >=(Color255 left, Color255 right) => left.CompareTo(right) >= 0;

    #endregion

    #region ToString

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override string ToString() => $"RGB({R:F0}, {G:F0}, {B:F0}, {A:P0})";

    #endregion

    #region Convert

    public Color To01
    {
        [MethodImpl(256 | 512)]
        get
        {
            var rgb = m_vector * Vector128.Create(1f / 255f);
            return new(
                Vector128.ConditionalSelect(
                    Vector128.Create(-1, -1, -1, 0).AsSingle(),
                    rgb,
                    m_vector
                )
            );
        }
        [MethodImpl(256 | 512)]
        set => this = value.To255;
    }

    public ColorHsv Hsv
    {
        [MethodImpl(256 | 512)]
        get => To01.Hsv;
        [MethodImpl(256 | 512)]
        set => this = value.Rgb255;
    }

    public ColorHsl Hsl
    {
        [MethodImpl(256 | 512)]
        get => To01.Hsl;
        [MethodImpl(256 | 512)]
        set => this = value.Rgb255;
    }

    #endregion

    #region Shuffle

    public Color255 RgbaToBgra
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(2, 1, 0, 3));
            return new(r);
        }
    }

    public Color255 BgraToRgba
    {
        [MethodImpl(256 | 512)]
        get => RgbaToBgra;
    }

    public Color255 RgbaToArgb
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(3, 0, 1, 2));
            return new(r);
        }
    }

    public Color255 ArgbToRgba
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(1, 2, 3, 0));
            return new(r);
        }
    }

    public Color255 RgbaToAbgr
    {
        [MethodImpl(256 | 512)]
        get
        {
            var vector = m_vector;
            var r = Vector128.Shuffle(vector, Vector128.Create(3, 2, 1, 0));
            return new(r);
        }
    }

    public Color255 AbgrToRgba
    {
        [MethodImpl(256 | 512)]
        get => RgbaToAbgr;
    }

    #endregion
}

public struct ColorHsv
    : IEquatable<ColorHsv>, IComparable<ColorHsv>, IComparable,
        IAdditionOperators<ColorHsv, ColorHsv, ColorHsv>,
        ISubtractionOperators<ColorHsv, ColorHsv, ColorHsv>,
        IMultiplyOperators<ColorHsv, ColorHsv, ColorHsv>,
        IDivisionOperators<ColorHsv, ColorHsv, ColorHsv>,
        IModulusOperators<ColorHsv, ColorHsv, ColorHsv>
{
    #region Fields

    internal Vector128<float> m_vector;

    #endregion

    #region Ctor

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public ColorHsv(float h, float s, float v, float a)
    {
        m_vector = Vector128.Create(h, s, v, a);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public ColorHsv(Vector128<float> vector)
    {
        m_vector = vector;
    }

    #endregion

    #region Consts

    public static readonly ColorHsv Transparent = new(0f, 0f, 0f, 0f);
    public static readonly ColorHsv Black = new(0f, 0f, 0f, 1f);
    public static readonly ColorHsv White = new(1f, 1f, 1f, 1f);

    public static readonly ColorHsv Red = new(0f, 1f, 1f, 1f);
    public static readonly ColorHsv Green = new(1f / 3f, 1f, 1f, 1f);
    public static readonly ColorHsv Blue = new(2f / 3f, 1f, 1f, 1f);

    #endregion

    #region Vector

    public Vector128<float> Vector
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = value;
    }

    #endregion

    #region Getter

    public float H
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(0);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(0, value);
    }
    public float S
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(1);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(1, value);
    }
    public float V
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(2, value);
    }
    public float A
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(3);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(3, value);
    }

    #endregion

    #region + - * /

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsv operator +(ColorHsv a, ColorHsv b) => new(a.Vector + b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsv operator -(ColorHsv a, ColorHsv b) => new(a.Vector - b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsv operator *(ColorHsv a, ColorHsv b) => new(a.Vector * b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsv operator /(ColorHsv a, ColorHsv b) => new(a.Vector / b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsv operator %(ColorHsv a, ColorHsv b) => new(Utils.Rem(a.Vector, b.Vector));

    #endregion

    #region Equals

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool Equals(ColorHsv other) => m_vector.Equals(other.m_vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override bool Equals(object? obj) => obj is ColorHsv other && Equals(other);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override int GetHashCode() => m_vector.GetHashCode();
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator ==(ColorHsv left, ColorHsv right) => left.Equals(right);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator !=(ColorHsv left, ColorHsv right) => !left.Equals(right);

    #endregion

    #region CompareTo

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(ColorHsv other)
    {
        var c = H.CompareTo(other.H);
        if (c != 0) return c;
        c = S.CompareTo(other.S);
        if (c != 0) return c;
        c = V.CompareTo(other.V);
        if (c != 0) return c;
        c = A.CompareTo(other.A);
        if (c != 0) return c;
        return 0;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(object? obj)
    {
        if (obj is null) return 1;
        return obj is ColorHsv other
            ? CompareTo(other)
            : throw new ArgumentException($"Object must be of type {nameof(ColorHsv)}");
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <(ColorHsv left, ColorHsv right) => left.CompareTo(right) < 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >(ColorHsv left, ColorHsv right) => left.CompareTo(right) > 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <=(ColorHsv left, ColorHsv right) => left.CompareTo(right) <= 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >=(ColorHsv left, ColorHsv right) => left.CompareTo(right) >= 0;

    #endregion

    #region ToString

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override string ToString() => $"HSVA({H * 360:F0}°, {S:P0}, {V:P0}, {A:P0})";

    #endregion

    #region Convert

    [MethodImpl(256 | 512)]
    internal static Vector128<float> HsvToRgb(Vector128<float> H, Vector128<float> S, Vector128<float> V)
    {
        var p = Vector128.Abs(
            Utils.Fma(
                Utils.Frac(H + Vector128.Create(1f, 2f / 3f, 1f / 3f, 0)),
                Vector128.Create(6f), Vector128.Create(-3f)
            )
        );
        return V * Utils.Lerp(
            Vector128<float>.One,
            Vector128.Clamp(p - Vector128<float>.One, Vector128<float>.Zero, Vector128<float>.One),
            S
        );
    }

    public Color Rgb
    {
        [MethodImpl(256 | 512)]
        get
        {
            var H = Vector128.Shuffle(m_vector, Vector128.Create(0, 0, 0, 0));
            var S = Vector128.Shuffle(m_vector, Vector128.Create(1, 1, 1, 1));
            var V = Vector128.Shuffle(m_vector, Vector128.Create(2, 2, 2, 2));
            var rgb = HsvToRgb(H, S, V);
            var v = Vector128.ConditionalSelect(
                Vector128.Create(-1, -1, -1, 0).AsSingle(),
                rgb,
                m_vector
            );
            return new(v);
        }
        [MethodImpl(256 | 512)]
        set => this = value.Hsv;
    }

    public Color255 Rgb255
    {
        [MethodImpl(256 | 512)]
        get => Rgb.To255;
        [MethodImpl(256 | 512)]
        set => this = value.Hsv;
    }

    public ColorHsl Hsl
    {
        [MethodImpl(256 | 512)]
        get
        {
            var S = Vector128.Shuffle(m_vector, Vector128.Create(1, 1, 1, 1));
            var V = Vector128.Shuffle(m_vector, Vector128.Create(2, 2, 2, 2));
            var L = V * Utils.Fnma(S, Vector128.Create(0.5f), Vector128<float>.One);
            var d = Vector128<float>.One - Vector128.Abs(Utils.Fma(L, Vector128.Create(2f), Vector128.Create(-1f)));
            var s = V * S / (d + Vector128.Create(Color.Epsilon));
            var sl = Vector128.Create(
                s.GetLower(),
                L.GetUpper()
            );
            var v = Vector128.ConditionalSelect(
                Vector128.Create(-1, 0, 0, -1).AsSingle(),
                m_vector,
                sl
            );
            return new(v);
        }
        [MethodImpl(256 | 512)]
        set => this = value.Hsv;
    }

    #endregion
}

public struct ColorHsl
    : IEquatable<ColorHsl>, IComparable<ColorHsl>, IComparable,
        IAdditionOperators<ColorHsl, ColorHsl, ColorHsl>,
        ISubtractionOperators<ColorHsl, ColorHsl, ColorHsl>,
        IMultiplyOperators<ColorHsl, ColorHsl, ColorHsl>,
        IDivisionOperators<ColorHsl, ColorHsl, ColorHsl>,
        IModulusOperators<ColorHsl, ColorHsl, ColorHsl>
{
    #region Fields

    internal Vector128<float> m_vector;

    #endregion

    #region Ctor

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public ColorHsl(float h, float s, float l, float a)
    {
        m_vector = Vector128.Create(h, s, l, a);
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public ColorHsl(Vector128<float> vector)
    {
        m_vector = vector;
    }

    #endregion

    #region Consts

    public static readonly ColorHsl Transparent = new(0f, 0f, 0f, 0f);
    public static readonly ColorHsl Black = new(0f, 0f, 0f, 1f);
    public static readonly ColorHsl White = new(1f, 1f, 1f, 1f);

    public static readonly ColorHsl Red = new(0f, 1f, 0.5f, 1f);
    public static readonly ColorHsl Green = new(1f / 3f, 1f, 0.5f, 1f);
    public static readonly ColorHsl Blue = new(2f / 3f, 1f, 0.5f, 1f);

    #endregion

    #region Vector

    public Vector128<float> Vector
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector;
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = value;
    }

    #endregion

    #region Getter

    public float H
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(0);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(0, value);
    }
    public float S
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(1);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(1, value);
    }
    public float L
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(2);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(2, value);
    }
    public float A
    {
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        get => m_vector.GetElement(3);
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        set => m_vector = m_vector.WithElement(3, value);
    }

    #endregion

    #region + - * /

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsl operator +(ColorHsl a, ColorHsl b) => new(a.Vector + b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsl operator -(ColorHsl a, ColorHsl b) => new(a.Vector - b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsl operator *(ColorHsl a, ColorHsl b) => new(a.Vector * b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsl operator /(ColorHsl a, ColorHsl b) => new(a.Vector / b.Vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ColorHsl operator %(ColorHsl a, ColorHsl b) => new(Utils.Rem(a.Vector, b.Vector));

    #endregion

    #region Equals

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool Equals(ColorHsl other) => m_vector.Equals(other.m_vector);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override bool Equals(object? obj) => obj is ColorHsl other && Equals(other);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override int GetHashCode() => m_vector.GetHashCode();
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator ==(ColorHsl left, ColorHsl right) => left.Equals(right);
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator !=(ColorHsl left, ColorHsl right) => !left.Equals(right);

    #endregion

    #region CompareTo

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(ColorHsl other)
    {
        var c = H.CompareTo(other.H);
        if (c != 0) return c;
        c = S.CompareTo(other.S);
        if (c != 0) return c;
        c = L.CompareTo(other.L);
        if (c != 0) return c;
        c = A.CompareTo(other.A);
        if (c != 0) return c;
        return 0;
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int CompareTo(object? obj)
    {
        if (obj is null) return 1;
        return obj is ColorHsl other
            ? CompareTo(other)
            : throw new ArgumentException($"Object must be of type {nameof(ColorHsl)}");
    }
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <(ColorHsl left, ColorHsl right) => left.CompareTo(right) < 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >(ColorHsl left, ColorHsl right) => left.CompareTo(right) > 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator <=(ColorHsl left, ColorHsl right) => left.CompareTo(right) <= 0;
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool operator >=(ColorHsl left, ColorHsl right) => left.CompareTo(right) >= 0;

    #endregion

    #region ToString

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public override string ToString() => $"HSLA({H * 360:F0}°, {S:P0}, {L:P0}, {A:P0})";

    #endregion

    #region Convert

    [MethodImpl(256 | 512)]
    internal (Vector128<float> S, Vector128<float> V) GetSV()
    {
        var S = Vector128.Shuffle(m_vector, Vector128.Create(1, 1, 1, 1));
        var L = Vector128.Shuffle(m_vector, Vector128.Create(2, 2, 2, 2));
        var V = Utils.Fma(Vector128.Min(L, Vector128<float>.One - L), S, L);
        var s = 2f * (Vector128<float>.One - L / Vector128.Max(V, Vector128.Create(Color.Epsilon)));
        return (s, V);
    }

    public ColorHsv Hsv
    {
        [MethodImpl(256 | 512)]
        get
        {
            var (S, V) = GetSV();
            var sv = Vector128.Create(S.GetLower(), V.GetUpper());
            var v = Vector128.ConditionalSelect(
                Vector128.Create(-1, 0, 0, -1).AsSingle(),
                m_vector,
                sv
            );
            return new(v);
        }
        [MethodImpl(256 | 512)]
        set => this = value.Hsl;
    }

    public Color Rgb
    {
        [MethodImpl(256 | 512)]
        get
        {
            var H = Vector128.Shuffle(m_vector, Vector128.Create(0, 0, 0, 0));
            var (S, V) = GetSV();
            var rgb = ColorHsv.HsvToRgb(H, S, V);
            var v = Vector128.ConditionalSelect(
                Vector128.Create(-1, -1, -1, 0).AsSingle(),
                rgb,
                m_vector
            );
            return new(v);
        }
        [MethodImpl(256 | 512)]
        set => this = value.Hsl;
    }

    public Color255 Rgb255
    {
        [MethodImpl(256 | 512)]
        get => Rgb.To255;
        [MethodImpl(256 | 512)]
        set => this = value.Hsl;
    }

    #endregion
}
