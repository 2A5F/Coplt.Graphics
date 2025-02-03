using System;
using System.Runtime.CompilerServices;

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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, nuint>)(lpVtbl[3]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, nuint>)(lpVtbl[4]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, B8>)(lpVtbl[5]))((FRcObject*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FRcObject*, B8>)(lpVtbl[6]))((FRcObject*)Unsafe.AsPointer(ref this));
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, nuint>)(lpVtbl[3]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, nuint>)(lpVtbl[4]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, B8>)(lpVtbl[5]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, B8>)(lpVtbl[6]))((FUnknown*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<FUnknown*, Guid, void*>)(lpVtbl[7]))((FUnknown*)Unsafe.AsPointer(ref this), id);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, nuint>)(lpVtbl[3]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, nuint>)(lpVtbl[4]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, B8>)(lpVtbl[5]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, B8>)(lpVtbl[6]))((META_FString8*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FString8*, Guid, void*>)(lpVtbl[7]))((META_FString8*)Unsafe.AsPointer(ref this), id);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, nuint>)(lpVtbl[3]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, nuint>)(lpVtbl[4]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, B8>)(lpVtbl[5]))((FString8*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, B8>)(lpVtbl[6]))((FString8*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<FString8*, Guid, void*>)(lpVtbl[7]))((FString8*)Unsafe.AsPointer(ref this), id);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, nuint>)(lpVtbl[3]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, nuint>)(lpVtbl[4]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, B8>)(lpVtbl[5]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, B8>)(lpVtbl[6]))((META_FString16*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FString16*, Guid, void*>)(lpVtbl[7]))((META_FString16*)Unsafe.AsPointer(ref this), id);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, nuint>)(lpVtbl[3]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, nuint>)(lpVtbl[4]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, B8>)(lpVtbl[5]))((FString16*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, B8>)(lpVtbl[6]))((FString16*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<FString16*, Guid, void*>)(lpVtbl[7]))((FString16*)Unsafe.AsPointer(ref this), id);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, nuint>)(lpVtbl[3]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, nuint>)(lpVtbl[4]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, B8>)(lpVtbl[5]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, B8>)(lpVtbl[6]))((META_FInstance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<META_FInstance*, Guid, void*>)(lpVtbl[7]))((META_FInstance*)Unsafe.AsPointer(ref this), id);
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

        [return: NativeTypeName("size_t")]
        public nuint AddRefWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, nuint>)(lpVtbl[3]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("size_t")]
        public nuint ReleaseWeak()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, nuint>)(lpVtbl[4]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryDowngrade()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, B8>)(lpVtbl[5]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        [return: NativeTypeName("Coplt::b8")]
        public B8 TryUpgrade()
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, B8>)(lpVtbl[6]))((FInstance*)Unsafe.AsPointer(ref this));
        }

        public void* QueryInterface([NativeTypeName("Coplt::Guid")] Guid id)
        {
            return ((delegate* unmanaged[Thiscall]<FInstance*, Guid, void*>)(lpVtbl[7]))((FInstance*)Unsafe.AsPointer(ref this), id);
        }
    }
}
