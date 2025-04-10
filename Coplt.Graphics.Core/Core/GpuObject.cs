using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public abstract unsafe class GpuObject : NativeObject
{
    #region Fields

    internal string? m_name;

    #endregion

    #region Props

    public new FGpuObject* Ptr => (FGpuObject*)m_ptr;
    public string? Name => m_name;

    #endregion

    #region Ctor

    internal GpuObject(FGpuObject* ptr, string? name) : base((FUnknown*)ptr)
    {
        m_name = name;
    }

    #endregion

    #region SetName

    public virtual void SetName(string name)
    {
        m_name = name;
        fixed (char* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            Ptr->SetName(&str).TryThrow();
        }
    }

    public virtual void SetName(ReadOnlySpan<byte> name, string? managed_version = null)
    {
        m_name = managed_version;
        fixed (byte* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            Ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{GetType().Name}(0x{(nuint)m_ptr:X})"
            : $"{GetType().Name}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
