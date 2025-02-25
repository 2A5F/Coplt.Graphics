using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum GpuQueueType : byte
{
    Direct,
    Compute,
    Copy,
}

public static partial class GraphicsExtensions
{
    public static FGpuQueueType ToFFI(this GpuQueueType value) => (FGpuQueueType)value;

    public static GpuQueueType FromFFI(this FGpuQueueType value) => (GpuQueueType)value;
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuQueue
{
    #region Fields

    internal FGpuQueue* m_ptr;
    internal string? m_name;
    internal readonly GpuDevice m_device;
    internal readonly Lock m_lock = new();
    internal readonly CommandList m_cmd;

    #endregion

    #region Props

    public FGpuQueue* Ptr => m_ptr;
    public GpuDevice Device => m_device;
    public GpuQueueType QueueType => m_ptr->m_queue_type.FromFFI();
    public SubmitId SubmitId => new(m_ptr->m_submit_id);
    public CommandList CommandList => m_cmd;

    #endregion

    #region Ctor

    internal GpuQueue(FGpuQueue* ptr, string? name, GpuDevice device)
    {
        m_name = name;
        m_ptr = ptr;
        m_device = device;
        m_cmd = new(this);
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        using var _ = m_lock.EnterScope();
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
            ? $"0x{nameof(GpuQueue)}({(nuint)m_ptr:X})"
            : $"0x{nameof(GpuQueue)}({(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region CreateOutputForHwnd

    public GpuOutput CreateOutputForHwnd(
        in GpuOutputOptions Options, IntPtr Hwnd,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
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
                FormatSelector =
                {
                    Specify = Options.FormatSelector.Specify,
                    Srgb = Options.FormatSelector.Srgb,
                    Hdr = (FHdrType)Options.FormatSelector.Hdr,
                },
            };
            FGpuOutput* ptr;
            m_ptr->CreateOutputForHwnd(&f_options, (void*)Hwnd, &ptr).TryThrow();
            return new(this, ptr, Name);
        }
    }

    #endregion

    #region CreateShaderBinding

    public ShaderBinding CreateShaderBinding(
        ShaderLayout Layout,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FShaderBindingCreateOptions f_options = new()
            {
                Name = new(Name, Name8, p_name, p_name8),
                Layout = Layout.m_ptr,
            };
            FShaderBinding* ptr;
            m_device.m_ptr->CreateShaderBinding(&f_options, &ptr).TryThrow();
            return new(ptr, Name, m_device, this, Layout);
        }
    }

    #endregion

    #region CreateBuffer

    public GpuBuffer CreateBuffer(
        in GpuBufferCreateOptions options,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuBufferCreateOptions f_options = new()
            {
                Base =
                {
                    Base =
                    {
                        Name = new(Name, Name8, p_name, p_name8),
                        Purpose = options.Purpose.ToFFI(),
                    },
                    CpuAccess = options.CpuAccess.ToFFI(),
                },
                Size = options.Size,
                Count = options.Count,
                Stride = options.Stride,
                Usage = options.Usage.ToFFI(),
            };
            FGpuBuffer* ptr;
            m_device.m_ptr->CreateBuffer(&f_options, &ptr).TryThrow();
            return new(ptr, Name, this);
        }
    }

    #endregion

    #region WriteToUpload

    public UploadLoc WriteToUpload(ReadOnlySpan<byte> Data)
    {
        using var _ = m_lock.EnterScope();
        ref var upload_buffers = ref m_ptr->m_context->m_upload_buffer;
        for (nuint i = 0; i < upload_buffers.LongLength; i++)
        {
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + (uint)Data.Length < block.size)
            {
                Data.CopyTo(new Span<byte>(block.mapped_ptr, (int)block.size)[(int)block.cur_offset..]);
                var loc = new UploadLoc(i, block.cur_offset, (uint)Data.Length, SubmitId);
                block.cur_offset += (uint)Data.Length;
                return loc;
            }
        }
        m_ptr->m_context->GrowUploadBuffer((uint)Data.Length).TryThrow();
        upload_buffers = ref m_ptr->m_context->m_upload_buffer;
        {
            var i = upload_buffers.LongLength - 1;
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + (uint)Data.Length >= block.size) throw new OutOfMemoryException();
            Data.CopyTo(new Span<byte>(block.mapped_ptr, (int)block.size)[(int)block.cur_offset..]);
            var loc = new UploadLoc(i, block.cur_offset, (uint)Data.Length, SubmitId);
            block.cur_offset += (uint)Data.Length;
            return loc;
        }
    }

    #endregion
}
