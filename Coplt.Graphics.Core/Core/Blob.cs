using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class Blob
{
    #region Fields

    internal FBlob* m_ptr;

    #endregion

    #region Props

    public FBlob* Ptr => m_ptr;
    public Span<byte> Span => m_ptr->Span;

    #endregion

    #region Ctor

    public Blob(FBlob* ptr)
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

    #region GetEnumerator

    public Span<byte>.Enumerator GetEnumerator() => Span.GetEnumerator();

    #endregion
}
