using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using static Coplt.Graphics.Native.D3d12.D3d12Native;

namespace Coplt.Graphics.Native.D3d12
{
    [Guid("2F979FA9-5CB0-4396-BB59-16094B0F9B1A")]
    [NativeTypeName("struct FD3d12Instance : Coplt::FInstance")]
    public unsafe partial struct FD3d12Instance : INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12Instance));

        public FInstance Base;
    }

    [Guid("DB7CE2BC-155A-4FCC-9F7F-137694191CA7")]
    [NativeTypeName("struct FD3d12GpuDevice : Coplt::FGpuDevice")]
    public unsafe partial struct FD3d12GpuDevice : FD3d12GpuDevice.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12GpuDevice));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, void>)(lpVtbl[0]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, nuint>)(lpVtbl[1]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, nuint>)(lpVtbl[2]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, Guid*, void*>)(lpVtbl[3]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRawDevice()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, void*>)(lpVtbl[5]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMainQueue([NativeTypeName("const FMainQueueCreateOptions &")] FMainQueueCreateOptions* options, FGpuQueue** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FMainQueueCreateOptions*, FGpuQueue**, FResult*>)(lpVtbl[6]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderModule([NativeTypeName("const FShaderModuleCreateOptions &")] FShaderModuleCreateOptions* options, FShaderModule** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FShaderModuleCreateOptions*, FShaderModule**, FResult*>)(lpVtbl[7]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderLayout([NativeTypeName("const FShaderLayoutCreateOptions &")] FShaderLayoutCreateOptions* options, FShaderLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FShaderLayoutCreateOptions*, FShaderLayout**, FResult*>)(lpVtbl[8]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShaderInputLayout([NativeTypeName("const FShaderInputLayoutCreateOptions &")] FShaderInputLayoutCreateOptions* options, FShaderInputLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FShaderInputLayoutCreateOptions*, FShaderInputLayout**, FResult*>)(lpVtbl[9]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateShader([NativeTypeName("const FShaderCreateOptions &")] FShaderCreateOptions* options, FShader** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FShaderCreateOptions*, FShader**, FResult*>)(lpVtbl[10]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateMeshLayout([NativeTypeName("const FMeshLayoutCreateOptions &")] FMeshLayoutCreateOptions* options, FMeshLayout** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FMeshLayoutCreateOptions*, FMeshLayout**, FResult*>)(lpVtbl[11]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult CreateGraphicsPipeline([NativeTypeName("const FGraphicsShaderPipelineCreateOptions &")] FGraphicsShaderPipelineCreateOptions* options, FGraphicsShaderPipeline** @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuDevice*, FResult*, FGraphicsShaderPipelineCreateOptions*, FGraphicsShaderPipeline**, FResult*>)(lpVtbl[12]))((FD3d12GpuDevice*)Unsafe.AsPointer(ref this), &result, options, @out);
        }

        public interface Interface : FGpuDevice.Interface
        {
        }
    }

    [Guid("11A59DB3-49D8-4158-A276-5D80FD37D85F")]
    [NativeTypeName("struct FD3d12GpuQueue : Coplt::FGpuQueue")]
    public unsafe partial struct FD3d12GpuQueue : INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12GpuQueue));

        public FGpuQueue Base;
    }

    [Guid("8D068A07-118C-4D1B-9AB0-384576B61897")]
    [NativeTypeName("struct FD3d12GpuOutput : Coplt::FGpuOutput")]
    public unsafe partial struct FD3d12GpuOutput : FD3d12GpuOutput.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12GpuOutput));

        public void** lpVtbl;

        public FGpuOutput Base;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, void>)(lpVtbl[0]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, nuint>)(lpVtbl[1]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, nuint>)(lpVtbl[2]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, Guid*, void*>)(lpVtbl[3]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetVSync([NativeTypeName("Coplt::b8")] B8 Enable)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, B8, FResult*>)(lpVtbl[5]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, Enable);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Resize([NativeTypeName("Coplt::u32")] uint Width, [NativeTypeName("Coplt::u32")] uint Height)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, uint, uint, FResult*>)(lpVtbl[6]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, Width, Height);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult Present([NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, FCommandSubmit*, FResult*>)(lpVtbl[7]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, submit);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult PresentNoWait([NativeTypeName("const FCommandSubmit *")] FCommandSubmit* submit)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, FCommandSubmit*, FResult*>)(lpVtbl[8]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, submit);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult WaitNextFrame()
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, FResult*>)(lpVtbl[9]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GetCurrentResourcePtr(void* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, void*, FResult*>)(lpVtbl[10]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, @out);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult GetCurrentRtv(void* @out)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GpuOutput*, FResult*, void*, FResult*>)(lpVtbl[11]))((FD3d12GpuOutput*)Unsafe.AsPointer(ref this), &result, @out);
        }

        public interface Interface : FGpuOutput.Interface
        {
            [return: NativeTypeName("Coplt::FResult")]
            FResult GetCurrentResourcePtr(void* @out);

            [return: NativeTypeName("Coplt::FResult")]
            FResult GetCurrentRtv(void* @out);
        }
    }

    [Guid("5EE90A8D-8C3D-48AD-915E-87F3D28DABE7")]
    [NativeTypeName("struct FD3d12GpuSwapChainOutput : Coplt::FD3d12GpuOutput")]
    public unsafe partial struct FD3d12GpuSwapChainOutput : INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12GpuSwapChainOutput));

        public FD3d12GpuOutput Base;
    }

    [Guid("384259CF-C392-4903-8D2C-EC959C287912")]
    [NativeTypeName("struct FD3d12ShaderLayout : Coplt::FShaderLayout")]
    public unsafe partial struct FD3d12ShaderLayout : FD3d12ShaderLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12ShaderLayout));

        public void** lpVtbl;

        public FShaderLayout Base;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, void>)(lpVtbl[0]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, nuint>)(lpVtbl[1]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, nuint>)(lpVtbl[2]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, Guid*, void*>)(lpVtbl[3]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderLayoutItemDefine *")]
        public FShaderLayoutItemDefine* GetItemDefines([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, uint*, FShaderLayoutItemDefine*>)(lpVtbl[5]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetRootSignaturePtr()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderLayout*, void*>)(lpVtbl[6]))((FD3d12ShaderLayout*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FShaderLayout.Interface
        {
            void* GetRootSignaturePtr();
        }
    }

    [Guid("3DFC8DE2-BCA2-48AE-B869-850CB11B3EE9")]
    [NativeTypeName("struct FD3d12ShaderInputLayout : Coplt::FShaderInputLayout")]
    public unsafe partial struct FD3d12ShaderInputLayout : FD3d12ShaderInputLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12ShaderInputLayout));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12ShaderInputLayout*, void>)(lpVtbl[0]))((FD3d12ShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderInputLayout*, nuint>)(lpVtbl[1]))((FD3d12ShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderInputLayout*, nuint>)(lpVtbl[2]))((FD3d12ShaderInputLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderInputLayout*, Guid*, void*>)(lpVtbl[3]))((FD3d12ShaderInputLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12ShaderInputLayout*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12ShaderInputLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FShaderInputLayoutElement *")]
        public FShaderInputLayoutElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12ShaderInputLayout*, uint*, FShaderInputLayoutElement*>)(lpVtbl[5]))((FD3d12ShaderInputLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        public interface Interface : FShaderInputLayout.Interface
        {
        }
    }

    [Guid("2C6E52E9-6FA1-457A-8F99-A734D63B62C2")]
    [NativeTypeName("struct FD3d12MeshLayout : Coplt::FMeshLayout")]
    public unsafe partial struct FD3d12MeshLayout : FD3d12MeshLayout.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12MeshLayout));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, void>)(lpVtbl[0]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, nuint>)(lpVtbl[1]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, nuint>)(lpVtbl[2]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, Guid*, void*>)(lpVtbl[3]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferDefine *")]
        public FMeshBufferDefine* GetBuffers([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, uint*, FMeshBufferDefine*>)(lpVtbl[5]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferElement *")]
        public FMeshBufferElement* GetElements([NativeTypeName("Coplt::u32 *")] uint* out_count)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, uint*, FMeshBufferElement*>)(lpVtbl[6]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this), out_count);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FMeshBufferElement *")]
        public FMeshBufferElement* TryGetElement([NativeTypeName("Coplt::u32")] uint SlotId, [NativeTypeName("Coplt::u32")] uint SlotIndex)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12MeshLayout*, uint, uint, FMeshBufferElement*>)(lpVtbl[7]))((FD3d12MeshLayout*)Unsafe.AsPointer(ref this), SlotId, SlotIndex);
        }

        public interface Interface : FMeshLayout.Interface
        {
        }
    }

    [Guid("6D9C7DB7-3261-4D79-BD21-1F1BB5E01E4D")]
    [NativeTypeName("struct FD3d12PipelineState : Coplt::FShaderPipeline")]
    public unsafe partial struct FD3d12PipelineState : FD3d12PipelineState.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12PipelineState));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, void>)(lpVtbl[0]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, nuint>)(lpVtbl[1]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, nuint>)(lpVtbl[2]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, Guid*, void*>)(lpVtbl[3]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, FShader*>)(lpVtbl[5]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, FShaderLayout*>)(lpVtbl[6]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, FShaderStageFlags>)(lpVtbl[7]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* GetPipelineStatePtr()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12PipelineState*, void*>)(lpVtbl[8]))((FD3d12PipelineState*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FShaderPipeline.Interface
        {
            void* GetPipelineStatePtr();
        }
    }

    [Guid("A1C6B8A9-1E4C-4E6C-85EA-2E64AC89AFC6")]
    [NativeTypeName("struct FD3d12GraphicsShaderPipeline : Coplt::FGraphicsShaderPipeline")]
    public unsafe partial struct FD3d12GraphicsShaderPipeline : FD3d12GraphicsShaderPipeline.Interface, INativeGuid
    {
        static Guid* INativeGuid.NativeGuid => (Guid*)Unsafe.AsPointer(ref Unsafe.AsRef(in IID_FD3d12GraphicsShaderPipeline));

        public void** lpVtbl;

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Dispose()
        {
            ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, void>)(lpVtbl[0]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint Release()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, nuint>)(lpVtbl[1]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("size_t")]
        public nuint AddRef()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, nuint>)(lpVtbl[2]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void* QueryInterface([NativeTypeName("const Guid &")] Guid* id)
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, Guid*, void*>)(lpVtbl[3]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this), id);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FResult")]
        public FResult SetName([NativeTypeName("const Str8or16 &")] Str8or16* name)
        {
            FResult result;
            return *((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, FResult*, Str8or16*, FResult*>)(lpVtbl[4]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this), &result, name);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShader *")]
        public FShader* GetShader()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, FShader*>)(lpVtbl[5]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderLayout *")]
        public FShaderLayout* GetLayout()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, FShaderLayout*>)(lpVtbl[6]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("Coplt::FShaderStageFlags")]
        public FShaderStageFlags GetStages()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, FShaderStageFlags>)(lpVtbl[7]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        [return: NativeTypeName("const FGraphicsPipelineState *")]
        public FGraphicsPipelineState* GetGraphicsState()
        {
            return ((delegate* unmanaged[Thiscall]<FD3d12GraphicsShaderPipeline*, FGraphicsPipelineState*>)(lpVtbl[8]))((FD3d12GraphicsShaderPipeline*)Unsafe.AsPointer(ref this));
        }

        public interface Interface : FGraphicsShaderPipeline.Interface
        {
        }
    }

    public static unsafe partial class D3d12Native
    {
        [DllImport("Coplt.Graphics.Native.D3d12", CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        [return: NativeTypeName("Coplt::FInstance *")]
        public static extern FInstance* Coplt_Graphics_D3d12_Create_Instance();

        public static readonly Guid IID_FD3d12Instance = new Guid(0x2F979FA9, 0x5CB0, 0x4396, 0xBB, 0x59, 0x16, 0x09, 0x4B, 0x0F, 0x9B, 0x1A);

        public static readonly Guid IID_FD3d12GpuDevice = new Guid(0xDB7CE2BC, 0x155A, 0x4FCC, 0x9F, 0x7F, 0x13, 0x76, 0x94, 0x19, 0x1C, 0xA7);

        public static readonly Guid IID_FD3d12GpuQueue = new Guid(0x11A59DB3, 0x49D8, 0x4158, 0xA2, 0x76, 0x5D, 0x80, 0xFD, 0x37, 0xD8, 0x5F);

        public static readonly Guid IID_FD3d12GpuOutput = new Guid(0x8D068A07, 0x118C, 0x4D1B, 0x9A, 0xB0, 0x38, 0x45, 0x76, 0xB6, 0x18, 0x97);

        public static readonly Guid IID_FD3d12GpuSwapChainOutput = new Guid(0x5EE90A8D, 0x8C3D, 0x48AD, 0x91, 0x5E, 0x87, 0xF3, 0xD2, 0x8D, 0xAB, 0xE7);

        public static readonly Guid IID_FD3d12ShaderLayout = new Guid(0x384259CF, 0xC392, 0x4903, 0x8D, 0x2C, 0xEC, 0x95, 0x9C, 0x28, 0x79, 0x12);

        public static readonly Guid IID_FD3d12ShaderInputLayout = new Guid(0x3DFC8DE2, 0xBCA2, 0x48AE, 0xB8, 0x69, 0x85, 0x0C, 0xB1, 0x1B, 0x3E, 0xE9);

        public static readonly Guid IID_FD3d12MeshLayout = new Guid(0x2C6E52E9, 0x6FA1, 0x457A, 0x8F, 0x99, 0xA7, 0x34, 0xD6, 0x3B, 0x62, 0xC2);

        public static readonly Guid IID_FD3d12PipelineState = new Guid(0x6D9C7DB7, 0x3261, 0x4D79, 0xBD, 0x21, 0x1F, 0x1B, 0xB5, 0xE0, 0x1E, 0x4D);

        public static readonly Guid IID_FD3d12GraphicsShaderPipeline = new Guid(0xA1C6B8A9, 0x1E4C, 0x4E6C, 0x85, 0xEA, 0x2E, 0x64, 0xAC, 0x89, 0xAF, 0xC6);
    }
}
