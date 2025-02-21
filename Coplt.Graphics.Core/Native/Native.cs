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

    [Guid("5A478800-E7DA-4A6B-B428-1E3FDA55997F")]
    [NativeTypeName("struct FString : Coplt::FUnknown")]
    public unsafe partial struct FString : FString.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FString));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FString*, void>)(lpVtbl[0]))((FString*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FString*, nuint>)(lpVtbl[1]))((FString*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FString*, nuint>)(lpVtbl[2]))((FString*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FString*, Guid*, void*>)(lpVtbl[3]))((FString*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::Str8or16")]
        public Str8or16 GetStr()
        {
            Str8or16 result;
            return *((delegate* unmanaged[Thiscall]<FString*, Str8or16*, Str8or16*>)(lpVtbl[4]))((FString*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::Str8or16")]
            Str8or16 GetStr();
        }
    }

    [Guid("63C7FC71-D775-42BB-891A-69DBB26E75C5")]
    [NativeTypeName("struct FString8 : Coplt::FString")]
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

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::Str8or16")]
        public Str8or16 GetStr()
        {
            Str8or16 result;
            return *((delegate* unmanaged[Thiscall]<FString8*, Str8or16*, Str8or16*>)(lpVtbl[4]))((FString8*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FString.Interface
        {
        }
    }

    [Guid("669355D5-9443-487C-8486-B0B0C00D2367")]
    [NativeTypeName("struct FString16 : Coplt::FString")]
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

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::Str8or16")]
        public Str8or16 GetStr()
        {
            Str8or16 result;
            return *((delegate* unmanaged[Thiscall]<FString16*, Str8or16*, Str8or16*>)(lpVtbl[4]))((FString16*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FString.Interface
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

    [Guid("C2A7F23D-4CCE-4EAE-A1E0-8FFAC4836870")]
    [NativeTypeName("struct FBlob : Coplt::FUnknown")]
    public unsafe partial struct FBlob : FBlob.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FBlob));

        public void** lpVtbl;

        [NativeTypeName("size_t")]
        public nuint m_strong;

        [NativeTypeName("size_t")]
        public nuint m_size;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FBlob*, void>)(lpVtbl[0]))((FBlob*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FBlob*, nuint>)(lpVtbl[1]))((FBlob*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FBlob*, nuint>)(lpVtbl[2]))((FBlob*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FBlob*, Guid*, void*>)(lpVtbl[3]))((FBlob*)Unsafe.AsPointer(ref this), id);
        }

        public interface Interface : FUnknown.Interface
        {
        }
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

        [NativeTypeName("Coplt::b8")]
        public B8 GpuBasedValidation;
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

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, FShaderModule** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderModuleCreateOptions*, FShaderModule**, FResult*>)(lpVtbl[7]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, FShaderLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderLayoutCreateOptions*, FShaderLayout**, FResult*>)(lpVtbl[8]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderInputLayoutCreateOptions*, FShaderInputLayout**, FResult*>)(lpVtbl[9]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, FShader** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderCreateOptions*, FShader**, FResult*>)(lpVtbl[10]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FMeshLayoutCreateOptions*, FMeshLayout**, FResult*>)(lpVtbl[11]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGraphicsShaderPipelineCreateOptions*, FGraphicsShaderPipeline**, FResult*>)(lpVtbl[12]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateBuffer([NativeTypeName("const FGpuBufferCreateOptions &")] FGpuBufferCreateOptions* options, FGpuBuffer** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGpuBufferCreateOptions*, FGpuBuffer**, FResult*>)(lpVtbl[13]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            void* GetRawDevice();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, FShaderModule** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, FShaderLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, FShader** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateBuffer([NativeTypeName("const FGpuBufferCreateOptions &")] FGpuBufferCreateOptions* options, FGpuBuffer** @out);
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
        public FResult CreateString8([NativeTypeName("const Char8 *")] byte* data, [NativeTypeName("Coplt::usize")] nuint size, FString8** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, byte*, nuint, FString8**, FResult*>)(lpVtbl[5]))((FInstance*)Unsafe.AsPointer(ref this), &result, data, size, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateString16([NativeTypeName("const Char16 *")] char* data, [NativeTypeName("Coplt::usize")] nuint size, FString16** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, char*, nuint, FString16**, FResult*>)(lpVtbl[6]))((FInstance*)Unsafe.AsPointer(ref this), &result, data, size, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateBlob([NativeTypeName("Coplt::usize")] nuint size, FBlob** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, nuint, FBlob**, FResult*>)(lpVtbl[7]))((FInstance*)Unsafe.AsPointer(ref this), &result, size, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[8]))((FInstance*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateString8([NativeTypeName("const Char8 *")] byte* data, [NativeTypeName("Coplt::usize")] nuint size, FString8** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateString16([NativeTypeName("const Char16 *")] char* data, [NativeTypeName("Coplt::usize")] nuint size, FString16** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateBlob([NativeTypeName("Coplt::usize")] nuint size, FBlob** @out);

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

    [NativeTypeName("Coplt::u8")]
    public enum FShaderStage : byte
    {
        Compute = 0,
        Pixel = 1,
        Vertex = 2,
        Mesh = 3,
        Task = 4,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderStageFlags : byte
    {
        None = 0,
        Compute = 1 << 0,
        Pixel = 1 << 1,
        Vertex = 1 << 2,
        Mesh = 1 << 3,
        Task = 1 << 4,
    }

    public unsafe partial struct FShaderModuleCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FString8 *")]
        public FString8* EntryPoint;

        public void* Data;

        [NativeTypeName("Coplt::usize")]
        public nuint Size;

        [NativeTypeName("Coplt::FShaderStage")]
        public FShaderStage Stage;
    }

    [Guid("5C0E1FDB-2ACD-4FCE-B985-09E12A7A1AAD")]
    [NativeTypeName("struct FShaderModule : Coplt::FGpuObject")]
    public unsafe partial struct FShaderModule : FShaderModule.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShaderModule));

        public void** lpVtbl;

        [NativeTypeName("Coplt::u8 *")]
        public byte* Data;

        [NativeTypeName("Coplt::usize")]
        public nuint Size;

        [NativeTypeName("Coplt::FShaderStage")]
        public FShaderStage Stage;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShaderModule*, void>)(lpVtbl[0]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, nuint>)(lpVtbl[1]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, nuint>)(lpVtbl[2]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, Guid*, void*>)(lpVtbl[3]))((FShaderModule*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderModule*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FShaderModule*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FString8 *")]
        public FString8* GetEntryPoint()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, FString8*>)(lpVtbl[5]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FString8 *")]
            FString8* GetEntryPoint();
        }
    }

    public partial struct FShaderLayout
    {
    }

    [Guid("552A498E-8F3A-47FF-A335-7AF2DE0901E8")]
    [NativeTypeName("struct FShaderLayout : Coplt::FGpuObject")]
    public unsafe partial struct FShaderLayout : FShaderLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShaderLayout));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShaderLayout*, void>)(lpVtbl[0]))((FShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, nuint>)(lpVtbl[1]))((FShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, nuint>)(lpVtbl[2]))((FShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, Guid*, void*>)(lpVtbl[3]))((FShaderLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderLayout*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FShaderLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderLayoutItemDefine *")]
        public FShaderLayoutItemDefine* GetItemDefines([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, uint*, FShaderLayoutItemDefine*>)(lpVtbl[5]))((FShaderLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("const FShaderLayoutItemDefine *")]
            FShaderLayoutItemDefine* GetItemDefines([NativeTypeName("Coplt::u32 *")] uint* out_count);
        }
    }

    public partial struct FShaderInputLayout
    {
    }

    [Guid("70229C9A-FB3D-46B4-B534-72FDB167D807")]
    [NativeTypeName("struct FShaderInputLayout : Coplt::FGpuObject")]
    public unsafe partial struct FShaderInputLayout : FShaderInputLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShaderInputLayout));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, void>)(lpVtbl[0]))((FShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, nuint>)(lpVtbl[1]))((FShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, nuint>)(lpVtbl[2]))((FShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, Guid*, void*>)(lpVtbl[3]))((FShaderInputLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderInputLayout*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FShaderInputLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderInputLayoutElement *")]
        public FShaderInputLayoutElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, uint*, FShaderInputLayoutElement*>)(lpVtbl[5]))((FShaderInputLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("const FShaderInputLayoutElement *")]
            FShaderInputLayoutElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count);
        }
    }

    public unsafe partial struct FShaderCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* Layout;

        [NativeTypeName("Coplt::FShaderInputLayout *")]
        public FShaderInputLayout* InputLayout;

        public FShaderModule** Modules;

        [NativeTypeName("Coplt::u8")]
        public byte Count;
    }

    [Guid("DE1352D5-023D-42B0-BEAC-122B3B296C9C")]
    [NativeTypeName("struct FShader : Coplt::FGpuObject")]
    public unsafe partial struct FShader : FShader.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShader));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags Stages;

        [NativeTypeName("const u32")]
        public const uint MaxShaderModuleCount = 3;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShader*, void>)(lpVtbl[0]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, nuint>)(lpVtbl[1]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, nuint>)(lpVtbl[2]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, Guid*, void*>)(lpVtbl[3]))((FShader*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShader*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FShader*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* Layout()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderLayout*>)(lpVtbl[5]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderInputLayout *")]
        public FShaderInputLayout* InputLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderInputLayout*>)(lpVtbl[6]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Compute()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[7]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Pixel()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[8]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Vertex()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[9]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Mesh()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[10]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Task()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[11]))((FShader*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShaderLayout *")]
            FShaderLayout* Layout();

            [return: NativeTypeName("Coplt::FShaderInputLayout *")]
            FShaderInputLayout* InputLayout();

            [return: NativeTypeName("Coplt::FShaderModule *")]
            FShaderModule* Compute();

            [return: NativeTypeName("Coplt::FShaderModule *")]
            FShaderModule* Pixel();

            [return: NativeTypeName("Coplt::FShaderModule *")]
            FShaderModule* Vertex();

            [return: NativeTypeName("Coplt::FShaderModule *")]
            FShaderModule* Mesh();

            [return: NativeTypeName("Coplt::FShaderModule *")]
            FShaderModule* Task();
        }
    }

    public unsafe partial struct FUploadBufferBlock
    {
        [NativeTypeName("Coplt::u8 *")]
        public byte* mapped_ptr;

        [NativeTypeName("Coplt::u64")]
        public ulong cur_offset;

        [NativeTypeName("Coplt::u64")]
        public ulong size;
    }

    [Guid("3A22DE68-8377-4E57-A167-3EFDB97C102A")]
    [NativeTypeName("struct FFrameContext : Coplt::FUnknown")]
    public unsafe partial struct FFrameContext : FFrameContext.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FFrameContext));

        public void** lpVtbl;

        public FList<FUploadBufferBlock> m_upload_buffer;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FFrameContext*, void>)(lpVtbl[0]))((FFrameContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FFrameContext*, nuint>)(lpVtbl[1]))((FFrameContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FFrameContext*, nuint>)(lpVtbl[2]))((FFrameContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FFrameContext*, Guid*, void*>)(lpVtbl[3]))((FFrameContext*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GrowUploadBuffer([NativeTypeName("Coplt::u64")] ulong min_required_size)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FFrameContext*, FResult*, ulong, FResult*>)(lpVtbl[4]))((FFrameContext*)Unsafe.AsPointer(ref this), &result, min_required_size);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult GrowUploadBuffer([NativeTypeName("Coplt::u64")] ulong min_required_size);
        }
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

        [NativeTypeName("Coplt::FFrameContext *")]
        public FFrameContext* m_context;

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
        public FResult CreateOutputFromRawSwapchain([NativeTypeName("const FGpuOutputFromSwapChainCreateOptions &")] FGpuOutputFromSwapChainCreateOptions* options, void* swapchain, FGpuOutput** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuOutputFromSwapChainCreateOptions*, void*, FGpuOutput**, FResult*>)(lpVtbl[6]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, options, swapchain, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuOutputCreateOptions*, void*, FGpuOutput**, FResult*>)(lpVtbl[7]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, options, hwnd, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            void* GetRawQueue();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateOutputFromRawSwapchain([NativeTypeName("const FGpuOutputFromSwapChainCreateOptions &")] FGpuOutputFromSwapChainCreateOptions* options, void* swapchain, FGpuOutput** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out);
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FResourceViewType : byte
    {
        None = 0,
        ConstantBuffer,
        ShaderResource,
        UnorderedAccess,
        RenderTarget,
        DepthStencil,
        IndexBuffer,
        VertexBuffer,
        StreamOutput,
    }

    [NativeTypeName("Coplt::u32")]
    public enum FResourcePurpose : uint
    {
        None = 0,
        ConstantBuffer = 1 << 0,
        ShaderResource = 1 << 1,
        UnorderedAccess = 1 << 2,
        RenderTarget = 1 << 3,
        DepthStencil = 1 << 4,
        IndexBuffer = 1 << 5,
        VertexBuffer = 1 << 6,
        StreamOutput = 1 << 7,
        RayTracing = 1 << 8,
        ShadingRate = 1 << 9,
        IndirectDrawArgs = 1 << 10,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FLifeTime : byte
    {
        Common,
        Static,
        Transient,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FCpuAccess : byte
    {
        None = 0,
        Write = 1,
        Read = 2,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBufferUsage : byte
    {
        Undefined,
        Structured,
        Raw,
    }

    public partial struct FGpuResourceCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FResourcePurpose")]
        public FResourcePurpose Purpose;

        [NativeTypeName("Coplt::FCpuAccess")]
        public FCpuAccess CpuAccess;

        [NativeTypeName("Coplt::FLifeTime")]
        public FLifeTime LifeTime;
    }

    [Guid("F99DCEEC-2F0C-4A28-B666-BEB7C35219D6")]
    [NativeTypeName("struct FGpuResource : Coplt::FGpuObject")]
    public unsafe partial struct FGpuResource : FGpuResource.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuResource));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState m_state;

        [NativeTypeName("Coplt::FResourcePurpose")]
        public FResourcePurpose m_purpose;

        [NativeTypeName("Coplt::FCpuAccess")]
        public FCpuAccess m_cpu_access;

        [NativeTypeName("Coplt::FLifeTime")]
        public FLifeTime m_life_time;

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

    [NativeTypeName("struct FGpuBufferCreateOptions : Coplt::FGpuResourceCreateOptions")]
    public partial struct FGpuBufferCreateOptions
    {
        public FGpuResourceCreateOptions Base;

        [NativeTypeName("Coplt::u64")]
        public ulong Size;

        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::u32")]
        public uint Stride;

        [NativeTypeName("Coplt::FBufferUsage")]
        public FBufferUsage Usage;
    }

    [Guid("283740E3-FE96-41D0-830A-0A4C6A725336")]
    [NativeTypeName("struct FGpuBuffer : Coplt::FGpuResource")]
    public unsafe partial struct FGpuBuffer : INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuBuffer));

        public FGpuResource Base;

        [NativeTypeName("Coplt::u64")]
        public ulong m_size;

        [NativeTypeName("Coplt::u32")]
        public uint m_count;

        [NativeTypeName("Coplt::u32")]
        public uint m_stride;

        [NativeTypeName("Coplt::FBufferUsage")]
        public FBufferUsage m_usage;
    }

    [StructLayout(LayoutKind.Explicit)]
    public partial struct FOptimizedClearColor
    {
        [FieldOffset(0)]
        [NativeTypeName("f32[4]")]
        public _Color_e__FixedBuffer Color;

        [FieldOffset(0)]
        [NativeTypeName("__AnonymousRecord_Resource_L119_C9")]
        public _Anonymous_e__Struct Anonymous;

        [UnscopedRef]
        public ref float Depth
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Depth;
            }
        }

        [UnscopedRef]
        public ref byte Stencil
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Stencil;
            }
        }

        public partial struct _Anonymous_e__Struct
        {
            [NativeTypeName("Coplt::f32")]
            public float Depth;

            [NativeTypeName("Coplt::u8")]
            public byte Stencil;
        }

        [InlineArray(4)]
        public partial struct _Color_e__FixedBuffer
        {
            public float e0;
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FImageDimension : byte
    {
        One,
        Two,
        Three,
        Cube,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FImageLayout : byte
    {
        Undefined,
        RowMajor,
        Undefined64kSwizzle,
        Standard64kSwizzle,
    }

    [NativeTypeName("struct FGpuTextureCreateOptions : Coplt::FGpuResourceCreateOptions")]
    public partial struct FGpuTextureCreateOptions
    {
        public FGpuResourceCreateOptions Base;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::u32")]
        public uint Width;

        [NativeTypeName("Coplt::u32")]
        public uint Height;

        [NativeTypeName("Coplt::u32")]
        public uint DepthOrLength;

        [NativeTypeName("Coplt::u16")]
        public ushort MipLevels;

        [NativeTypeName("Coplt::FOptimizedClearColor")]
        public FOptimizedClearColor OptimizedClearValue;

        [NativeTypeName("Coplt::b8")]
        public B8 HasOptimizedClearValue;

        [NativeTypeName("Coplt::u8")]
        public byte MultisampleCount;

        [NativeTypeName("Coplt::FImageDimension")]
        public FImageDimension Dimension;

        [NativeTypeName("Coplt::FImageLayout")]
        public FImageLayout Layout;
    }

    [Guid("667EFA36-21C7-4561-ABAD-85780FA4929E")]
    [NativeTypeName("struct FGpuImage : Coplt::FGpuResource")]
    public unsafe partial struct FGpuImage : INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuImage));

        public FGpuResource Base;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat m_format;

        [NativeTypeName("Coplt::u32")]
        public uint m_width;

        [NativeTypeName("Coplt::u32")]
        public uint m_height;

        [NativeTypeName("Coplt::u32")]
        public uint m_depth_or_length;

        [NativeTypeName("Coplt::u16")]
        public ushort m_mip_levels;

        [NativeTypeName("Coplt::u8")]
        public byte m_multisample_count;

        [NativeTypeName("Coplt::FImageDimension")]
        public FImageDimension m_dimension;

        [NativeTypeName("Coplt::FImageLayout")]
        public FImageLayout m_layout;
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

    public partial struct FGpuOutputFromSwapChainCreateOptions
    {
        [NativeTypeName("Coplt::b8")]
        public B8 VSync;
    }

    public partial struct FGpuOutputCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::u32")]
        public uint Width;

        [NativeTypeName("Coplt::u32")]
        public uint Height;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

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
        [NativeTypeName("Coplt::u32")]
        public uint CommandCount;

        [NativeTypeName("Coplt::u32")]
        public uint ResourceCount;

        [NativeTypeName("Coplt::FCommandItem *")]
        public FCommandItem* Commands;

        [NativeTypeName("Coplt::FResourceMeta *")]
        public FResourceMeta* Resources;

        [NativeTypeName("Coplt::u8 *")]
        public byte* Payload;
    }

    [Guid("F1C59CB4-7EE6-4EE2-80F4-07CC568920D2")]
    [NativeTypeName("struct FGpuOutput : Coplt::FGpuObject")]
    public unsafe partial struct FGpuOutput : FGpuOutput.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuOutput));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState m_state;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat m_format;

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

    public enum FGraphicsFormat
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
        GenericRead = VertexBuffer | ConstantBuffer | ShaderResource | IndirectBuffer | CopySrc,
    }

    [NativeTypeName("Coplt::u32")]
    public enum FCommandType : uint
    {
        None,
        Transition,
        Present,
        ClearColor,
        ClearDepthStencil,
        SetRenderTargets,
        SetViewportScissor,
        SetPipeline,
        SetMeshBuffers,
        Draw,
        Dispatch,
        BufferCopy,
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
        Image,
        Buffer,
        Output,
    }

    public unsafe partial struct FResourceMeta
    {
        [NativeTypeName("__AnonymousRecord_Command_L45_C9")]
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

        [UnscopedRef]
        public ref FGpuBuffer* Buffer
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Buffer;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public unsafe partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGpuOutput *")]
            public FGpuOutput* Output;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGpuBuffer *")]
            public FGpuBuffer* Buffer;
        }
    }

    public partial struct FResourceRef
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

    public partial struct FViewport
    {
        [NativeTypeName("Coplt::f32")]
        public float X;

        [NativeTypeName("Coplt::f32")]
        public float Y;

        [NativeTypeName("Coplt::f32")]
        public float Width;

        [NativeTypeName("Coplt::f32")]
        public float Height;

        [NativeTypeName("Coplt::f32")]
        public float MinDepth;

        [NativeTypeName("Coplt::f32")]
        public float MaxDepth;
    }

    public partial struct FCommandTransition
    {
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Resource;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState SrcState;

        [NativeTypeName("Coplt::FResourceState")]
        public FResourceState DstState;
    }

    public partial struct FCommandPresent
    {
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Image;
    }

    public partial struct FCommandClearColor
    {
        [NativeTypeName("Coplt::u32")]
        public uint RectCount;

        [NativeTypeName("Coplt::u32")]
        public uint RectIndex;

        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Image;

        [NativeTypeName("f32[4]")]
        public _Color_e__FixedBuffer Color;

        [InlineArray(4)]
        public partial struct _Color_e__FixedBuffer
        {
            public float e0;
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDepthStencilClearFlags : byte
    {
        None = 0,
        Depth = 1,
        Stencil = 2,
    }

    public partial struct FCommandClearDepthStencil
    {
        [NativeTypeName("Coplt::u32")]
        public uint RectCount;

        [NativeTypeName("Coplt::u32")]
        public uint RectIndex;

        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Image;

        [NativeTypeName("Coplt::f32")]
        public float Depth;

        [NativeTypeName("Coplt::u8")]
        public byte Stencil;

        [NativeTypeName("Coplt::FDepthStencilClearFlags")]
        public FDepthStencilClearFlags Clear;
    }

    public partial struct FCommandSetRenderTargets
    {
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Dsv;

        [NativeTypeName("Coplt::u32")]
        public uint NumRtv;

        [NativeTypeName("FResourceRef[8]")]
        public _Rtv_e__FixedBuffer Rtv;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportCount;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportIndex;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectCount;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectIndex;

        [InlineArray(8)]
        public partial struct _Rtv_e__FixedBuffer
        {
            public FResourceRef e0;
        }
    }

    public partial struct FCommandSetViewportScissor
    {
        [NativeTypeName("Coplt::u32")]
        public uint ViewportCount;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportIndex;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectCount;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectIndex;
    }

    public unsafe partial struct FCommandSetPipeline
    {
        [NativeTypeName("Coplt::FShaderPipeline *")]
        public FShaderPipeline* Pipeline;
    }

    public partial struct FBufferRange
    {
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Buffer;

        [NativeTypeName("Coplt::u32")]
        public uint Offset;

        [NativeTypeName("Coplt::u32")]
        public uint Size;
    }

    [NativeTypeName("struct FVertexBufferRange : Coplt::FBufferRange")]
    public partial struct FVertexBufferRange
    {
        public FBufferRange Base;

        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    public unsafe partial struct FCommandSetMeshBuffers
    {
        [NativeTypeName("Coplt::FMeshLayout *")]
        public FMeshLayout* MeshLayout;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat IndexFormat;

        [NativeTypeName("Coplt::FBufferRange")]
        public FBufferRange IndexBuffer;

        [NativeTypeName("Coplt::u32")]
        public uint VertexStartSlot;

        [NativeTypeName("Coplt::u32")]
        public uint VertexBufferCount;

        [NativeTypeName("Coplt::u32")]
        public uint VertexBuffersIndex;
    }

    public unsafe partial struct FCommandDraw
    {
        [NativeTypeName("Coplt::FShaderPipeline *")]
        public FShaderPipeline* Pipeline;

        [NativeTypeName("Coplt::u32")]
        public uint VertexOrIndexCount;

        [NativeTypeName("Coplt::u32")]
        public uint InstanceCount;

        [NativeTypeName("Coplt::u32")]
        public uint FirstVertexOrIndex;

        [NativeTypeName("Coplt::u32")]
        public uint FirstInstance;

        [NativeTypeName("Coplt::u32")]
        public uint VertexOffset;

        [NativeTypeName("Coplt::b8")]
        public B8 Indexed;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDispatchType : byte
    {
        Auto,
        Compute,
        Mesh,
    }

    public unsafe partial struct FCommandDispatch
    {
        [NativeTypeName("Coplt::FShaderPipeline *")]
        public FShaderPipeline* Pipeline;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountX;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountY;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountZ;

        [NativeTypeName("Coplt::FDispatchType")]
        public FDispatchType Type;
    }

    public partial struct FUploadLoc
    {
        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    [StructLayout(LayoutKind.Explicit)]
    public partial struct FBufferRef
    {
        [FieldOffset(0)]
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Buffer;

        [FieldOffset(0)]
        [NativeTypeName("Coplt::FUploadLoc")]
        public FUploadLoc Upload;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBufferRefType : byte
    {
        Buffer = 0,
        Upload,
    }

    public partial struct FCommandBufferCopy
    {
        [NativeTypeName("Coplt::u64")]
        public ulong Size;

        [NativeTypeName("Coplt::u64")]
        public ulong DstOffset;

        [NativeTypeName("Coplt::u64")]
        public ulong SrcOffset;

        [NativeTypeName("Coplt::FBufferRef")]
        public FBufferRef Dst;

        [NativeTypeName("Coplt::FBufferRef")]
        public FBufferRef Src;

        [NativeTypeName("Coplt::FBufferRefType")]
        public FBufferRefType DstType;

        [NativeTypeName("Coplt::FBufferRefType")]
        public FBufferRefType SrcType;
    }

    public partial struct FCommandItem
    {
        [NativeTypeName("Coplt::FCommandType")]
        public FCommandType Type;

        [NativeTypeName("Coplt::FCommandFlags")]
        public FCommandFlags Flags;

        [NativeTypeName("__AnonymousRecord_Command_L272_C9")]
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

        [UnscopedRef]
        public ref FCommandClearDepthStencil ClearDepthStencil
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.ClearDepthStencil;
            }
        }

        [UnscopedRef]
        public ref FCommandSetRenderTargets SetRenderTargets
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetRenderTargets;
            }
        }

        [UnscopedRef]
        public ref FCommandSetViewportScissor SetViewportScissor
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetViewportScissor;
            }
        }

        [UnscopedRef]
        public ref FCommandSetPipeline SetPipeline
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetPipeline;
            }
        }

        [UnscopedRef]
        public ref FCommandSetMeshBuffers SetMeshBuffers
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetMeshBuffers;
            }
        }

        [UnscopedRef]
        public ref FCommandDraw Draw
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Draw;
            }
        }

        [UnscopedRef]
        public ref FCommandDispatch Dispatch
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Dispatch;
            }
        }

        [UnscopedRef]
        public ref FCommandBufferCopy BufferCopy
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BufferCopy;
            }
        }

        [UnscopedRef]
        public Span<byte> _pad
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return Anonymous._pad;
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

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandClearDepthStencil")]
            public FCommandClearDepthStencil ClearDepthStencil;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetRenderTargets")]
            public FCommandSetRenderTargets SetRenderTargets;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetViewportScissor")]
            public FCommandSetViewportScissor SetViewportScissor;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetPipeline")]
            public FCommandSetPipeline SetPipeline;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetMeshBuffers")]
            public FCommandSetMeshBuffers SetMeshBuffers;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandDraw")]
            public FCommandDraw Draw;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandDispatch")]
            public FCommandDispatch Dispatch;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBufferCopy")]
            public FCommandBufferCopy BufferCopy;

            [FieldOffset(0)]
            [NativeTypeName("u8[56]")]
            public __pad_e__FixedBuffer _pad;

            [InlineArray(56)]
            public partial struct __pad_e__FixedBuffer
            {
                public byte e0;
            }
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutItemView : byte
    {
        Cbv,
        Srv,
        Uav,
        Sampler,
        Constants,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutItemType : byte
    {
        ConstantBuffer,
        RawBuffer,
        StructureBuffer,
        StructureBufferWithCounter,
        Texture1D,
        Texture1DArray,
        Texture2D,
        Texture2DArray,
        Texture2DMultisample,
        Texture2DArrayMultisample,
        Texture3D,
        TextureCube,
        TextureCubeArray,
        Sampler,
        RayTracingAccelerationStructure,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutItemUsage : byte
    {
        Common,
        Material,
        Instant,
    }

    public partial struct FShaderLayoutItemDefine
    {
        [NativeTypeName("Coplt::u32")]
        public uint Slot;

        [NativeTypeName("Coplt::u32")]
        public uint Space;

        [NativeTypeName("Coplt::u32")]
        public uint CountOrIndex;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::FShaderStage")]
        public FShaderStage Stage;

        [NativeTypeName("Coplt::FShaderLayoutItemView")]
        public FShaderLayoutItemView View;

        [NativeTypeName("Coplt::FShaderLayoutItemType")]
        public FShaderLayoutItemType Type;

        [NativeTypeName("Coplt::FShaderLayoutItemUsage")]
        public FShaderLayoutItemUsage Usage;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutFlags : byte
    {
        None = 0,
        BindLess = 1 << 0,
        InputAssembler = 1 << 1,
        StreamOutput = 1 << 2,
    }

    public unsafe partial struct FShaderLayoutCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::FShaderLayoutItemDefine *")]
        public FShaderLayoutItemDefine* Items;

        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    public unsafe partial struct FShaderInputLayoutCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FShaderInputLayoutElement *")]
        public FShaderInputLayoutElement* Element;

        [NativeTypeName("Coplt::u32")]
        public uint Count;
    }

    public unsafe partial struct FMeshLayoutCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FMeshBufferDefine *")]
        public FMeshBufferDefine* Buffers;

        [NativeTypeName("Coplt::FMeshBufferElement *")]
        public FMeshBufferElement* Elements;

        [NativeTypeName("Coplt::u32")]
        public uint BufferCount;

        [NativeTypeName("Coplt::u32")]
        public uint ElementCount;
    }

    [Guid("8FE5121F-C2CE-46F5-AA14-F28595F35361")]
    [NativeTypeName("struct FMeshLayout : Coplt::FGpuObject")]
    public unsafe partial struct FMeshLayout : FMeshLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FMeshLayout));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FMeshLayout*, void>)(lpVtbl[0]))((FMeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, nuint>)(lpVtbl[1]))((FMeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, nuint>)(lpVtbl[2]))((FMeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, Guid*, void*>)(lpVtbl[3]))((FMeshLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FMeshLayout*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FMeshLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferDefine *")]
        public FMeshBufferDefine* GetBuffers([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, uint*, FMeshBufferDefine*>)(lpVtbl[5]))((FMeshLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferElement *")]
        public FMeshBufferElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, uint*, FMeshBufferElement*>)(lpVtbl[6]))((FMeshLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferElement *")]
        public FMeshBufferElement* TryGetElement([NativeTypeName("Coplt::u32")] uint SlotId, [NativeTypeName("Coplt::u32")] uint SlotIndex)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, uint, uint, FMeshBufferElement*>)(lpVtbl[7]))((FMeshLayout*)Unsafe.AsPointer(ref this), SlotId, SlotIndex);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("const FMeshBufferDefine *")]
            FMeshBufferDefine* GetBuffers([NativeTypeName("Coplt::u32 *")] uint* out_count);

            [return: NativeTypeName("const FMeshBufferElement *")]
            FMeshBufferElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count);

            [return: NativeTypeName("const FMeshBufferElement *")]
            FMeshBufferElement* TryGetElement([NativeTypeName("Coplt::u32")] uint SlotId, [NativeTypeName("Coplt::u32")] uint SlotIndex);
        }
    }

    public unsafe partial struct FShaderPipelineCreateOptions
    {
        [NativeTypeName("Coplt::Str8or16")]
        public Str8or16 Name;

        [NativeTypeName("Coplt::FShader *")]
        public FShader* Shader;
    }

    [Guid("356A2610-34E3-4C01-9904-22E3C5CE2F4E")]
    [NativeTypeName("struct FShaderPipeline : Coplt::FGpuObject")]
    public unsafe partial struct FShaderPipeline : FShaderPipeline.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShaderPipeline));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShaderPipeline*, void>)(lpVtbl[0]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, nuint>)(lpVtbl[1]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, nuint>)(lpVtbl[2]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, Guid*, void*>)(lpVtbl[3]))((FShaderPipeline*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderPipeline*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FShader*>)(lpVtbl[5]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FShaderLayout*>)(lpVtbl[6]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FShaderStageFlags>)(lpVtbl[7]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShader *")]
            FShader* GetShader();

            [return: NativeTypeName("Coplt::FShaderLayout *")]
            FShaderLayout* GetLayout();

            [return: NativeTypeName("Coplt::FShaderStageFlags")]
            FShaderStageFlags GetStages();
        }
    }

    [Guid("5241C089-2EE2-43EE-ADD4-0A10C04A56CE")]
    [NativeTypeName("struct FComputeShaderPipeline : Coplt::FShaderPipeline")]
    public unsafe partial struct FComputeShaderPipeline : FComputeShaderPipeline.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FComputeShaderPipeline));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, void>)(lpVtbl[0]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, nuint>)(lpVtbl[1]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, nuint>)(lpVtbl[2]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, Guid*, void*>)(lpVtbl[3]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FShader*>)(lpVtbl[5]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FShaderLayout*>)(lpVtbl[6]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FShaderStageFlags>)(lpVtbl[7]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FShaderPipeline.Interface
        {
        }
    }

    [NativeTypeName("struct FGraphicsShaderPipelineCreateOptions : Coplt::FShaderPipelineCreateOptions")]
    public unsafe partial struct FGraphicsShaderPipelineCreateOptions
    {
        public FShaderPipelineCreateOptions Base;

        [NativeTypeName("Coplt::FMeshLayout *")]
        public FMeshLayout* MeshLayout;

        [NativeTypeName("Coplt::FGraphicsPipelineState")]
        public FGraphicsPipelineState GraphicsState;
    }

    [Guid("32A67D44-132C-449B-972D-BAD3413783E5")]
    [NativeTypeName("struct FGraphicsShaderPipeline : Coplt::FShaderPipeline")]
    public unsafe partial struct FGraphicsShaderPipeline : FGraphicsShaderPipeline.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGraphicsShaderPipeline));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, void>)(lpVtbl[0]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, nuint>)(lpVtbl[1]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, nuint>)(lpVtbl[2]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, Guid*, void*>)(lpVtbl[3]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShader*>)(lpVtbl[5]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShaderLayout*>)(lpVtbl[6]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShaderStageFlags>)(lpVtbl[7]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FGraphicsPipelineState *")]
        public FGraphicsPipelineState* GetGraphicsState()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FGraphicsPipelineState*>)(lpVtbl[8]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FShaderPipeline.Interface
        {
            [return: NativeTypeName("const FGraphicsPipelineState *")]
            FGraphicsPipelineState* GetGraphicsState();
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FPrimitiveTopologyType : byte
    {
        Triangle,
        TriangleStrip,
        Point,
        Line,
        LineStrip,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FFillMode : byte
    {
        WireFrame = 2,
        Solid = 3,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FCullMode : byte
    {
        Off = 1,
        Front = 2,
        Back = 3,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBlendType : byte
    {
        Zero = 1,
        One = 2,
        SrcColor = 3,
        InvSrcColor = 4,
        SrcAlpha = 5,
        InvSrcAlpha = 6,
        DstAlpha = 7,
        InvDstAlpha = 8,
        DstColor = 9,
        InvDstColor = 10,
        SrcAlphaSat = 11,
        BlendFactor = 14,
        InvBlendFactor = 15,
        Src1Color = 16,
        InvSrc1Color = 17,
        Src1Alpha = 18,
        InvSrc1Alpha = 19,
        AlphaFactor = 20,
        InvAlphaFactor = 21,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FLogicOp : byte
    {
        None = 0,
        Zero,
        One,
        Copy,
        CopyInv,
        Noop,
        Inv,
        And,
        NAnd,
        Or,
        Nor,
        Xor,
        Equiv,
        AndRev,
        AndInv,
        OrRev,
        OrInv,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBlendOp : byte
    {
        None = 0,
        Add = 1,
        Sub = 2,
        RevSub = 3,
        Min = 4,
        Max = 5,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FCmpFunc : byte
    {
        Off = 0,
        Never = 1,
        Less = 2,
        Equal = 3,
        LessEqual = 4,
        Greater = 5,
        NotEqual = 6,
        GreaterEqual = 7,
        Always = 8,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FStencilOp : byte
    {
        Keep = 1,
        Zero = 2,
        Replace = 3,
        IncSat = 4,
        DecSat = 5,
        Invert = 6,
        Inc = 7,
        Dec = 8,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDepthWriteMask : byte
    {
        Zero = 0,
        All = 1,
    }

    public partial struct FStencilState
    {
        [NativeTypeName("Coplt::FStencilOp")]
        public FStencilOp Fail;

        [NativeTypeName("Coplt::FStencilOp")]
        public FStencilOp DepthFail;

        [NativeTypeName("Coplt::FStencilOp")]
        public FStencilOp Pass;

        [NativeTypeName("Coplt::FCmpFunc")]
        public FCmpFunc Func;
    }

    public partial struct FDepthStencilState
    {
        [NativeTypeName("Coplt::b8")]
        public B8 Enable;

        [NativeTypeName("Coplt::b8")]
        public B8 DepthWrite;

        [NativeTypeName("Coplt::FCmpFunc")]
        public FCmpFunc DepthFunc;

        [NativeTypeName("Coplt::b8")]
        public B8 StencilEnable;

        [NativeTypeName("Coplt::u8")]
        public byte StencilReadMask;

        [NativeTypeName("Coplt::u8")]
        public byte StencilWriteMask;

        [NativeTypeName("Coplt::FStencilState")]
        public FStencilState Front;

        [NativeTypeName("Coplt::FStencilState")]
        public FStencilState Back;
    }

    public partial struct FRtBlendState
    {
        [NativeTypeName("Coplt::FBlendType")]
        public FBlendType Src;

        [NativeTypeName("Coplt::FBlendType")]
        public FBlendType Dst;

        [NativeTypeName("Coplt::FBlendOp")]
        public FBlendOp Op;

        [NativeTypeName("Coplt::FBlendType")]
        public FBlendType AlphaSrc;

        [NativeTypeName("Coplt::FBlendType")]
        public FBlendType AlphaDst;

        [NativeTypeName("Coplt::FBlendOp")]
        public FBlendOp AlphaOp;

        [NativeTypeName("Coplt::FLogicOp")]
        public FLogicOp LogicOp;

        [NativeTypeName("Coplt::u8")]
        public byte WriteMask;
    }

    public partial struct FBlendState
    {
        [NativeTypeName("Coplt::b8")]
        public B8 IndependentBlendEnable;

        [NativeTypeName("FRtBlendState[8]")]
        public _Rts_e__FixedBuffer Rts;

        [InlineArray(8)]
        public partial struct _Rts_e__FixedBuffer
        {
            public FRtBlendState e0;
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FRotationDirection : byte
    {
        CounterClockWise,
        ClockWise,
    }

    public partial struct FRasterizerState
    {
        [NativeTypeName("Coplt::FFillMode")]
        public FFillMode Fill;

        [NativeTypeName("Coplt::FCullMode")]
        public FCullMode Cull;

        [NativeTypeName("Coplt::i32")]
        public int DepthBias;

        [NativeTypeName("Coplt::f32")]
        public float DepthBiasClamp;

        [NativeTypeName("Coplt::f32")]
        public float SlopeScaledDepthBias;

        [NativeTypeName("Coplt::b8")]
        public B8 DepthClip;

        [NativeTypeName("Coplt::b8")]
        public B8 AlphaAALine;

        [NativeTypeName("Coplt::FRotationDirection")]
        public FRotationDirection FrontFace;

        [NativeTypeName("Coplt::b8")]
        public B8 ConservativeRaster;
    }

    public partial struct FMultiSampleState
    {
        [NativeTypeName("Coplt::u32")]
        public uint SampleMask;

        [NativeTypeName("Coplt::u8")]
        public byte SampleCount;

        [NativeTypeName("Coplt::b8")]
        public B8 AlphaToCoverageEnable;
    }

    public partial struct FGraphicsPipelineState
    {
        [NativeTypeName("Coplt::FRasterizerState")]
        public FRasterizerState RasterizerState;

        [NativeTypeName("Coplt::FMultiSampleState")]
        public FMultiSampleState MultiSample;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat DsvFormat;

        [NativeTypeName("FGraphicsFormat[8]")]
        public _RtvFormats_e__FixedBuffer RtvFormats;

        [NativeTypeName("Coplt::FBlendState")]
        public FBlendState BlendState;

        [NativeTypeName("Coplt::FDepthStencilState")]
        public FDepthStencilState DepthStencilState;

        [NativeTypeName("Coplt::u8")]
        public byte NumRts;

        [NativeTypeName("Coplt::FPrimitiveTopologyType")]
        public FPrimitiveTopologyType Topology;

        [InlineArray(8)]
        public partial struct _RtvFormats_e__FixedBuffer
        {
            public FGraphicsFormat e0;
        }
    }

    public unsafe partial struct FShaderInputLayoutElement
    {
        [NativeTypeName("Coplt::FString8 *")]
        public FString8* SlotName;

        [NativeTypeName("Coplt::u32")]
        public uint SlotId;

        [NativeTypeName("Coplt::u32")]
        public uint SlotIndex;

        [NativeTypeName("Coplt::u32")]
        public uint Location;
    }

    public partial struct FMeshBufferElement
    {
        [NativeTypeName("Coplt::u32")]
        public uint BufferIndex;

        [NativeTypeName("Coplt::u32")]
        public uint SlotId;

        [NativeTypeName("Coplt::u32")]
        public uint SlotIndex;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::u32")]
        public uint Offset;

        [NativeTypeName("Coplt::u32")]
        public uint InstanceRepeat;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FMeshBufferElementRate : byte
    {
        Vertex,
        Instance,
    }

    public partial struct FMeshBufferDefine
    {
        [NativeTypeName("Coplt::u32")]
        public uint Stride;

        [NativeTypeName("Coplt::FMeshBufferElementRate")]
        public FMeshBufferElementRate Rate;
    }

    public static partial class Native
    {
        public static readonly Guid IID_FUnknown = new Guid(0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

        public static readonly Guid IID_FString = new Guid(0x5A478800, 0xE7DA, 0x4A6B, 0xB4, 0x28, 0x1E, 0x3F, 0xDA, 0x55, 0x99, 0x7F);

        public static readonly Guid IID_FString8 = new Guid(0x63C7FC71, 0xD775, 0x42BB, 0x89, 0x1A, 0x69, 0xDB, 0xB2, 0x6E, 0x75, 0xC5);

        public static readonly Guid IID_FString16 = new Guid(0x669355D5, 0x9443, 0x487C, 0x84, 0x86, 0xB0, 0xB0, 0xC0, 0x0D, 0x23, 0x67);

        public static readonly Guid IID_FBlob = new Guid(0xC2A7F23D, 0x4CCE, 0x4EAE, 0xA1, 0xE0, 0x8F, 0xFA, 0xC4, 0x83, 0x68, 0x70);

        public static readonly Guid IID_FGpuObject = new Guid(0x9FC6890B, 0xE9A2, 0x4F4A, 0x92, 0x17, 0xA1, 0x4A, 0x42, 0x23, 0xA7, 0x15);

        public static readonly Guid IID_FAllocator = new Guid(0x534B3C0C, 0x098D, 0x4157, 0x8A, 0xDD, 0x9B, 0x79, 0xEB, 0x5D, 0x69, 0x0F);

        public static readonly Guid IID_FInstance = new Guid(0xCC2894BA, 0x57C7, 0x474A, 0xB7, 0x77, 0x1A, 0x3E, 0x3A, 0x7C, 0x92, 0x2C);

        public static readonly Guid IID_FGpuDevice = new Guid(0x557F032D, 0xED50, 0x403A, 0xAD, 0xC5, 0x21, 0x4F, 0xDD, 0xBE, 0x6C, 0x6B);

        public static readonly Guid IID_FShaderModule = new Guid(0x5C0E1FDB, 0x2ACD, 0x4FCE, 0xB9, 0x85, 0x09, 0xE1, 0x2A, 0x7A, 0x1A, 0xAD);

        public static readonly Guid IID_FShader = new Guid(0xDE1352D5, 0x023D, 0x42B0, 0xBE, 0xAC, 0x12, 0x2B, 0x3B, 0x29, 0x6C, 0x9C);

        public static readonly Guid IID_FFrameContext = new Guid(0x3A22DE68, 0x8377, 0x4E57, 0xA1, 0x67, 0x3E, 0xFD, 0xB9, 0x7C, 0x10, 0x2A);

        public static readonly Guid IID_FGpuQueue = new Guid(0x95E60E28, 0xE387, 0x4055, 0x9B, 0x33, 0x2D, 0x23, 0xAF, 0x90, 0x1F, 0x8A);

        public static readonly Guid IID_FGpuResource = new Guid(0xF99DCEEC, 0x2F0C, 0x4A28, 0xB6, 0x66, 0xBE, 0xB7, 0xC3, 0x52, 0x19, 0xD6);

        public static readonly Guid IID_FGpuBuffer = new Guid(0x283740E3, 0xFE96, 0x41D0, 0x83, 0x0A, 0x0A, 0x4C, 0x6A, 0x72, 0x53, 0x36);

        public static readonly Guid IID_FGpuImage = new Guid(0x667EFA36, 0x21C7, 0x4561, 0xAB, 0xAD, 0x85, 0x78, 0x0F, 0xA4, 0x92, 0x9E);

        public static readonly Guid IID_FGpuOutput = new Guid(0xF1C59CB4, 0x7EE6, 0x4EE2, 0x80, 0xF4, 0x07, 0xCC, 0x56, 0x89, 0x20, 0xD2);

        public static readonly Guid IID_FShaderLayout = new Guid(0x552A498E, 0x8F3A, 0x47FF, 0xA3, 0x35, 0x7A, 0xF2, 0xDE, 0x09, 0x01, 0xE8);

        public static readonly Guid IID_FShaderInputLayout = new Guid(0x70229C9A, 0xFB3D, 0x46B4, 0xB5, 0x34, 0x72, 0xFD, 0xB1, 0x67, 0xD8, 0x07);

        public static readonly Guid IID_FMeshLayout = new Guid(0x8FE5121F, 0xC2CE, 0x46F5, 0xAA, 0x14, 0xF2, 0x85, 0x95, 0xF3, 0x53, 0x61);

        public static readonly Guid IID_FShaderPipeline = new Guid(0x356A2610, 0x34E3, 0x4C01, 0x99, 0x04, 0x22, 0xE3, 0xC5, 0xCE, 0x2F, 0x4E);

        public static readonly Guid IID_FComputeShaderPipeline = new Guid(0x5241C089, 0x2EE2, 0x43EE, 0xAD, 0xD4, 0x0A, 0x10, 0xC0, 0x4A, 0x56, 0xCE);

        public static readonly Guid IID_FGraphicsShaderPipeline = new Guid(0x32A67D44, 0x132C, 0x449B, 0x97, 0x2D, 0xBA, 0xD3, 0x41, 0x37, 0x83, 0xE5);
    }
}
