using Coplt.Dropping;
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

    #endregion

    #region Props

    public FGpuDevice* Ptr => m_ptr;

    #endregion

    #region Ctor

    public GpuDevice(FGpuDevice* ptr) => m_ptr = ptr;

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
}
