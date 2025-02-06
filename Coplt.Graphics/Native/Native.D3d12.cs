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

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, nuint>)(lpVtbl[2]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, Guid*, void*>)(lpVtbl[3]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, FResult*, FLogger*, FResult*>)(lpVtbl[4]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this), &result, logger);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FD3d12Instance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[5]))((META_FD3d12Instance*)Unsafe.AsPointer(ref this), &result, options, @out);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, nuint>)(lpVtbl[2]))((FD3d12Instance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12Instance*, Guid*, void*>)(lpVtbl[3]))((FD3d12Instance*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12Instance*, FResult*, FLogger*, FResult*>)(lpVtbl[4]))((FD3d12Instance*)Unsafe.AsPointer(ref this), &result, logger);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12Instance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[5]))((FD3d12Instance*)Unsafe.AsPointer(ref this), &result, options, @out);
        }
    }

    [NativeTypeName("struct META_FD3d12GpuDevice : Coplt::FGpuDevice")]
    public unsafe partial struct META_FD3d12GpuDevice
    {
        public FGpuDevice Base;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "db7ce2bc-155a-4fcc-9f7f-137694191ca7";
    }

    [NativeTypeName("struct FD3d12GpuDevice : Coplt::_internal::META_FD3d12GpuDevice")]
    public unsafe partial struct FD3d12GpuDevice
    {
        public META_FD3d12GpuDevice Base;
    }

    [NativeTypeName("struct META_FD3d12GpuQueue : Coplt::FGpuQueue")]
    public unsafe partial struct META_FD3d12GpuQueue
    {
        public FGpuQueue Base;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "11a59db3-49d8-4158-a276-5d80fd37d85f";
    }

    [NativeTypeName("struct FD3d12GpuQueue : Coplt::_internal::META_FD3d12GpuQueue")]
    public unsafe partial struct FD3d12GpuQueue
    {
        public META_FD3d12GpuQueue Base;
    }

    [NativeTypeName("struct META_FD3d12GpuOutput : Coplt::FGpuOutput")]
    public unsafe partial struct META_FD3d12GpuOutput
    {
        public FGpuOutput Base;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "8d068a07-118c-4d1b-9ab0-384576b61897";
    }

    [NativeTypeName("struct FD3d12GpuOutput : Coplt::_internal::META_FD3d12GpuOutput")]
    public unsafe partial struct FD3d12GpuOutput
    {
        public META_FD3d12GpuOutput Base;
    }

    [NativeTypeName("struct META_FD3d12GpuSwapChainOutput : Coplt::FD3d12GpuOutput")]
    public unsafe partial struct META_FD3d12GpuSwapChainOutput
    {
        public FD3d12GpuOutput Base;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "5ee90a8d-8c3d-48ad-915e-87f3d28dabe7";
    }

    [NativeTypeName("struct FD3d12GpuSwapChainOutput : Coplt::_internal::META_FD3d12GpuSwapChainOutput")]
    public unsafe partial struct FD3d12GpuSwapChainOutput
    {
        public META_FD3d12GpuSwapChainOutput Base;
    }

    public static unsafe partial class D3d12Native
    {
        [DllImport("Coplt.Graphics.Native.D3d12", CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        [return: NativeTypeName("Coplt::FInstance *")]
        public static extern FInstance* Coplt_Graphics_D3d12_Create_Instance();
    }
}
