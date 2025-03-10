using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuView : IQueueOwned
{
    #region Fields

    internal FGpuView* m_ptr;
    internal string? m_name;
    internal readonly GpuQueue m_queue;

    #endregion

    #region Props

    public FGpuView* Ptr => m_ptr;
    public GpuDevice Device => m_queue.m_device;
    public GpuQueue Queue => m_queue;
    public ResourcePurpose Purpose => (ResourcePurpose)m_ptr->m_purpose;

    #endregion

    #region Ctor

    internal GpuView(FGpuView* ptr, string? name, GpuQueue queue)
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
            ? $"{nameof(GpuView)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuView)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
