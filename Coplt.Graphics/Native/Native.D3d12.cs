using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Coplt.Graphics.Native.D3d12
{
    [NativeTypeName("struct META_FD3d12Instance : Coplt::FInstance")]
    public unsafe partial struct META_FD3d12Instance
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "2f979fa9-5cb0-4396-bb59-16094b0f9b1a";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, void>)(lpVtbl[0]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, nuint>)(lpVtbl[1]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        public void* ObjectStart()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, void*>)(lpVtbl[2]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, nuint>)(lpVtbl[3]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, nuint>)(lpVtbl[4]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, nuint>)(lpVtbl[5]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, B8>)(lpVtbl[6]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, B8>)(lpVtbl[7]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, Guid*, void*>)(lpVtbl[8]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, FResult*, FLogger*, FResult*>)(lpVtbl[9]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this), &result, logger);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[10]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this), &result, options, @out);
        }
    }

    [NativeTypeName("struct FD3d12Instance : Coplt::_internal::META_FD3d12Instance")]
    public unsafe partial struct FD3d12Instance
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12Instance*, void>)(lpVtbl[0]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, nuint>)(lpVtbl[1]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        public void* ObjectStart()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, void*>)(lpVtbl[2]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, nuint>)(lpVtbl[3]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, nuint>)(lpVtbl[4]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, nuint>)(lpVtbl[5]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, B8>)(lpVtbl[6]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, B8>)(lpVtbl[7]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, Guid*, void*>)(lpVtbl[8]))((FD3d12Instance*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12Instance*, FResult*, FLogger*, FResult*>)(lpVtbl[9]))((FD3d12Instance*)Unsafe.AsPointer(ref this), &result, logger);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12Instance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[10]))((FD3d12Instance*)Unsafe.AsPointer(ref this), &result, options, @out);
        }
    }

    [NativeTypeName("struct META_FD3d12GpuDevice : Coplt::FGpuDevice")]
    public unsafe partial struct META_FD3d12GpuDevice
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "db7ce2bc-155a-4fcc-9f7f-137694191ca7";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, void>)(lpVtbl[0]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, nuint>)(lpVtbl[1]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        public void* ObjectStart()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, void*>)(lpVtbl[2]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, nuint>)(lpVtbl[3]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, nuint>)(lpVtbl[4]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, nuint>)(lpVtbl[5]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, B8>)(lpVtbl[6]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, B8>)(lpVtbl[7]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, Guid*, void*>)(lpVtbl[8]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FD3d12GpuDevice*, FResult*, Str8or16*, FResult*>)(lpVtbl[9]))((META_FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, name);
        }
    }

    [NativeTypeName("struct FD3d12GpuDevice : Coplt::_internal::META_FD3d12GpuDevice")]
    public unsafe partial struct FD3d12GpuDevice
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, void>)(lpVtbl[0]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, nuint>)(lpVtbl[1]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        public void* ObjectStart()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, void*>)(lpVtbl[2]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, nuint>)(lpVtbl[3]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, nuint>)(lpVtbl[4]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, nuint>)(lpVtbl[5]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, B8>)(lpVtbl[6]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, B8>)(lpVtbl[7]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, Guid*, void*>)(lpVtbl[8]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, Str8or16*, FResult*>)(lpVtbl[9]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, name);
        }
    }

    public static unsafe partial class D3d12Native
    {
        [DllImport("Coplt.Graphics.Native.D3d12", CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        [return: NativeTypeName("Coplt::FInstance *")]
        public static extern FInstance* Coplt_Graphics_D3d12_Create_Instance();
    }
}
