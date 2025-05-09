using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using static Coplt.Graphics.Native.Native;

namespace Coplt.Graphics.Native
{
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

    [StructLayout(LayoutKind.Explicit)]
    public partial struct FOrRectViewport
    {
        [FieldOffset(0)]
        [NativeTypeName("Coplt::FRect")]
        public FRect Rect;

        [FieldOffset(0)]
        [NativeTypeName("Coplt::FViewport")]
        public FViewport Viewport;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDepthStencilClearFlags : byte
    {
        None = 0,
        Depth = 1,
        Stencil = 2,
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

    [NativeTypeName("Coplt::u8")]
    public enum FResolveMode : byte
    {
        Decompress,
        Min,
        Max,
        Average,
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

    public partial struct FBufferImageCopyRange
    {
        [NativeTypeName("Coplt::u64")]
        public ulong BufferOffset;

        [NativeTypeName("Coplt::u32")]
        public uint BytesPerRow;

        [NativeTypeName("Coplt::u32")]
        public uint RowsPerImage;

        [NativeTypeName("u32[3]")]
        public _ImageOffset_e__FixedBuffer ImageOffset;

        [NativeTypeName("u32[3]")]
        public _ImageExtent_e__FixedBuffer ImageExtent;

        [NativeTypeName("Coplt::u32")]
        public uint ImageIndex;

        [NativeTypeName("Coplt::u32")]
        public uint ImageCount;

        [NativeTypeName("Coplt::u16")]
        public ushort MipLevel;

        [NativeTypeName("Coplt::FImagePlane")]
        public FImagePlane Plane;

        [InlineArray(3)]
        public partial struct _ImageOffset_e__FixedBuffer
        {
            public uint e0;
        }

        [InlineArray(3)]
        public partial struct _ImageExtent_e__FixedBuffer
        {
            public uint e0;
        }
    }

    public partial struct FUploadLoc
    {
        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FDispatchType : byte
    {
        Compute,
        Mesh,
    }

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
        public FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, [NativeTypeName("Coplt::FGpuDeviceCreateResult *")] FGpuDeviceCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuAdapter*, FResult*, FGpuDeviceCreateOptions*, FGpuDeviceCreateResult*, FResult*>)(lpVtbl[5]))((FGpuAdapter*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FUnknown.Interface
        {
            [return: NativeTypeName("Coplt::b8")]
            B8 IsSupport([NativeTypeName("const FDeviceRequires &")] FDeviceRequires* Requires);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateDevice([NativeTypeName("const FGpuDeviceCreateOptions &")] FGpuDeviceCreateOptions* options, [NativeTypeName("Coplt::FGpuDeviceCreateResult *")] FGpuDeviceCreateResult* @out);
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

    public partial struct FGpuDeviceCreateResult
    {
    }

    public unsafe partial struct FGpuDeviceCreateResult
    {
        [NativeTypeName("Coplt::FGpuDevice *")]
        public FGpuDevice* Device;

        [NativeTypeName("Coplt::FGpuIsolateCreateResult")]
        public FGpuIsolateCreateResult MainIsolate;

        [NativeTypeName("Coplt::FGpuIsolateCreateResult")]
        public FGpuIsolateCreateResult CopyIsolate;
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
        public FResult CreateDevice([NativeTypeName("const FGpuAutoSelectDeviceCreateOptions &")] FGpuAutoSelectDeviceCreateOptions* options, [NativeTypeName("Coplt::FGpuDeviceCreateResult *")] FGpuDeviceCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FInstance*, FResult*, FGpuAutoSelectDeviceCreateOptions*, FGpuDeviceCreateResult*, FResult*>)(lpVtbl[9]))((FInstance*)Unsafe.AsPointer(ref this), &result, options, @out);
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
            FResult CreateDevice([NativeTypeName("const FGpuAutoSelectDeviceCreateOptions &")] FGpuAutoSelectDeviceCreateOptions* options, [NativeTypeName("Coplt::FGpuDeviceCreateResult *")] FGpuDeviceCreateResult* @out);
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

        [NativeTypeName("Coplt::b8")]
        public B8 ExtraIsolate;

        [NativeTypeName("Coplt::b8")]
        public B8 UMA;

        [NativeTypeName("Coplt::b8")]
        public B8 CacheCoherentUMA;
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
        public FResult CreateIsolate([NativeTypeName("const FGpuIsolateCreateOptions &")] FGpuIsolateCreateOptions* options, [NativeTypeName("Coplt::FGpuIsolateCreateResult &")] FGpuIsolateCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGpuIsolateCreateOptions*, FGpuIsolateCreateResult*, FResult*>)(lpVtbl[8]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, [NativeTypeName("Coplt::FShaderLayoutCreateResult *")] FShaderLayoutCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderLayoutCreateOptions*, FShaderLayoutCreateResult*, FResult*>)(lpVtbl[9]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateBindGroupLayout([NativeTypeName("const FBindGroupLayoutCreateOptions &")] FBindGroupLayoutCreateOptions* options, [NativeTypeName("Coplt::FBindGroupLayoutCreateResult *")] FBindGroupLayoutCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FBindGroupLayoutCreateOptions*, FBindGroupLayoutCreateResult*, FResult*>)(lpVtbl[10]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateBindingLayout([NativeTypeName("const FBindingLayoutCreateOptions &")] FBindingLayoutCreateOptions* options, FBindingLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FBindingLayoutCreateOptions*, FBindingLayout**, FResult*>)(lpVtbl[11]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GetEmptyShaderLayout([NativeTypeName("const FGetEmptyShaderLayoutOptions &")] FGetEmptyShaderLayoutOptions* options, [NativeTypeName("Coplt::FShaderLayoutCreateResult *")] FShaderLayoutCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGetEmptyShaderLayoutOptions*, FShaderLayoutCreateResult*, FResult*>)(lpVtbl[12]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GetEmptyBindGroupLayout([NativeTypeName("Coplt::FBindGroupLayoutCreateResult *")] FBindGroupLayoutCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FBindGroupLayoutCreateResult*, FResult*>)(lpVtbl[13]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GetEmptyBindingLayout([NativeTypeName("const FGetEmptyBindingLayoutOptions &")] FGetEmptyBindingLayoutOptions* options, FBindingLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGetEmptyBindingLayoutOptions*, FBindingLayout**, FResult*>)(lpVtbl[14]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderInputLayoutCreateOptions*, FShaderInputLayout**, FResult*>)(lpVtbl[15]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FMeshLayoutCreateOptions*, FMeshLayout**, FResult*>)(lpVtbl[16]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, [NativeTypeName("Coplt::FShaderModuleCreateResult *")] FShaderModuleCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderModuleCreateOptions*, FShaderModuleCreateResult*, FResult*>)(lpVtbl[17]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, [NativeTypeName("Coplt::FShaderCreateResult *")] FShaderCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderCreateOptions*, FShaderCreateResult*, FResult*>)(lpVtbl[18]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderBindGroup([NativeTypeName("const FShaderBindGroupCreateOptions &")] FShaderBindGroupCreateOptions* options, [NativeTypeName("Coplt::FShaderBindGroupCreateResult *")] FShaderBindGroupCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderBindGroupCreateOptions*, FShaderBindGroupCreateResult*, FResult*>)(lpVtbl[19]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderBinding([NativeTypeName("const FShaderBindingCreateOptions &")] FShaderBindingCreateOptions* options, [NativeTypeName("Coplt::FShaderBindingCreateResult *")] FShaderBindingCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderBindingCreateOptions*, FShaderBindingCreateResult*, FResult*>)(lpVtbl[20]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGraphicsShaderPipelineCreateOptions*, FGraphicsShaderPipeline**, FResult*>)(lpVtbl[21]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateComputePipeline([NativeTypeName("const FShaderPipelineCreateOptions &")] FShaderPipelineCreateOptions* options, FComputeShaderPipeline** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FShaderPipelineCreateOptions*, FComputeShaderPipeline**, FResult*>)(lpVtbl[22]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateBuffer([NativeTypeName("const FGpuBufferCreateOptions &")] FGpuBufferCreateOptions* options, FGpuBuffer** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGpuBufferCreateOptions*, FGpuBuffer**, FResult*>)(lpVtbl[23]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateImage([NativeTypeName("const FGpuImageCreateOptions &")] FGpuImageCreateOptions* options, FGpuImage** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGpuImageCreateOptions*, FGpuImage**, FResult*>)(lpVtbl[24]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateSampler([NativeTypeName("const FGpuSamplerCreateOptions &")] FGpuSamplerCreateOptions* options, FGpuSampler** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuDevice*, FResult*, FGpuSamplerCreateOptions*, FGpuSampler**, FResult*>)(lpVtbl[25]))((FGpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FGpuAdapter *")]
            FGpuAdapter* GetAdapter();

            void* GetRawDevice();

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateIsolate([NativeTypeName("const FGpuIsolateCreateOptions &")] FGpuIsolateCreateOptions* options, [NativeTypeName("Coplt::FGpuIsolateCreateResult &")] FGpuIsolateCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, [NativeTypeName("Coplt::FShaderLayoutCreateResult *")] FShaderLayoutCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateBindGroupLayout([NativeTypeName("const FBindGroupLayoutCreateOptions &")] FBindGroupLayoutCreateOptions* options, [NativeTypeName("Coplt::FBindGroupLayoutCreateResult *")] FBindGroupLayoutCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateBindingLayout([NativeTypeName("const FBindingLayoutCreateOptions &")] FBindingLayoutCreateOptions* options, FBindingLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult GetEmptyShaderLayout([NativeTypeName("const FGetEmptyShaderLayoutOptions &")] FGetEmptyShaderLayoutOptions* options, [NativeTypeName("Coplt::FShaderLayoutCreateResult *")] FShaderLayoutCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult GetEmptyBindGroupLayout([NativeTypeName("Coplt::FBindGroupLayoutCreateResult *")] FBindGroupLayoutCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult GetEmptyBindingLayout([NativeTypeName("const FGetEmptyBindingLayoutOptions &")] FGetEmptyBindingLayoutOptions* options, FBindingLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, [NativeTypeName("Coplt::FShaderModuleCreateResult *")] FShaderModuleCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, [NativeTypeName("Coplt::FShaderCreateResult *")] FShaderCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderBindGroup([NativeTypeName("const FShaderBindGroupCreateOptions &")] FShaderBindGroupCreateOptions* options, [NativeTypeName("Coplt::FShaderBindGroupCreateResult *")] FShaderBindGroupCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateShaderBinding([NativeTypeName("const FShaderBindingCreateOptions &")] FShaderBindingCreateOptions* options, [NativeTypeName("Coplt::FShaderBindingCreateResult *")] FShaderBindingCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateComputePipeline([NativeTypeName("const FShaderPipelineCreateOptions &")] FShaderPipelineCreateOptions* options, FComputeShaderPipeline** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateBuffer([NativeTypeName("const FGpuBufferCreateOptions &")] FGpuBufferCreateOptions* options, FGpuBuffer** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateImage([NativeTypeName("const FGpuImageCreateOptions &")] FGpuImageCreateOptions* options, FGpuImage** @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateSampler([NativeTypeName("const FGpuSamplerCreateOptions &")] FGpuSamplerCreateOptions* options, FGpuSampler** @out);
        }
    }

    public partial struct FGpuRecord
    {
    }

    [Guid("0EF83584-CA65-44DE-B38A-648BFB3A85A6")]
    [NativeTypeName("struct FGpuRecord : Coplt::FGpuObject")]
    public unsafe partial struct FGpuRecord : FGpuRecord.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuRecord));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuRecord*, void>)(lpVtbl[0]))((FGpuRecord*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuRecord*, nuint>)(lpVtbl[1]))((FGpuRecord*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuRecord*, nuint>)(lpVtbl[2]))((FGpuRecord*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuRecord*, Guid*, void*>)(lpVtbl[3]))((FGpuRecord*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuRecord*, ulong>)(lpVtbl[4]))((FGpuRecord*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuRecord*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuRecord*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuRecordData *")]
        public FGpuRecordData* GpuFGpuRecordData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuRecord*, FGpuRecordData*>)(lpVtbl[6]))((FGpuRecord*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult End()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuRecord*, FResult*, FResult*>)(lpVtbl[7]))((FGpuRecord*)Unsafe.AsPointer(ref this), &result);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FGpuRecordData *")]
            FGpuRecordData* GpuFGpuRecordData();

            [return: NativeTypeName("Coplt::FResult")]
            FResult End();
        }
    }

    public partial struct FGpuRecordCreateResult
    {
    }

    public unsafe partial struct FGpuRecordCreateResult
    {
        [NativeTypeName("Coplt::FGpuRecord *")]
        public FGpuRecord* Record;

        [NativeTypeName("Coplt::FGpuRecordData *")]
        public FGpuRecordData* Data;
    }

    public enum FGpuIsolateType
    {
        Main,
        Copy,
    }

    public partial struct FGpuIsolateCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FGpuIsolateType")]
        public FGpuIsolateType Type;
    }

    public partial struct FGpuIsolateConfig
    {
        [NativeTypeName("Coplt::b8")]
        public B8 MultiThreadRecord;

        [NativeTypeName("Coplt::b8")]
        public B8 UseSplitBarrier;
    }

    public unsafe partial struct FGpuIsolateData
    {
        [NativeTypeName("Coplt::u64")]
        public ulong FrameId;

        [NativeTypeName("Coplt::FGpuIsolateConfig *")]
        public FGpuIsolateConfig* Config;
    }

    [Guid("777C5774-8EB8-4550-A977-62CCCD7BDDA6")]
    [NativeTypeName("struct FGpuIsolate : Coplt::FGpuObject")]
    public unsafe partial struct FGpuIsolate : FGpuIsolate.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuIsolate));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuIsolate*, void>)(lpVtbl[0]))((FGpuIsolate*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuIsolate*, nuint>)(lpVtbl[1]))((FGpuIsolate*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuIsolate*, nuint>)(lpVtbl[2]))((FGpuIsolate*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuIsolate*, Guid*, void*>)(lpVtbl[3]))((FGpuIsolate*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuIsolate*, ulong>)(lpVtbl[4]))((FGpuIsolate*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuIsolateData *")]
        public FGpuIsolateData* GpuIsolateData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuIsolate*, FGpuIsolateData*>)(lpVtbl[6]))((FGpuIsolate*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult RentRecords([NativeTypeName("Coplt::u32")] uint NumRecords, [NativeTypeName("Coplt::FGpuRecordCreateResult *")] FGpuRecordCreateResult* OutRecords)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, uint, FGpuRecordCreateResult*, FResult*>)(lpVtbl[7]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, NumRecords, OutRecords);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult ReturnRecords([NativeTypeName("Coplt::u32")] uint NumRecords, FGpuRecord** Records)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, uint, FGpuRecord**, FResult*>)(lpVtbl[8]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, NumRecords, Records);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Submit([NativeTypeName("Coplt::u32")] uint NumRecords, FGpuRecord** Records, [NativeTypeName("Coplt::FGpuRecordCreateResult *")] FGpuRecordCreateResult* OutRecords)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, uint, FGpuRecord**, FGpuRecordCreateResult*, FResult*>)(lpVtbl[9]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, NumRecords, Records, OutRecords);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SubmitReturn([NativeTypeName("Coplt::u32")] uint NumRecords, FGpuRecord** Records)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, uint, FGpuRecord**, FResult*>)(lpVtbl[10]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, NumRecords, Records);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateSwapChainFromExists([NativeTypeName("const FGpuSwapChainFromExistsCreateOptions &")] FGpuSwapChainFromExistsCreateOptions* options, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, FGpuSwapChainFromExistsCreateOptions*, FGpuSwapChainCreateResult*, FResult*>)(lpVtbl[11]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateSwapChainForComposition([NativeTypeName("const FGpuSwapChainCreateOptions &")] FGpuSwapChainCreateOptions* options, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, FGpuSwapChainCreateOptions*, FGpuSwapChainCreateResult*, FResult*>)(lpVtbl[12]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateSwapChainForHwnd([NativeTypeName("const FGpuSwapChainCreateOptions &")] FGpuSwapChainCreateOptions* options, void* hwnd, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, FGpuSwapChainCreateOptions*, void*, FGpuSwapChainCreateResult*, FResult*>)(lpVtbl[13]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, options, hwnd, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateSwapChainForCoreWindow([NativeTypeName("const FGpuSwapChainCreateOptions &")] FGpuSwapChainCreateOptions* options, void* w, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuIsolate*, FResult*, FGpuSwapChainCreateOptions*, void*, FGpuSwapChainCreateResult*, FResult*>)(lpVtbl[14]))((FGpuIsolate*)Unsafe.AsPointer(ref this), &result, options, w, @out);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FGpuIsolateData *")]
            FGpuIsolateData* GpuIsolateData();

            [return: NativeTypeName("Coplt::FResult")]
            FResult RentRecords([NativeTypeName("Coplt::u32")] uint NumRecords, [NativeTypeName("Coplt::FGpuRecordCreateResult *")] FGpuRecordCreateResult* OutRecords);

            [return: NativeTypeName("Coplt::FResult")]
            FResult ReturnRecords([NativeTypeName("Coplt::u32")] uint NumRecords, FGpuRecord** Records);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Submit([NativeTypeName("Coplt::u32")] uint NumRecords, FGpuRecord** Records, [NativeTypeName("Coplt::FGpuRecordCreateResult *")] FGpuRecordCreateResult* OutRecords);

            [return: NativeTypeName("Coplt::FResult")]
            FResult SubmitReturn([NativeTypeName("Coplt::u32")] uint NumRecords, FGpuRecord** Records);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateSwapChainFromExists([NativeTypeName("const FGpuSwapChainFromExistsCreateOptions &")] FGpuSwapChainFromExistsCreateOptions* options, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateSwapChainForComposition([NativeTypeName("const FGpuSwapChainCreateOptions &")] FGpuSwapChainCreateOptions* options, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateSwapChainForHwnd([NativeTypeName("const FGpuSwapChainCreateOptions &")] FGpuSwapChainCreateOptions* options, void* hwnd, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult CreateSwapChainForCoreWindow([NativeTypeName("const FGpuSwapChainCreateOptions &")] FGpuSwapChainCreateOptions* options, void* w, [NativeTypeName("Coplt::FGpuSwapChainCreateResult &")] FGpuSwapChainCreateResult* @out);
        }
    }

    public partial struct FGpuIsolate
    {
    }

    public unsafe partial struct FGpuIsolateCreateResult
    {
        [NativeTypeName("Coplt::FGpuIsolate *")]
        public FGpuIsolate* Isolate;

        [NativeTypeName("Coplt::FGpuIsolateData *")]
        public FGpuIsolateData* Data;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuRecordMode : byte
    {
        Direct = 0,
        Compute = 1,
        Copy = 2,
    }

    public unsafe partial struct FGpuRecordData
    {
        [NativeTypeName("Coplt::u64")]
        public ulong Id;

        [NativeTypeName("Coplt::u64")]
        public ulong Version;

        [NativeTypeName("Coplt::FRecordContext *")]
        public FRecordContext* Context;

        public FList<FCmdItem> Commands;

        public FList<FCmdRes> Resources;

        public FList<FCmdBinding> Bindings;

        public FList<FBindingChange> BindingChange;

        public FList<FRect> PayloadRect;

        public FList<FViewport> PayloadViewport;

        public FList<FRenderInfo2> PayloadRenderInfo;

        public FList<FResolveInfo2> PayloadResolveInfo;

        public FList<FBufferCopyRange> PayloadBufferCopyRange;

        public FList<FBufferImageCopyRange> PayloadBufferImageCopyRange;

        public FList<FMeshBuffers2> PayloadMeshBuffers;

        public FList<FVertexBufferRange2> PayloadVertexBufferRange;

        [NativeTypeName("FList<u32>")]
        public FList<uint> Payload32Bits;

        public FList<FSetBindItem> PayloadSetBindItem;

        [NativeTypeName("FList<u8>")]
        public FList<byte> Blob;

        [NativeTypeName("Coplt::u32")]
        public uint NumSyncBindings;

        [NativeTypeName("Coplt::u32")]
        public uint SumDynArraySize;

        [NativeTypeName("Coplt::b8")]
        public B8 Ended;

        [NativeTypeName("Coplt::FGpuRecordMode")]
        public FGpuRecordMode Mode;
    }

    [NativeTypeName("Coplt::u32")]
    public enum FCmdType : uint
    {
        None = 0,
        End = 1,
        Label,
        BeginScope,
        EndScope,
        PreparePresent,
        ClearColor,
        ClearDepthStencil,
        BufferCopy,
        BufferImageCopy,
        Render,
        Compute,
        SetPipeline,
        SetBinding,
        SetConstants,
        SetDynArraySize,
        SetBindItem,
        SetViewportScissor,
        SetMeshBuffers,
        Draw,
        Dispatch,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FCmdResType : byte
    {
        Image,
        Buffer,
        Output,
    }

    public unsafe partial struct FCmdRes
    {
        [NativeTypeName("__AnonymousRecord_Cmd_L74_C9")]
        public _Anonymous_e__Union Anonymous;

        [NativeTypeName("Coplt::FCmdResType")]
        public FCmdResType Type;

        [UnscopedRef]
        public ref FGpuOutput2* Output
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

        [UnscopedRef]
        public ref FGpuImage* Image
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Image;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public unsafe partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGpuOutput2 *")]
            public FGpuOutput2* Output;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGpuBuffer *")]
            public FGpuBuffer* Buffer;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FGpuImage *")]
            public FGpuImage* Image;
        }
    }

    public partial struct FResIndex
    {
        [NativeTypeName("Coplt::u32")]
        public uint Value;
    }

    public partial struct FCmdResRef
    {
        [NativeTypeName("Coplt::u32")]
        public uint IndexPlusOne;
    }

    public partial struct FResolveInfo2
    {
        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Src;

        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Dst;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::FResolveMode")]
        public FResolveMode Mode;
    }

    public partial struct FRenderInfo2
    {
        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Dsv;

        [NativeTypeName("Coplt::u32")]
        public uint NumRtv;

        [NativeTypeName("FCmdResRef[8]")]
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
            public FCmdResRef e0;
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

    [StructLayout(LayoutKind.Explicit)]
    public partial struct FBufferRef2
    {
        [FieldOffset(0)]
        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Buffer;

        [FieldOffset(0)]
        [NativeTypeName("Coplt::FUploadLoc")]
        public FUploadLoc Upload;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBufferRefType2 : byte
    {
        Buffer = 0,
        Upload,
    }

    public partial struct FCmdBase
    {
        [NativeTypeName("Coplt::FCmdType")]
        public FCmdType Type;
    }

    [NativeTypeName("struct FCmdLabel : Coplt::FCmdBase")]
    public partial struct FCmdLabel
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint StringLength;

        [NativeTypeName("Coplt::usize")]
        public nuint StringIndex;

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

    [NativeTypeName("struct FCmdBeginScope : Coplt::FCmdBase")]
    public partial struct FCmdBeginScope
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint StringLength;

        [NativeTypeName("Coplt::usize")]
        public nuint StringIndex;

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

    [NativeTypeName("struct FCmdEndScope : Coplt::FCmdBase")]
    public partial struct FCmdEndScope
    {
        public FCmdBase Base;
    }

    [NativeTypeName("struct FCmdPreparePresent : Coplt::FCmdBase")]
    public partial struct FCmdPreparePresent
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Output;
    }

    [NativeTypeName("struct FCmdClearColor : Coplt::FCmdBase")]
    public partial struct FCmdClearColor
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint RectCount;

        [NativeTypeName("Coplt::u32")]
        public uint RectIndex;

        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Image;

        [NativeTypeName("f32[4]")]
        public _Color_e__FixedBuffer Color;

        [InlineArray(4)]
        public partial struct _Color_e__FixedBuffer
        {
            public float e0;
        }
    }

    [NativeTypeName("struct FCmdClearDepthStencil : Coplt::FCmdBase")]
    public partial struct FCmdClearDepthStencil
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint RectCount;

        [NativeTypeName("Coplt::u32")]
        public uint RectIndex;

        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Image;

        [NativeTypeName("Coplt::f32")]
        public float Depth;

        [NativeTypeName("Coplt::u8")]
        public byte Stencil;

        [NativeTypeName("Coplt::FDepthStencilClearFlags")]
        public FDepthStencilClearFlags Clear;
    }

    [NativeTypeName("struct FCmdBufferCopy : Coplt::FCmdBase")]
    public partial struct FCmdBufferCopy
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint RangeIndex;

        [NativeTypeName("Coplt::FBufferRef2")]
        public FBufferRef2 Dst;

        [NativeTypeName("Coplt::FBufferRef2")]
        public FBufferRef2 Src;

        [NativeTypeName("Coplt::FBufferRefType2")]
        public FBufferRefType2 DstType;

        [NativeTypeName("Coplt::FBufferRefType2")]
        public FBufferRefType2 SrcType;
    }

    [NativeTypeName("struct FCmdBufferImageCopy : Coplt::FCmdBase")]
    public partial struct FCmdBufferImageCopy
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint RangeIndex;

        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Image;

        [NativeTypeName("Coplt::FBufferRef2")]
        public FBufferRef2 Buffer;

        [NativeTypeName("Coplt::FBufferRefType2")]
        public FBufferRefType2 BufferType;

        [NativeTypeName("Coplt::b8")]
        public B8 ImageToBuffer;
    }

    [NativeTypeName("struct FCmdRender : Coplt::FCmdBase")]
    public partial struct FCmdRender
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint InfoIndex;

        [NativeTypeName("Coplt::u32")]
        public uint CommandCount;
    }

    [NativeTypeName("struct FCmdCompute : Coplt::FCmdBase")]
    public partial struct FCmdCompute
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint CommandCount;
    }

    [NativeTypeName("struct FCmdSetPipeline : Coplt::FCmdBase")]
    public unsafe partial struct FCmdSetPipeline
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::FShaderPipeline *")]
        public FShaderPipeline* Pipeline;
    }

    public unsafe partial struct FCmdBinding
    {
        [NativeTypeName("Coplt::FShaderBinding *")]
        public FShaderBinding* Binding;
    }

    public partial struct FBindingChange
    {
        [NativeTypeName("Coplt::u32")]
        public uint Binding;
    }

    [NativeTypeName("struct FCmdSetBinding : Coplt::FCmdBase")]
    public partial struct FCmdSetBinding
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint Binding;

        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    [NativeTypeName("struct FCmdSyncBinding : Coplt::FCmdBase")]
    public partial struct FCmdSyncBinding
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint SyncBindingIndex;
    }

    [NativeTypeName("struct FCmdSetConstants : Coplt::FCmdBase")]
    public unsafe partial struct FCmdSetConstants
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::FShaderBindGroup *")]
        public FShaderBindGroup* Group;

        [NativeTypeName("Coplt::u32")]
        public uint SetConstantsIndex;

        [NativeTypeName("Coplt::u32")]
        public uint Slot;

        [NativeTypeName("Coplt::u32")]
        public uint ValueIndex;

        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::u32")]
        public uint Offset;
    }

    [NativeTypeName("struct FCmdSetDynArraySize : Coplt::FCmdBase")]
    public unsafe partial struct FCmdSetDynArraySize
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::FShaderBindGroup *")]
        public FShaderBindGroup* Group;

        [NativeTypeName("Coplt::u32")]
        public uint Size;
    }

    [NativeTypeName("struct FCmdSetBindItem : Coplt::FCmdBase")]
    public unsafe partial struct FCmdSetBindItem
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::FShaderBindGroup *")]
        public FShaderBindGroup* Group;

        [NativeTypeName("Coplt::u32")]
        public uint ItemIndex;

        [NativeTypeName("Coplt::u32")]
        public uint Count;
    }

    [NativeTypeName("struct FCmdSetViewportScissor : Coplt::FCmdBase")]
    public partial struct FCmdSetViewportScissor
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportCount;

        [NativeTypeName("Coplt::u32")]
        public uint ViewportIndex;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectCount;

        [NativeTypeName("Coplt::u32")]
        public uint ScissorRectIndex;
    }

    public partial struct FBufferRange2
    {
        [NativeTypeName("Coplt::FCmdResRef")]
        public FCmdResRef Buffer;

        [NativeTypeName("Coplt::u32")]
        public uint Offset;

        [NativeTypeName("Coplt::u32")]
        public uint Size;
    }

    [NativeTypeName("struct FVertexBufferRange2 : Coplt::FBufferRange2")]
    public partial struct FVertexBufferRange2
    {
        public FBufferRange2 Base;

        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    public unsafe partial struct FMeshBuffers2
    {
        [NativeTypeName("Coplt::FMeshLayout *")]
        public FMeshLayout* MeshLayout;

        [NativeTypeName("Coplt::FBufferRange2")]
        public FBufferRange2 IndexBuffer;

        [NativeTypeName("Coplt::u32")]
        public uint VertexBufferCount;

        [NativeTypeName("Coplt::u32")]
        public uint VertexBuffersIndex;
    }

    [NativeTypeName("struct FCmdSetMeshBuffers : Coplt::FCmdBase")]
    public partial struct FCmdSetMeshBuffers
    {
        public FCmdBase Base;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat IndexFormat;

        [NativeTypeName("Coplt::u32")]
        public uint VertexStartSlot;

        [NativeTypeName("Coplt::u32")]
        public uint PayloadIndex;
    }

    [NativeTypeName("struct FCmdDraw : Coplt::FCmdSyncBinding")]
    public partial struct FCmdDraw
    {
        public FCmdSyncBinding Base;

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

    [NativeTypeName("struct FCmdDispatch : Coplt::FCmdSyncBinding")]
    public partial struct FCmdDispatch
    {
        public FCmdSyncBinding Base;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountX;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountY;

        [NativeTypeName("Coplt::u32")]
        public uint GroupCountZ;

        [NativeTypeName("Coplt::FDispatchType")]
        public FDispatchType Type;
    }

    public partial struct FCmdItem
    {
        [NativeTypeName("__AnonymousRecord_Cmd_L399_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FCmdType Type
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Type;
            }
        }

        [UnscopedRef]
        public ref FCmdLabel Label
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Label;
            }
        }

        [UnscopedRef]
        public ref FCmdBeginScope BeginScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BeginScope;
            }
        }

        [UnscopedRef]
        public ref FCmdEndScope EndScope
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.EndScope;
            }
        }

        [UnscopedRef]
        public ref FCmdPreparePresent PreparePresent
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.PreparePresent;
            }
        }

