using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuAdapter
{
    #region Fields

    internal FGpuAdapter* m_ptr;
    internal readonly GraphicsInstance m_instance;
    internal string? m_name;
    internal string? m_driver;
    internal string? m_driver_info;

    #endregion

    #region Props

    public FGpuAdapter* Ptr => m_ptr;
    public GraphicsInstance Instance => m_instance;
    public string Name => m_name ??= m_ptr->m_name.ToString();
    public string Driver => m_driver ??= m_ptr->m_driver.ToString();
    public string DriverInfo => m_driver_info ??= m_ptr->m_driver_info.ToString();
    public uint VendorId => m_ptr->m_vendor_id;
    public uint DeviceId => m_ptr->m_device_id;
    public D3dFeatureLevel D3dFeatureLevel => (D3dFeatureLevel)m_ptr->m_d3d_feature_level;
    public VulkanVersion VulkanVersion => (VulkanVersion)m_ptr->m_vulkan_version;
    public DeviceType DeviceType => (DeviceType)m_ptr->m_device_type;
    public Backend Backend => (Backend)m_ptr->m_backend;
    public DeviceFeatures Features => new(m_ptr->m_features);

    #endregion

    #region Ctor

    internal GpuAdapter(FGpuAdapter* ptr, GraphicsInstance instance)
    {
        m_ptr = ptr;
        m_instance = instance;
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

    #region ToString

    public override string ToString() => string.IsNullOrEmpty(Name)
        ? $"{nameof(GpuAdapter)}(0x{(nuint)m_ptr:X})"
        : $"{nameof(GpuAdapter)}({Name})";

    #endregion

    #region CreateDevice

    public GpuDevice CreateDevice(
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        var QueueName = !Instance.DebugEnabled || Name is null ? null : $"{Name} Main Queue";
        var QueueName8 = !Instance.DebugEnabled || Name8.Length == 0
            ? Name8
            : Utils.JoinUtf8String(Name8, " Main Queue"u8);

        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuDeviceCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
            };
            FGpuDevice* ptr;
            m_ptr->CreateDevice(&f_options, &ptr).TryThrow();
            return new(ptr, Instance, this, Name, QueueName: QueueName, QueueName8: QueueName8);
        }
    }

    #endregion
}
