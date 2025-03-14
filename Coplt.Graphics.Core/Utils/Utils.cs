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

    #region Aligned256

    public static uint Aligned256(this uint value) => (value + 0xFFu) & ~0xFFu;
    public static ulong Aligned256(this ulong value) => (value + 0xFFu) & ~0xFFu;
    public static nuint Aligned256(this nuint value) => (value + 0xFFu) & ~0xFFu;

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

    #endregion
}
