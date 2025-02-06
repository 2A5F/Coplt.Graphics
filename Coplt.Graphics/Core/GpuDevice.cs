﻿using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics;

public record struct GpuDeviceOptions
{
    public VulkanVersion VulkanVersion { get; set; }
    public D3dFeatureLevel D3dFeatureLevel { get; set; }
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuDevice
{
    #region Fields

    internal FGpuDevice* m_ptr;
    [Drop]
    internal readonly GpuQueue m_main_queue;

    #endregion

    #region Props

    public FGpuDevice* Ptr => m_ptr;
    public GpuQueue MainQueue => m_main_queue;

    #endregion

    #region Ctor

    public GpuDevice(FGpuDevice* ptr, string? QueueName = null, ReadOnlySpan<byte> QueueName8 = default)
    {
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
        fixed (char* ptr = name)
        {
            Str8or16 str = new() { str16 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        fixed (byte* ptr = name)
        {
            Str8or16 str = new() { str8 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region GetRawDevice

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
                return new(ptr);
            }
        }
    }

    #endregion
}
