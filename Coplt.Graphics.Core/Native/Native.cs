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
        [return: NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 GetStr()
        {
            FStr8or16 result;
            return *((delegate* unmanaged[Thiscall]<FString*, FStr8or16*, FStr8or16*>)(lpVtbl[4]))((FString*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::FStr8or16")]
            FStr8or16 GetStr();
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
        [return: NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 GetStr()
        {
            FStr8or16 result;
            return *((delegate* unmanaged[Thiscall]<FString8*, FStr8or16*, FStr8or16*>)(lpVtbl[4]))((FString8*)Unsafe.AsPointer(ref this), &result);
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
        [return: NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 GetStr()
        {
            FStr8or16 result;
            return *((delegate* unmanaged[Thiscall]<FString16*, FStr8or16*, FStr8or16*>)(lpVtbl[4]))((FString16*)Unsafe.AsPointer(ref this), &result);
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

    public unsafe partial struct FStr8
    {
        [NativeTypeName("const char *")]
        public byte* str;

        [NativeTypeName("Coplt::i32")]
        public int len;
    }

    public unsafe partial struct FStr16
    {
        [NativeTypeName("const Char16 *")]
        public char* str;

        [NativeTypeName("Coplt::i32")]
        public int len;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FStrType : byte
    {
        Str16,
        Str8,
    }

    public unsafe partial struct FStr8or16
    {
        [NativeTypeName("__AnonymousRecord_String_L82_C9")]
        public _Anonymous_e__Union Anonymous;

        [NativeTypeName("Coplt::i32")]
        public int len;

        [NativeTypeName("Coplt::FStrType")]
        public FStrType type;

        [UnscopedRef]
        public ref char* str16
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.str16;
            }
        }

        [UnscopedRef]
        public ref byte* str8
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.str8;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public unsafe partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("const Char16 *")]
            public char* str16;

            [FieldOffset(0)]
            [NativeTypeName("const char *")]
            public byte* str8;
        }
    }

    public unsafe partial struct FCStr8or16
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, ulong>)(lpVtbl[4]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuObject*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuObject*)Unsafe.AsPointer(ref this), &result, name);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::u64")]
            ulong ObjectId();

            [return: NativeTypeName("Coplt::FResult")]
            FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name);
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBackend : byte
    {
        None,
        Dx12,
        Vulkan,
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

    public partial struct FGpuAdapter
    {
    }

    [Guid("B3BBBED6-E997-4BD5-BE53-0AAC0CA8114D")]
    [NativeTypeName("struct FGpuAdapter : Coplt::FUnknown")]
    public unsafe partial struct FGpuAdapter : FGpuAdapter.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuAdapter));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 m_name;

        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 m_driver;

        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 m_driver_info;

        [NativeTypeName("Coplt::u32")]
        public uint m_vendor_id;

        [NativeTypeName("Coplt::u32")]
        public uint m_device_id;

        [NativeTypeName("Coplt::FD3dFeatureLevel")]
        public FD3dFeatureLevel m_d3d_feature_level;

        [NativeTypeName("Coplt::FVulkanVersion")]
        public FVulkanVersion m_vulkan_version;

        [NativeTypeName("Coplt::FDeviceType")]
        public FDeviceType m_device_type;

        [NativeTypeName("Coplt::FBackend")]
        public FBackend m_backend;

        [NativeTypeName("Coplt::FDeviceFeatures")]
        public FDeviceFeatures m_features;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuAdapter*, void>)(lpVtbl[0]))((FGpuAdapter*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuAdapter*, nuint>)(lpVtbl[1]))((FGpuAdapter*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuAdapter*, nuint>)(lpVtbl[2]))((FGpuAdapter*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuAdapter*, Guid*, void*>)(lpVtbl[3]))((FGpuAdapter*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::b8")]
        public B8 IsSupport([NativeTypeName("const FDeviceRequires &")] FDeviceRequires* Requires)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuAdapter*, FDeviceRequires*, B8>)(lpVtbl[4]))((FGpuAdapter*)Unsafe.AsPointer(ref this), Requires);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuAdapter*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[5]))((FGpuAdapter*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::b8")]
            B8 IsSupport([NativeTypeName("const FDeviceRequires &")] FDeviceRequires* Requires);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out);
        }
    }

    public partial struct FGpuAdapter
    {
    }

    public partial struct FGpuAutoSelectDeviceCreateOptions
    {
    }

    [NativeTypeName("struct FGpuAutoSelectDeviceCreateOptions : Coplt::FGpuDeviceCreateOptions")]
    public partial struct FGpuAutoSelectDeviceCreateOptions
    {
        public FGpuDeviceCreateOptions Base;

        [NativeTypeName("Coplt::FDeviceRequires")]
        public FDeviceRequires Requires;
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, ulong>)(lpVtbl[4]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuAdapter *")]
        public FGpuAdapter* GetAdapter()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, FGpuAdapter*>)(lpVtbl[6]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawDevice()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, void*>)(lpVtbl[7]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FMainQueueCreateOptions*, FGpuQueue**, FResult*>)(lpVtbl[8]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, FShaderModule** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderModuleCreateOptions*, FShaderModule**, FResult*>)(lpVtbl[9]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, FShaderLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderLayoutCreateOptions*, FShaderLayout**, FResult*>)(lpVtbl[10]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GetEmptyShaderLayout([NativeTypeName("const FGetEmptyShaderLayoutOptions &")] FGetEmptyShaderLayoutOptions* options, FShaderLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGetEmptyShaderLayoutOptions*, FShaderLayout**, FResult*>)(lpVtbl[11]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderInputLayoutCreateOptions*, FShaderInputLayout**, FResult*>)(lpVtbl[12]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, FShader** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderCreateOptions*, FShader**, FResult*>)(lpVtbl[13]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderBinding([NativeTypeName("const FShaderBindingCreateOptions &")] FShaderBindingCreateOptions* options, FShaderBinding** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderBindingCreateOptions*, FShaderBinding**, FResult*>)(lpVtbl[14]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FMeshLayoutCreateOptions*, FMeshLayout**, FResult*>)(lpVtbl[15]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGraphicsShaderPipelineCreateOptions*, FGraphicsShaderPipeline**, FResult*>)(lpVtbl[16]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateBuffer([NativeTypeName("const FGpuBufferCreateOptions &")] FGpuBufferCreateOptions* options, FGpuBuffer** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGpuBufferCreateOptions*, FGpuBuffer**, FResult*>)(lpVtbl[17]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FGpuAdapter *")]
            FGpuAdapter* GetAdapter();

            void* GetRawDevice();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, FShaderModule** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, FShaderLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult GetEmptyShaderLayout([NativeTypeName("const FGetEmptyShaderLayoutOptions &")] FGetEmptyShaderLayoutOptions* options, FShaderLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, FShader** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderBinding([NativeTypeName("const FShaderBindingCreateOptions &")] FShaderBindingCreateOptions* options, FShaderBinding** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateBuffer([NativeTypeName("const FGpuBufferCreateOptions &")] FGpuBufferCreateOptions* options, FGpuBuffer** @out);
        }
    }

    public partial struct FInstanceCreateOptions
    {
        [NativeTypeName("Coplt::b8")]
        public B8 Debug;

        [NativeTypeName("Coplt::b8")]
        public B8 GpuBasedValidation;
    }

    [Guid("CC2894BA-57C7-474A-B777-1A3E3A7C922C")]
    [NativeTypeName("struct FInstance : Coplt::FUnknown")]
    public unsafe partial struct FInstance : FInstance.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FInstance));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FAllocator *")]
        public FAllocator* m_allocator;

        [NativeTypeName("Coplt::b8")]
        public B8 m_debug_enabled;

        [NativeTypeName("Coplt::b8")]
        public B8 m_gpu_based_validation_enabled;

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
        [return: NativeTypeName("FGpuAdapter *const *")]
        public FGpuAdapter** GetAdapters([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, uint*, FGpuAdapter**>)(lpVtbl[8]))((FInstance*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuAutoSelectDeviceCreateOptions &")] FGpuAutoSelectDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FGpuAutoSelectDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[9]))((FInstance*)Unsafe.AsPointer(ref this), &result, options, @out);
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

            [return: NativeTypeName("FGpuAdapter *const *")]
            FGpuAdapter** GetAdapters([NativeTypeName("Coplt::u32 *")] uint* out_count);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateDevice([NativeTypeName("const FGpuAutoSelectDeviceCreateOptions &")] FGpuAutoSelectDeviceCreateOptions* options, FGpuDevice** @out);
        }
    }

    [NativeTypeName("Coplt::u32")]
    public enum FShaderModelLevel : uint
    {
        Any = 0,
        SM_5_1 = 0x51,
        SM_6_0 = 0x60,
        SM_6_1 = 0x61,
        SM_6_2 = 0x62,
        SM_6_3 = 0x63,
        SM_6_4 = 0x64,
        SM_6_5 = 0x65,
        SM_6_6 = 0x66,
        SM_6_7 = 0x67,
        SM_6_8 = 0x68,
        SM_6_9 = 0x69,
    }

    public partial struct FDeviceFeatureRequires
    {
        [NativeTypeName("Coplt::FShaderModelLevel")]
        public FShaderModelLevel ShaderModelLevel;

        [NativeTypeName("Coplt::b8")]
        public B8 RayTracing;

        [NativeTypeName("Coplt::b8")]
        public B8 MeshShader;

        [NativeTypeName("Coplt::b8")]
        public B8 DescriptorHeap;

        [NativeTypeName("Coplt::b8")]
        public B8 EnhancedBarriers;

        [NativeTypeName("Coplt::b8")]
        public B8 ArrBindless;

        [NativeTypeName("Coplt::b8")]
        public B8 DynBindless;

        [NativeTypeName("Coplt::b8")]
        public B8 DynBindlessOnly;
    }

    public partial struct FDeviceFeatures
    {
        [NativeTypeName("Coplt::FShaderModelLevel")]
        public FShaderModelLevel ShaderModelLevel;

        [NativeTypeName("Coplt::b8")]
        public B8 RayTracing;

        [NativeTypeName("Coplt::b8")]
        public B8 MeshShader;

        [NativeTypeName("Coplt::b8")]
        public B8 DescriptorHeap;

        [NativeTypeName("Coplt::b8")]
        public B8 EnhancedBarriers;

        [NativeTypeName("Coplt::b8")]
        public B8 ArrBindless;

        [NativeTypeName("Coplt::b8")]
        public B8 DynBindless;

        [NativeTypeName("Coplt::b8")]
        public B8 DynBindlessOnly;
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
    public enum FDeviceTypeRequire : byte
    {
        Any,
        Gpu,
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDeviceType : byte
    {
        Other,
        IntegratedGpu,
        DiscreteGpu,
        VirtualGpu,
        Cpu,
    }

    public partial struct FDeviceRequires
    {
        [NativeTypeName("Coplt::FD3dFeatureLevel")]
        public FD3dFeatureLevel D3dFeatureLevel;

        [NativeTypeName("Coplt::FVulkanVersion")]
        public FVulkanVersion VulkanVersion;

        [NativeTypeName("Coplt::FDeviceTypeRequire")]
        public FDeviceTypeRequire DeviceType;

        [NativeTypeName("Coplt::FDeviceFeatureRequires")]
        public FDeviceFeatureRequires FeatureRequires;
    }

    public partial struct FGpuDeviceCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;
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

    [NativeTypeName("Coplt::u32")]
    public enum FShaderStageFlags : uint
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
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, ulong>)(lpVtbl[4]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderModule*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShaderModule*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FString8 *")]
        public FString8* GetEntryPoint()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, FString8*>)(lpVtbl[6]))((FShaderModule*)Unsafe.AsPointer(ref this));
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, ulong>)(lpVtbl[4]))((FShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderLayout*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShaderLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderLayoutItemDefine *")]
        public FShaderLayoutItemDefine* GetItemDefines([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, uint*, FShaderLayoutItemDefine*>)(lpVtbl[6]))((FShaderLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderLayoutItemInfo *")]
        public FShaderLayoutItemInfo* GetItemInfos([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, uint*, FShaderLayoutItemInfo*>)(lpVtbl[7]))((FShaderLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderLayoutGroupClass *")]
        public FShaderLayoutGroupClass* GetGroupClasses([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, uint*, FShaderLayoutGroupClass*>)(lpVtbl[8]))((FShaderLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("const FShaderLayoutItemDefine *")]
            FShaderLayoutItemDefine* GetItemDefines([NativeTypeName("Coplt::u32 *")] uint* out_count);

            [return: NativeTypeName("const FShaderLayoutItemInfo *")]
            FShaderLayoutItemInfo* GetItemInfos([NativeTypeName("Coplt::u32 *")] uint* out_count);

            [return: NativeTypeName("const FShaderLayoutGroupClass *")]
            FShaderLayoutGroupClass* GetGroupClasses([NativeTypeName("Coplt::u32 *")] uint* out_count);
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, ulong>)(lpVtbl[4]))((FShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderInputLayout*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShaderInputLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderInputLayoutElement *")]
        public FShaderInputLayoutElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderInputLayout*, uint*, FShaderInputLayoutElement*>)(lpVtbl[6]))((FShaderInputLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("const FShaderInputLayoutElement *")]
            FShaderInputLayoutElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count);
        }
    }

    public unsafe partial struct FShaderCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, ulong>)(lpVtbl[4]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShader*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShader*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* Layout()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderLayout*>)(lpVtbl[6]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderInputLayout *")]
        public FShaderInputLayout* InputLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderInputLayout*>)(lpVtbl[7]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Compute()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[8]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Pixel()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[9]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Vertex()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[10]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Mesh()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[11]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Task()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[12]))((FShader*)Unsafe.AsPointer(ref this));
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

    public unsafe partial struct FShaderBindingCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* Layout;
    }

    public partial struct FBindItem
    {
        [NativeTypeName("Coplt::FView")]
        public FView View;

        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    [Guid("A3CA644A-0E02-4D25-9A18-8835D66600F7")]
    [NativeTypeName("struct FShaderBinding : Coplt::FGpuObject")]
    public unsafe partial struct FShaderBinding : FShaderBinding.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShaderBinding));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShaderBinding*, void>)(lpVtbl[0]))((FShaderBinding*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBinding*, nuint>)(lpVtbl[1]))((FShaderBinding*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBinding*, nuint>)(lpVtbl[2]))((FShaderBinding*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBinding*, Guid*, void*>)(lpVtbl[3]))((FShaderBinding*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBinding*, ulong>)(lpVtbl[4]))((FShaderBinding*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderBinding*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShaderBinding*)Unsafe.AsPointer(ref this), &result, name);
        }

        public interface Interface : FGpuObject.Interface
        {
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
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuQueueType : byte
    {
        Direct,
        Compute,
        Copy,
        Video,
    }

    [Guid("95E60E28-E387-4055-9B33-2D23AF901F8A")]
    [NativeTypeName("struct FGpuQueue : Coplt::FGpuObject")]
    public unsafe partial struct FGpuQueue : FGpuQueue.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuQueue));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FFrameContext *")]
        public FFrameContext* m_context;

        [NativeTypeName("Coplt::u64")]
        public ulong m_submit_id;

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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, ulong>)(lpVtbl[4]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawQueue()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, void*>)(lpVtbl[6]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateOutputFromRawSwapchain([NativeTypeName("const FGpuOutputFromSwapChainCreateOptions &")] FGpuOutputFromSwapChainCreateOptions* options, void* swapchain, FGpuOutput** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuOutputFromSwapChainCreateOptions*, void*, FGpuOutput**, FResult*>)(lpVtbl[7]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, options, swapchain, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuOutputCreateOptions*, void*, FGpuOutput**, FResult*>)(lpVtbl[8]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, options, hwnd, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Submit([NativeTypeName("Coplt::FGpuExecutor *")] FGpuExecutor* Executor, [NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuExecutor*, FCommandSubmit*, FResult*>)(lpVtbl[9]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, Executor, submit);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SubmitNoWait([NativeTypeName("Coplt::FGpuExecutor *")] FGpuExecutor* Executor, [NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuExecutor*, FCommandSubmit*, FResult*>)(lpVtbl[10]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, Executor, submit);
        }

        public interface Interface : FGpuObject.Interface
        {
            void* GetRawQueue();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateOutputFromRawSwapchain([NativeTypeName("const FGpuOutputFromSwapChainCreateOptions &")] FGpuOutputFromSwapChainCreateOptions* options, void* swapchain, FGpuOutput** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Submit([NativeTypeName("Coplt::FGpuExecutor *")] FGpuExecutor* Executor, [NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit);

            [return: NativeTypeName("Coplt::FResult")]
            FResult SubmitNoWait([NativeTypeName("Coplt::FGpuExecutor *")] FGpuExecutor* Executor, [NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit);
        }
    }

    [Guid("EDAB0989-E0C2-4B1E-826F-C73380088FC6")]
    [NativeTypeName("struct FGpuExecutor : Coplt::FGpuObject")]
    public unsafe partial struct FGpuExecutor : FGpuExecutor.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuExecutor));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuExecutor*, void>)(lpVtbl[0]))((FGpuExecutor*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuExecutor*, nuint>)(lpVtbl[1]))((FGpuExecutor*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuExecutor*, nuint>)(lpVtbl[2]))((FGpuExecutor*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuExecutor*, Guid*, void*>)(lpVtbl[3]))((FGpuExecutor*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuExecutor*, ulong>)(lpVtbl[4]))((FGpuExecutor*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuExecutor*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuExecutor*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Wait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuExecutor*, FResult*, FResult*>)(lpVtbl[6]))((FGpuExecutor*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult Wait();
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum ResourceType : byte
    {
        Unknown = 0,
        Buffer = 1,
        Texture = 2,
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

    public partial struct FGpuViewCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FResourcePurpose")]
        public FResourcePurpose Purpose;
    }

    public partial struct FGpuViewableData
    {
        [NativeTypeName("Coplt::FResourcePurpose")]
        public FResourcePurpose m_purpose;
    }

    [Guid("B3AEB8A5-1FA6-4866-97EF-1A5FA401E18F")]
    [NativeTypeName("struct FGpuViewable : Coplt::FGpuObject")]
    public unsafe partial struct FGpuViewable : FGpuViewable.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuViewable));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuViewable*, void>)(lpVtbl[0]))((FGpuViewable*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuViewable*, nuint>)(lpVtbl[1]))((FGpuViewable*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuViewable*, nuint>)(lpVtbl[2]))((FGpuViewable*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuViewable*, Guid*, void*>)(lpVtbl[3]))((FGpuViewable*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuViewable*, ulong>)(lpVtbl[4]))((FGpuViewable*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuViewable*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuViewable*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuViewableData *")]
        public FGpuViewableData* GpuViewData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuViewable*, FGpuViewableData*>)(lpVtbl[6]))((FGpuViewable*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FGpuViewableData *")]
            FGpuViewableData* GpuViewData();
        }
    }

    [NativeTypeName("struct FGpuResourceCreateOptions : Coplt::FGpuViewCreateOptions")]
    public partial struct FGpuResourceCreateOptions
    {
        public FGpuViewCreateOptions Base;

        [NativeTypeName("Coplt::FCpuAccess")]
        public FCpuAccess CpuAccess;
    }

    [NativeTypeName("struct FGpuResourceData : Coplt::FGpuViewableData")]
    public partial struct FGpuResourceData
    {
        public FGpuViewableData Base;

        [NativeTypeName("Coplt::FResState")]
        public FResState m_state;

        [NativeTypeName("Coplt::FCpuAccess")]
        public FCpuAccess m_cpu_access;
    }

    [Guid("F99DCEEC-2F0C-4A28-B666-BEB7C35219D6")]
    [NativeTypeName("struct FGpuResource : Coplt::FGpuViewable")]
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, ulong>)(lpVtbl[4]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuResource*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuResource*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuViewableData *")]
        public FGpuViewableData* GpuViewData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, FGpuViewableData*>)(lpVtbl[6]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuResourceData *")]
        public FGpuResourceData* GpuResourceData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, FGpuResourceData*>)(lpVtbl[7]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::ResourceType")]
        public ResourceType GetResourceType()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, ResourceType>)(lpVtbl[8]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuViewable.Interface
        {
            [return: NativeTypeName("Coplt::FGpuResourceData *")]
            FGpuResourceData* GpuResourceData();

            [return: NativeTypeName("Coplt::ResourceType")]
            ResourceType GetResourceType();
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

    [NativeTypeName("struct FGpuBufferData : Coplt::FGpuResourceData")]
    public partial struct FGpuBufferData
    {
        public FGpuResourceData Base;

        [NativeTypeName("Coplt::u64")]
        public ulong m_size;

        [NativeTypeName("Coplt::u32")]
        public uint m_count;

        [NativeTypeName("Coplt::u32")]
        public uint m_stride;

        [NativeTypeName("Coplt::FBufferUsage")]
        public FBufferUsage m_usage;
    }

    [Guid("283740E3-FE96-41D0-830A-0A4C6A725336")]
    [NativeTypeName("struct FGpuBuffer : Coplt::FGpuResource")]
    public unsafe partial struct FGpuBuffer : FGpuBuffer.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuBuffer));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuBuffer*, void>)(lpVtbl[0]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, nuint>)(lpVtbl[1]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, nuint>)(lpVtbl[2]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, Guid*, void*>)(lpVtbl[3]))((FGpuBuffer*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, ulong>)(lpVtbl[4]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuBuffer*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuBuffer*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuViewableData *")]
        public FGpuViewableData* GpuViewData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, FGpuViewableData*>)(lpVtbl[6]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuResourceData *")]
        public FGpuResourceData* GpuResourceData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, FGpuResourceData*>)(lpVtbl[7]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::ResourceType")]
        public ResourceType GetResourceType()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, ResourceType>)(lpVtbl[8]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuBufferData *")]
        public FGpuBufferData* GpuBufferData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, FGpuBufferData*>)(lpVtbl[9]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Map(void** ptr, [NativeTypeName("Coplt::b8")] B8 Discard)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuBuffer*, FResult*, void**, B8, FResult*>)(lpVtbl[10]))((FGpuBuffer*)Unsafe.AsPointer(ref this), &result, ptr, Discard);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Unmap([NativeTypeName("Coplt::b8")] B8 Discard)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuBuffer*, FResult*, B8, FResult*>)(lpVtbl[11]))((FGpuBuffer*)Unsafe.AsPointer(ref this), &result, Discard);
        }

        public interface Interface : FGpuResource.Interface
        {
            [return: NativeTypeName("Coplt::FGpuBufferData *")]
            FGpuBufferData* GpuBufferData();

            [return: NativeTypeName("Coplt::FResult")]
            FResult Map(void** ptr, [NativeTypeName("Coplt::b8")] B8 Discard);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Unmap([NativeTypeName("Coplt::b8")] B8 Discard);
        }
    }

    [StructLayout(LayoutKind.Explicit)]
    public partial struct FOptimizedClearColor
    {
        [FieldOffset(0)]
        [NativeTypeName("f32[4]")]
        public _Color_e__FixedBuffer Color;

        [FieldOffset(0)]
        [NativeTypeName("__AnonymousRecord_Resource_L175_C9")]
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

    public partial struct FGpuImageCreateOptions
    {
    }

    [NativeTypeName("struct FGpuImageData : Coplt::FGpuResourceData")]
    public partial struct FGpuImageData
    {
        public FGpuResourceData Base;

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

    [Guid("667EFA36-21C7-4561-ABAD-85780FA4929E")]
    [NativeTypeName("struct FGpuImage : Coplt::FGpuResource")]
    public unsafe partial struct FGpuImage : FGpuImage.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuImage));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuImage*, void>)(lpVtbl[0]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, nuint>)(lpVtbl[1]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, nuint>)(lpVtbl[2]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, Guid*, void*>)(lpVtbl[3]))((FGpuImage*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, ulong>)(lpVtbl[4]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuImage*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuImage*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuViewableData *")]
        public FGpuViewableData* GpuViewData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, FGpuViewableData*>)(lpVtbl[6]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuResourceData *")]
        public FGpuResourceData* GpuResourceData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, FGpuResourceData*>)(lpVtbl[7]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::ResourceType")]
        public ResourceType GetResourceType()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, ResourceType>)(lpVtbl[8]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuImageData *")]
        public FGpuImageData* GpuImageData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, FGpuImageData*>)(lpVtbl[9]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuResource.Interface
        {
            [return: NativeTypeName("Coplt::FGpuImageData *")]
            FGpuImageData* GpuImageData();
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FViewType : byte
    {
        None,
        Buffer,
    }

    public unsafe partial struct FView
    {
        [NativeTypeName("__AnonymousRecord_Resource_L263_C9")]
        public _Anonymous_e__Union Anonymous;

        [NativeTypeName("Coplt::FViewType")]
        public FViewType Type;

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
            [NativeTypeName("Coplt::FGpuBuffer *")]
            public FGpuBuffer* Buffer;
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

    public partial struct FGpuOutputFromSwapChainCreateOptions
    {
        [NativeTypeName("Coplt::b8")]
        public B8 VSync;
    }

    public partial struct FGpuOutputCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

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
        [NativeTypeName("Coplt::FCommandItem *")]
        public FCommandItem* Commands;

        [NativeTypeName("Coplt::FRenderCommandItem *")]
        public FRenderCommandItem* RenderCommands;

        [NativeTypeName("Coplt::FComputeCommandItem *")]
        public FComputeCommandItem* ComputeCommands;

        [NativeTypeName("Coplt::FResourceMeta *")]
        public FResourceMeta* Resources;

        [NativeTypeName("Coplt::FRenderInfo *")]
        public FRenderInfo* RenderInfos;

        [NativeTypeName("Coplt::FComputeInfo *")]
        public FComputeInfo* ComputeInfos;

        [NativeTypeName("Coplt::FResolveInfo *")]
        public FResolveInfo* ResolveInfos;

        [NativeTypeName("Coplt::FRect *")]
        public FRect* Rects;

        [NativeTypeName("Coplt::FViewport *")]
        public FViewport* Viewports;

        [NativeTypeName("Coplt::FMeshBuffers *")]
        public FMeshBuffers* MeshBuffers;

        [NativeTypeName("Coplt::FVertexBufferRange *")]
        public FVertexBufferRange* VertexBufferRanges;

        [NativeTypeName("Coplt::FBufferCopyRange *")]
        public FBufferCopyRange* BufferCopyRanges;

        [NativeTypeName("Coplt::FBindItem *")]
        public FBindItem* BindItems;

        [NativeTypeName("Coplt::FBarrier *")]
        public FBarrier* Barriers;

        [NativeTypeName("Coplt::Char8 *")]
        public byte* Str8;

        [NativeTypeName("Coplt::Char16 *")]
        public char* Str16;

        [NativeTypeName("Coplt::u32")]
        public uint CommandCount;

        [NativeTypeName("Coplt::u32")]
        public uint ResourceCount;

        [NativeTypeName("Coplt::u32")]
        public uint SyncBindingCount;

        [NativeTypeName("Coplt::u32")]
        public uint GrowingResourceBindingCapacity;

        [NativeTypeName("Coplt::u32")]
        public uint GrowingSamplerBindingCapacity;
    }

    [Guid("F1C59CB4-7EE6-4EE2-80F4-07CC568920D2")]
    [NativeTypeName("struct FGpuOutput : Coplt::FGpuExecutor")]
    public unsafe partial struct FGpuOutput : FGpuOutput.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuOutput));

        public void** lpVtbl;

        [NativeTypeName("Coplt::FResState")]
        public FResState m_state;

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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, ulong>)(lpVtbl[4]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Wait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FResult*>)(lpVtbl[6]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, B8, FResult*>)(lpVtbl[7]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, Enable);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, uint, uint, FResult*>)(lpVtbl[8]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, Width, Height);
        }

        public interface Interface : FGpuExecutor.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height);
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
    public enum FLegacyState : uint
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
    public enum FResLayout : uint
    {
        None = 0,
        Common,
        Present = Common,
        GenericRead,
        RenderTarget,
        UnorderedAccess,
        DepthStencilRead,
        DepthStencilWrite,
        ShaderResource,
        CopySrc,
        CopyDst,
        ResolveSrc,
        ResolveDst,
        ShadingRate,
        VideoDecodeRead,
        VideoDecodeWrite,
        VideoProcessRead,
        VideoProcessWrite,
        VideoEncodeRead,
        VideoEncodeWrite,
    }

    [NativeTypeName("Coplt::u32")]
    public enum FResAccess : uint
    {
        NoAccess = 1U << 31,
        Common = 0,
        ConstantBuffer = 1 << 0,
        VertexBuffer = 1 << 1,
        IndexBuffer = 1 << 2,
        RenderTarget = 1 << 3,
        UnorderedAccess = 1 << 4,
        DepthStencilRead = 1 << 5,
        DepthStencilWrite = 1 << 6,
        ShaderResource = 1 << 7,
        StreamOutput = 1 << 8,
        IndirectOrPredicationBuffer = 1 << 9,
        CopySrc = 1 << 10,
        CopyDst = 1 << 11,
        ResolveSrc = 1 << 12,
        ResolveDst = 1 << 13,
        RayTracingAccelerationStructureRead = 1 << 14,
        RayTracingAccelerationStructureWrite = 1 << 15,
        ShadingRate = 1 << 16,
        VideoDecodeRead = 1 << 17,
        VideoDecodeWrite = 1 << 18,
        VideoProcessRead = 1 << 19,
        VideoProcessWrite = 1 << 20,
        VideoEncodeRead = 1 << 21,
        VideoEncodeWrite = 1 << 22,
    }

    public partial struct FResState
    {
        [NativeTypeName("Coplt::FResLayout")]
        public FResLayout Layout;

        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess Access;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags Stages;

        [NativeTypeName("Coplt::FLegacyState")]
        public FLegacyState Legacy;

        [NativeTypeName("Coplt::b8")]
        public B8 CrossQueue;
    }

    [NativeTypeName("Coplt::u32")]
    public enum FCommandType : uint
    {
        None,
        Label,
        BeginScope,
        EndScope,
        Present,
        Barrier,
        ClearColor,
        ClearDepthStencil,
        Bind,
        BufferCopy,
        Render,
        Compute,
        SetPipeline,
        SetBinding,
        SetViewportScissor,
        SetMeshBuffers,
        Draw,
        Dispatch,
        SyncBinding,
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
        [NativeTypeName("__AnonymousRecord_Command_L64_C9")]
        public _Anonymous_e__Union Anonymous;

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

    public partial struct FSubResourceRange
    {
        [NativeTypeName("Coplt::u32")]
        public uint IndexOrFirstMipLevel;

        [NativeTypeName("Coplt::u32")]
        public uint NumMipLevels;

        [NativeTypeName("Coplt::u32")]
        public uint FirstArraySlice;

        [NativeTypeName("Coplt::u32")]
        public uint NumArraySlices;

        [NativeTypeName("Coplt::u32")]
        public uint FirstPlane;

        [NativeTypeName("Coplt::u32")]
        public uint NumPlanes;
    }

    [NativeTypeName("Coplt::u32")]
    public enum FImageBarrierFlags : uint
    {
        None,
        Discard = 1 << 0,
        CrossQueue = 1 << 1,
    }

    public partial struct FImageBarrier
    {
        [NativeTypeName("Coplt::FLegacyState")]
        public FLegacyState LegacyBefore;

        [NativeTypeName("Coplt::FLegacyState")]
        public FLegacyState LegacyAfter;

        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess AccessBefore;

        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess AccessAfter;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags StagesBefore;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags StagesAfter;

        [NativeTypeName("Coplt::FResLayout")]
        public FResLayout LayoutBefore;

        [NativeTypeName("Coplt::FResLayout")]
        public FResLayout LayoutAfter;

        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Image;

        [NativeTypeName("Coplt::FSubResourceRange")]
        public FSubResourceRange SubResourceRange;

        [NativeTypeName("Coplt::FImageBarrierFlags")]
        public FImageBarrierFlags Flags;
    }

    public partial struct FBufferBarrier
    {
        [NativeTypeName("Coplt::FLegacyState")]
        public FLegacyState LegacyBefore;

        [NativeTypeName("Coplt::FLegacyState")]
        public FLegacyState LegacyAfter;

        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess AccessBefore;

        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess AccessAfter;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags StagesBefore;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags StagesAfter;

        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Buffer;

        [NativeTypeName("Coplt::u64")]
        public ulong Offset;

        [NativeTypeName("Coplt::u64")]
        public ulong Size;
    }

    public partial struct FGlobalBarrier
    {
        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess AccessBefore;

        [NativeTypeName("Coplt::FResAccess")]
        public FResAccess AccessAfter;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags StagesBefore;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags StagesAfter;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBarrierType : byte
    {
        None,
        Global,
        Buffer,
        Image,
    }

    public partial struct FBarrier
    {
        [NativeTypeName("Coplt::FBarrierType")]
        public FBarrierType Type;

        [NativeTypeName("__AnonymousRecord_Command_L170_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FGlobalBarrier Global
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Global;
            }
        }

        [UnscopedRef]
        public ref FBufferBarrier Buffer
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Buffer;
            }
        }

        [UnscopedRef]
        public ref FImageBarrier Image
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Image;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGlobalBarrier")]
            public FGlobalBarrier Global;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FBufferBarrier")]
            public FBufferBarrier Buffer;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FImageBarrier")]
            public FImageBarrier Image;
        }
    }

    public partial struct FRect
    {
        [NativeTypeName("Coplt::u32")]
        public uint Left;

        [NativeTypeName("Coplt::u32")]
        public uint Top;

        [NativeTypeName("Coplt::u32")]
        public uint Right;

        [NativeTypeName("Coplt::u32")]
        public uint Bottom;
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

    [NativeTypeName("Coplt::u8")]
    public enum FDepthStencilClearFlags : byte
    {
        None = 0,
        Depth = 1,
        Stencil = 2,
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

    public partial struct FBufferCopyRange
    {
        [NativeTypeName("Coplt::u64")]
        public ulong Size;

        [NativeTypeName("Coplt::u64")]
        public ulong DstOffset;

        [NativeTypeName("Coplt::u64")]
        public ulong SrcOffset;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FResolveMode : byte
    {
        Decompress,
        Min,
        Max,
        Average,
    }

    public partial struct FResolveInfo
    {
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Src;

        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Dst;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::FResolveMode")]
        public FResolveMode Mode;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FLoadOp : byte
    {
        Load,
        Clear,
        Discard,
        NoAccess,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FStoreOp : byte
    {
        Store,
        Discard,
        Resolve,
        NoAccess,
    }

    public partial struct FRenderInfo
    {
        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Dsv;

        [NativeTypeName("Coplt::u32")]
        public uint NumRtv;

        [NativeTypeName("FResourceRef[8]")]
        public _Rtv_e__FixedBuffer Rtv;

        [NativeTypeName("u32[8]")]
        public _ResolveInfoIndex_e__FixedBuffer ResolveInfoIndex;

        [NativeTypeName("Coplt::u32")]
        public uint DsvResolveInfoIndex;

        [NativeTypeName("FGraphicsFormat[8]")]
        public _RtvFormat_e__FixedBuffer RtvFormat;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat DsvFormat;

        [NativeTypeName("f32[32]")]
        public _Color_e__FixedBuffer Color;

        [NativeTypeName("Coplt::f32")]
        public float Depth;

        [NativeTypeName("Coplt::u8")]
        public byte Stencil;

        [NativeTypeName("FLoadOp[2]")]
        public _DsvLoadOp_e__FixedBuffer DsvLoadOp;

        [NativeTypeName("FStoreOp[2]")]
        public _DsvStoreOp_e__FixedBuffer DsvStoreOp;

        [NativeTypeName("FLoadOp[8]")]
        public _RtvLoadOp_e__FixedBuffer RtvLoadOp;

        [NativeTypeName("FStoreOp[8]")]
        public _RtvStoreOp_e__FixedBuffer RtvStoreOp;

        [NativeTypeName("Coplt::b8")]
        public B8 HasUavWrites;

        [InlineArray(8)]
        public partial struct _Rtv_e__FixedBuffer
        {
            public FResourceRef e0;
        }

        [InlineArray(8)]
        public partial struct _ResolveInfoIndex_e__FixedBuffer
        {
            public uint e0;
        }

        [InlineArray(8)]
        public partial struct _RtvFormat_e__FixedBuffer
        {
            public FGraphicsFormat e0;
        }

        [InlineArray(32)]
        public partial struct _Color_e__FixedBuffer
        {
            public float e0;
        }

        [InlineArray(2)]
        public partial struct _DsvLoadOp_e__FixedBuffer
        {
            public FLoadOp e0;
        }

        [InlineArray(2)]
        public partial struct _DsvStoreOp_e__FixedBuffer
        {
            public FStoreOp e0;
        }

        [InlineArray(8)]
        public partial struct _RtvLoadOp_e__FixedBuffer
        {
            public FLoadOp e0;
        }

        [InlineArray(8)]
        public partial struct _RtvStoreOp_e__FixedBuffer
        {
            public FStoreOp e0;
        }
    }

    public partial struct FComputeInfo
    {
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

    public unsafe partial struct FMeshBuffers
    {
        [NativeTypeName("Coplt::FMeshLayout *")]
        public FMeshLayout* MeshLayout;

        [NativeTypeName("Coplt::FBufferRange")]
        public FBufferRange IndexBuffer;

        [NativeTypeName("Coplt::u32")]
        public uint VertexBufferCount;

        [NativeTypeName("Coplt::u32")]
        public uint VertexBuffersIndex;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDispatchType : byte
    {
        Compute,
        Mesh,
    }

    public partial struct FCommandBase
    {
        [NativeTypeName("Coplt::FCommandType")]
        public FCommandType Type;
    }

    [NativeTypeName("struct FCommandLabel : Coplt::FCommandBase")]
    public partial struct FCommandLabel
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint StringIndex;

        [NativeTypeName("Coplt::u32")]
        public uint StringLength;

        [NativeTypeName("Coplt::FStrType")]
        public FStrType StrType;

        [NativeTypeName("u8[3]")]
        public _Color_e__FixedBuffer Color;

        [NativeTypeName("Coplt::b8")]
        public B8 HasColor;

        [InlineArray(3)]
        public partial struct _Color_e__FixedBuffer
        {
            public byte e0;
        }
    }

    [NativeTypeName("struct FCommandBeginScope : Coplt::FCommandBase")]
    public partial struct FCommandBeginScope
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint StringIndex;

        [NativeTypeName("Coplt::u32")]
        public uint StringLength;

        [NativeTypeName("Coplt::FStrType")]
        public FStrType StrType;

        [NativeTypeName("u8[3]")]
        public _Color_e__FixedBuffer Color;

        [NativeTypeName("Coplt::b8")]
        public B8 HasColor;

        [InlineArray(3)]
        public partial struct _Color_e__FixedBuffer
        {
            public byte e0;
        }
    }

    [NativeTypeName("struct FCommandEndScope : Coplt::FCommandBase")]
    public partial struct FCommandEndScope
    {
        public FCommandBase Base;
    }

    [NativeTypeName("struct FCommandPresent : Coplt::FCommandBase")]
    public partial struct FCommandPresent
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::FResourceRef")]
        public FResourceRef Image;
    }

    [NativeTypeName("struct FCommandBarrier : Coplt::FCommandBase")]
    public partial struct FCommandBarrier
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint BarrierIndex;

        [NativeTypeName("Coplt::u32")]
        public uint BarrierCount;
    }

    [NativeTypeName("struct FCommandClearColor : Coplt::FCommandBase")]
    public partial struct FCommandClearColor
    {
        public FCommandBase Base;

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

    [NativeTypeName("struct FCommandClearDepthStencil : Coplt::FCommandBase")]
    public partial struct FCommandClearDepthStencil
    {
        public FCommandBase Base;

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

    [NativeTypeName("struct FCommandBind : Coplt::FCommandBase")]
    public unsafe partial struct FCommandBind
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint ItemCount;

        [NativeTypeName("Coplt::u32")]
        public uint ItemsIndex;

        [NativeTypeName("Coplt::FShaderBinding *")]
        public FShaderBinding* Binding;
    }

    [NativeTypeName("struct FCommandBufferCopy : Coplt::FCommandBase")]
    public partial struct FCommandBufferCopy
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint RangeIndex;

        [NativeTypeName("Coplt::FBufferRef")]
        public FBufferRef Dst;

        [NativeTypeName("Coplt::FBufferRef")]
        public FBufferRef Src;

        [NativeTypeName("Coplt::FBufferRefType")]
        public FBufferRefType DstType;

        [NativeTypeName("Coplt::FBufferRefType")]
        public FBufferRefType SrcType;
    }

    [NativeTypeName("struct FCommandRender : Coplt::FCommandBase")]
    public partial struct FCommandRender
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint InfoIndex;

        [NativeTypeName("Coplt::u32")]
        public uint CommandStartIndex;

        [NativeTypeName("Coplt::u32")]
        public uint CommandCount;
    }

    [NativeTypeName("struct FCommandCompute : Coplt::FCommandBase")]
    public partial struct FCommandCompute
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint InfoIndex;

        [NativeTypeName("Coplt::u32")]
        public uint CommandStartIndex;

        [NativeTypeName("Coplt::u32")]
        public uint CommandCount;
    }

    [NativeTypeName("struct FCommandSetPipeline : Coplt::FCommandBase")]
    public unsafe partial struct FCommandSetPipeline
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::FShaderPipeline *")]
        public FShaderPipeline* Pipeline;
    }

    [NativeTypeName("struct FCommandSetBinding : Coplt::FCommandBase")]
    public unsafe partial struct FCommandSetBinding
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::FShaderBinding *")]
        public FShaderBinding* Binding;
    }

    [NativeTypeName("struct FCommandSetViewportScissor : Coplt::FCommandBase")]
    public partial struct FCommandSetViewportScissor
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportCount;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportIndex;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectCount;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectIndex;
    }

    [NativeTypeName("struct FCommandSetMeshBuffers : Coplt::FCommandBase")]
    public partial struct FCommandSetMeshBuffers
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat IndexFormat;

        [NativeTypeName("Coplt::u32")]
        public uint VertexStartSlot;

        [NativeTypeName("Coplt::u32")]
        public uint PayloadIndex;
    }

    [NativeTypeName("struct FCommandDraw : Coplt::FCommandBase")]
    public partial struct FCommandDraw
    {
        public FCommandBase Base;

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

    [NativeTypeName("struct FCommandDispatch : Coplt::FCommandBase")]
    public partial struct FCommandDispatch
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountX;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountY;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountZ;

        [NativeTypeName("Coplt::FDispatchType")]
        public FDispatchType Type;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBindingSyncType : byte
    {
        Transient,
        Persistent,
    }

    [NativeTypeName("struct FCommandSyncBinding : Coplt::FCommandBase")]
    public partial struct FCommandSyncBinding
    {
        public FCommandBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint Index;

        [NativeTypeName("Coplt::FBindingSyncType")]
        public FBindingSyncType Type;
    }

    public partial struct FCommandItem
    {
        [NativeTypeName("__AnonymousRecord_Command_L503_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FCommandType Type
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Type;
            }
        }

        [UnscopedRef]
        public ref FCommandLabel Label
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Label;
            }
        }

        [UnscopedRef]
        public ref FCommandBeginScope BeginScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BeginScope;
            }
        }

        [UnscopedRef]
        public ref FCommandEndScope EndScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.EndScope;
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
        public ref FCommandBarrier Barrier
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Barrier;
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
        public ref FCommandBind Bind
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Bind;
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
        public ref FCommandRender Render
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Render;
            }
        }

        [UnscopedRef]
        public ref FCommandCompute Compute
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Compute;
            }
        }

        [UnscopedRef]
        public ref FCommandSyncBinding SyncBinding
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SyncBinding;
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
            [NativeTypeName("Coplt::FCommandType")]
            public FCommandType Type;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandLabel")]
            public FCommandLabel Label;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBeginScope")]
            public FCommandBeginScope BeginScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandEndScope")]
            public FCommandEndScope EndScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandPresent")]
            public FCommandPresent Present;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBarrier")]
            public FCommandBarrier Barrier;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandClearColor")]
            public FCommandClearColor ClearColor;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandClearDepthStencil")]
            public FCommandClearDepthStencil ClearDepthStencil;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBind")]
            public FCommandBind Bind;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBufferCopy")]
            public FCommandBufferCopy BufferCopy;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandRender")]
            public FCommandRender Render;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandCompute")]
            public FCommandCompute Compute;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSyncBinding")]
            public FCommandSyncBinding SyncBinding;

            [FieldOffset(0)]
            [NativeTypeName("u8[32]")]
            public __pad_e__FixedBuffer _pad;

            [InlineArray(32)]
            public partial struct __pad_e__FixedBuffer
            {
                public byte e0;
            }
        }
    }

    public partial struct FRenderCommandItem
    {
        [NativeTypeName("__AnonymousRecord_Command_L533_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FCommandType Type
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Type;
            }
        }

        [UnscopedRef]
        public ref FCommandLabel Label
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Label;
            }
        }

        [UnscopedRef]
        public ref FCommandBeginScope BeginScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BeginScope;
            }
        }

        [UnscopedRef]
        public ref FCommandEndScope EndScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.EndScope;
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
        public ref FCommandSetBinding SetBinding
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetBinding;
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
        public ref FCommandSyncBinding SyncBinding
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SyncBinding;
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
            [NativeTypeName("Coplt::FCommandType")]
            public FCommandType Type;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandLabel")]
            public FCommandLabel Label;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBeginScope")]
            public FCommandBeginScope BeginScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandEndScope")]
            public FCommandEndScope EndScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetPipeline")]
            public FCommandSetPipeline SetPipeline;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetBinding")]
            public FCommandSetBinding SetBinding;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetViewportScissor")]
            public FCommandSetViewportScissor SetViewportScissor;

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
            [NativeTypeName("Coplt::FCommandSyncBinding")]
            public FCommandSyncBinding SyncBinding;

            [FieldOffset(0)]
            [NativeTypeName("u8[32]")]
            public __pad_e__FixedBuffer _pad;

            [InlineArray(32)]
            public partial struct __pad_e__FixedBuffer
            {
                public byte e0;
            }
        }
    }

    public partial struct FComputeCommandItem
    {
        [NativeTypeName("__AnonymousRecord_Command_L558_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FCommandType Type
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Type;
            }
        }

        [UnscopedRef]
        public ref FCommandLabel Label
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Label;
            }
        }

        [UnscopedRef]
        public ref FCommandBeginScope BeginScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BeginScope;
            }
        }

        [UnscopedRef]
        public ref FCommandEndScope EndScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.EndScope;
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
        public ref FCommandSetBinding SetBinding
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetBinding;
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
        public ref FCommandSyncBinding SyncBinding
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SyncBinding;
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
            [NativeTypeName("Coplt::FCommandType")]
            public FCommandType Type;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandLabel")]
            public FCommandLabel Label;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandBeginScope")]
            public FCommandBeginScope BeginScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandEndScope")]
            public FCommandEndScope EndScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetPipeline")]
            public FCommandSetPipeline SetPipeline;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSetBinding")]
            public FCommandSetBinding SetBinding;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandDispatch")]
            public FCommandDispatch Dispatch;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCommandSyncBinding")]
            public FCommandSyncBinding SyncBinding;

            [FieldOffset(0)]
            [NativeTypeName("u8[32]")]
            public __pad_e__FixedBuffer _pad;

            [InlineArray(32)]
            public partial struct __pad_e__FixedBuffer
            {
                public byte e0;
            }
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FResourceAccess : byte
    {
        Unknown,
        ReadOnly,
        WriteOnly,
        ReadWrite,
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
    public enum FShaderLayoutGroupView : byte
    {
        Cbv,
        Srv,
        Uav,
        Sampler,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutItemType : byte
    {
        ConstantBuffer,
        Buffer,
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
        Dynamic,
        Persist,
        Instant,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutGroupScope : byte
    {
        Dynamic,
        Persist,
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

        [NativeTypeName("Coplt::FResourceAccess")]
        public FResourceAccess UavAccess;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutItemPlace : byte
    {
        None,
        Const,
        Direct,
        Grouped,
        StaticSampler,
    }

    public partial struct FShaderLayoutItemInfo
    {
        [NativeTypeName("Coplt::u32")]
        public uint Index;

        [NativeTypeName("Coplt::u32")]
        public uint Class;

        [NativeTypeName("Coplt::u32")]
        public uint Group;

        [NativeTypeName("Coplt::FShaderLayoutItemPlace")]
        public FShaderLayoutItemPlace Place;
    }

    public partial struct FShaderLayoutGroupInfo
    {
        [NativeTypeName("Coplt::u32")]
        public uint Index;

        [NativeTypeName("Coplt::u32")]
        public uint Size;

        [NativeTypeName("Coplt::FShaderStage")]
        public FShaderStage Stage;

        [NativeTypeName("Coplt::FShaderLayoutGroupView")]
        public FShaderLayoutGroupView View;
    }

    public unsafe partial struct FShaderLayoutGroupClass
    {
        [NativeTypeName("Coplt::FShaderLayoutGroupInfo *")]
        public FShaderLayoutGroupInfo* Infos;

        [NativeTypeName("Coplt::u32")]
        public uint Size;

        [NativeTypeName("Coplt::FShaderLayoutGroupScope")]
        public FShaderLayoutGroupScope Scope;

        [NativeTypeName("Coplt::b8")]
        public B8 Sampler;
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
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::FShaderLayoutItemDefine *")]
        public FShaderLayoutItemDefine* Items;

        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    public partial struct FGetEmptyShaderLayoutOptions
    {
        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    public unsafe partial struct FShaderInputLayoutCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FShaderInputLayoutElement *")]
        public FShaderInputLayoutElement* Element;

        [NativeTypeName("Coplt::u32")]
        public uint Count;
    }

    public unsafe partial struct FMeshLayoutCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, ulong>)(lpVtbl[4]))((FMeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FMeshLayout*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FMeshLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferDefine *")]
        public FMeshBufferDefine* GetBuffers([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, uint*, FMeshBufferDefine*>)(lpVtbl[6]))((FMeshLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferElement *")]
        public FMeshBufferElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, uint*, FMeshBufferElement*>)(lpVtbl[7]))((FMeshLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferElement *")]
        public FMeshBufferElement* TryGetElement([NativeTypeName("Coplt::u32")] uint SlotId, [NativeTypeName("Coplt::u32")] uint SlotIndex)
        {
            return ((delegate* unmanaged[Thiscall]<FMeshLayout*, uint, uint, FMeshBufferElement*>)(lpVtbl[8]))((FMeshLayout*)Unsafe.AsPointer(ref this), SlotId, SlotIndex);
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
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, ulong>)(lpVtbl[4]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderPipeline*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FShader*>)(lpVtbl[6]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FShaderLayout*>)(lpVtbl[7]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FShaderStageFlags>)(lpVtbl[8]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, ulong>)(lpVtbl[4]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FShader*>)(lpVtbl[6]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FShaderLayout*>)(lpVtbl[7]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FShaderStageFlags>)(lpVtbl[8]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
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
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, ulong>)(lpVtbl[4]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShader*>)(lpVtbl[6]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShaderLayout*>)(lpVtbl[7]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShaderStageFlags>)(lpVtbl[8]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FGraphicsPipelineState *")]
        public FGraphicsPipelineState* GetGraphicsState()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FGraphicsPipelineState*>)(lpVtbl[9]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderInputLayout *")]
        public FShaderInputLayout* GetInputLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FShaderInputLayout*>)(lpVtbl[10]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshLayout *")]
        public FMeshLayout* GetMeshLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FMeshLayout*>)(lpVtbl[11]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FShaderPipeline.Interface
        {
            [return: NativeTypeName("const FGraphicsPipelineState *")]
            FGraphicsPipelineState* GetGraphicsState();

            [return: NativeTypeName("const FShaderInputLayout *")]
            FShaderInputLayout* GetInputLayout();

            [return: NativeTypeName("const FMeshLayout *")]
            FMeshLayout* GetMeshLayout();
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

        public static readonly Guid IID_FGpuAdapter = new Guid(0xB3BBBED6, 0xE997, 0x4BD5, 0xBE, 0x53, 0x0A, 0xAC, 0x0C, 0xA8, 0x11, 0x4D);

        public static readonly Guid IID_FGpuDevice = new Guid(0x557F032D, 0xED50, 0x403A, 0xAD, 0xC5, 0x21, 0x4F, 0xDD, 0xBE, 0x6C, 0x6B);

        public static readonly Guid IID_FShaderModule = new Guid(0x5C0E1FDB, 0x2ACD, 0x4FCE, 0xB9, 0x85, 0x09, 0xE1, 0x2A, 0x7A, 0x1A, 0xAD);

        public static readonly Guid IID_FShader = new Guid(0xDE1352D5, 0x023D, 0x42B0, 0xBE, 0xAC, 0x12, 0x2B, 0x3B, 0x29, 0x6C, 0x9C);

        public static readonly Guid IID_FShaderBinding = new Guid(0xA3CA644A, 0x0E02, 0x4D25, 0x9A, 0x18, 0x88, 0x35, 0xD6, 0x66, 0x00, 0xF7);

        public static readonly Guid IID_FFrameContext = new Guid(0x3A22DE68, 0x8377, 0x4E57, 0xA1, 0x67, 0x3E, 0xFD, 0xB9, 0x7C, 0x10, 0x2A);

        public static readonly Guid IID_FGpuQueue = new Guid(0x95E60E28, 0xE387, 0x4055, 0x9B, 0x33, 0x2D, 0x23, 0xAF, 0x90, 0x1F, 0x8A);

        public static readonly Guid IID_FGpuExecutor = new Guid(0xEDAB0989, 0xE0C2, 0x4B1E, 0x82, 0x6F, 0xC7, 0x33, 0x80, 0x08, 0x8F, 0xC6);

        public static readonly Guid IID_FGpuViewable = new Guid(0xB3AEB8A5, 0x1FA6, 0x4866, 0x97, 0xEF, 0x1A, 0x5F, 0xA4, 0x01, 0xE1, 0x8F);

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
