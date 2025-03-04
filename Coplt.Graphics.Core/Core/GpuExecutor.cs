using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuExecutor
{
    #region Fields

    internal FGpuExecutor* m_ptr;
    internal string? m_name;
    internal readonly GpuQueue m_queue;
    internal CommandList? m_recycle_cmd_list;
    internal readonly Lock m_wait_lock = new();

    #endregion

    #region Props

    public FGpuExecutor* Ptr => m_ptr;
    public GpuDevice Device => m_queue.m_device;
    public GpuQueue Queue => m_queue;

    public bool NeedWait => m_recycle_cmd_list != null;

    #endregion

    #region Ctor

    internal GpuExecutor(FGpuExecutor* ptr, string? name, GpuQueue queue)
    {
        m_ptr = ptr;
        m_name = name;
        m_queue = queue;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        Wait(true);
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        ptr->Release();
    }

    #endregion

    #region SetName

    public void SetName(string name)
    {
        m_name = name;
        fixed (char* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        m_name = null;
        fixed (byte* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            m_ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuExecutor)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuExecutor)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region SetNeedWait

    internal void SetNeedWait(CommandList cmd)
    {
        if (Interlocked.CompareExchange(ref m_recycle_cmd_list, cmd, null) != null)
            throw new InvalidOperationException("The current executor has not been waited and cannot be reused");
    }

    #endregion

    #region Wait

    /// <summary>
    /// 等待可重用
    /// </summary>
    public void Wait() => Wait(false);

    /// <summary>
    /// 等待可重用
    /// </summary>
    private void Wait(bool discard)
    {
    re:
        var recycle_cmd_list = m_recycle_cmd_list;
        if (recycle_cmd_list == null) return;
        using (m_wait_lock.EnterScope())
        {
            if (recycle_cmd_list == m_recycle_cmd_list) goto re;
            try
            {
                m_ptr->Wait().TryThrow();
            }
            finally
            {
                if (!discard)
                {
                    recycle_cmd_list.Reset();
                    Queue.m_cmd_pool.Enqueue(recycle_cmd_list);
                }
                m_recycle_cmd_list = null;
            }
        }
    }

    #endregion
}
