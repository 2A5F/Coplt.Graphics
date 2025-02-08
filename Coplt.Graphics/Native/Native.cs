using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using static Coplt.Graphics.Native.Native;

namespace Coplt.Graphics.Native
{
    public unsafe partial struct FObject : FObject.Interface
    {
        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FObject*, void>)(lpVtbl[0]))((FObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FObject*, nuint>)(lpVtbl[1]))((FObject*)Unsafe.AsPointer(ref this));
        }

        public interface Interface
        {
            void Dispose();

            [return: NativeTypeName("size_t")]
            nuint Release();
        }
    }

    [NativeTypeName("struct FRcObject : Coplt::FObject")]
    public unsafe partial struct FRcObject : FRcObject.Interface
    {
        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FRcObject*, void>)(lpVtbl[0]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, nuint>)(lpVtbl[1]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, nuint>)(lpVtbl[2]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FObject.Interface
        {
            [return: NativeTypeName("size_t")]
            nuint AddRef();
        }
    }

    [Guid("00000000-0000-0000-0000-000000000000")]
    [NativeTypeName("struct FUnknown : Coplt::FRcObject")]
    public unsafe partial struct FUnknown : FUnknown.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FUnknown));

        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "00000000-0000-0000-0000-000000000000";

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FUnknown*, void>)(lpVtbl[0]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, nuint>)(lpVtbl[1]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, nuint>)(lpVtbl[2]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, Guid*, void*>)(lpVtbl[3]))((FUnknown*)Unsafe.AsPointer(ref this), id);
        }

        public interface Interface : FRcObject.Interface
        {
            void* QueryInterface([NativeTypeName("const Guid &")] Guid* id);
        }
    }

    [Guid("63C7FC71-D775-42BB-891A-69DBB26E75C5")]
    [NativeTypeName("struct FString8 : Coplt::FUnknown")]
    public unsafe partial struct FString8 : FString8.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FString8));

        public void** lpVtbl;

        [NativeTypeName("const Char8 *")]
        public byte* m_data;

        [NativeTypeName("Coplt::usize")]
        public nuint m_size;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FString8*, void>)(lpVtbl[0]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, nuint>)(lpVtbl[1]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, nuint>)(lpVtbl[2]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, Guid*, void*>)(lpVtbl[3]))((FString8*)Unsafe.AsPointer(ref this), id);
        }

        public interface Interface : FUnknown.Interface
        {
        }
    }

    [Guid("669355D5-9443-487C-8486-B0B0C00D2367")]
    [NativeTypeName("struct FString16 : Coplt::FUnknown")]
    public unsafe partial struct FString16 : FString16.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FString16));

        public void** lpVtbl;

        [NativeTypeName("const Char16 *")]
        public char* m_data;

        [NativeTypeName("Coplt::usize")]
        public nuint m_size;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FString16*, void>)(lpVtbl[0]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, nuint>)(lpVtbl[1]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, nuint>)(lpVtbl[2]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, Guid*, void*>)(lpVtbl[3]))((FString16*)Unsafe.AsPointer(ref this), id);
        }

        public interface Interface : FUnknown.Interface
        {
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FMessageType : byte
    {
        CStr8,
        CStr16,
        Slice8,
        Slice16,
        String8,
        String16,
    }

    [StructLayout(LayoutKind.Explicit)]
    public unsafe partial struct FMessageUnion
    {
        [FieldOffset(0)]
        [NativeTypeName("const char *")]
        public byte* str;

        [FieldOffset(0)]
        [NativeTypeName("const wchar_t *")]
        public char* wstr;

        [FieldOffset(0)]
        [NativeTypeName("const Char8 *")]
        public byte* str8;

        [FieldOffset(0)]
        [NativeTypeName("const Char16 *")]
        public char* str16;

        [FieldOffset(0)]
        [NativeTypeName("Coplt::FString8 *")]
        public FString8* string8;

        [FieldOffset(0)]
        [NativeTypeName("Coplt::FString16 *")]
        public FString16* string16;
    }

    public partial struct FMessage
    {
        [NativeTypeName("Coplt::FMessageUnion")]
        public FMessageUnion msg;

        [NativeTypeName("Coplt::i32")]
        public int len;

        [NativeTypeName("Coplt::FMessageType")]
        public FMessageType type;
    }

    public unsafe partial struct Str8
    {
        [NativeTypeName("const char *")]
        public byte* str;

        [NativeTypeName("Coplt::i32")]
        public int len;
    }

    public unsafe partial struct Str16
    {
        [NativeTypeName("const Char16 *")]
        public char* str;

        [NativeTypeName("Coplt::i32")]
        public int len;
    }

    public unsafe partial struct Str8or16
    {
        [NativeTypeName("const char *")]
        public byte* str8;

        [NativeTypeName("const Char16 *")]
        public char* str16;

        [NativeTypeName("Coplt::i32")]
        public int len;
    }

    public unsafe partial struct CStr8or16
    {
        [NativeTypeName("const char *")]
        public byte* str8;

        [NativeTypeName("const Char16 *")]
        public char* str16;
    }

    [NativeTypeName("Coplt::u32")]
    public enum FLogLevel : uint
    {
        Fatal = 0,
        Error,
        Warn,
        Info,
        Debug,
        Trace,
    }

    [NativeTypeName("Coplt::u32")]
    public enum FLogType : uint
    {
        Common,
        DirectX,
    }

    public unsafe partial struct FLogger
    {
        public void* p_object;

        [NativeTypeName("Coplt::FLoggerEnableFn *")]
        public delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, B8> pf_enable;

        [NativeTypeName("Coplt::FLoggerFn *")]
        public delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, FMessage*, void> pf_logger;

        [NativeTypeName("Coplt::FLoggerDropFn *")]
        public delegate* unmanaged[Cdecl]<void*, void> pf_drop;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FResultCode : byte
    {
        Success,
        Error,
    }

    public partial struct FResult
    {
        [NativeTypeName("Coplt::FMessage")]
        public FMessage msg;

        [NativeTypeName("Coplt::FResultCode")]
        public FResultCode code;
    }

    [Guid("9FC6890B-E9A2-4F4A-9217-A14A4223A715")]
    [NativeTypeName("struct FGpuObject : Coplt::FUnknown")]
    public unsafe partial struct FGpuObject : FGpuObject.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuObject));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuObject*, void>)(lpVtbl[0]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, nuint>)(lpVtbl[1]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, nuint>)(lpVtbl[2]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, Guid*, void*>)(lpVtbl[3]))((FGpuObject*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuObject*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuObject*)Unsafe.AsPointer(ref this), &result, name);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name);
        }
    }

    [Guid("534B3C0C-098D-4157-8ADD-9B79EB5D690F")]
    [NativeTypeName("struct FAllocator : Coplt::FUnknown")]
    public unsafe partial struct FAllocator : FAllocator.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FAllocator));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FAllocator*, void>)(lpVtbl[0]))((FAllocator*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, nuint>)(lpVtbl[1]))((FAllocator*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, nuint>)(lpVtbl[2]))((FAllocator*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, Guid*, void*>)(lpVtbl[3]))((FAllocator*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* MemoryAlloc([NativeTypeName("size_t")] nuint size, [NativeTypeName("size_t")] nuint align)
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, nuint, nuint, void*>)(lpVtbl[4]))((FAllocator*)Unsafe.AsPointer(ref this), size, align);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* MemoryAlloc([NativeTypeName("size_t")] nuint size)
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, nuint, void*>)(lpVtbl[5]))((FAllocator*)Unsafe.AsPointer(ref this), size);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* MemoryReAlloc(void* p, [NativeTypeName("size_t")] nuint size, [NativeTypeName("size_t")] nuint align)
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, void*, nuint, nuint, void*>)(lpVtbl[6]))((FAllocator*)Unsafe.AsPointer(ref this), p, size, align);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* MemoryReAlloc(void* p, [NativeTypeName("size_t")] nuint size)
        {
            return ((delegate* unmanaged[Thiscall]<FAllocator*, void*, nuint, void*>)(lpVtbl[7]))((FAllocator*)Unsafe.AsPointer(ref this), p, size);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void MemoryFree(void* p, [NativeTypeName("size_t")] nuint align)
        {
            ((delegate* unmanaged[Thiscall]<FAllocator*, void*, nuint, void>)(lpVtbl[8]))((FAllocator*)Unsafe.AsPointer(ref this), p, align);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void MemoryFree(void* p)
        {
            ((delegate* unmanaged[Thiscall]<FAllocator*, void*, void>)(lpVtbl[9]))((FAllocator*)Unsafe.AsPointer(ref this), p);
        }

        public interface Interface : FUnknown.Interface
        {
            void* MemoryAlloc([NativeTypeName("size_t")] nuint size, [NativeTypeName("size_t")] nuint align);

            void* MemoryAlloc([NativeTypeName("size_t")] nuint size);

            void* MemoryReAlloc(void* p, [NativeTypeName("size_t")] nuint size, [NativeTypeName("size_t")] nuint align);

            void* MemoryReAlloc(void* p, [NativeTypeName("size_t")] nuint size);

            void MemoryFree(void* p, [NativeTypeName("size_t")] nuint align);

            void MemoryFree(void* p);
        }
    }

    public partial struct FGpuDeviceCreateOptions
    {
    }

    public partial struct FGpuDeviceCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FD3dFeatureLevel")]
        public FD3dFeatureLevel D3dFeatureLevel;

        [NativeTypeName("Coplt::FVulkanVersion")]
        public FVulkanVersion VulkanVersion;

        [NativeTypeName("Coplt::FGpuPreference")]
        public FGpuPreference Preference;

        [NativeTypeName("Coplt::b8")]
        public B8 Debug;
    }

    public partial struct FGpuDevice
    {
    }

    [Guid("557F032D-ED50-403A-ADC5-214FDDBE6C6B")]
    [NativeTypeName("struct FGpuDevice : Coplt::FGpuObject")]
    public unsafe partial struct FGpuDevice : FGpuDevice.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuDevice));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuDevice*, void>)(lpVtbl[0]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, nuint>)(lpVtbl[1]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, nuint>)(lpVtbl[2]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, Guid*, void*>)(lpVtbl[3]))((FGpuDevice*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawDevice()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, void*>)(lpVtbl[5]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FMainQueueCreateOptions*, FGpuQueue**, FResult*>)(lpVtbl[6]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            void* GetRawDevice();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out);
        }
    }

    [Guid("CC2894BA-57C7-474A-B777-1A3E3A7C922C")]
    [NativeTypeName("struct FInstance : Coplt::FUnknown")]
    public unsafe partial struct FInstance : FInstance.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FInstance));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FAllocator *")]
        public FAllocator* m_allocator;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FInstance*, void>)(lpVtbl[0]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, nuint>)(lpVtbl[1]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, nuint>)(lpVtbl[2]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, Guid*, void*>)(lpVtbl[3]))((FInstance*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FLogger*, FResult*>)(lpVtbl[4]))((FInstance*)Unsafe.AsPointer(ref this), &result, logger);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[5]))((FInstance*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out);
        }
    }

    [NativeTypeName("Coplt::i32")]
    public enum FVulkanVersion
    {
        Unset = 0,
        _1_2 = 1002,
        _1_3 = 1003,
        _1_4 = 1004,
    }

    [NativeTypeName("Coplt::i32")]
    public enum FD3dFeatureLevel
    {
        Unset = 0,
        _12_1 = 0xc100,
        _12_2 = 0xc200,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuPreference : byte
    {
        HighPerformance = 0,
        MinimumPower = 1,
        Any = 255,
    }

    public partial struct FMainQueueCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuQueueType : byte
    {
        Direct,
        Compute,
        Copy,
    }

    [Guid("95E60E28-E387-4055-9B33-2D23AF901F8A")]
    [NativeTypeName("struct FGpuQueue : Coplt::FGpuObject")]
    public unsafe partial struct FGpuQueue : FGpuQueue.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuQueue));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FGpuQueueType")]
        public FGpuQueueType m_queue_type;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuQueue*, void>)(lpVtbl[0]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, nuint>)(lpVtbl[1]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, nuint>)(lpVtbl[2]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, Guid*, void*>)(lpVtbl[3]))((FGpuQueue*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawQueue()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, void*>)(lpVtbl[5]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuOutputCreateOptions*, void*, FGpuOutput**, FResult*>)(lpVtbl[6]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, options, hwnd, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            void* GetRawQueue();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out);
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuViewType : byte
    {
        None = 0,
        Cbv,
        Srv,
        Uav,
        Rtv,
        Dsv,
        Ibv,
        Vbv,
        Sov,
    }

    [Guid("F99DCEEC-2F0C-4A28-B666-BEB7C35219D6")]
    [NativeTypeName("struct FGpuResource : Coplt::FGpuObject")]
    public unsafe partial struct FGpuResource : FGpuResource.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuResource));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuResource*, void>)(lpVtbl[0]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, nuint>)(lpVtbl[1]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, nuint>)(lpVtbl[2]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, Guid*, void*>)(lpVtbl[3]))((FGpuResource*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuResource*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuResource*)Unsafe.AsPointer(ref this), &result, name);
        }

        public interface Interface : FGpuObject.Interface
        {
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FPresentMode : byte
    {
        NoBuffer,
        DoubleBuffer,
        TripleBuffer,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FOutputAlphaMode : byte
    {
        Opaque,
        PrePremultiplied,
        PostPremultiplied,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FHdrType : byte
    {
        None,
        UNorm10,
        Float16,
    }

    public partial struct FGpuOutputFormatSelector
    {
        [NativeTypeName("Coplt::b8")]
        public B8 Specify;

        [NativeTypeName("Coplt::b8")]
        public B8 Srgb;

        [NativeTypeName("Coplt::FHdrType")]
        public FHdrType Hdr;
    }

    public partial struct FGpuOutputCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::u32")]
        public uint Width;

        [NativeTypeName("Coplt::u32")]
        public uint Height;

        [NativeTypeName("Coplt::FTextureFormat")]
        public FTextureFormat Format;

        [NativeTypeName("Coplt::FPresentMode")]
        public FPresentMode PresentMode;

        [NativeTypeName("Coplt::FOutputAlphaMode")]
        public FOutputAlphaMode AlphaMode;

        [NativeTypeName("Coplt::b8")]
        public B8 VSync;

        [NativeTypeName("Coplt::FGpuOutputFormatSelector")]
        public FGpuOutputFormatSelector FormatSelector;
    }

    public partial struct FCommandSubmit
    {
    }

    public unsafe partial struct FCommandSubmit
    {
        [NativeTypeName("Coplt::FResourceMeta *")]
        public FResourceMeta* Resources;

        [NativeTypeName("Coplt::FCommandItem *")]
        public FCommandItem* Items;

        [NativeTypeName("Coplt::u8 *")]
        public byte* Payload;

        [NativeTypeName("Coplt::u32")]
        public uint Count;
    }

    [Guid("F1C59CB4-7EE6-4EE2-80F4-07CC568920D2")]
    [NativeTypeName("struct FGpuOutput : Coplt::FGpuObject")]
    public unsafe partial struct FGpuOutput : FGpuOutput.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuOutput));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState m_state;

        [NativeTypeName("Coplt::FTextureFormat")]
        public FTextureFormat m_format;

        [NativeTypeName("Coplt::u32")]
        public uint m_width;

        [NativeTypeName("Coplt::u32")]
        public uint m_height;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuOutput*, void>)(lpVtbl[0]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, nuint>)(lpVtbl[1]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, nuint>)(lpVtbl[2]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, Guid*, void*>)(lpVtbl[3]))((FGpuOutput*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, B8, FResult*>)(lpVtbl[5]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, Enable);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, uint, uint, FResult*>)(lpVtbl[6]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, Width, Height);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Present([NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FCommandSubmit*, FResult*>)(lpVtbl[7]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, submit);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult PresentNoWait([NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FCommandSubmit*, FResult*>)(lpVtbl[8]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, submit);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult WaitNextFrame()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FResult*>)(lpVtbl[9]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Present([NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit);

            [return: NativeTypeName("Coplt::FResult")]
            FResult PresentNoWait([NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit);

            [return: NativeTypeName("Coplt::FResult")]
            FResult WaitNextFrame();
        }
    }

    public enum FTextureFormat
    {
        Unknown = 0,
        R32G32B32A32_TypeLess = 1,
        R32G32B32A32_Float = 2,
        R32G32B32A32_UInt = 3,
        R32G32B32A32_SInt = 4,
        R32G32B32_TypeLess = 5,
        R32G32B32_Float = 6,
        R32G32B32_UInt = 7,
        R32G32B32_SInt = 8,
        R16G16B16A16_TypeLess = 9,
        R16G16B16A16_Float = 10,
        R16G16B16A16_UNorm = 11,
        R16G16B16A16_UInt = 12,
        R16G16B16A16_SNorm = 13,
        R16G16B16A16_SInt = 14,
        R32G32_TypeLess = 15,
        R32G32_Float = 16,
        R32G32_UInt = 17,
        R32G32_SInt = 18,
        R32G8X24_TypeLess = 19,
        D32_Float_S8X24_UInt = 20,
        R32_Float_X8X24_TypeLess = 21,
        X32_TypeLess_G8X24_Float = 22,
        R10G10B10A2_TypeLess = 23,
        R10G10B10A2_UNorm = 24,
        R10G10B10A2_UInt = 25,
        R11G11B10_Float = 26,
        R8G8B8A8_TypeLess = 27,
        R8G8B8A8_UNorm = 28,
        R8G8B8A8_UNorm_sRGB = 29,
        R8G8B8A8_UInt = 30,
        R8G8B8A8_SNorm = 31,
        R8G8B8A8_SInt = 32,
        R16G16_TypeLess = 33,
        R16G16_Float = 34,
        R16G16_UNorm = 35,
        R16G16_UInt = 36,
        R16G16_SNorm = 37,
        R16G16_SInt = 38,
        R32_TypeLess = 39,
        D32_Float = 40,
        R32_Float = 41,
        R32_UInt = 42,
        R32_SInt = 43,
        R24G8_TypeLess = 44,
        D24_UNorm_S8_UInt = 45,
        R24_UNorm_X8_TypeLess = 46,
        X24_TypeLess_G8_UInt = 47,
        R8G8_TypeLess = 48,
        R8G8_UNorm = 49,
        R8G8_UInt = 50,
        R8G8_SNorm = 51,
        R8G8_SInt = 52,
        R16_TypeLess = 53,
        R16_Float = 54,
        D16_UNorm = 55,
        R16_UNorm = 56,
        R16_UInt = 57,
        R16_SNorm = 58,
        R16_SInt = 59,
        R8_TypeLess = 60,
        R8_UNorm = 61,
        R8_UInt = 62,
        R8_SNorm = 63,
        R8_SInt = 64,
        A8_UNorm = 65,
        R1_UNorm = 66,
        R9G9B9E5_SharedExp = 67,
        R8G8_B8G8_UNorm = 68,
        G8R8_G8B8_UNorm = 69,
        BC1_TypeLess = 70,
        BC1_UNorm = 71,
        BC1_UNorm_sRGB = 72,
        BC2_TypeLess = 73,
        BC2_UNorm = 74,
        BC2_UNorm_sRGB = 75,
        BC3_TypeLess = 76,
        BC3_UNorm = 77,
        BC3_UNorm_sRGB = 78,
        BC4_TypeLess = 79,
        BC4_UNorm = 80,
        BC4_SNorm = 81,
        BC5_TypeLess = 82,
        BC5_UNorm = 83,
        BC5_SNorm = 84,
        B5G6R5_UNorm = 85,
        B5G5R5A1_UNorm = 86,
        B8G8R8A8_UNorm = 87,
        B8G8R8X8_UNorm = 88,
        R10G10B10_XR_Bias_A2_UNorm = 89,
        B8G8R8A8_TypeLess = 90,
        B8G8R8A8_UNorm_sRGB = 91,
        B8G8R8X8_TypeLess = 92,
        B8G8R8X8_UNorm_sRGB = 93,
        BC6H_TypeLess = 94,
        BC6H_UF16 = 95,
        BC6H_SF16 = 96,
        BC7_TypeLess = 97,
        BC7_UNorm = 98,
        BC7_UNorm_sRGB = 99,
    }

    [NativeTypeName("Coplt::u32")]
    public enum FResourceState : uint
    {
        Manual = 0,
        Common = 1 << 0,
        Present = 1 << 1,
        VertexBuffer = 1 << 2,
        IndexBuffer = 1 << 3,
        ConstantBuffer = 1 << 4,
        IndirectBuffer = 1 << 5,
        RenderTarget = 1 << 6,
        DepthRead = 1 << 7,
        DepthWrite = 1 << 8,
        ShaderResource = 1 << 9,
        UnorderedAccess = 1 << 10,
        CopySrc = 1 << 11,
        CopyDst = 1 << 12,
        ResolveSrc = 1 << 13,
        ResolveDst = 1 << 14,
        RayTracing = 1 << 15,
        ShadingRate = 1 << 16,
        StreamOutput = 1 << 17,
    }

    public partial struct FSamplerState
    {
        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::u32")]
        public uint Quality;
    }

    [NativeTypeName("Coplt::u32")]
    public enum FCommandType : uint
    {
        None,
        Transition,
        Present,
        ClearColor,
    }

    [NativeTypeName("Coplt::u32")]
    public enum FCommandFlags : uint
    {
        None = 0,
        DontTransition = 1 << 0,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FResourceRefType : byte
    {
        Texture,
        Output,
    }

    public unsafe partial struct FResourceMeta
    {
        [NativeTypeName("__AnonymousRecord_Command_L34_C9")]
        public _Anonymous_e__Union Anonymous;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState CurrentState;

        [NativeTypeName("Coplt::FResourceRefType")]
        public FResourceRefType Type;

        [UnscopedRef]
        public ref FGpuOutput* Output
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Output;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public unsafe partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGpuOutput *")]
            public FGpuOutput* Output;
        }
    }

    public partial struct FResourceSrc
    {
        [NativeTypeName("Coplt::u32")]
        public uint ResourceIndex;
    }

    public partial struct FRect
    {
        [NativeTypeName("Coplt::u32")]
        public uint left;

        [NativeTypeName("Coplt::u32")]
        public uint top;

        [NativeTypeName("Coplt::u32")]
        public uint right;

        [NativeTypeName("Coplt::u32")]
        public uint bottom;
    }

    public partial struct FCommandTransition
    {
        [NativeTypeName("Coplt::FResourceSrc")]
        public FResourceSrc Resource;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState SrcState;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState DstState;
    }

    public partial struct FCommandPresent
    {
        [NativeTypeName("Coplt::FResourceSrc")]
        public FResourceSrc Image;
    }

    public partial struct FCommandClearColor
    {
        [NativeTypeName("Coplt::u32")]
        public uint RectCount;

        [NativeTypeName("Coplt::u32")]
        public uint RectIndex;

        [NativeTypeName("Coplt::FResourceSrc")]
        public FResourceSrc Image;

        [NativeTypeName("f32[4]")]
        public _Color_e__FixedBuffer Color;

        [InlineArray(4)]
        public partial struct _Color_e__FixedBuffer
        {
            public float e0;
        }
    }

    public partial struct FCommandItem
    {
        [NativeTypeName("Coplt::FCommandType")]
        public FCommandType Type;

        [NativeTypeName("Coplt::FCommandFlags")]
        public FCommandFlags Flags;

        [NativeTypeName("__AnonymousRecord_Command_L102_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FCommandTransition Transition
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Transition;
            }
        }

        [UnscopedRef]
        public ref FCommandPresent Present
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Present;
            }
        }

        [UnscopedRef]
        public ref FCommandClearColor ClearColor
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.ClearColor;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandTransition")]
            public FCommandTransition Transition;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandPresent")]
            public FCommandPresent Present;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandClearColor")]
            public FCommandClearColor ClearColor;
        }
    }

    public static partial class Native
    {
        public static readonly Guid IID_FUnknown = new Guid(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

        public static readonly Guid IID_FString8 = new Guid(0x63C7FC71, 0xD775, 0x42BB, 0x89, 0x1A, 0x69, 0xDB, 0xB2, 0x6E, 0x75, 0xC5);

        public static readonly Guid IID_FString16 = new Guid(0x669355D5, 0x9443, 0x487C, 0x84, 0x86, 0xB0, 0xB0, 0xC0, 0x0D, 0x23, 0x67);

        public static readonly Guid IID_FGpuObject = new Guid(0x9FC6890B, 0xE9A2, 0x4F4A, 0x92, 0x17, 0xA1, 0x4A, 0x42, 0x23, 0xA7, 0x15);

        public static readonly Guid IID_FAllocator = new Guid(0x534B3C0C, 0x098D, 0x4157, 0x8A, 0xDD, 0x9B, 0x79, 0xEB, 0x5D, 0x69, 0x0F);

        public static readonly Guid IID_FInstance = new Guid(0xCC2894BA, 0x57C7, 0x474A, 0xB7, 0x77, 0x1A, 0x3E, 0x3A, 0x7C, 0x92, 0x2C);

        public static readonly Guid IID_FGpuDevice = new Guid(0x557F032D, 0xED50, 0x403A, 0xAD, 0xC5, 0x21, 0x4F, 0xDD, 0xBE, 0x6C, 0x6B);

        public static readonly Guid IID_FGpuQueue = new Guid(0x95E60E28, 0xE387, 0x4055, 0x9B, 0x33, 0x2D, 0x23, 0xAF, 0x90, 0x1F, 0x8A);

        public static readonly Guid IID_FGpuResource = new Guid(0xF99DCEEC, 0x2F0C, 0x4A28, 0xB6, 0x66, 0xBE, 0xB7, 0xC3, 0x52, 0x19, 0xD6);

        public static readonly Guid IID_FGpuOutput = new Guid(0xF1C59CB4, 0x7EE6, 0x4EE2, 0x80, 0xF4, 0x07, 0xCC, 0x56, 0x89, 0x20, 0xD2);
    }
}
