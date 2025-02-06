using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuQueue
{
    #region Fields

    internal FGpuQueue* m_ptr;

    #endregion

    #region Props

    public FGpuQueue* Ptr => m_ptr;

    public GpuQueueType QueueType => m_ptr->m_queue_type.FromFFI();

    #endregion

    #region Ctor

    public GpuQueue(FGpuQueue* ptr) => m_ptr = ptr;

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
    }

    #endregion

    #region CreateOutputForHwnd

    public GpuOutput CreateOutputForHwnd(
        in GpuOutputOptions Options, IntPtr Hwnd,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        {
            fixed (byte* p_name8 = Name8)
            {
                FGpuOutputCreateOptions f_options = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    Width = Options.Width,
                    Height = Options.Height,
                    Format = Options.Format.ToFFI(),
                    PresentMode = (FPresentMode)Options.PresentMode,
                    AlphaMode = (FOutputAlphaMode)Options.AlphaMode,
                    VSync = Options.VSync,
                };
                FGpuOutput* ptr;
                m_ptr->CreateOutputForHwnd(&f_options, (void*)Hwnd, &ptr).TryThrow();
                return new(ptr);
            }
        }
    }

    #endregion
}
