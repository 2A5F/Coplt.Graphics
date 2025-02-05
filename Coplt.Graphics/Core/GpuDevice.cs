using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics;

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
}
