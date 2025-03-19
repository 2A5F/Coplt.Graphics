using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Native;

[Dropping(Unmanaged = true)]
[StructLayout(LayoutKind.Sequential)]
public unsafe partial struct FList<T> where T : unmanaged
{
    #region Static

    public const nuint InitCapacity = 4;

    #endregion

    #region Fields

    public FAllocator* m_allocator;
    public T* m_ptr;
    public nuint m_len;
    public nuint m_cap;

    #endregion

    #region Props

    public ref T this[nuint index]
    {
        get
        {
            if (index >= m_len) throw new IndexOutOfRangeException();
            return ref m_ptr[index];
        }
    }

    public ref T this[int index]
    {
        get
        {
            if (index < 0 || (uint)index >= m_len) throw new IndexOutOfRangeException();
            return ref m_ptr[index];
        }
    }

    public nuint LongLength => m_len;

    public int Length => (int)m_len;

    public Span<T> AsSpan => new(m_ptr, (int)m_len);

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        if (m_allocator != null)
        {
            if (m_ptr != null)
            {
                m_allocator->MemoryFree(m_ptr, (nuint)Utils.AlignOf<T>());
                m_ptr = null;
            }
            m_allocator->Release();
            m_allocator = null;
        }
    }

    #endregion

    #region Move

    public FList<T> Move()
    {
        FList<T> r = new()
        {
            m_allocator = m_allocator,
            m_ptr = m_ptr,
            m_len = m_len,
            m_cap = m_cap
        };
        m_allocator = null;
        m_ptr = null;
        m_len = 0;
        m_cap = 0;
        return r;
    }

    #endregion

    #region Grow

    private void Grow()
    {
        if (m_ptr == null)
        {
            m_cap = InitCapacity;
            m_ptr = (T*)m_allocator->MemoryAlloc(m_cap * (nuint)sizeof(T), (nuint)Utils.AlignOf<T>());
        }
        else
        {
            m_cap *= 2;
            m_ptr = (T*)m_allocator->MemoryReAlloc(m_ptr, m_cap * (nuint)sizeof(T), (nuint)Utils.AlignOf<T>());
        }
    }

    #endregion

    #region EnsureCap

    public void EnsureCap(nuint cap)
    {
        if ((cap & 1) != 0) cap += 1;
        if (m_ptr == null)
        {
            m_cap = cap;
            m_ptr = (T*)m_allocator->MemoryAlloc(m_cap * (nuint)sizeof(T), (nuint)Utils.AlignOf<T>());
        }
        else if (m_cap < cap)
        {
            m_cap = cap;
            m_ptr = (T*)m_allocator->MemoryReAlloc(m_ptr, m_cap * (nuint)sizeof(T), (nuint)Utils.AlignOf<T>());
        }
        
    }

    #endregion

    #region Add

    public ref T UnsafeAdd()
    {
        if (m_len >= m_cap) Grow();
        ref var slot = ref m_ptr[m_len];
        m_len++;
        return ref slot;
    }

    public void Add(T item)
    {
        UnsafeAdd() = item;
    }

    #endregion

    #region RemoveAt

    public void RemoveAt(int index)
    {
        if (index < 0) throw new IndexOutOfRangeException();
        RemoveAt((uint)index);
    }

    public void RemoveAt(nuint index)
    {
        if (index >= m_len) throw new IndexOutOfRangeException();
        m_len--;
        if (index < m_len)
        {
            var size = (m_len - index) * (nuint)sizeof(T);
            Buffer.MemoryCopy(&m_ptr[m_len + 1], &m_ptr[m_len], size, size);
        }
    }

    #endregion

    #region Clear

    public void Clear()
    {
        m_len = 0;
    }

    #endregion
}
