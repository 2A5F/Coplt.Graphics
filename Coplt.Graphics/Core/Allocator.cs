using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class Allocator
{
    #region Fields

    internal FAllocator* m_ptr;

    #endregion

    #region Props

    public FAllocator* Ptr => m_ptr;

    #endregion

    #region Ctor

    public Allocator(FAllocator* ptr)
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

    #region Alloc

    public void* Alloc(nuint size) => m_ptr->MemoryAlloc(size);
    public void* Alloc(nuint size, nuint align) => m_ptr->MemoryAlloc(size, align);
    public void* ReAlloc(void* p, nuint size) => m_ptr->MemoryReAlloc(p, size);
    public void* ReAlloc(void* p, nuint size, nuint align) => m_ptr->MemoryReAlloc(p, size, align);
    public void Free(void* p) => m_ptr->MemoryFree(p);
    public void Free(void* p, nuint align) => m_ptr->MemoryFree(p, align);

    #endregion
}
