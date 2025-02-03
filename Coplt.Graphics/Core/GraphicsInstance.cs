using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Native.D3d12;

namespace Coplt.Graphics;

[Dropping(Unmanaged = true)]
public unsafe partial class GraphicsInstance
{
    #region Fields

    internal FInstance* m_ptr;

    #endregion

    #region Ctor

    public GraphicsInstance(FInstance* ptr) => m_ptr = ptr;

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        ptr->Release();
    }

    #endregion

    #region Load

    public static GraphicsInstance LoadD3d12() => new(D3d12Native.Coplt_Graphics_D3d12_Create_Instance());

    #endregion
}
