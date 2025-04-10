using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Runtime.Intrinsics;
using Coplt.Graphics.Core;
using System.Runtime.Intrinsics.Arm;
using System.Runtime.Intrinsics.Wasm;
using System.Runtime.Intrinsics.X86;
using X86 = System.Runtime.Intrinsics.X86;

namespace Coplt.Graphics.Utilities;

public static unsafe class Utils
{
    #region NativeDropGCHandle

    [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
    public static void NativeDropGCHandle(void* obj)
    {
        try
        {
            var handle = GCHandle.FromIntPtr((nint)obj);
            handle.Free();
        }
        catch (Exception e)
        {
            try
            {
                GraphicsInstance.EmitOnUnhandledException(e);
            }
            catch
            {
                // ignored
            }
        }
    }

    #endregion

    #region JoinUtf8String

    public static byte[] JoinUtf8String(ReadOnlySpan<byte> a, ReadOnlySpan<byte> b)
    {
        var array = new byte[a.Length + b.Length];
        a.CopyTo(array);
        b.CopyTo(array.AsSpan(a.Length));
        return array;
    }

    #endregion

    #region IsAligned

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool IsAligned(this uint value, uint alignment)
    {
        if (alignment == 0) throw new ArgumentException("Alignment must > 0", nameof(alignment));
        if (alignment == 1) return true;
        var mask = alignment - 1;
        if ((mask & alignment) != 0) throw new ArgumentException("Alignment must be power of 2", nameof(alignment));
        return (value & mask) == 0;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool IsAligned(this ulong value, ulong alignment)
    {
        if (alignment == 0) throw new ArgumentException("Alignment must > 0", nameof(alignment));
        if (alignment == 1) return true;
        var mask = alignment - 1;
        if ((mask & alignment) != 0) throw new ArgumentException("Alignment must be power of 2", nameof(alignment));
        return (value & mask) == 0;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static bool IsAligned(this nuint value, nuint alignment)
    {
        if (alignment == 0) throw new ArgumentException("Alignment must > 0", nameof(alignment));
        if (alignment == 1) return true;
        var mask = alignment - 1;
        if ((mask & alignment) != 0) throw new ArgumentException("Alignment must be power of 2", nameof(alignment));
        return (value & mask) == 0;
    }

    #endregion

    #region Aligned

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static uint Aligned(this uint value, uint alignment)
    {
        if (alignment == 0) throw new ArgumentException("Alignment must > 0", nameof(alignment));
        if (alignment == 1) return value;
        var mask = alignment - 1;
        if ((mask & alignment) != 0) throw new ArgumentException("Alignment must be power of 2", nameof(alignment));
        return (value + mask) & ~mask;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static ulong Aligned(this ulong value, ulong alignment)
    {
        if (alignment == 0) throw new ArgumentException("Alignment must > 0", nameof(alignment));
        if (alignment == 1) return value;
        var mask = alignment - 1;
        if ((mask & alignment) != 0) throw new ArgumentException("Alignment must be power of 2", nameof(alignment));
        return (value + mask) & ~mask;
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static nuint Aligned(this nuint value, nuint alignment)
    {
        if (alignment == 0) throw new ArgumentException("Alignment must > 0", nameof(alignment));
        if (alignment == 1) return value;
        var mask = alignment - 1;
        if ((mask & alignment) != 0) throw new ArgumentException("Alignment must be power of 2", nameof(alignment));
        return (value + mask) & ~mask;
    }

    #endregion

    #region AlignOf

    [StructLayout(LayoutKind.Sequential)]
    private struct AlignOfHelper<T> where T : unmanaged
    {
        public byte dummy;
        public T data;
    }

    public static int AlignOf<T>() where T : unmanaged => sizeof(AlignOfHelper<T>) - sizeof(T);

    #endregion

    #region List Span

    internal static Span<T> AsSpan<T>(this List<T> self) => CollectionsMarshal.AsSpan(self);
    internal static ref T At<T>(this List<T> self, int index) => ref CollectionsMarshal.AsSpan(self)[index];

    #endregion

    #region Simd Math

    [MethodImpl(256 | 512)]
    public static Vector128<float> Ne(Vector128<float> a, Vector128<float> b)
    {
        if (Sse.IsSupported)
        {
            return Sse.CompareNotEqual(a, b);
        }
        if (PackedSimd.IsSupported)
        {
            return PackedSimd.CompareNotEqual(a, b);
        }
        return ~Vector128.Equals(a, b);
    }

    [MethodImpl(256 | 512)]
    public static Vector128<float> Rem(Vector128<float> x, Vector128<float> y)
    {
        var div = x / y;
        var flr = RoundToZero(div);
        return Fnma(flr, y, x);
    }

    [MethodImpl(256 | 512)]
    private static Vector64<float> Round(Vector64<float> x)
    {
        if (Sse41.IsSupported)
        {
            return Sse41.RoundToNearestInteger(x.ToVector128()).GetLower();
        }
        if (AdvSimd.IsSupported)
        {
            return AdvSimd.RoundToNearest(x);
        }
        if (PackedSimd.IsSupported)
        {
            return PackedSimd.RoundToNearest(x.ToVector128()).GetLower();
        }
        return Vector64.Create(
            MathF.Round(x.GetElement(0)),
            MathF.Round(x.GetElement(1))
        );
    }

    [MethodImpl(256 | 512)]
    public static Vector128<float> Round(Vector128<float> x)
    {
        if (Sse41.IsSupported)
        {
            return Sse41.RoundToNearestInteger(x);
        }
        if (AdvSimd.IsSupported)
        {
            return AdvSimd.RoundToNearest(x);
        }
        if (PackedSimd.IsSupported)
        {
            return PackedSimd.RoundToNearest(x);
        }
        if (Vector64.IsHardwareAccelerated)
        {
            return Vector128.Create(
                Round(x.GetLower()),
                Round(x.GetUpper())
            );
        }
        return Vector128.Create(
            MathF.Round(x.GetElement(0)),
            MathF.Round(x.GetElement(1)),
            MathF.Round(x.GetElement(2)),
            MathF.Round(x.GetElement(3))
        );
    }

    [MethodImpl(256 | 512)]
    private static Vector64<float> RoundToZero(Vector64<float> x)
    {
        if (Sse41.IsSupported)
        {
            return Sse41.RoundToZero(x.ToVector128()).GetLower();
        }
        if (AdvSimd.IsSupported)
        {
            return AdvSimd.RoundToZero(x);
        }
        if (PackedSimd.IsSupported)
        {
            return PackedSimd.Truncate(x.ToVector128()).GetLower();
        }
        return Vector64.Create(
            MathF.Round(x.GetElement(0), MidpointRounding.ToZero),
            MathF.Round(x.GetElement(1), MidpointRounding.ToZero)
        );
    }

    [MethodImpl(256 | 512)]
    public static Vector128<float> RoundToZero(Vector128<float> x)
    {
        if (Sse41.IsSupported)
        {
            return Sse41.RoundToZero(x);
        }
        if (AdvSimd.IsSupported)
        {
            return AdvSimd.RoundToZero(x);
        }
        if (PackedSimd.IsSupported)
        {
            return PackedSimd.Truncate(x);
        }
        if (Vector64.IsHardwareAccelerated)
        {
            return Vector128.Create(
                RoundToZero(x.GetLower()),
                RoundToZero(x.GetUpper())
            );
        }
        return Vector128.Create(
            MathF.Round(x.GetElement(0), MidpointRounding.ToZero),
            MathF.Round(x.GetElement(1), MidpointRounding.ToZero),
            MathF.Round(x.GetElement(2), MidpointRounding.ToZero),
            MathF.Round(x.GetElement(3), MidpointRounding.ToZero)
        );
    }

    /// <returns><code>a * b + c</code></returns>
    [MethodImpl(256 | 512)]
    public static Vector128<float> Fma(Vector128<float> a, Vector128<float> b, Vector128<float> c)
    {
        if (X86.Fma.IsSupported)
        {
            return X86.Fma.MultiplyAdd(a, b, c);
        }
        if (AdvSimd.IsSupported)
        {
            return AdvSimd.FusedMultiplyAdd(c, a, b);
        }
        return a * b + c;
    }

    /// <returns><code>c - a * b</code> or <code>-(a * b) + c</code></returns>
    [MethodImpl(256 | 512)]
    public static Vector128<float> Fnma(Vector128<float> a, Vector128<float> b, Vector128<float> c)
    {
        if (X86.Fma.IsSupported)
        {
            return X86.Fma.MultiplyAddNegated(a, b, c);
        }
        if (AdvSimd.IsSupported)
        {
            return AdvSimd.FusedMultiplySubtract(c, a, b);
        }
        return c - a * b;
    }

    [MethodImpl(256 | 512)]
    public static Vector128<float> Lerp(Vector128<float> start, Vector128<float> end, Vector128<float> t) =>
        Fma(t, end - start, start);

    [MethodImpl(256 | 512)]
    public static Vector128<float> Frac(Vector128<float> a) => a - Vector128.Floor(a);

    [MethodImpl(256 | 512)]
    public static Vector128<float> Log2(Vector128<float> a)
    {
        var xl = Vector128.Max(a, Vector128<float>.Zero).AsInt32();
        var mantissa = (xl >>> 23) - Vector128.Create(0x7F);
        var r = Vector128.ConvertToSingle(mantissa);

        xl = (xl & Vector128.Create(0x7FFFFF)) | Vector128.Create(0x7F << 23);

        var d = (xl.AsSingle() | Vector128<float>.One) * Vector128.Create(2.0f / 3.0f);

        #region Approx

        // A Taylor Series approximation of ln(x) that relies on the identity that ln(x) = 2*atan((x-1)/(x+1)).
        d = (d - Vector128<float>.One) / (d + Vector128<float>.One);
        var sq = d * d;

        var rx = Fma(sq, Vector128.Create(0.2371599674224853515625f), Vector128.Create(0.285279005765914916992188f));
        rx = Fma(rx, sq, Vector128.Create(0.400005519390106201171875f));
        rx = Fma(rx, sq, Vector128.Create(0.666666567325592041015625f));
        rx = Fma(rx, sq, Vector128.Create(2.0f));

        d *= rx;

        #endregion

        r += Fma(d, Vector128.Create(1.4426950408889634f), Vector128.Create(0.58496250072115619f));

        r = Vector128.ConditionalSelect(
            Vector128.GreaterThan(a, Vector128<float>.Zero),
            r, Vector128.Create(float.NaN)
        );
        r = Vector128.ConditionalSelect(
            Vector128.Equals(a, Vector128.Create(float.PositiveInfinity)),
            Vector128.Create(float.PositiveInfinity), r
        );
        r = Vector128.ConditionalSelect(
            Vector128.Equals(a, Vector128<float>.Zero),
            Vector128.Create(float.NegativeInfinity), r
        );

        return r;
    }

    [MethodImpl(256 | 512)]
    public static Vector128<float> Exp2(Vector128<float> x)
    {
        // 1 / log(2)
        const float F_1_Div_Log2 = 1.4426950408889634073599246810018921374266459541529859341354494069f;

        var e = Vector128.GreaterThanOrEqual(x, Vector128.Create(89f)) & Vector128.Create(float.PositiveInfinity);
        e += Ne(x, x);

        var xx = Vector128.Max(
            Vector128.Min(x, Vector128.Create(81.0f * F_1_Div_Log2)),
            Vector128.Create(-81.0f * F_1_Div_Log2)
        );

        var fx = Round(xx);

        xx -= fx;
        var r = Fma(xx, Vector128.Create(1.530610536076361E-05f), Vector128.Create(0.000154631026827329f));
        r = Fma(r, xx, Vector128.Create(0.0013333465742372899f));
        r = Fma(r, xx, Vector128.Create(0.00961804886829518f));
        r = Fma(r, xx, Vector128.Create(0.05550410925060949f));
        r = Fma(r, xx, Vector128.Create(0.240226509999339f));
        r = Fma(r, xx, Vector128.Create(0.6931471805500692f));
        r = Fma(r, xx, Vector128.Create(1.0f));

        fx = ((Vector128.ConvertToInt32(fx) + Vector128.Create(127)) << 23).AsSingle();

        r = Fma(r, fx, e);
        r = Vector128.AndNot(r, Vector128.Equals(x, Vector128.Create(float.NegativeInfinity)));

        return r;
    }

    [MethodImpl(256 | 512)]
    public static Vector128<float> Pow(Vector128<float> a, Vector128<float> b)
    {
        var sig = Vector128.LessThan(a, default)
            & Ne(Rem(b, Vector128.Create(2.0f)), default)
            & Vector128.Create(0x8000_0000).AsSingle();
        var r = Exp2(Log2(Vector128.Abs(a)) * b);
        return r | sig;
    }

    #endregion
}
