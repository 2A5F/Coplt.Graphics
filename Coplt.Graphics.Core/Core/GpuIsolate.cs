using System.Buffers;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public ref struct GpuIsolateConfig(ref FGpuIsolateConfig @ref)
{
    internal ref FGpuIsolateConfig m_ref = ref @ref;
    public ref bool MultiThreadRecord => ref Unsafe.As<B8, bool>(ref m_ref.MultiThreadRecord);
    public ref bool UseSplitBarrier => ref Unsafe.As<B8, bool>(ref m_ref.UseSplitBarrier);
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuIsolate : DeviceChild
{
    #region Fields

    internal FGpuIsolateData* m_data;

    #endregion

    #region Props

    public new FGpuIsolate* Ptr => (FGpuIsolate*)m_ptr;
    public ref readonly FGpuIsolateData Data => ref *m_data;
    public GpuIsolateConfig Config => new(ref *Data.Config);
    public FrameId FrameId => new(Data.FrameId);

    #endregion

    #region Ctor

    internal GpuIsolate(FGpuIsolate* ptr, FGpuIsolateData* data, string? name, GpuDevice device)
        : base((FGpuObject*)ptr, name, device)
    {
        m_data = data;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion

    #region Rent Reocrds

    public GpuRecord RentRecord()
    {
        GpuRecord? record = null;
        RentRecords(new Span<GpuRecord?>(ref record));
        return record!;
    }

    public void RentRecords(Span<GpuRecord?> records)
    {
        if (records.Length == 0) return;
        if (records.Length < 8)
        {
            var p = stackalloc FGpuRecordCreateResult[records.Length];
            Ptr->RentRecords((uint)records.Length, p).TryThrow();
            for (var i = 0; i < records.Length; i++)
            {
                records[i] = new(p[i].Record, p[i].Data, null, this);
            }
        }
        else
        {
            var arr = ArrayPool<FGpuRecordCreateResult>.Shared.Rent(records.Length);
            try
            {
                fixed (FGpuRecordCreateResult* p = arr)
                {
                    Ptr->RentRecords((uint)records.Length, p).TryThrow();
                    for (var i = 0; i < records.Length; i++)
                    {
                        records[i] = new(p[i].Record, p[i].Data, null, this);
                    }
                }
            }
            finally
            {
                ArrayPool<FGpuRecordCreateResult>.Shared.Return(arr);
            }
        }
    }

    #endregion

    #region Return Reocrds

    public void ReturnRecords(params ReadOnlySpan<GpuRecord> records)
    {
        if (records.Length == 0) return;
        bool disposed = false;
        if (records.Length < 8)
        {
            var p = stackalloc FGpuRecord*[records.Length];
            var c = 0u;
            foreach (var t in records)
            {
                if (ExchangeUtils.ExchangePtr(ref t.m_ptr, null, out var ptr) is null) disposed = true;
                else p[c++] = (FGpuRecord*)ptr;
            }
            if (c > 0) Ptr->ReturnRecords(c, p).TryThrow();
            if (disposed) throw new ObjectDisposedException(nameof(GpuRecord));
        }
        else
        {
            var arr = ArrayPool<IntPtr>.Shared.Rent(records.Length);
            var c = 0u;
            try
            {
                foreach (var t in records)
                {
                    if (ExchangeUtils.ExchangePtr(ref t.m_ptr, null, out var ptr) is null) disposed = true;
                    else arr[c++] = (IntPtr)ptr;
                }
                if (c > 0)
                {
                    fixed (IntPtr* p = arr)
                    {
                        Ptr->ReturnRecords(c, (FGpuRecord**)p).TryThrow();
                    }
                }
                if (disposed) throw new ObjectDisposedException(nameof(GpuRecord));
            }
            finally
            {
                ArrayPool<IntPtr>.Shared.Return(arr);
            }
        }
    }

    #endregion

    #region Submit

    public void Submit(params ReadOnlySpan<GpuRecord> records)
    {
        if (records.Length == 0) return;
        if (records.Length < 8)
        {
            var p = stackalloc FGpuRecord*[records.Length];
            var o = stackalloc FGpuRecordCreateResult[records.Length];
            for (var i = 0; i < records.Length; i++)
            {
                if ((p[i] = records[i].Ptr) == null) throw new NullReferenceException();
                records[i].AssertEndOrCanEnd();
            }
            Ptr->Submit((uint)records.Length, p, o).TryThrow();
            for (var i = 0; i < records.Length; i++)
            {
                records[i].m_ptr = (FUnknown*)o[i].Record;
                records[i].m_data = o[i].Data;
                records[i].Reset();
            }
        }
        else
        {
            var arr = ArrayPool<IntPtr>.Shared.Rent(records.Length);
            var @out = ArrayPool<FGpuRecordCreateResult>.Shared.Rent(records.Length);
            try
            {
                for (var i = 0; i < records.Length; i++)
                {
                    if ((arr[i] = (IntPtr)records[i].Ptr) == 0) throw new NullReferenceException();
                    records[i].AssertEndOrCanEnd();
                }
                fixed (IntPtr* p = arr)
                fixed (FGpuRecordCreateResult* o = @out)
                {
                    Ptr->Submit((uint)records.Length, (FGpuRecord**)p, o).TryThrow();
                    for (var i = 0; i < records.Length; i++)
                    {
                        records[i].m_ptr = (FUnknown*)o[i].Record;
                        records[i].m_data = o[i].Data;
                        records[i].Reset();
                    }
                }
            }
            finally
            {
                ArrayPool<IntPtr>.Shared.Return(arr);
                ArrayPool<FGpuRecordCreateResult>.Shared.Return(@out);
            }
        }
    }

    #endregion

    #region Submit Return

    public void SubmitReturn(params ReadOnlySpan<GpuRecord> records)
    {
        if (records.Length == 0) return;
        if (records.Length < 8)
        {
            var p = stackalloc FGpuRecord*[records.Length];
            for (var i = 0; i < records.Length; i++)
            {
                if ((p[i] = records[i].Ptr) == null) throw new NullReferenceException();
            }
            Ptr->SubmitReturn((uint)records.Length, p).TryThrow();
            foreach (var a in records)
            {
                a.m_ptr = null;
                a.m_data = null;
                a.Reset();
            }
        }
        else
        {
            var arr = ArrayPool<IntPtr>.Shared.Rent(records.Length);
            try
            {
                for (var i = 0; i < records.Length; i++)
                {
                    if ((arr[i] = (IntPtr)records[i].Ptr) == 0) throw new NullReferenceException();
                }
                fixed (IntPtr* p = arr)
                {
                    Ptr->SubmitReturn((uint)records.Length, (FGpuRecord**)p).TryThrow();
                    foreach (var a in records)
                    {
                        a.m_ptr = null;
                        a.m_data = null;
                        a.Reset();
                    }
                }
            }
            finally
            {
                ArrayPool<IntPtr>.Shared.Return(arr);
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
                    Hdr = (FHdrType)Options.Hdr,
                    Srgb = Options.Srgb,
                },
                Width = Options.Width,
                Height = Options.Height,
                AlphaMode = (FOutputAlphaMode)Options.AlphaMode,
                VSync = Options.VSync,
            };
            FGpuSwapChainCreateResult r;
            Ptr->CreateSwapChainForHwnd(&f_options, (void*)Hwnd, &r).TryThrow();
            return new(r.SwapChain, r.Data, Name, this);
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
            Device.Ptr->CreateBuffer(&f_options, &ptr).TryThrow();
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
            Device.Ptr->CreateImage(&f_options, &ptr).TryThrow();
            return new(ptr, Name, this);
        }
    }

    #endregion
}
