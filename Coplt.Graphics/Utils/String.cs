using System.Text;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Utilities;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class String8
{
    #region Fields

    internal FString8* m_ptr;
    internal string? m_str_cache;

    #endregion
    
    #region Props

    public FString8* Ptr => m_ptr;

    #endregion

    #region Ctor

    public String8(FString8* ptr)
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

    #region Span

    public ReadOnlySpan<byte> AsSpan => m_ptr == null ? default : new(m_ptr->m_data, (int)m_ptr->m_size);

    #endregion

    #region ToString

    public override string ToString() => m_str_cache ??= Encoding.UTF8.GetString(AsSpan);

    #endregion

    #region TakeString

    public string TakeString()
    {
        var str = ToString();
        Dispose();
        return str;
    }

    #endregion
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class String16
{
    #region Fields

    internal FString16* m_ptr;
    internal string? m_str_cache;

    #endregion
    
    #region Props

    public FString16* Ptr => m_ptr;

    #endregion

    #region Ctor

    public String16(FString16* ptr)
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

    #region Span

    public ReadOnlySpan<char> AsSpan => m_ptr == null ? default : new(m_ptr->m_data, (int)m_ptr->m_size);

    #endregion

    #region ToString

    public override string ToString() => m_str_cache ??= AsSpan.ToString();

    #endregion

    #region TakeString

    public string TakeString()
    {
        var str = ToString();
        Dispose();
        return str;
    }

    #endregion
}
