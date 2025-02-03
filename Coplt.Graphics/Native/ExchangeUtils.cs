using System.Runtime.CompilerServices;
using InlineIL;
using static InlineIL.IL.Emit;

namespace Coplt.Graphics.Native;

public static unsafe class ExchangeUtils
{
    ///<inheritdoc cref="Interlocked.Exchange(ref IntPtr, IntPtr)"/>
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T* ExchangePtr<T>(ref T* ptr, T* new_value)
    {
        Ldarg_0();
        Ldarg_1();
        Call(new MethodRef(typeof(Interlocked), nameof(Interlocked.Exchange), typeof(IntPtr).MakeByRefType(),
            typeof(IntPtr)));
        Ret();
        throw IL.Unreachable();
    }

    ///<inheritdoc cref="Interlocked.Exchange(ref IntPtr, IntPtr)"/>
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static T* ExchangePtr<T>(ref T* ptr, T* new_value, out T* old_value) =>
        old_value = ExchangePtr(ref ptr, new_value);

    ///<inheritdoc cref="Interlocked.Exchange(ref IntPtr, IntPtr)"/>
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void* ExchangePtr(ref void* ptr, void* new_value)
    {
        Ldarg_0();
        Ldarg_1();
        Call(new MethodRef(typeof(Interlocked), nameof(Interlocked.Exchange), typeof(IntPtr).MakeByRefType(),
            typeof(IntPtr)));
        Ret();
        throw IL.Unreachable();
    }

    ///<inheritdoc cref="Interlocked.Exchange(ref IntPtr, IntPtr)"/>
    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static void* ExchangePtr(ref void* ptr, void* new_value, out void* old_value) =>
        old_value = ExchangePtr(ref ptr, new_value);
}
