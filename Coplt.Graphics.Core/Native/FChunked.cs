using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Native;

[StructLayout(LayoutKind.Sequential)]
public unsafe struct FChunkedMemory<T> where T : unmanaged
{
    #region Fields

    public T* m_ptr;

    #endregion
}

[Dropping(Unmanaged = true)]
[StructLayout(LayoutKind.Sequential)]
public unsafe partial struct FChunkedAllocator<T> where T : unmanaged
{
    #region Fields

    [Drop]
    public FList<FChunkedMemory<T>> m_list;
    public nuint m_cur;
    public nuint m_chunk_size;

    #endregion

    #region Drop

    [Drop(Order = -1)]
    private void Drop()
    {
        if (m_list.m_allocator != null && m_list.m_ptr != null)
        {
            for (nuint i = 0; i < m_list.m_len; ++i)
            {
                var chunk = m_list[i];
                m_list.m_allocator->MemoryFree(chunk.m_ptr, (nuint)Utils.AlignOf<T>());
            }
        }
    }

    #endregion

    #region Alloc

    public FChunkedMemory<T> Alloc()
    {
        if (m_cur >= m_list.m_len)
        {
            if (m_list.m_allocator == null) throw new NullReferenceException();
            FChunkedMemory<T> chunk = new() { m_ptr = (T*)m_list.m_allocator->MemoryAlloc(m_chunk_size, (nuint)Utils.AlignOf<T>()) };
            m_list.Add(chunk);
            return chunk;
        }
        return m_list[m_cur++];
    }

    #endregion

    #region Reset

    public void Reset()
    {
        m_cur = 0;
    }

    #endregion
}
