using System.Buffers;
using System.Collections.Concurrent;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Text;
using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.States;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuDevice : GpuObject
{
    #region Fields

    internal readonly GraphicsInstance m_instance;
    internal readonly GpuAdapter m_adapter;
    [Drop]
    internal readonly GpuIsolate? m_main_isolate;
    [Drop]
    internal readonly GpuIsolate? m_copy_isolate;

    #endregion

    #region Props

    public new FGpuDevice* Ptr => (FGpuDevice*)m_ptr;
    public GraphicsInstance Instance => m_instance;
    public GpuAdapter Adapter => m_adapter;
    public GpuIsolate MainIsolate => m_main_isolate!;
    public GpuIsolate? CopyIsolate => m_copy_isolate;

    #endregion

    #region Ctor

    internal GpuDevice(FGpuDeviceCreateResult result, GraphicsInstance instance, GpuAdapter? adapter, string? name) : base((FGpuObject*)result.Device, name)
    {
        m_instance = instance;
        m_name = name;
        m_adapter = adapter ?? m_instance.m_ptr_to_adapters[(nuint)Ptr->GetAdapter()];
        m_main_isolate = new GpuIsolate(result.MainIsolate.Isolate, result.MainIsolate.Data, null, this);
        if (result.CopyIsolate.Isolate != null)
        {
            m_copy_isolate = new GpuIsolate(result.CopyIsolate.Isolate, result.CopyIsolate.Data, null, this);
        }
    }

    #endregion

    #region GetRawDevice

    /// <returns>Dx12 返回 ID3D12Device*</returns>
    public void* GetRawDevice() => Ptr->GetRawDevice();

    #endregion

    #region Blob

    public Blob CreateBlob(nuint size) => m_instance.CreateBlob(size);

    #endregion

    #region CreateIsolate

    public GpuIsolate CreateIsolate(string? Name = null, ReadOnlySpan<byte> Name8 = default)
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuIsolateCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
            };
            FGpuIsolateCreateResult f_result;
            Ptr->CreateIsolate(&f_options, &f_result).TryThrow();
            return new GpuIsolate(f_result.Isolate, f_result.Data, null, this);
        }
    }

    #endregion

    #region CreateShaderModule

    public ShaderModule CreateShaderModule(
        ShaderStage Stage, ReadOnlySpan<byte> Blob,
        String8? EntryPoint = null,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        fixed (byte* data = Blob)
        {
            FShaderModuleCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
                EntryPoint = EntryPoint == null ? null : EntryPoint.m_ptr,
                Data = data,
                Size = (nuint)Blob.Length,
                Stage = Stage.ToFFI(),
            };
            FShaderModuleCreateResult result;
            Ptr->CreateShaderModule(&f_options, &result).TryThrow();
            return new(result, EntryPoint, Name, this);
        }
    }

    #endregion

    #region CreateShaderLayout

    public ShaderLayout CreateShaderLayout(
        ReadOnlySpan<ShaderLayoutItemDefine> Defines,
        ReadOnlySpan<byte> Samplers = default, // todo
        ShaderLayoutFlags Flags = ShaderLayoutFlags.None,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            fixed (ShaderLayoutItemDefine* p_define = Defines)
            {
                FShaderLayoutCreateOptions f_options = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    Count = (uint)Defines.Length,
                    Items = (FShaderLayoutItemDefine*)p_define,
                    Flags = (FShaderLayoutFlags)Flags,
                };
                FShaderLayoutCreateResult result;
                Ptr->CreateShaderLayout(&f_options, &result).TryThrow();
                return new(result, Name, this);
            }
        }
    }

    private readonly ConcurrentDictionary<ShaderLayoutFlags, ShaderLayout> m_empty_shader_layouts = new();

    public ShaderLayout GetEmptyShaderLayout(ShaderLayoutFlags Flags = ShaderLayoutFlags.None) =>
        m_empty_shader_layouts.GetOrAdd(
            Flags, static (flags, self) =>
            {
                FGetEmptyShaderLayoutOptions f_options = new()
                {
                    Flags = (FShaderLayoutFlags)flags,
                };
                FShaderLayoutCreateResult result;
                self.Ptr->GetEmptyShaderLayout(&f_options, &result).TryThrow();
                return new(result, $"Empty Shader Layout ({flags})", self);
            }, this
        );

    #endregion

    #region CreateShaderInputLayout

    public ShaderInputLayout CreateShaderInputLayout(
        ReadOnlySpan<ShaderInputLayoutElement> Elements,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        var meta = Elements.Length == 0
            ? Array.Empty<ShaderInputLayoutElementMeta>()
            : new ShaderInputLayoutElementMeta[Elements.Length];
        var p_elements = stackalloc FShaderInputLayoutElement[Elements.Length];
        for (var i = 0; i < Elements.Length; i++)
        {
            ref readonly var item = ref Elements[i];
            var slot_name8 =
                (item.SlotName8 ?? (item.SlotName == null ? null : m_instance.CacheString8(item.SlotName))) ??
                (item.SlotId.HasValue ? m_instance.TryGetSlotName8(item.SlotId.Value) : null);
            var slot_name = item.SlotName8?.ToString() ?? item.SlotName;
            var slot_id = item.SlotId ?? (string.IsNullOrWhiteSpace(slot_name)
                ? throw new ArgumentException("Slot id is not provided and Slot Name is null/empty")
                : m_instance.GetSlotId(slot_name));
            var location = item.Location == uint.MaxValue ? (uint)i : item.Location;
            meta[i] = new()
            {
                SlotName8 = slot_name8,
                SlotId = slot_id.Value,
                SlotIndex = item.SlotIndex,
                Location = location,
            };
            p_elements[i] = new()
            {
                SlotName = slot_name8 == null ? null : slot_name8.m_ptr,
                SlotId = slot_id.Value,
                SlotIndex = item.SlotIndex,
                Location = location,
            };
        }
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FShaderInputLayoutCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
                Element = p_elements,
                Count = (uint)Elements.Length,
            };
            FShaderInputLayout* ptr;
            Ptr->CreateShaderInputLayout(&f_options, &ptr).TryThrow();
            return new(ptr, meta, Name, this);
        }
    }

    private ShaderInputLayout? m_empty_shader_input_layout;
    public ShaderInputLayout EmptyShaderInputLayout =>
        m_empty_shader_input_layout ??= CreateShaderInputLayout([], "Empty Shader Input Layout");

    #endregion

    #region CreateShader

    public Shader CreateShader(
        ReadOnlySpan<ShaderModule> Modules, ShaderLayout? Layout, ShaderInputLayout? InputLayout = null,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        if (Modules.Length == 0) throw new ArgumentException("No modules provided");
        if (Modules.Length > FShader.MaxShaderModuleCount)
            throw new ArgumentException(
                $"Too many shader modules, there can be a maximum of {FShader.MaxShaderModuleCount}"
            );
        Shader.ShaderModuleArray arr = new();
        var p_modules = stackalloc FShaderModule*[Modules.Length];
        for (var i = 0; i < Modules.Length; i++)
        {
            var module = Modules[i];
            arr[i] = module;
            p_modules[i] = module.Ptr;
        }
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FShaderCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
                Layout = Layout == null ? null : Layout.Ptr,
                InputLayout = InputLayout == null ? null : InputLayout.Ptr,
                Modules = p_modules,
                Count = (byte)Modules.Length,
            };
            FShaderCreateResult result;
            Ptr->CreateShader(&f_options, &result).TryThrow();
            return new(result, arr, Layout, InputLayout, Name, this);
        }
    }

    #endregion

    #region CreateMeshLayout

    public MeshLayout CreateMeshLayout(
        ReadOnlySpan<MeshBufferDefine> Buffers,
        ReadOnlySpan<MeshBufferElement> Elements,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (MeshBufferDefine* p_buffers = Buffers)
        fixed (MeshBufferElement* p_elements = Elements)
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FMeshLayoutCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
                Buffers = (FMeshBufferDefine*)p_buffers,
                Elements = (FMeshBufferElement*)p_elements,
                BufferCount = (uint)Buffers.Length,
                ElementCount = (uint)Elements.Length,
            };
            FMeshLayout* ptr;
            Ptr->CreateMeshLayout(&f_options, &ptr).TryThrow();
            return new(ptr, Name, this);
        }
    }

    #endregion

    #region CreateGraphicsShaderPipeline

    public GraphicsShaderPipeline CreateGraphicsShaderPipeline(
        Shader Shader, GraphicsPipelineState PipelineState, MeshLayout? MeshLayout = null,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGraphicsShaderPipelineCreateOptions f_options = new()
            {
                Base = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    Shader = Shader.Ptr,
                },
                MeshLayout = MeshLayout == null ? null : MeshLayout.Ptr,
                GraphicsState = PipelineState.ToFFI(),
            };
            FGraphicsShaderPipeline* ptr;
            Ptr->CreateGraphicsPipeline(&f_options, &ptr).TryThrow();
            return new(ptr, Name, Shader, PipelineState, MeshLayout);
        }
    } 

    #endregion

    #region CreateBuffer

    public GpuBuffer CreateBuffer(
        in GpuBufferCreateOptions options,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    ) => MainIsolate.CreateBuffer(in options, Name, Name8);

    #endregion

    #region CreateImage

    public GpuImage CreateImage(
        in GpuImageCreateOptions options,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    ) => MainIsolate.CreateImage(in options, Name, Name8);

    #endregion

    #region CreateSampler

    public Sampler CreateSampler(
        in SamplerInfo info,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuSamplerCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
                Info = Unsafe.As<SamplerInfo, FSamplerInfo>(ref Unsafe.AsRef(in info)),
            };
            FGpuSampler* ptr;
            Ptr->CreateSampler(&f_options, &ptr).TryThrow();
            return new(ptr, Name, this);
        }
    }

    #endregion
}
