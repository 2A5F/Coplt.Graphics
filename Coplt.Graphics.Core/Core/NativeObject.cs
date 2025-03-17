using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class NativeObject
{
    #region Fields

    internal FUnknown* m_ptr;

    #endregion

    #region Props

    public FUnknown* Ptr => m_ptr;

    #endregion

    #region Ctor

    internal NativeObject(FUnknown* ptr)
    {
        m_ptr = ptr;
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

    public override string ToString() => $"0x{GetType().Name}({(nuint)m_ptr:X})";

    #endregion
}
