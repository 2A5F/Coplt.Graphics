using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
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
    internal CommandList m_current_cmd_list;
    internal readonly ConcurrentQueue<CommandList> m_cmd_pool = new();

    #endregion

    #region Props

    public FGpuQueue* Ptr => m_ptr;
    public GpuDevice Device => m_device;
    public GpuQueueType QueueType => m_ptr->m_queue_type.FromFFI();
    public SubmitId SubmitId => new(m_ptr->m_submit_id);
    public CommandList CommandList => m_current_cmd_list;

    #endregion

    #region Ctor

    internal GpuQueue(FGpuQueue* ptr, string? name, GpuDevice device)
    {
        m_name = name;
        m_ptr = ptr;
        m_device = device;
        m_current_cmd_list = new(this);
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

    #region Submit

    public void Submit(GpuExecutor Executor, bool NoWait = false)
    {
        using var _ = m_lock.EnterScope();
        if (Executor is GpuOutput output)
        {
            if (m_current_cmd_list.m_current_output != null && m_current_cmd_list.m_current_output != output)
                throw new ArgumentException(
                    $"The command is already used by another output and cannot be presented in this output"
                );
            m_current_cmd_list.Present(output);
        }
        m_current_cmd_list.BuildSubmitStruct(Executor, NoWait);
    }

    internal void ActualSubmit(GpuExecutor Executor, FCommandSubmit* submit, bool NoWait)
    {
        if (NoWait)
        {
            m_ptr->SubmitNoWait(Executor.m_ptr, submit).TryThrow();
            if (!m_cmd_pool.TryDequeue(out var new_cmd_list)) new_cmd_list = new(this);
            Executor.SetNeedWait(m_current_cmd_list);
            m_current_cmd_list = new_cmd_list;
        }
        else
        {
            m_ptr->Submit(Executor.m_ptr, submit).TryThrow();
            m_current_cmd_list.Reset();
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
            return new(ptr, Name, this);
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

    #region CreateImage

    public GpuImage CreateImage(
        in GpuImageCreateOptions options,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        if (options.Format == GraphicsFormat.Unknown)
            throw new ArgumentException("The image format cannot be Unknown");
        if (options.Purpose == ResourcePurpose.None)
            throw new ArgumentException("The image purpose cannot be None");
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuImageCreateOptions f_options = new()
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
                Format = options.Format.ToFFI(),
                Width = Math.Max(options.Width, 1),
                Height = Math.Max(options.Height, 1),
                DepthOrLength = Math.Max(options.DepthOrLength, 1),
                MipLevels = (ushort)Math.Max(options.MipLevels, 1),
                MultisampleCount = (byte)Math.Max(options.MultisampleCount, 1),
                Dimension = options.Dimension.ToFFI(),
                Layout = options.Layout.ToFFI(),
                HasOptimizedClearValue = !options.OptimizedClearColor.IsNone,
            };
            if (options.OptimizedClearColor.IsColor)
            {
                f_options.OptimizedClearValue.Format = options.OptimizedClearColor.Color.Format.ToFFI();
                f_options.OptimizedClearValue.Anonymous.Color =
                    Unsafe.BitCast<Color, FOptimizedClearColor._Anonymous_e__Union._Color_e__FixedBuffer>(
                        options.OptimizedClearColor.Color.Color
                    );
            }
            else if (options.OptimizedClearColor.IsDepth)
            {
                f_options.OptimizedClearValue.Format = options.OptimizedClearColor.Depth.Format.ToFFI();
                f_options.OptimizedClearValue.Depth = options.OptimizedClearColor.Depth.Depth;
                f_options.OptimizedClearValue.Stencil = options.OptimizedClearColor.Depth.Stencil;
            }
            FGpuImage* ptr;
            m_device.m_ptr->CreateImage(&f_options, &ptr).TryThrow();
            return new(ptr, Name, this);
        }
    }

    #endregion

    #region WriteToUpload

    /// <summary>
    /// 返回的 Span 只能写入
    /// </summary>
    public Span<byte> AllocUploadMemory(uint Size, out UploadLoc loc)
    {
        using var _ = m_lock.EnterScope();
        ref var upload_buffers = ref m_ptr->m_context->m_upload_buffer;
        for (nuint i = 0; i < upload_buffers.LongLength; i++)
        {
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + Size < block.size)
            {
                var r = new Span<byte>(block.mapped_ptr, (int)block.size)[(int)block.cur_offset..];
                loc = new UploadLoc(i, block.cur_offset, Size, SubmitId);
                block.cur_offset += Size;
                return r;
            }
        }
        m_ptr->m_context->GrowUploadBuffer(Size).TryThrow();
        upload_buffers = ref m_ptr->m_context->m_upload_buffer;
        {
            var i = upload_buffers.LongLength - 1;
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + Size >= block.size) throw new OutOfMemoryException();
            var r = new Span<byte>(block.mapped_ptr, (int)block.size)[(int)block.cur_offset..];
            loc = new UploadLoc(i, block.cur_offset, Size, SubmitId);
            block.cur_offset += Size;
            return r;
        }
    }

    public UploadLoc WriteToUpload(ReadOnlySpan<byte> Data)
    {
        Data.CopyTo(AllocUploadMemory((uint)Data.Length, out var loc));
        return loc;
    }

    #endregion
}
