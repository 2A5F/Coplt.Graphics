using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuResource
{
    #region Fields

    internal FGpuResource* m_ptr;
    internal string? m_name;
    internal GpuQueue m_queue;

    #endregion

    #region Props

    public FGpuResource* Ptr => m_ptr;
    public GpuDevice Device => m_queue.m_device;
    public GpuQueue Queue => m_queue;
    public ResourceState State => m_ptr->m_state.FromFFI();
    public ResourcePurpose Purpose => (ResourcePurpose)m_ptr->m_purpose;
    public CpuAccess CpuAccess => (CpuAccess)m_ptr->m_cpu_access;
    public LifeTime LifeTime => (LifeTime)m_ptr->m_life_time;

    #endregion

    #region Ctor

    internal GpuResource(FGpuResource* ptr, string? name, GpuQueue queue)
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
            Str8or16 str = new() { str16 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        fixed (byte* ptr = name)
        {
            Str8or16 str = new() { str8 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
        m_name = null;
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuResource)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuResource)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
