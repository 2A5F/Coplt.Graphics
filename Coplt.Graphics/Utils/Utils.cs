using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

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
}
