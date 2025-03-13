using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuViewable : IQueueOwned
{
    #region Fields

    internal FGpuViewable* m_ptr;
    internal readonly FGpuViewableData* m_data;
    internal string? m_name;
    internal readonly GpuQueue m_queue;

    #endregion

    #region Props

    public FGpuViewable* Ptr => m_ptr;
    public FGpuViewableData* Data => m_data;
    public GpuDevice Device => m_queue.m_device;
    public GpuQueue Queue => m_queue;
    public ResourcePurpose Purpose => (ResourcePurpose)Data->m_purpose;

    #endregion

    #region Ctor

    internal GpuViewable(FGpuViewable* ptr, FGpuViewableData* data, string? name, GpuQueue queue)
    {
        m_ptr = ptr;
        m_data = data;
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
            ? $"{nameof(GpuViewable)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuViewable)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