        [UnscopedRef]
        public ref FCmdClearColor ClearColor
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.ClearColor;
            }
        }

        [UnscopedRef]
        public ref FCmdClearDepthStencil ClearDepthStencil
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.ClearDepthStencil;
            }
        }

        [UnscopedRef]
        public ref FCmdBufferCopy BufferCopy
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BufferCopy;
            }
        }

        [UnscopedRef]
        public ref FCmdBufferImageCopy BufferImageCopy
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.BufferImageCopy;
            }
        }

        [UnscopedRef]
        public ref FCmdRender Render
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Render;
            }
        }

        [UnscopedRef]
        public ref FCmdCompute Compute
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Compute;
            }
        }

        [UnscopedRef]
        public ref FCmdSetPipeline SetPipeline
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetPipeline;
            }
        }

        [UnscopedRef]
        public ref FCmdSetBinding SetBinding
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetBinding;
            }
        }

        [UnscopedRef]
        public ref FCmdSetConstants SetConstants
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetConstants;
            }
        }

        [UnscopedRef]
        public ref FCmdSetDynArraySize SetDynArraySize
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetDynArraySize;
            }
        }

        [UnscopedRef]
        public ref FCmdSetBindItem SetBindItem
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetBindItem;
            }
        }

        [UnscopedRef]
        public ref FCmdSetViewportScissor SetViewportScissor
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetViewportScissor;
            }
        }

        [UnscopedRef]
        public ref FCmdSetMeshBuffers SetMeshBuffers
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.SetMeshBuffers;
            }
        }

        [UnscopedRef]
        public ref FCmdDraw Draw
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Draw;
            }
        }

        [UnscopedRef]
        public ref FCmdDispatch Dispatch
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Dispatch;
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
            [NativeTypeName("Coplt::FCmdType")]
            public FCmdType Type;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdLabel")]
            public FCmdLabel Label;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdBeginScope")]
            public FCmdBeginScope BeginScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdEndScope")]
            public FCmdEndScope EndScope;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdPreparePresent")]
            public FCmdPreparePresent PreparePresent;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdClearColor")]
            public FCmdClearColor ClearColor;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdClearDepthStencil")]
            public FCmdClearDepthStencil ClearDepthStencil;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdBufferCopy")]
            public FCmdBufferCopy BufferCopy;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdBufferImageCopy")]
            public FCmdBufferImageCopy BufferImageCopy;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdRender")]
            public FCmdRender Render;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdCompute")]
            public FCmdCompute Compute;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetPipeline")]
            public FCmdSetPipeline SetPipeline;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetBinding")]
            public FCmdSetBinding SetBinding;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetConstants")]
            public FCmdSetConstants SetConstants;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetDynArraySize")]
            public FCmdSetDynArraySize SetDynArraySize;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetBindItem")]
            public FCmdSetBindItem SetBindItem;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetViewportScissor")]
            public FCmdSetViewportScissor SetViewportScissor;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdSetMeshBuffers")]
            public FCmdSetMeshBuffers SetMeshBuffers;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdDraw")]
            public FCmdDraw Draw;

            [FieldOffset(0)]
            [NativeTypeName("Coplt::FCmdDispatch")]
            public FCmdDispatch Dispatch;

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

    public unsafe partial struct FShaderModuleData
    {
        [NativeTypeName("Coplt::u8 *")]
        public byte* Data;

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
        [return: NativeTypeName("Coplt::FShaderModuleData *")]
        public FShaderModuleData* ShaderModuleData()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, FShaderModuleData*>)(lpVtbl[6]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FString8 *")]
        public FString8* GetEntryPoint()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderModule*, FString8*>)(lpVtbl[7]))((FShaderModule*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShaderModuleData *")]
            FShaderModuleData* ShaderModuleData();

            [return: NativeTypeName("Coplt::FString8 *")]
            FString8* GetEntryPoint();
        }
    }

    public unsafe partial struct FShaderModuleCreateResult
    {
        [NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* ShaderModule;

        [NativeTypeName("Coplt::FShaderModuleData *")]
        public FShaderModuleData* Data;
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
        [return: NativeTypeName("Coplt::FShaderLayoutData *")]
        public FShaderLayoutData* ShaderLayoutData()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderLayout*, FShaderLayoutData*>)(lpVtbl[6]))((FShaderLayout*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShaderLayoutData *")]
            FShaderLayoutData* ShaderLayoutData();
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

    public partial struct FShaderData
    {
        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags Stages;
    }

    [Guid("DE1352D5-023D-42B0-BEAC-122B3B296C9C")]
    [NativeTypeName("struct FShader : Coplt::FGpuObject")]
    public unsafe partial struct FShader : FShader.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShader));

        public void** lpVtbl;

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
        [return: NativeTypeName("Coplt::FShaderData *")]
        public FShaderData* ShaderData()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderData*>)(lpVtbl[6]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* Layout()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderLayout*>)(lpVtbl[7]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderInputLayout *")]
        public FShaderInputLayout* InputLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderInputLayout*>)(lpVtbl[8]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Compute()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[9]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Pixel()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[10]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Vertex()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[11]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Mesh()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[12]))((FShader*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderModule *")]
        public FShaderModule* Task()
        {
            return ((delegate* unmanaged[Thiscall]<FShader*, FShaderModule*>)(lpVtbl[13]))((FShader*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShaderData *")]
            FShaderData* ShaderData();

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

    public unsafe partial struct FShaderCreateResult
    {
        [NativeTypeName("Coplt::FShader *")]
        public FShader* Shader;

        [NativeTypeName("Coplt::FShaderData *")]
        public FShaderData* Data;
    }

    public partial struct FSetBindItem
    {
        [NativeTypeName("Coplt::FView")]
        public FView View;

        [NativeTypeName("Coplt::u32")]
        public uint Slot;

        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    public unsafe partial struct FShaderBindGroupCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FBindGroupLayout *")]
        public FBindGroupLayout* Layout;

        [NativeTypeName("Coplt::FSetBindItem *")]
        public FSetBindItem* Bindings;

        [NativeTypeName("Coplt::u32")]
        public uint NumBindings;
    }

    public partial struct FShaderBindGroupData
    {
        [NativeTypeName("Coplt::u32")]
        public uint CountSlots;
    }

    [Guid("AE54EFE5-C372-4291-B995-55298758D2B2")]
    [NativeTypeName("struct FShaderBindGroup : Coplt::FGpuObject")]
    public unsafe partial struct FShaderBindGroup : FShaderBindGroup.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FShaderBindGroup));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FShaderBindGroup*, void>)(lpVtbl[0]))((FShaderBindGroup*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBindGroup*, nuint>)(lpVtbl[1]))((FShaderBindGroup*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBindGroup*, nuint>)(lpVtbl[2]))((FShaderBindGroup*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBindGroup*, Guid*, void*>)(lpVtbl[3]))((FShaderBindGroup*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBindGroup*, ulong>)(lpVtbl[4]))((FShaderBindGroup*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FShaderBindGroup*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FShaderBindGroup*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderBindGroupData *")]
        public FShaderBindGroupData* ShaderBindGroupData()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBindGroup*, FShaderBindGroupData*>)(lpVtbl[6]))((FShaderBindGroup*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShaderBindGroupData *")]
            FShaderBindGroupData* ShaderBindGroupData();
        }
    }

    public unsafe partial struct FShaderBindGroupCreateResult
    {
        [NativeTypeName("Coplt::FShaderBindGroup *")]
        public FShaderBindGroup* BindGroup;

        [NativeTypeName("Coplt::FShaderBindGroupData *")]
        public FShaderBindGroupData* Data;
    }

    public unsafe partial struct FSetBindGroupItem
    {
        [NativeTypeName("Coplt::FShaderBindGroup *")]
        public FShaderBindGroup* BindGroup;

        [NativeTypeName("Coplt::u32")]
        public uint Index;
    }

    public unsafe partial struct FShaderBindingCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FBindingLayout *")]
        public FBindingLayout* Layout;

        [NativeTypeName("Coplt::FSetBindGroupItem *")]
        public FSetBindGroupItem* BindGroups;

        [NativeTypeName("Coplt::u32")]
        public uint NumBindGroups;
    }

    public partial struct FShaderBindingData
    {
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

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderBindingData *")]
        public FShaderBindingData* ShaderBindingData()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderBinding*, FShaderBindingData*>)(lpVtbl[6]))((FShaderBinding*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FShaderBindingData *")]
            FShaderBindingData* ShaderBindingData();
        }
    }

    public unsafe partial struct FShaderBindingCreateResult
    {
        [NativeTypeName("Coplt::FShaderBinding *")]
        public FShaderBinding* Binding;

        [NativeTypeName("Coplt::FShaderBindingData *")]
        public FShaderBindingData* Data;
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

    [Guid("15B4821D-0648-4B78-9C21-C6A5B6A5ED75")]
    [NativeTypeName("struct FRecordContext : Coplt::FGpuObject")]
    public unsafe partial struct FRecordContext : FRecordContext.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FRecordContext));

        public void** lpVtbl;

        public FList<FUploadBufferBlock> m_upload_buffer;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FRecordContext*, void>)(lpVtbl[0]))((FRecordContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FRecordContext*, nuint>)(lpVtbl[1]))((FRecordContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FRecordContext*, nuint>)(lpVtbl[2]))((FRecordContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FRecordContext*, Guid*, void*>)(lpVtbl[3]))((FRecordContext*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FRecordContext*, ulong>)(lpVtbl[4]))((FRecordContext*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FRecordContext*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FRecordContext*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GrowUploadBuffer([NativeTypeName("Coplt::u64")] ulong min_required_size)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FRecordContext*, FResult*, ulong, FResult*>)(lpVtbl[6]))((FRecordContext*)Unsafe.AsPointer(ref this), &result, min_required_size);
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult GrowUploadBuffer([NativeTypeName("Coplt::u64")] ulong min_required_size);
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuQueueType : byte
    {
        Direct,
        Compute,
        Copy,
        VideoEncode,
        VideoDecode,
        VideoProcess,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FGpuQueueFlags : byte
    {
        None = 0,
        Direct = 1 << 0,
        Compute = 1 << 1,
        Copy = 1 << 2,
        VideoEncode = 1 << 3,
        VideoDecode = 1 << 4,
        VideoProcess = 1 << 5,
    }

    public partial struct FGpuQueueData
    {
        [NativeTypeName("Coplt::FGpuQueueType")]
        public FGpuQueueType QueueType;
    }

    [Guid("FC94E4D0-5F01-4D4F-894C-6AEFAAD90499")]
    [NativeTypeName("struct FGpuQueue2 : Coplt::FGpuObject")]
    public unsafe partial struct FGpuQueue2 : FGpuQueue2.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuQueue2));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuQueue2*, void>)(lpVtbl[0]))((FGpuQueue2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue2*, nuint>)(lpVtbl[1]))((FGpuQueue2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue2*, nuint>)(lpVtbl[2]))((FGpuQueue2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue2*, Guid*, void*>)(lpVtbl[3]))((FGpuQueue2*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue2*, ulong>)(lpVtbl[4]))((FGpuQueue2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuQueue2*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuQueue2*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuQueueData *")]
        public FGpuQueueData* GpuQueueData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue2*, FGpuQueueData*>)(lpVtbl[6]))((FGpuQueue2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawQueue()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuQueue2*, void*>)(lpVtbl[7]))((FGpuQueue2*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FGpuQueueData *")]
            FGpuQueueData* GpuQueueData();

            void* GetRawQueue();
        }
    }

    public unsafe partial struct FGpuQueueCreateResult
    {
        [NativeTypeName("Coplt::FGpuQueue2 *")]
        public FGpuQueue2* Queue;

        [NativeTypeName("Coplt::FGpuQueueData *")]
        public FGpuQueueData* Data;
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

    public partial struct FGpuBuffer
    {
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
        [return: NativeTypeName("Coplt::FGpuResourceData *")]
        public FGpuResourceData* GpuResourceData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, FGpuResourceData*>)(lpVtbl[6]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuBufferData *")]
        public FGpuBufferData* GpuBufferData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuBuffer*, FGpuBufferData*>)(lpVtbl[7]))((FGpuBuffer*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Map(void** ptr, [NativeTypeName("Coplt::b8")] B8 discard)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuBuffer*, FResult*, void**, B8, FResult*>)(lpVtbl[8]))((FGpuBuffer*)Unsafe.AsPointer(ref this), &result, ptr, discard);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Unmap([NativeTypeName("Coplt::b8")] B8 discard)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuBuffer*, FResult*, B8, FResult*>)(lpVtbl[9]))((FGpuBuffer*)Unsafe.AsPointer(ref this), &result, discard);
        }

        public interface Interface : FGpuResource.Interface
        {
            [return: NativeTypeName("Coplt::FGpuBufferData *")]
            FGpuBufferData* GpuBufferData();

            [return: NativeTypeName("Coplt::FResult")]
            FResult Map(void** ptr, [NativeTypeName("Coplt::b8")] B8 discard);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Unmap([NativeTypeName("Coplt::b8")] B8 discard);
        }
    }

    public partial struct FGpuImage
    {
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
        [return: NativeTypeName("Coplt::FGpuResourceData *")]
        public FGpuResourceData* GpuResourceData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, FGpuResourceData*>)(lpVtbl[6]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuImageData *")]
        public FGpuImageData* GpuImageData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuImage*, FGpuImageData*>)(lpVtbl[7]))((FGpuImage*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuResource.Interface
        {
            [return: NativeTypeName("Coplt::FGpuImageData *")]
            FGpuImageData* GpuImageData();
        }
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

        public interface Interface : FGpuObject.Interface
        {
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FViewType : byte
    {
        None = 0,
        Sampler = 1,
        Buffer = 2,
        Image1D = 3,
        Image1DArray = 4,
        Image2D = 5,
        Image2DArray = 6,
        Image2DMs = 7,
        Image2DMsArray = 8,
        Image3D = 9,
        ImageCube = 10,
        ImageCubeArray = 11,
        UploadBuffer = 12,
    }

    public partial struct FViewData
    {
        [NativeTypeName("__AnonymousRecord_View_L34_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public ref FViewType Type
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Type;
            }
        }

        [UnscopedRef]
        public ref _Anonymous_e__Union._Buffer_e__Struct Buffer
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Buffer;
            }
        }

        [UnscopedRef]
        public ref _Anonymous_e__Union._UploadBuffer_e__Struct UploadBuffer
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.UploadBuffer;
            }
        }

        [UnscopedRef]
        public ref _Anonymous_e__Union._Image_e__Struct Image
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
            [NativeTypeName("Coplt::FViewType")]
            public FViewType Type;

            [FieldOffset(0)]
            [NativeTypeName("__AnonymousRecord_View_L38_C13")]
            public _Buffer_e__Struct Buffer;

            [FieldOffset(0)]
            [NativeTypeName("__AnonymousRecord_View_L47_C13")]
            public _UploadBuffer_e__Struct UploadBuffer;

            [FieldOffset(0)]
            [NativeTypeName("__AnonymousRecord_View_L55_C13")]
            public _Image_e__Struct Image;

            public partial struct _Buffer_e__Struct
            {
                [NativeTypeName("Coplt::FViewType")]
                public FViewType Type;

                [NativeTypeName("Coplt::FGraphicsFormat")]
                public FGraphicsFormat Format;

                [NativeTypeName("Coplt::u64")]
                public ulong Offset;

                [NativeTypeName("Coplt::u32")]
                public uint Size;

                [NativeTypeName("Coplt::u32")]
                public uint Stride;
            }

            public partial struct _UploadBuffer_e__Struct
            {
                [NativeTypeName("Coplt::FViewType")]
                public FViewType Type;

                [NativeTypeName("Coplt::u32")]
                public uint Size;

                [NativeTypeName("Coplt::u64")]
                public ulong Index;

                [NativeTypeName("Coplt::u64")]
                public ulong Offset;
            }

            public partial struct _Image_e__Struct
            {
                [NativeTypeName("Coplt::FViewType")]
                public FViewType Type;

                [NativeTypeName("Coplt::u8")]
                public byte Mip;

                [NativeTypeName("Coplt::u8")]
                public byte NumMips;

                [NativeTypeName("Coplt::u8")]
                public byte Plane;

                [NativeTypeName("Coplt::FGraphicsFormat")]
                public FGraphicsFormat Format;

                [NativeTypeName("__AnonymousRecord_View_L63_C17")]
                public _Anonymous1_e__Union Anonymous1;

                [NativeTypeName("__AnonymousRecord_View_L69_C17")]
                public _Anonymous2_e__Union Anonymous2;

                [UnscopedRef]
                public ref uint Index
                {
                    [MethodImpl(MethodImplOptions.AggressiveInlining)]
                    get
                    {
                        return ref Anonymous1.Index;
                    }
                }

                [UnscopedRef]
                public ref uint Z
                {
                    [MethodImpl(MethodImplOptions.AggressiveInlining)]
                    get
                    {
                        return ref Anonymous1.Z;
                    }
                }

                [UnscopedRef]
                public ref uint Size
                {
                    [MethodImpl(MethodImplOptions.AggressiveInlining)]
                    get
                    {
                        return ref Anonymous2.Size;
                    }
                }

                [UnscopedRef]
                public ref uint Depth
                {
                    [MethodImpl(MethodImplOptions.AggressiveInlining)]
                    get
                    {
                        return ref Anonymous2.Depth;
                    }
                }

                [StructLayout(LayoutKind.Explicit)]
                public partial struct _Anonymous1_e__Union
                {
                    [FieldOffset(0)]
                    [NativeTypeName("Coplt::u32")]
                    public uint Index;

                    [FieldOffset(0)]
                    [NativeTypeName("Coplt::u32")]
                    public uint Z;
                }

                [StructLayout(LayoutKind.Explicit)]
                public partial struct _Anonymous2_e__Union
                {
                    [FieldOffset(0)]
                    [NativeTypeName("Coplt::u32")]
                    public uint Size;

                    [FieldOffset(0)]
                    [NativeTypeName("Coplt::u32")]
                    public uint Depth;
                }
            }
        }
    }

    public unsafe partial struct FView
    {
        [NativeTypeName("Coplt::FGpuViewable *")]
        public FGpuViewable* Viewable;

        [NativeTypeName("Coplt::FViewData")]
        public FViewData Data;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FCpuAccess : byte
    {
        None = 0,
        Write = 1,
        Read = 2,
        ReadWrite = 3,
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

    public partial struct FGpuResourceCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FCpuAccess")]
        public FCpuAccess CpuAccess;

        [NativeTypeName("Coplt::FResourcePurpose")]
        public FResourcePurpose Purpose;
    }

    [NativeTypeName("Coplt::u8")]
    public enum ResourceType : byte
    {
        Unknown = 0,
        Buffer = 1,
        Image = 2,
    }

    public unsafe partial struct FGpuResourceData
    {
        public void* m_raw_resource_ptr;

        [NativeTypeName("Coplt::FResourcePurpose")]
        public FResourcePurpose m_purpose;

        [NativeTypeName("Coplt::FCpuAccess")]
        public FCpuAccess m_cpu_access;

        [NativeTypeName("Coplt::ResourceType")]
        public ResourceType m_resource_type;
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
        [return: NativeTypeName("Coplt::FGpuResourceData *")]
        public FGpuResourceData* GpuResourceData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuResource*, FGpuResourceData*>)(lpVtbl[6]))((FGpuResource*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuViewable.Interface
        {
            [return: NativeTypeName("Coplt::FGpuResourceData *")]
            FGpuResourceData* GpuResourceData();
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBufferUsage : byte
    {
        Undefined,
        Structured,
        Raw,
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

    public partial struct FOptimizedClearColor
    {
        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("__AnonymousRecord_Resource_L120_C9")]
        public _Anonymous_e__Union Anonymous;

        [UnscopedRef]
        public Span<float> Color
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return Anonymous.Color;
            }
        }

        [UnscopedRef]
        public ref float Depth
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Anonymous.Depth;
            }
        }

        [UnscopedRef]
        public ref byte Stencil
        {
            [MethodImpl(MethodImplOptions.AggressiveInlining)]
            get
            {
                return ref Anonymous.Anonymous.Stencil;
            }
        }

        [StructLayout(LayoutKind.Explicit)]
        public partial struct _Anonymous_e__Union
        {
            [FieldOffset(0)]
            [NativeTypeName("f32[4]")]
            public _Color_e__FixedBuffer Color;

            [FieldOffset(0)]
            [NativeTypeName("__AnonymousRecord_Resource_L124_C13")]
            public _Anonymous_e__Struct Anonymous;

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

    [NativeTypeName("struct FGpuImageCreateOptions : Coplt::FGpuResourceCreateOptions")]
    public partial struct FGpuImageCreateOptions
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

        [NativeTypeName("Coplt::u8")]
        public byte m_planes;

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

    public partial struct FGpuOutputData
    {
        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::u32")]
        public uint Width;

        [NativeTypeName("Coplt::u32")]
        public uint Height;

        [NativeTypeName("Coplt::b8")]
        public B8 Srgb;

        [NativeTypeName("Coplt::FHdrType")]
        public FHdrType Hdr;

        [NativeTypeName("Coplt::FPresentMode")]
        public FPresentMode PresentMode;
    }

    public partial struct FGpuOutput2CreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FPresentMode")]
        public FPresentMode PresentMode;

        [NativeTypeName("Coplt::b8")]
        public B8 Srgb;

        [NativeTypeName("Coplt::FHdrType")]
        public FHdrType Hdr;
    }

    [Guid("3F82BE7F-2CF5-48A9-8CA4-BB2F9CFE58B2")]
    [NativeTypeName("struct FGpuOutput2 : Coplt::FGpuExecutor")]
    public unsafe partial struct FGpuOutput2 : FGpuOutput2.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuOutput2));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuOutput2*, void>)(lpVtbl[0]))((FGpuOutput2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput2*, nuint>)(lpVtbl[1]))((FGpuOutput2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput2*, nuint>)(lpVtbl[2]))((FGpuOutput2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput2*, Guid*, void*>)(lpVtbl[3]))((FGpuOutput2*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput2*, ulong>)(lpVtbl[4]))((FGpuOutput2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput2*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuOutput2*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Wait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput2*, FResult*, FResult*>)(lpVtbl[6]))((FGpuOutput2*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FGpuOutputData *")]
        public FGpuOutputData* GpuOutputData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput2*, FGpuOutputData*>)(lpVtbl[7]))((FGpuOutput2*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput2*, FResult*, uint, uint, FResult*>)(lpVtbl[8]))((FGpuOutput2*)Unsafe.AsPointer(ref this), &result, Width, Height);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Present()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput2*, FResult*, FResult*>)(lpVtbl[9]))((FGpuOutput2*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult PresentNoWait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuOutput2*, FResult*, FResult*>)(lpVtbl[10]))((FGpuOutput2*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool IsAvailable()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuOutput2*, byte>)(lpVtbl[11]))((FGpuOutput2*)Unsafe.AsPointer(ref this)) != 0;
        }

        public interface Interface : FGpuExecutor.Interface
        {
            [return: NativeTypeName("const FGpuOutputData *")]
            FGpuOutputData* GpuOutputData();

            [return: NativeTypeName("Coplt::FResult")]
            FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height);

            [return: NativeTypeName("Coplt::FResult")]
            FResult Present();

            [return: NativeTypeName("Coplt::FResult")]
            FResult PresentNoWait();

            bool IsAvailable();
        }
    }

    [NativeTypeName("struct FGpuSwapChainData : Coplt::FGpuOutputData")]
    public partial struct FGpuSwapChainData
    {
        public FGpuOutputData Base;

        [NativeTypeName("Coplt::FOutputAlphaMode")]
        public FOutputAlphaMode AlphaMode;

        [NativeTypeName("Coplt::b8")]
        public B8 VSync;

        [NativeTypeName("Coplt::b8")]
        public B8 Srgb;

        [NativeTypeName("Coplt::FHdrType")]
        public FHdrType Hdr;
    }

    [NativeTypeName("struct FGpuSwapChainCreateOptions : Coplt::FGpuOutput2CreateOptions")]
    public partial struct FGpuSwapChainCreateOptions
    {
        public FGpuOutput2CreateOptions Base;

        [NativeTypeName("Coplt::u32")]
        public uint Width;

        [NativeTypeName("Coplt::u32")]
        public uint Height;

        [NativeTypeName("Coplt::FOutputAlphaMode")]
        public FOutputAlphaMode AlphaMode;

        [NativeTypeName("Coplt::b8")]
        public B8 VSync;
    }

    [NativeTypeName("struct FGpuSwapChainFromExistsCreateOptions : Coplt::FGpuOutput2CreateOptions")]
    public unsafe partial struct FGpuSwapChainFromExistsCreateOptions
    {
        public FGpuOutput2CreateOptions Base;

        public void* ExistsSwapChain;

        [NativeTypeName("Coplt::FOutputAlphaMode")]
        public FOutputAlphaMode AlphaMode;

        [NativeTypeName("Coplt::b8")]
        public B8 VSync;
    }

    [Guid("2BC677A1-77FC-4540-89E2-1FB7E3417DC2")]
    [NativeTypeName("struct FGpuSwapChain : Coplt::FGpuOutput2")]
    public unsafe partial struct FGpuSwapChain : FGpuSwapChain.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuSwapChain));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, void>)(lpVtbl[0]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, nuint>)(lpVtbl[1]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, nuint>)(lpVtbl[2]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, Guid*, void*>)(lpVtbl[3]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, ulong>)(lpVtbl[4]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Wait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FResult*, FResult*>)(lpVtbl[6]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FGpuOutputData *")]
        public FGpuOutputData* GpuOutputData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FGpuOutputData*>)(lpVtbl[7]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FResult*, uint, uint, FResult*>)(lpVtbl[8]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), &result, Width, Height);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Present()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FResult*, FResult*>)(lpVtbl[9]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult PresentNoWait()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FResult*, FResult*>)(lpVtbl[10]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool IsAvailable()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, byte>)(lpVtbl[11]))((FGpuSwapChain*)Unsafe.AsPointer(ref this)) != 0;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FGpuSwapChainData *")]
        public FGpuSwapChainData* GpuSwapChainData()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FGpuSwapChainData*>)(lpVtbl[12]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FGpuIsolate *")]
        public FGpuIsolate* GetIsolate()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FGpuIsolate*>)(lpVtbl[13]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSwapChain*, FResult*, B8, FResult*>)(lpVtbl[14]))((FGpuSwapChain*)Unsafe.AsPointer(ref this), &result, Enable);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawPtr()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSwapChain*, void*>)(lpVtbl[15]))((FGpuSwapChain*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuOutput2.Interface
        {
            [return: NativeTypeName("const FGpuSwapChainData *")]
            FGpuSwapChainData* GpuSwapChainData();

            [return: NativeTypeName("Coplt::FGpuIsolate *")]
            FGpuIsolate* GetIsolate();

            [return: NativeTypeName("Coplt::FResult")]
            FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable);

            void* GetRawPtr();
        }
    }

    public unsafe partial struct FGpuSwapChainCreateResult
    {
        [NativeTypeName("Coplt::FGpuSwapChain *")]
        public FGpuSwapChain* SwapChain;

        [NativeTypeName("Coplt::FGpuSwapChainData *")]
        public FGpuSwapChainData* Data;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FImagePlane : byte
    {
        All = 0,
        Depth = 0,
        Stencil = 1,
        Y = 0,
        CbCr = 1,
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
        AYUV = 100,
        Y410 = 101,
        Y416 = 102,
        NV12 = 103,
        P010 = 104,
        P016 = 105,
        Opaque_420 = 106,
        YUY2 = 107,
        Y210 = 108,
        Y216 = 109,
        NV11 = 110,
        AI44 = 111,
        IA44 = 112,
        P8 = 113,
        A8P8 = 114,
        B4G4R4A4_UNorm = 115,
        P208 = 130,
        V208 = 131,
        V408 = 132,
        A4B4G4R4_UNorm = 191,
        S8_UInt = 127,
        Etc2_R8G8B8_UNorm = 147,
        Etc2_R8G8B8_sRGB = 148,
        Etc2_R8G8B8A1_UNorm = 149,
        Etc2_R8G8B8A1_sRGB = 150,
        Etc2_R8G8B8A8_UNorm = 151,
        Etc2_R8G8B8A8_sRGB = 152,
        Eac_R11_UNorm = 153,
        Eac_R11_SNorm = 154,
        Eac_R11G11_UNorm = 155,
        Eac_R11G11_SNorm = 156,
        Astc_4x4_UNorm = 157,
        Astc_4x4_sRGB = 158,
        Astc_5x4_UNorm = 159,
        Astc_5x4_sRGB = 160,
        Astc_5x5_UNorm = 161,
        Astc_5x5_sRGB = 162,
        Astc_6x5_UNorm = 163,
        Astc_6x5_sRGB = 164,
        Astc_6x6_UNorm = 165,
        Astc_6x6_sRGB = 166,
        Astc_8x5_UNorm = 167,
        Astc_8x5_sRGB = 168,
        Astc_8x6_UNorm = 169,
        Astc_8x6_sRGB = 170,
        Astc_8x8_UNorm = 171,
        Astc_8x8_sRGB = 172,
        Astc_10x5_UNorm = 173,
        Astc_10x5_sRGB = 174,
        Astc_10x6_UNorm = 175,
        Astc_10x6_sRGB = 176,
        Astc_10x8_UNorm = 177,
        Astc_10x8_sRGB = 178,
        Astc_10x10_UNorm = 179,
        Astc_10x10_sRGB = 180,
        Astc_12x10_UNorm = 181,
        Astc_12x10_sRGB = 182,
        Astc_12x12_UNorm = 183,
        Astc_12x12_sRGB = 184,
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
        StaticSampler,
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

    public partial struct FShaderLayoutItem
    {
        [NativeTypeName("Coplt::u64")]
        public ulong Id;

        [NativeTypeName("Coplt::u64")]
        public ulong Scope;

        [NativeTypeName("Coplt::u32")]
        public uint Slot;

        [NativeTypeName("Coplt::u32")]
        public uint Space;

        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::FShaderStage")]
        public FShaderStage Stage;

        [NativeTypeName("Coplt::FShaderLayoutItemView")]
        public FShaderLayoutItemView View;

        [NativeTypeName("Coplt::FShaderLayoutItemType")]
        public FShaderLayoutItemType Type;

        [NativeTypeName("Coplt::FResourceAccess")]
        public FResourceAccess UavAccess;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FShaderLayoutFlags : byte
    {
        None = 0,
        DynBindLess = 1 << 0,
        InputAssembler = 1 << 1,
        StreamOutput = 1 << 2,
    }

    public unsafe partial struct FShaderLayoutCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FShaderLayoutItem *")]
        public FShaderLayoutItem* Items;

        [NativeTypeName("Coplt::u32")]
        public uint NumItems;

        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    public partial struct FGetEmptyShaderLayoutOptions
    {
        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    public unsafe partial struct FShaderLayoutData
    {
        [NativeTypeName("const FShaderLayoutItem *")]
        public FShaderLayoutItem* Items;

        [NativeTypeName("Coplt::u32")]
        public uint NumItems;

        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    public unsafe partial struct FShaderLayoutCreateResult
    {
        [NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* Layout;

        [NativeTypeName("Coplt::FShaderLayoutData *")]
        public FShaderLayoutData* Data;
    }

    public partial struct FBindGroupItem
    {
        [NativeTypeName("Coplt::u64")]
        public ulong Id;

        [NativeTypeName("Coplt::u64")]
        public ulong Scope;

        [NativeTypeName("Coplt::u32")]
        public uint Count;

        [NativeTypeName("Coplt::u32")]
        public uint StaticSamplerIndex;

        [NativeTypeName("Coplt::FGraphicsFormat")]
        public FGraphicsFormat Format;

        [NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags Stages;

        [NativeTypeName("Coplt::FShaderLayoutItemView")]
        public FShaderLayoutItemView View;

        [NativeTypeName("Coplt::FShaderLayoutItemType")]
        public FShaderLayoutItemType Type;

        [NativeTypeName("Coplt::FResourceAccess")]
        public FResourceAccess UavAccess;
    }

    [NativeTypeName("Coplt::u8")]
    public enum FBindGroupUsage : byte
    {
        Common = 0,
        Dynamic = 1,
        Freeze = 2,
    }

    public unsafe partial struct FBindGroupLayoutCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FBindGroupItem *")]
        public FBindGroupItem* Items;

        [NativeTypeName("Coplt::FStaticSamplerInfo *")]
        public FStaticSamplerInfo* StaticSamplers;

        [NativeTypeName("Coplt::u32")]
        public uint NumItems;

        [NativeTypeName("Coplt::u32")]
        public uint NumStaticSamplers;

        [NativeTypeName("Coplt::FBindGroupUsage")]
        public FBindGroupUsage Usage;
    }

    public unsafe partial struct FBindGroupLayoutData
    {
        [NativeTypeName("const FBindGroupItem *")]
        public FBindGroupItem* Items;

        [NativeTypeName("const FStaticSamplerInfo *")]
        public FStaticSamplerInfo* StaticSamplers;

        [NativeTypeName("Coplt::u32")]
        public uint NumItems;

        [NativeTypeName("Coplt::u32")]
        public uint NumStaticSamplers;

        [NativeTypeName("Coplt::u32")]
        public uint DynamicArrayIndex;

        [NativeTypeName("Coplt::FBindGroupUsage")]
        public FBindGroupUsage Usage;
    }

    [Guid("312C75EB-30F5-40B3-B79E-ACB5498CA9DC")]
    [NativeTypeName("struct FBindGroupLayout : Coplt::FGpuObject")]
    public unsafe partial struct FBindGroupLayout : FBindGroupLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FBindGroupLayout));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FBindGroupLayout*, void>)(lpVtbl[0]))((FBindGroupLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FBindGroupLayout*, nuint>)(lpVtbl[1]))((FBindGroupLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FBindGroupLayout*, nuint>)(lpVtbl[2]))((FBindGroupLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FBindGroupLayout*, Guid*, void*>)(lpVtbl[3]))((FBindGroupLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FBindGroupLayout*, ulong>)(lpVtbl[4]))((FBindGroupLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FBindGroupLayout*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FBindGroupLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FBindGroupLayoutData *")]
        public FBindGroupLayoutData* BindGroupLayoutData()
        {
            return ((delegate* unmanaged[Thiscall]<FBindGroupLayout*, FBindGroupLayoutData*>)(lpVtbl[6]))((FBindGroupLayout*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuObject.Interface
        {
            [return: NativeTypeName("Coplt::FBindGroupLayoutData *")]
            FBindGroupLayoutData* BindGroupLayoutData();
        }
    }

    public unsafe partial struct FBindGroupLayoutCreateResult
    {
        [NativeTypeName("Coplt::FBindGroupLayout *")]
        public FBindGroupLayout* Layout;

        [NativeTypeName("Coplt::FBindGroupLayoutData *")]
        public FBindGroupLayoutData* Data;
    }

    public unsafe partial struct FBindingLayoutCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* ShaderLayout;

        public FBindGroupLayout** Groups;

        [NativeTypeName("Coplt::u32")]
        public uint NumGroups;
    }

    public partial struct FGetEmptyBindingLayoutOptions
    {
        [NativeTypeName("Coplt::FShaderLayoutFlags")]
        public FShaderLayoutFlags Flags;
    }

    [Guid("BC0B662E-2918-4769-9D0C-1FEE25B32C5D")]
    [NativeTypeName("struct FBindingLayout : Coplt::FGpuObject")]
    public unsafe partial struct FBindingLayout : FBindingLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FBindingLayout));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FBindingLayout*, void>)(lpVtbl[0]))((FBindingLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FBindingLayout*, nuint>)(lpVtbl[1]))((FBindingLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FBindingLayout*, nuint>)(lpVtbl[2]))((FBindingLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FBindingLayout*, Guid*, void*>)(lpVtbl[3]))((FBindingLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FBindingLayout*, ulong>)(lpVtbl[4]))((FBindingLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FBindingLayout*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FBindingLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        public interface Interface : FGpuObject.Interface
        {
        }
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

        [NativeTypeName("Coplt::FBindingLayout *")]
        public FBindingLayout* Layout;
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
        [return: NativeTypeName("Coplt::FBindingLayout *")]
        public FBindingLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FShaderPipeline*, FBindingLayout*>)(lpVtbl[7]))((FShaderPipeline*)Unsafe.AsPointer(ref this));
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

            [return: NativeTypeName("Coplt::FBindingLayout *")]
            FBindingLayout* GetLayout();

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
        [return: NativeTypeName("Coplt::FBindingLayout *")]
        public FBindingLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FComputeShaderPipeline*, FBindingLayout*>)(lpVtbl[7]))((FComputeShaderPipeline*)Unsafe.AsPointer(ref this));
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
        [return: NativeTypeName("Coplt::FBindingLayout *")]
        public FBindingLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FGraphicsShaderPipeline*, FBindingLayout*>)(lpVtbl[7]))((FGraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
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

    [NativeTypeName("Coplt::u8")]
    public enum FFilter : byte
    {
        Point,
        Linear,
    }

    [NativeTypeName("Coplt::u8")]
    public enum FAddressMode : byte
    {
        Repeat,
        Mirror,
        Clamp,
        Border,
        MirrorOnce,
    }

    public partial struct FSamplerInfo
    {
        [NativeTypeName("f32[4]")]
        public _BorderColor_e__FixedBuffer BorderColor;

        [NativeTypeName("Coplt::u32")]
        public uint MaxAnisotropy;

        [NativeTypeName("Coplt::f32")]
        public float MipLodBias;

        [NativeTypeName("Coplt::f32")]
        public float MinLod;

        [NativeTypeName("Coplt::f32")]
        public float MaxLod;

        [NativeTypeName("Coplt::FCmpFunc")]
        public FCmpFunc Cmp;

        [NativeTypeName("Coplt::FFilter")]
        public FFilter Mag;

        [NativeTypeName("Coplt::FFilter")]
        public FFilter Min;

        [NativeTypeName("Coplt::FFilter")]
        public FFilter Mipmap;

        [NativeTypeName("Coplt::FAddressMode")]
        public FAddressMode U;

        [NativeTypeName("Coplt::FAddressMode")]
        public FAddressMode V;

        [NativeTypeName("Coplt::FAddressMode")]
        public FAddressMode W;

        [InlineArray(4)]
        public partial struct _BorderColor_e__FixedBuffer
        {
            public float e0;
        }
    }

    public partial struct FGpuSamplerCreateOptions
    {
        [NativeTypeName("Coplt::FStr8or16")]
        public FStr8or16 Name;

        [NativeTypeName("Coplt::FSamplerInfo")]
        public FSamplerInfo Info;
    }

    [Guid("16A5B373-AD9E-4033-89FD-6A5B4AABAEF2")]
    [NativeTypeName("struct FGpuSampler : Coplt::FGpuViewable")]
    public unsafe partial struct FGpuSampler : FGpuSampler.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FGpuSampler));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FGpuSampler*, void>)(lpVtbl[0]))((FGpuSampler*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSampler*, nuint>)(lpVtbl[1]))((FGpuSampler*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSampler*, nuint>)(lpVtbl[2]))((FGpuSampler*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSampler*, Guid*, void*>)(lpVtbl[3]))((FGpuSampler*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::u64")]
        public ulong ObjectId()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSampler*, ulong>)(lpVtbl[4]))((FGpuSampler*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const FStr8or16 &")] FStr8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FGpuSampler*, FResult*, FStr8or16*, FResult*>)(lpVtbl[5]))((FGpuSampler*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FSamplerInfo *")]
        public FSamplerInfo* Info()
        {
            return ((delegate* unmanaged[Thiscall]<FGpuSampler*, FSamplerInfo*>)(lpVtbl[6]))((FGpuSampler*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGpuViewable.Interface
        {
            [return: NativeTypeName("const FSamplerInfo *")]
            FSamplerInfo* Info();
        }
    }

    [NativeTypeName("Coplt::u8")]
    public enum FStaticSamplerBorderColor : byte
    {
        Transparent,
        Black,
        White,
        BlackUInt,
        WhiteUInt,
    }

    public partial struct FStaticSamplerInfo
    {
        [NativeTypeName("Coplt::u32")]
        public uint MaxAnisotropy;

        [NativeTypeName("Coplt::f32")]
        public float MipLodBias;

        [NativeTypeName("Coplt::f32")]
        public float MinLod;

        [NativeTypeName("Coplt::f32")]
        public float MaxLod;

        [NativeTypeName("Coplt::FCmpFunc")]
        public FCmpFunc Cmp;

        [NativeTypeName("Coplt::FFilter")]
        public FFilter Mag;

        [NativeTypeName("Coplt::FFilter")]
        public FFilter Min;

        [NativeTypeName("Coplt::FFilter")]
        public FFilter Mipmap;

        [NativeTypeName("Coplt::FAddressMode")]
        public FAddressMode U;

        [NativeTypeName("Coplt::FAddressMode")]
        public FAddressMode V;

        [NativeTypeName("Coplt::FAddressMode")]
        public FAddressMode W;

        [NativeTypeName("Coplt::FStaticSamplerBorderColor")]
        public FStaticSamplerBorderColor BorderColor;
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

        public static readonly Guid IID_FGpuIsolate = new Guid(0x777C5774, 0x8EB8, 0x4550, 0xA9, 0x77, 0x62, 0xCC, 0xCD, 0x7B, 0xDD, 0xA6);

        public static readonly Guid IID_FGpuRecord = new Guid(0x0EF83584, 0xCA65, 0x44DE, 0xB3, 0x8A, 0x64, 0x8B, 0xFB, 0x3A, 0x85, 0xA6);

        public static readonly Guid IID_FShaderModule = new Guid(0x5C0E1FDB, 0x2ACD, 0x4FCE, 0xB9, 0x85, 0x09, 0xE1, 0x2A, 0x7A, 0x1A, 0xAD);

        public static readonly Guid IID_FShader = new Guid(0xDE1352D5, 0x023D, 0x42B0, 0xBE, 0xAC, 0x12, 0x2B, 0x3B, 0x29, 0x6C, 0x9C);

        public static readonly Guid IID_FShaderBindGroup = new Guid(0xAE54EFE5, 0xC372, 0x4291, 0xB9, 0x95, 0x55, 0x29, 0x87, 0x58, 0xD2, 0xB2);

        public static readonly Guid IID_FShaderBinding = new Guid(0xA3CA644A, 0x0E02, 0x4D25, 0x9A, 0x18, 0x88, 0x35, 0xD6, 0x66, 0x00, 0xF7);

        public static readonly Guid IID_FRecordContext = new Guid(0x15B4821D, 0x0648, 0x4B78, 0x9C, 0x21, 0xC6, 0xA5, 0xB6, 0xA5, 0xED, 0x75);

        public static readonly Guid IID_FGpuQueue2 = new Guid(0xFC94E4D0, 0x5F01, 0x4D4F, 0x89, 0x4C, 0x6A, 0xEF, 0xAA, 0xD9, 0x04, 0x99);

        public static readonly Guid IID_FGpuExecutor = new Guid(0xEDAB0989, 0xE0C2, 0x4B1E, 0x82, 0x6F, 0xC7, 0x33, 0x80, 0x08, 0x8F, 0xC6);

        public static readonly Guid IID_FGpuViewable = new Guid(0xB3AEB8A5, 0x1FA6, 0x4866, 0x97, 0xEF, 0x1A, 0x5F, 0xA4, 0x01, 0xE1, 0x8F);

        public static readonly Guid IID_FGpuResource = new Guid(0xF99DCEEC, 0x2F0C, 0x4A28, 0xB6, 0x66, 0xBE, 0xB7, 0xC3, 0x52, 0x19, 0xD6);

        public static readonly Guid IID_FGpuBuffer = new Guid(0x283740E3, 0xFE96, 0x41D0, 0x83, 0x0A, 0x0A, 0x4C, 0x6A, 0x72, 0x53, 0x36);

        public static readonly Guid IID_FGpuImage = new Guid(0x667EFA36, 0x21C7, 0x4561, 0xAB, 0xAD, 0x85, 0x78, 0x0F, 0xA4, 0x92, 0x9E);

        public static readonly Guid IID_FGpuOutput2 = new Guid(0x3F82BE7F, 0x2CF5, 0x48A9, 0x8C, 0xA4, 0xBB, 0x2F, 0x9C, 0xFE, 0x58, 0xB2);

        public static readonly Guid IID_FGpuSwapChain = new Guid(0x2BC677A1, 0x77FC, 0x4540, 0x89, 0xE2, 0x1F, 0xB7, 0xE3, 0x41, 0x7D, 0xC2);

        public static readonly Guid IID_FShaderLayout = new Guid(0x552A498E, 0x8F3A, 0x47FF, 0xA3, 0x35, 0x7A, 0xF2, 0xDE, 0x09, 0x01, 0xE8);

        public static readonly Guid IID_FBindGroupLayout = new Guid(0x312C75EB, 0x30F5, 0x40B3, 0xB7, 0x9E, 0xAC, 0xB5, 0x49, 0x8C, 0xA9, 0xDC);

        public static readonly Guid IID_FBindingLayout = new Guid(0xBC0B662E, 0x2918, 0x4769, 0x9D, 0x0C, 0x1F, 0xEE, 0x25, 0xB3, 0x2C, 0x5D);

        public static readonly Guid IID_FShaderInputLayout = new Guid(0x70229C9A, 0xFB3D, 0x46B4, 0xB5, 0x34, 0x72, 0xFD, 0xB1, 0x67, 0xD8, 0x07);

        public static readonly Guid IID_FMeshLayout = new Guid(0x8FE5121F, 0xC2CE, 0x46F5, 0xAA, 0x14, 0xF2, 0x85, 0x95, 0xF3, 0x53, 0x61);

        public static readonly Guid IID_FShaderPipeline = new Guid(0x356A2610, 0x34E3, 0x4C01, 0x99, 0x04, 0x22, 0xE3, 0xC5, 0xCE, 0x2F, 0x4E);

        public static readonly Guid IID_FComputeShaderPipeline = new Guid(0x5241C089, 0x2EE2, 0x43EE, 0xAD, 0xD4, 0x0A, 0x10, 0xC0, 0x4A, 0x56, 0xCE);

        public static readonly Guid IID_FGraphicsShaderPipeline = new Guid(0x32A67D44, 0x132C, 0x449B, 0x97, 0x2D, 0xBA, 0xD3, 0x41, 0x37, 0x83, 0xE5);

        public static readonly Guid IID_FGpuSampler = new Guid(0x16A5B373, 0xAD9E, 0x4033, 0x89, 0xFD, 0x6A, 0x5B, 0x4A, 0xAB, 0xAE, 0xF2);
    }
}
