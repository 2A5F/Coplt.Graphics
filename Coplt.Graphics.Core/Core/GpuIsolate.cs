using System.Buffers;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuIsolate : DeviceChild
{
    #region Fields

    internal FGpuIsolateData* m_data;
    internal readonly GpuQueue2[] m_queues;

    #endregion

    #region Props

    public new FGpuIsolate* Ptr => (FGpuIsolate*)m_ptr;
    public ref readonly FGpuIsolateData Data => ref *m_data;
    public ReadOnlySpan<GpuQueue2> Queues => m_queues;

    #endregion

    #region Ctor

    internal GpuIsolate(FGpuIsolate* ptr, string? name, GpuDevice device, FGpuIsolateData* data, GpuQueue2[] queues)
        : base((FGpuObject*)ptr, name, device)
    {
        m_data = data;
        m_queues = queues;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
        foreach (var queue in m_queues)
        {
            queue.Dispose();
        }
    }

    #endregion

    #region SetName

    public override void SetName(string name)
    {
        m_name = name;
        if (!Device.Instance.DebugEnabled) return;
        for (var i = 0; i < Queues.Length; i++)
        {
            var queue = Queues[i];
            queue.SetName($"[{name}]::Queue {i} ({queue.QueueType})");
        }
    }

    public override void SetName(ReadOnlySpan<byte> name, string? managed_version = null)
    {
        m_name = managed_version;
        if (!Device.Instance.DebugEnabled) return;
        for (var i = 0; i < Queues.Length; i++)
        {
            var queue = Queues[i];
            if (managed_version != null) queue.m_name = $"[{managed_version}]::Queue {i} ({queue.QueueType})";
            var tmp_bytes = ArrayPool<byte>.Shared.Rent(name.Length + 256);
            try
            {
                tmp_bytes[0] = "["u8[0];
                name.CopyTo(tmp_bytes.AsSpan(1));
                var cur = name.Length + 1;
                "]::Queue"u8.CopyTo(tmp_bytes.AsSpan(cur));
                cur += "]::Queue "u8.Length;
                i.TryFormat(tmp_bytes.AsSpan(cur), out var len);
                cur += len;
                tmp_bytes[cur] = " "u8[0];
                cur++;
                tmp_bytes[cur] = "("u8[0];
                cur++;
                var qt = queue.QueueType.ToUtf8String();
                qt.CopyTo(tmp_bytes.AsSpan(cur));
                cur += qt.Length;
                tmp_bytes[cur] = ")"u8[0];
                tmp_bytes[cur + 1] = 0;
                queue.SetName(tmp_bytes.AsSpan(0, cur + 2));
            }
            finally
            {
                ArrayPool<byte>.Shared.Return(tmp_bytes);
            }
        }
    }

    #endregion

    #region CreateSwapChainForHwnd

    /// <summary>
    /// Will fail on non Windows systems
    /// </summary>
    public GpuSwapChain CreateSwapChainForHwnd(IntPtr Hwnd, in GpuSwapChainCreateOptions Options, string? Name = null, ReadOnlySpan<byte> Name8 = default)
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuSwapChainCreateOptions f_options = new()
            {
                Base = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    PresentMode = (FPresentMode)Options.PresentMode,
                },
                Width = Options.Width,
                Height = Options.Height,
                Format = Options.Format.ToFFI(),
                AlphaMode = (FOutputAlphaMode)Options.AlphaMode,
                VSync = Options.VSync,
                FormatSelector =
                {
                    Specify = Options.FormatSelector.Specify,
                    Srgb = Options.FormatSelector.Srgb,
                    Hdr = (FHdrType)Options.FormatSelector.Hdr,
                },
            };
            FGpuSwapChainCreateResult r;
            Ptr->CreateSwapChainForHwnd(&f_options, (void*)Hwnd, &r).TryThrow();
            return new(r.SwapChain, r.Data, Name, this);
        }
    }

    #endregion
}
