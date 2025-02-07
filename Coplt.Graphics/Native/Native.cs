using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Coplt.Graphics.Native
{
    public unsafe partial struct FObject
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FObject*, void>)(lpVtbl[0]))((FObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FObject*, nuint>)(lpVtbl[1]))((FObject*)Unsafe.AsPointer(ref this));
        }
    }

    [NativeTypeName("struct FRcObject : Coplt::FObject")]
    public unsafe partial struct FRcObject
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FRcObject*, void>)(lpVtbl[0]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, nuint>)(lpVtbl[1]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, nuint>)(lpVtbl[2]))((FRcObject*)Unsafe.AsPointer(ref this));
        }
    }

    [NativeTypeName("struct FUnknown : Coplt::FRcObject")]
    public unsafe partial struct FUnknown
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "00000000-0000-0000-0000-000000000000";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FUnknown*, void>)(lpVtbl[0]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, nuint>)(lpVtbl[1]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, nuint>)(lpVtbl[2]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, Guid*, void*>)(lpVtbl[3]))((FUnknown*)Unsafe.AsPointer(ref this), id);
        }
    }

    [NativeTypeName("struct META_FString8 : Coplt::FUnknown")]
    public unsafe partial struct META_FString8
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "63c7fc71-d775-42bb-891a-69dbb26e75c5";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FString8*, void>)(lpVtbl[0]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, nuint>)(lpVtbl[1]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, nuint>)(lpVtbl[2]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, Guid*, void*>)(lpVtbl[3]))((META_FString8*)Unsafe.AsPointer(ref this), id);
        }
    }

    [NativeTypeName("struct FString8 : Coplt::_internal::META_FString8")]
    public unsafe partial struct FString8
    {
        public void** lpVtbl;

        [NativeTypeName("const Char8 *")]
        public byte* m_data;

        [NativeTypeName("Coplt::usize")]
        public nuint m_size;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FString8*, void>)(lpVtbl[0]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, nuint>)(lpVtbl[1]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, nuint>)(lpVtbl[2]))((FString8*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, Guid*, void*>)(lpVtbl[3]))((FString8*)Unsafe.AsPointer(ref this), id);
        }
    }

    [NativeTypeName("struct META_FString16 : Coplt::FUnknown")]
    public unsafe partial struct META_FString16
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "669355d5-9443-487c-8486-b0b0c00d2367";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FString16*, void>)(lpVtbl[0]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, nuint>)(lpVtbl[1]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, nuint>)(lpVtbl[2]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, Guid*, void*>)(lpVtbl[3]))((META_FString16*)Unsafe.AsPointer(ref this), id);
        }
    }

    [NativeTypeName("struct FString16 : Coplt::_internal::META_FString16")]
    public unsafe partial struct FString16
    {
        public void** lpVtbl;

        [NativeTypeName("const Char16 *")]
        public char* m_data;

        [NativeTypeName("Coplt::usize")]
        public nuint m_size;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FString16*, void>)(lpVtbl[0]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, nuint>)(lpVtbl[1]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, nuint>)(lpVtbl[2]))((FString16*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, Guid*, void*>)(lpVtbl[3]))((FString16*)Unsafe.AsPointer(ref this), id);
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

    [NativeTypeName("struct META_FGpuObject : Coplt::FUnknown")]
    public unsafe partial struct META_FGpuObject
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "9fc6890b-e9a2-4f4a-9217-a14a4223a715";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FGpuObject*, void>)(lpVtbl[0]))((META_FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuObject*, nuint>)(lpVtbl[1]))((META_FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuObject*, nuint>)(lpVtbl[2]))((META_FGpuObject*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuObject*, Guid*, void*>)(lpVtbl[3]))((META_FGpuObject*)Unsafe.AsPointer(ref this), id);
        }
    }

    [NativeTypeName("struct FGpuObject : Coplt::_internal::META_FGpuObject")]
    public unsafe partial struct FGpuObject
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuObject*, void>)(lpVtbl[0]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, nuint>)(lpVtbl[1]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, nuint>)(lpVtbl[2]))((FGpuObject*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuObject*, Guid*, void*>)(lpVtbl[3]))((FGpuObject*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuObject*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuObject*)Unsafe.AsPointer(ref this), &result, name);
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

    [NativeTypeName("struct FGpuDevice : Coplt::_internal::META_FGpuDevice")]
    public unsafe partial struct FGpuDevice
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuDevice*, void>)(lpVtbl[0]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, nuint>)(lpVtbl[1]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, nuint>)(lpVtbl[2]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, Guid*, void*>)(lpVtbl[3]))((FGpuDevice*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, name);
        }

        public void* GetRawDevice()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuDevice*, void*>)(lpVtbl[5]))((FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FMainQueueCreateOptions*, FGpuQueue**, FResult*>)(lpVtbl[6]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }
    }

    [NativeTypeName("struct META_FInstance : Coplt::FUnknown")]
    public unsafe partial struct META_FInstance
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "cc2894ba-57c7-474a-b777-1a3e3a7c922c";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FInstance*, void>)(lpVtbl[0]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, nuint>)(lpVtbl[1]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, nuint>)(lpVtbl[2]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, Guid*, void*>)(lpVtbl[3]))((META_FInstance*)Unsafe.AsPointer(ref this), id);
        }
    }

    [NativeTypeName("struct FInstance : Coplt::_internal::META_FInstance")]
    public unsafe partial struct FInstance
    {
        public void** lpVtbl;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FInstance*, void>)(lpVtbl[0]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, nuint>)(lpVtbl[1]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, nuint>)(lpVtbl[2]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, Guid*, void*>)(lpVtbl[3]))((FInstance*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetLogger([NativeTypeName("const FLogger &")] FLogger* logger)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FLogger*, FResult*>)(lpVtbl[4]))((FInstance*)Unsafe.AsPointer(ref this), &result, logger);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, FGpuDevice** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FGpuDeviceCreateOptions*, FGpuDevice**, FResult*>)(lpVtbl[5]))((FInstance*)Unsafe.AsPointer(ref this), &result, options, @out);
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

    [NativeTypeName("struct META_FGpuDevice : Coplt::FGpuObject")]
    public unsafe partial struct META_FGpuDevice
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "557f032d-ed50-403a-adc5-214fddbe6c6b";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FGpuDevice*, void>)(lpVtbl[0]))((META_FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuDevice*, nuint>)(lpVtbl[1]))((META_FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuDevice*, nuint>)(lpVtbl[2]))((META_FGpuDevice*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuDevice*, Guid*, void*>)(lpVtbl[3]))((META_FGpuDevice*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FGpuDevice*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((META_FGpuDevice*)Unsafe.AsPointer(ref this), &result, name);
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

    [NativeTypeName("struct META_FGpuQueue : Coplt::FGpuObject")]
    public unsafe partial struct META_FGpuQueue
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "95e60e28-e387-4055-9b33-2d23af901f8a";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FGpuQueue*, void>)(lpVtbl[0]))((META_FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuQueue*, nuint>)(lpVtbl[1]))((META_FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuQueue*, nuint>)(lpVtbl[2]))((META_FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuQueue*, Guid*, void*>)(lpVtbl[3]))((META_FGpuQueue*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FGpuQueue*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((META_FGpuQueue*)Unsafe.AsPointer(ref this), &result, name);
        }
    }

    [NativeTypeName("struct FGpuQueue : Coplt::_internal::META_FGpuQueue")]
    public unsafe partial struct FGpuQueue
    {
        public void** lpVtbl;

        [NativeTypeName("Coplt::FGpuQueueType")]
        public FGpuQueueType m_queue_type;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuQueue*, void>)(lpVtbl[0]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, nuint>)(lpVtbl[1]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, nuint>)(lpVtbl[2]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, Guid*, void*>)(lpVtbl[3]))((FGpuQueue*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, name);
        }

        public void* GetRawQueue()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue*, void*>)(lpVtbl[5]))((FGpuQueue*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateOutputForHwnd([NativeTypeName("const FGpuOutputCreateOptions &")] FGpuOutputCreateOptions* options, void* hwnd, FGpuOutput** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue*, FResult*, FGpuOutputCreateOptions*, void*, FGpuOutput**, FResult*>)(lpVtbl[6]))((FGpuQueue*)Unsafe.AsPointer(ref this), &result, options, hwnd, @out);
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

    [NativeTypeName("struct META_FGpuOutput : Coplt::FGpuObject")]
    public unsafe partial struct META_FGpuOutput
    {
        public void** lpVtbl;

        [NativeTypeName("const wchar_t *const")]
        public const string s_FFI_GUID = "f1c59cb4-7ee6-4ee2-80f4-07cc568920d2";

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<META_FGpuOutput*, void>)(lpVtbl[0]))((META_FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuOutput*, nuint>)(lpVtbl[1]))((META_FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuOutput*, nuint>)(lpVtbl[2]))((META_FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FGpuOutput*, Guid*, void*>)(lpVtbl[3]))((META_FGpuOutput*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<META_FGpuOutput*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((META_FGpuOutput*)Unsafe.AsPointer(ref this), &result, name);
        }
    }

    [NativeTypeName("struct FGpuOutput : Coplt::_internal::META_FGpuOutput")]
    public unsafe partial struct FGpuOutput
    {
        public void** lpVtbl;

        [NativeTypeName("Coplt::FTextureFormat")]
        public FTextureFormat m_format;

        [NativeTypeName("Coplt::u32")]
        public uint m_width;

        [NativeTypeName("Coplt::u32")]
        public uint m_height;

        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuOutput*, void>)(lpVtbl[0]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, nuint>)(lpVtbl[1]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, nuint>)(lpVtbl[2]))((FGpuOutput*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput*, Guid*, void*>)(lpVtbl[3]))((FGpuOutput*)Unsafe.AsPointer(ref this), id);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, name);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, B8, FResult*>)(lpVtbl[5]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, Enable);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, uint, uint, FResult*>)(lpVtbl[6]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result, Width, Height);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult Present()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FResult*>)(lpVtbl[7]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult PresentNoWait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FResult*>)(lpVtbl[8]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result);
        }

        [return: NativeTypeName("Coplt::FResult")]
        public FResult WaitNextFrame()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput*, FResult*, FResult*>)(lpVtbl[9]))((FGpuOutput*)Unsafe.AsPointer(ref this), &result);
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
}
