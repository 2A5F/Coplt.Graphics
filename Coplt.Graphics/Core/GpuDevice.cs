﻿using System.Buffers;
using System.Text;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

#region Enums

public enum D3dFeatureLevel
{
    Unset = 0,
    _12_1 = 0xc100,
    _12_2 = 0xc200,
}

public enum VulkanVersion
{
    Unset = 0,
    _1_2 = 1002,
    _1_3 = 1003,
    _1_4 = 1004,
}

public enum GpuPreference : byte
{
    HighPerformance = 0,
    MinimumPower = 1,
    Any = 255,
}

#endregion

#region GpuDeviceOptions

public record struct GpuDeviceOptions
{
    public VulkanVersion VulkanVersion;
    public D3dFeatureLevel D3dFeatureLevel;
}

#endregion

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuDevice
{
    #region Fields

    internal FGpuDevice* m_ptr;
    internal readonly GraphicsInstance m_instance;
    internal string? m_name;
    [Drop]
    internal readonly GpuQueue m_main_queue;

    #endregion

    #region Props

    public FGpuDevice* Ptr => m_ptr;
    public GraphicsInstance Instance => m_instance;
    public GpuQueue MainQueue => m_main_queue;
    public CommandList MainCommandList => MainQueue.CommandList;

    #endregion

    #region Ctor

    internal GpuDevice(FGpuDevice* ptr, GraphicsInstance instance,
        string? name, string? QueueName = null, ReadOnlySpan<byte> QueueName8 = default
    )
    {
        m_instance = instance;
        m_name = name;
        m_ptr = ptr;
        m_main_queue = CreateMainQueue(Name: QueueName, Name8: QueueName8);
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        ptr->Release();
    }

    #endregion

    #region SetName

    public void SetName(string name)
    {
        m_name = name;
        fixed (char* ptr = name)
        {
            Str8or16 str = new() { str16 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        m_name = null;
        fixed (byte* ptr = name)
        {
            Str8or16 str = new() { str8 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuDevice)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuDevice)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region GetRawDevice

    /// <returns>Dx12 返回 ID3D12Device*</returns>
    public void* GetRawDevice() => m_ptr->GetRawDevice();

    #endregion

    #region CreateMainQueue

    public GpuQueue CreateMainQueue(string? Name = null, ReadOnlySpan<byte> Name8 = default)
    {
        fixed (char* p_name = Name)
        {
            fixed (byte* p_name8 = Name8)
            {
                FMainQueueCreateOptions f_options = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                };
                FGpuQueue* ptr;
                m_ptr->CreateMainQueue(&f_options, &ptr).TryThrow();
                return new(ptr, Name);
            }
        }
    }

    #endregion

    #region CreateShaderModule

    public ShaderModule CreateShaderModule(
        ShaderStage Stage, ReadOnlySpan<byte> Blob,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        {
            fixed (byte* p_name8 = Name8)
            {
                fixed (byte* data = Blob)
                {
                    FShaderModuleCreateOptions f_options = new()
                    {
                        Name = new(Name, Name8, p_name, p_name8),
                        Data = data,
                        Size = (nuint)Blob.Length,
                        Stage = Stage.ToFFI(),
                    };
                    FShaderModule* ptr;
                    m_ptr->CreateShaderModule(&f_options, &ptr).TryThrow();
                    return new(ptr, Name);
                }
            }
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
        {
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
                    FShaderLayout* ptr;
                    m_ptr->CreateShaderLayout(&f_options, &ptr).TryThrow();
                    return new(ptr, Name);
                }
            }
        }
    }

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
                item.SlotName8 ?? (item.SlotName == null ? null : m_instance.CacheString8(item.SlotName));
            var slot_name = item.SlotName8?.ToString() ?? item.SlotName;
            var slot_id = item.SlotId ?? (string.IsNullOrWhiteSpace(slot_name)
                ? throw new ArgumentException("Slot id is not provided and Slot Name is null/empty")
                : m_instance.GetSlotId(slot_name));
            meta[i] = new()
            {
                SlotName8 = slot_name8,
                SlotId = slot_id,
                SlotIndex = item.SlotIndex,
            };
            p_elements[i] = new()
            {
                SlotName = slot_name8 == null ? null : slot_name8.m_ptr,
                SlotId = slot_id,
                SlotIndex = item.SlotIndex,
            };
        }
        fixed (char* p_name = Name)
        {
            fixed (byte* p_name8 = Name8)
            {
                FShaderInputLayoutCreateOptions f_options = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    Element = p_elements,
                    Count = (uint)Elements.Length,
                };
                FShaderInputLayout* ptr;
                m_ptr->CreateShaderInputLayout(&f_options, &ptr).TryThrow();
                return new(ptr, meta, Name);
            }
        }
    }

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
                $"Too many shader modules, there can be a maximum of {FShader.MaxShaderModuleCount}");
        Shader.ShaderModuleArray arr = new();
        var p_modules = stackalloc FShaderModule*[Modules.Length];
        for (var i = 0; i < Modules.Length; i++)
        {
            var module = Modules[i];
            arr[i] = module;
            p_modules[i] = module.m_ptr;
        }
        fixed (char* p_name = Name)
        {
            fixed (byte* p_name8 = Name8)
            {
                FShaderCreateOptions f_options = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    Layout = Layout == null ? null : Layout.m_ptr,
                    InputLayout = InputLayout == null ? null : InputLayout.m_ptr,
                    Modules = p_modules,
                    Count = (byte)Modules.Length,
                };
                FShader* ptr;
                m_ptr->CreateShader(&f_options, &ptr).TryThrow();
                return new(ptr, arr, Layout, InputLayout, Name);
            }
        }
    }

    #endregion
}
