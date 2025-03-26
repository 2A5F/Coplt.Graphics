using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;

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

    public static ReadOnlySpan<byte> ToUtf8String(this GpuQueueType value) => value switch
    {
        GpuQueueType.Direct  => "Direct"u8,
        GpuQueueType.Compute => "Compute"u8,
        GpuQueueType.Copy    => "Copy"u8,
        _                    => throw new ArgumentOutOfRangeException(nameof(value), value, null)
    };
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
            m_device.Ptr->CreateShaderBinding(&f_options, &ptr).TryThrow();
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
            m_device.Ptr->CreateBuffer(&f_options, &ptr).TryThrow();
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
            m_device.Ptr->CreateImage(&f_options, &ptr).TryThrow();
            return new(ptr, Name, this);
        }
    }

    #endregion

    #region WriteToUpload

    /// <summary>
    /// 返回的 Span 只能写入
    /// </summary>
    public FSlice<byte> AllocUploadMemory(uint Size, out UploadLoc loc, uint Align = 4)
    {
        if (Align > 1) Size += Align - 1;
        using var _ = m_lock.EnterScope();
        ref var upload_buffers = ref m_ptr->m_context->m_upload_buffer;
        for (nuint i = 0; i < upload_buffers.LongLength; i++)
        {
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + Size < block.size)
            {
                var offset = block.cur_offset.Aligned(Align);
                var r = new FSlice<byte>(block.mapped_ptr, (nuint)block.size).Slice((nuint)offset);
                loc = new UploadLoc(i, offset, Size, SubmitId);
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
            var offset = block.cur_offset.Aligned(Align);
            var r = new FSlice<byte>(block.mapped_ptr, (nuint)block.size).Slice((nuint)offset);
            loc = new UploadLoc(i, offset, Size, SubmitId);
            block.cur_offset += Size;
            return r;
        }
    }

    public UploadLoc WriteToUpload(ReadOnlySpan<byte> Data, uint Align = 4)
    {
        Data.CopyTo(AllocUploadMemory((uint)Data.Length, out var loc, Align));
        return loc;
    }

    /// <summary>
    /// 分配用于上传纹理的 256 对齐的内存
    /// </summary>
    public ImageUploadBufferMemory AllocImageUploadMemory2D(uint PixelSize, uint Width, uint Height, uint Length = 1) =>
        AllocImageUploadMemory2D(PixelSize, Width, Height, Length, out _);

    /// <summary>
    /// 分配用于上传纹理的 256 对齐的内存
    /// </summary>
    public ImageUploadBufferMemory AllocImageUploadMemory2D(uint PixelSize, uint Width, uint Height, uint Length, out UploadLoc loc)
    {
        if (PixelSize < 1 || Width < 1 || Height < 1 || Length < 1) throw new ArgumentOutOfRangeException();
        var row_stride = (PixelSize * Width).Aligned(256);
        var row_count = Height * Length;
        var buffer_size = row_stride * row_count + 256u;
        var slice = AllocUploadMemory(buffer_size, out loc, 256u);
        return new ImageUploadBufferMemory(slice, row_stride, row_count, Length, Height, loc);
    }

    #endregion
}
