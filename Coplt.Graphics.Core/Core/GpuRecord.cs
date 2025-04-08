using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;
using Coplt.Union;

namespace Coplt.Graphics.Core;

/// <summary>
/// 命令录制对象，非线程安全，需要外部同步
/// </summary>
public sealed unsafe class GpuRecord : IsolateChild
{
    #region Fields

    internal FGpuRecordData* m_data;
    internal readonly Dictionary<object, uint> m_resources = new();
    internal readonly HashSet<object> m_objects = new();
    internal uint m_debug_scope_count;
    internal bool m_in_render_or_compute_scope;

    #endregion

    #region Props

    public new FGpuRecord* Ptr => (FGpuRecord*)m_ptr;
    internal ref FGpuRecordData Data => ref *m_data;
    public bool DebugEnabled { get; }

    #endregion

    #region Ctor

    internal GpuRecord(FGpuRecord* ptr, FGpuRecordData* data, string? name, GpuIsolate isolate) : base((FGpuObject*)ptr, name, isolate)
    {
        m_data = data;
        DebugEnabled = Isolate.Device.Instance.DebugEnabled;
    }

    #endregion

    #region Dispose

    protected override void Dispose(bool disposing)
    {
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        var p = (FGpuRecord*)ptr;
        Isolate.Ptr->ReturnRecords(1, &p).TryThrow();
        m_data = null;
    }

    #endregion

    #region Reset

    internal void Reset()
    {
        m_resources.Clear();
        m_objects.Clear();
    }

    #endregion

    #region End

    public void AssertNotEnded()
    {
        if (Data.Ended) throw new InvalidOperationException("Gpu record is already ended");
    }

    public void End()
    {
        AssertNotEnded();
        AssertEndOrCanEnd();
        Ptr->End().TryThrow();
    }

    public void EnsureEnded()
    {
        if (Data.Ended) return;
        AssertEndOrCanEnd();
        Ptr->End().TryThrow();
    }

    internal void AssertEndOrCanEnd()
    {
        if (Data.Ended) return;
        if (m_debug_scope_count != 0)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
        if (m_in_render_or_compute_scope)
            throw new InvalidOperationException(
                "There is still Render or Compute scope not ended, please check whether Dispose is missed."
            );
    }

    #endregion

    #region Alloc Upload

    /// <summary>
    /// 返回的 Span 只能写入
    /// </summary>
    public FSlice<byte> AllocUploadMemory(uint Size, out UploadLoc2 loc, uint Align = 4)
    {
        if (Align > 1) Size += Align - 1;
        ref var upload_buffers = ref Data.Context->m_upload_buffer;
        for (nuint i = 0; i < upload_buffers.LongLength; i++)
        {
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + Size < block.size)
            {
                var offset = block.cur_offset.Aligned(Align);
                var r = new FSlice<byte>(block.mapped_ptr, (nuint)block.size).Slice((nuint)offset);
                loc = new UploadLoc2(Data.Id, Data.Version, i, offset, Size);
                block.cur_offset += Size;
                return r;
            }
        }
        Data.Context->GrowUploadBuffer(Size).TryThrow();
        upload_buffers = ref Data.Context->m_upload_buffer;
        {
            var i = upload_buffers.LongLength - 1;
            ref var block = ref upload_buffers[i];
            if (block.cur_offset + Size >= block.size) throw new OutOfMemoryException();
            var offset = block.cur_offset.Aligned(Align);
            var r = new FSlice<byte>(block.mapped_ptr, (nuint)block.size).Slice((nuint)offset);
            loc = new UploadLoc2(Data.Id, Data.Version, i, offset, Size);
            block.cur_offset += Size;
            return r;
        }
    }

    public UploadLoc2 WriteToUpload(ReadOnlySpan<byte> Data, uint Align = 4)
    {
        Data.CopyTo(AllocUploadMemory((uint)Data.Length, out var loc, Align));
        return loc;
    }

    /// <summary>
    /// 分配用于上传纹理的 256 对齐的内存
    /// </summary>
    public ImageUploadBufferMemory2 AllocImageUploadMemory2D(uint PixelSize, uint Width, uint Height, uint Length = 1) =>
        AllocImageUploadMemory2D(PixelSize, Width, Height, Length, out _);

    /// <summary>
    /// 分配用于上传纹理的 256 对齐的内存
    /// </summary>
    public ImageUploadBufferMemory2 AllocImageUploadMemory2D(uint PixelSize, uint Width, uint Height, uint Length, out UploadLoc2 loc)
    {
        if (PixelSize < 1 || Width < 1 || Height < 1 || Length < 1) throw new ArgumentOutOfRangeException();
        var row_stride = (PixelSize * Width).Aligned(256);
        var row_count = Height * Length;
        var buffer_size = row_stride * row_count + 256u;
        var slice = AllocUploadMemory(buffer_size, out loc, 256u);
        return new ImageUploadBufferMemory2(slice, row_stride, row_count, Length, Height, loc);
    }

    #endregion

    #region Payload

    #region AddResource

    internal FCmdResRef AddResource(IGpuResource resource)
    {
        ref var slot = ref CollectionsMarshal.GetValueRefOrAddDefault(m_resources, resource, out var exists);
        if (!exists)
        {
            var len = Data.Resources.LongLength;
            if (len + 1 >= uint.MaxValue) throw new IndexOutOfRangeException("Resources >= uint32 max are not supported");
            slot = (uint)len;
            Data.Resources.Add(resource.IntoCmd());
        }
        return new() { IndexPlusOne = slot + 1 };
    }

    #endregion

    #region AddObject

    /// <summary>
    /// 返回是否首次
    /// </summary>
    internal bool AddObject(object obj) => m_objects.Add(obj);

    #endregion

    #region AddRects

    internal uint AddRects(ReadOnlySpan<URect> Rects)
    {
        var len = Data.PayloadRect.LongLength;
        if (len >= uint.MaxValue) throw new IndexOutOfRangeException("Payload >= uint32 max are not supported");
        Data.PayloadRect.EnsureCap(len + (uint)Rects.Length);
        Data.PayloadRect.AddRange(MemoryMarshal.Cast<URect, FRect>(Rects));
        return (uint)len;
    }

    #endregion

    #region AddViewports

    internal uint AddViewports(ReadOnlySpan<UViewPort> Viewports)
    {
        var len = Data.PayloadViewport.LongLength;
        if (len >= uint.MaxValue) throw new IndexOutOfRangeException("Payload >= uint32 max are not supported");
        Data.PayloadViewport.EnsureCap(len + (uint)Viewports.Length);
        Data.PayloadViewport.AddRange(MemoryMarshal.Cast<UViewPort, FViewport>(Viewports));
        return (uint)len;
    }

    #endregion

    #region AddString

    internal uint AddString(string str)
    {
        var index = Data.Blob.LongLength;
        Data.Blob.EnsureCap((nuint)str.Length * 2 + 2);
        Data.Blob.AddRange(MemoryMarshal.AsBytes(str.AsSpan()));
        Data.Blob.AddRange([0, 0]);
        return (uint)index;
    }

    internal uint AddString(ReadOnlySpan<byte> str)
    {
        var index = Data.Blob.LongLength;
        Data.Blob.EnsureCap((nuint)str.Length + 1);
        Data.Blob.AddRange(str);
        Data.Blob.Add(0);
        return (uint)index;
    }

    #endregion

    #endregion

    #region Debug

    #region Label

    public void Label(string Label, Color? Color = null)
    {
        AssertNotEnded();
        if (!DebugEnabled) return;
        var cmd = new FCmdLabel
        {
            Base = { Type = FCmdType.Label },
            StringIndex = AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str16,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { Label = cmd });
    }

    public void Label(ReadOnlySpan<byte> Label, Color? Color = null)
    {
        AssertNotEnded();
        if (!DebugEnabled) return;
        var cmd = new FCmdLabel
        {
            Base = { Type = FCmdType.Label },
            StringIndex = AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str8,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { Label = cmd });
    }

    #endregion

    #region Scope

    public DebugScope2 Scope(string Name, Color? Color = null)
    {
        AssertNotEnded();
        if (!DebugEnabled) return new(this);
        var cmd = new FCmdBeginScope
        {
            Base = { Type = FCmdType.BeginScope },
            StringIndex = AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str16,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { BeginScope = cmd });
        m_debug_scope_count++;
        return new(this);
    }

    public DebugScope2 Scope(ReadOnlySpan<byte> Name, Color? Color = null)
    {
        AssertNotEnded();
        if (!DebugEnabled) return new(this);
        var cmd = new FCmdBeginScope
        {
            Base = { Type = FCmdType.BeginScope },
            StringIndex = AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str8,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { BeginScope = cmd });
        m_debug_scope_count++;
        return new(this);
    }

    #endregion

    #endregion

    #region PreparePresent

    public void PreparePresent(GpuOutput2 Output)
    {
        AssertNotEnded();
        Output.AssertSameIsolate(Isolate);
        var cmd = new FCmdPreparePresent
        {
            Base = { Type = FCmdType.PreparePresent },
            Output = AddResource(Output),
        };
        Data.Commands.Add(new() { PreparePresent = cmd });
    }

    #endregion

    #region ClearColor

    public void ClearColor(IRtv Image, ReadOnlySpan<URect> Rects = default) =>
        ClearColor(Image, new(0, 0, 0, 1), Rects);

    public void ClearColor(IRtv Image, Color Color, ReadOnlySpan<URect> Rects = default)
    {
        AssertNotEnded();
        Image.AssertSameIsolate(Isolate);
        if (!Image.TryRtv()) throw new ArgumentException($"Resource {Image} cannot be used as Rtv");
        var cmd = new FCmdClearColor
        {
            Base = { Type = FCmdType.ClearColor },
            RectCount = (uint)Rects.Length,
            RectIndex = Rects.Length <= 0 ? 0 : AddRects(Rects),
            Image = AddResource(Image.Resource),
            Color = Unsafe.BitCast<Color, FCmdClearColor._Color_e__FixedBuffer>(Color),
        };
        Data.Commands.Add(new() { ClearColor = cmd });
    }

    #endregion

    #region BufferCopy

    public void Copy(
        GpuBuffer Dst,
        GpuBuffer Src,
        ulong DstOffset = 0,
        ulong SrcOffset = 0,
        ulong Size = ulong.MaxValue
    )
    {
        AssertNotEnded();
        Dst.AssertSameIsolate(Isolate);
        Src.AssertSameIsolate(Isolate);
        if (Size != ulong.MaxValue)
        {
            if (DstOffset + Size > Dst.Size) throw new ArgumentException("The copy range exceeds the buffer range");
            if (SrcOffset + Size > Src.Size) throw new ArgumentException("The copy range exceeds the buffer range");
        }
        else if (Dst == Src || Dst.Size != Src.Size) Size = Math.Min(Dst.Size, Src.Size);
        var cmd = new FCmdBufferCopy
        {
            Base = { Type = FCmdType.BufferCopy },
            RangeIndex = (uint)Data.PayloadBufferCopyRange.LongLength,
            Dst = { Buffer = AddResource(Dst) },
            DstType = FBufferRefType2.Buffer,
            Src = { Buffer = AddResource(Src) },
            SrcType = FBufferRefType2.Buffer,
        };
        Data.PayloadBufferCopyRange.Add(
            new()
            {
                Size = Size,
                DstOffset = DstOffset,
                SrcOffset = SrcOffset,
            }
        );
        Data.Commands.Add(new() { BufferCopy = cmd });
    }

    #endregion

    #region BufferUpload

    public void Upload<T>(
        GpuBuffer Dst,
        ReadOnlySpan<T> Data,
        ulong DstOffset = 0
    ) where T : unmanaged => Upload(Dst, MemoryMarshal.AsBytes(Data), DstOffset);

    public void Upload(
        GpuBuffer Dst,
        ReadOnlySpan<byte> Data,
        ulong DstOffset = 0
    )
    {
        AssertNotEnded();
        Dst.AssertSameIsolate(Isolate);
        if (Data.Length == 0) return;
        var Size = Math.Min(Dst.Size, (uint)Data.Length);
        var src = WriteToUpload(Data);
        var cmd = new FCmdBufferCopy
        {
            Base = { Type = FCmdType.BufferCopy },
            RangeIndex = (uint)this.Data.PayloadBufferCopyRange.LongLength,
            Dst = { Buffer = AddResource(Dst) },
            DstType = FBufferRefType2.Buffer,
            Src = { Upload = src },
            SrcType = FBufferRefType2.Upload,
        };
        this.Data.PayloadBufferCopyRange.Add(
            new()
            {
                Size = Size,
                DstOffset = DstOffset,
                SrcOffset = src.Offset,
            }
        );
        this.Data.Commands.Add(new() { BufferCopy = cmd });
    }

    public void Upload(
        GpuBuffer Dst,
        UploadLoc2 Loc,
        ulong DstOffset = 0
    )
    {
        AssertNotEnded();
        Dst.AssertSameIsolate(Isolate);
        if (Loc.RecordId != Data.Id)
            throw new ArgumentException("Cannot use UploadLoc different GpuRecord");
        if (Loc.RecordVersion != Data.Version)
            throw new ArgumentException("An attempt was made to use an expired upload location");
        var Size = Math.Min(Dst.Size, Loc.Size);
        var cmd = new FCmdBufferCopy
        {
            Base = { Type = FCmdType.BufferCopy },
            RangeIndex = (uint)Data.PayloadBufferCopyRange.LongLength,
            Dst = { Buffer = AddResource(Dst) },
            DstType = FBufferRefType2.Buffer,
            Src = { Upload = Loc },
            SrcType = FBufferRefType2.Upload,
        };
        Data.PayloadBufferCopyRange.Add(
            new()
            {
                Size = Size,
                DstOffset = DstOffset,
                SrcOffset = Loc.Offset,
            }
        );
        Data.Commands.Add(new() { BufferCopy = cmd });
    }

    #endregion

    #region Render

    public RenderScope2 Render(
        ReadOnlySpan<RenderInfo.RtvInfo> Rtvs, RenderInfo.DsvInfo? Dsv = null,
        bool AutoViewportScissor = true,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    ) => Render(new(Rtvs, Dsv), AutoViewportScissor, Name, Name8);
    public RenderScope2 Render(
        RenderInfo.DsvInfo Dsv, ReadOnlySpan<RenderInfo.RtvInfo> Rtvs = default,
        bool AutoViewportScissor = true,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    ) => Render(new(Rtvs, Dsv), AutoViewportScissor, Name, Name8);
    public RenderScope2 Render(
        in RenderInfo Info,
        bool AutoViewportScissor = true,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        AssertNotEnded();

        #region Check Nested

        if (m_in_render_or_compute_scope) throw new InvalidOperationException("Cannot nest Render or Compute scopes");
        m_in_render_or_compute_scope = true;

        #endregion

        #region Check Args

        if (Info.Dsv == null && Info.Rtvs.Length == 0) throw new ArgumentException("No any rtv or dsv provided");
        if (Info.Rtvs.Length > 8) throw new ArgumentException("Too many rtvs provided, it can't be more than 8");

        #endregion

        #region Cmd

        var info_index = (uint)Data.PayloadRenderInfo.LongLength;
        var cmd = new FCmdRender
        {
            Base = { Type = FCmdType.Render },
            InfoIndex = info_index,
        };

        #endregion

        #region Init

        var has_dsv = Info.Dsv.HasValue;
        var dsv = Info.Dsv ?? default;
        if (has_dsv) dsv.View!.AssertSameIsolate(Isolate);

        #endregion

        #region Info

        FRenderInfo2 info = new()
        {
            Dsv = has_dsv ? AddResource(dsv.View!.Resource) : default,
            NumRtv = (uint)Info.Rtvs.Length,
            Depth = has_dsv && dsv.DepthLoad is { IsClear: true, Clear: var cd } ? cd : 1,
            Stencil = has_dsv && dsv.StencilLoad is { IsClear: true, Clear: var cs } ? (byte)cs : (byte)0,
        };

        #endregion

        #region DsvLoadOp

        info.DsvLoadOp[0] = has_dsv
            ? dsv.DepthLoad.Tag switch
            {
                LoadOp<float>.Tags.Load     => FLoadOp.Load,
                LoadOp<float>.Tags.Clear    => FLoadOp.Clear,
                LoadOp<float>.Tags.Discard  => FLoadOp.Discard,
                LoadOp<float>.Tags.NoAccess => FLoadOp.NoAccess,
                _                           => throw new ArgumentOutOfRangeException()
            }
            : FLoadOp.NoAccess;
        info.DsvLoadOp[1] = has_dsv
            ? dsv.StencilLoad.Tag switch
            {
                LoadOp<uint>.Tags.Load     => FLoadOp.Load,
                LoadOp<uint>.Tags.Clear    => FLoadOp.Clear,
                LoadOp<uint>.Tags.Discard  => FLoadOp.Discard,
                LoadOp<uint>.Tags.NoAccess => FLoadOp.NoAccess,
                _                          => throw new ArgumentOutOfRangeException()
            }
            : FLoadOp.NoAccess;

        #endregion

        #region DsvStoreOp

        info.DsvStoreOp[0] = has_dsv
            ? dsv.DepthStore.Tag switch
            {
                StoreOp.Tags.Store    => FStoreOp.Store,
                StoreOp.Tags.Discard  => FStoreOp.Discard,
                StoreOp.Tags.NoAccess => FStoreOp.NoAccess,
                _                     => throw new ArgumentOutOfRangeException()
            }
            : FStoreOp.NoAccess;

        info.DsvStoreOp[1] = has_dsv
            ? dsv.StencilStore.Tag switch
            {
                StoreOp.Tags.Store    => FStoreOp.Store,
                StoreOp.Tags.Discard  => FStoreOp.Discard,
                StoreOp.Tags.NoAccess => FStoreOp.NoAccess,
                _                     => throw new ArgumentOutOfRangeException()
            }
            : FStoreOp.NoAccess;

        #endregion

        #region Rtv

        var rt_size = Info.Dsv?.View.USize2d ?? Info.Rtvs[0].View.USize2d;
        for (var i = 0; i < Info.Rtvs.Length; i++)
        {
            ref readonly var rtv = ref Info.Rtvs[i];
            rtv.View.AssertSameIsolate(Isolate);
            if (!rtv.View.USize2d.Equals(rt_size))
                throw new ArgumentException("RenderTargets And DepthStencil must be the same size");
            info.Rtv[i] = AddResource(rtv.View.Resource);
            info.ResolveInfoIndex[i] = uint.MaxValue; // todo
            if (rtv.Load is { IsClear: true, Clear: var cc })
                Unsafe.As<float, Color>(ref info.Color[i * 4]) = cc;
            info.RtvLoadOp[i] = rtv.Load.Tag switch
            {
                LoadOp<Color>.Tags.Load     => FLoadOp.Load,
                LoadOp<Color>.Tags.Clear    => FLoadOp.Clear,
                LoadOp<Color>.Tags.Discard  => FLoadOp.Discard,
                LoadOp<Color>.Tags.NoAccess => FLoadOp.NoAccess,
                _                           => throw new ArgumentOutOfRangeException()
            };
            info.RtvStoreOp[i] = rtv.Store.Tag switch
            {
                StoreOp.Tags.Store    => FStoreOp.Store,
                StoreOp.Tags.Discard  => FStoreOp.Discard,
                StoreOp.Tags.NoAccess => FStoreOp.NoAccess,
                _                     => throw new ArgumentOutOfRangeException()
            };
        }

        #endregion

        #region DebugScope

        var debug_scope = false;
        if (Name != null)
        {
            debug_scope = true;
            Scope(Name);
        }
        else if (Name8.Length > 0)
        {
            debug_scope = true;
            Scope(Name8);
        }

        #endregion

        #region EndInfo

        Data.PayloadRenderInfo.Add(info);

        #endregion

        #region EndCmd

        var cmd_index = (uint)Data.Commands.LongLength;
        Data.Commands.Add(new() { Render = cmd });

        #endregion

        #region Scope

        RenderScope2 scope = new(this, info_index, cmd_index, debug_scope, m_debug_scope_count);

        #endregion

        #region AutoViewportScissor

        if (AutoViewportScissor)
        {
            scope.SetViewportScissor(
                [new UViewPort { Width = rt_size.Width, Height = rt_size.Height }],
                [new URect { Right = rt_size.Width, Bottom = rt_size.Height }]
            );
        }

        #endregion

        return scope;
    }

    #endregion

    #region Compute

    public ComputeScope2 Compute(string? Name = null, ReadOnlySpan<byte> Name8 = default)
    {
        AssertNotEnded();

        #region Check Nested

        if (m_in_render_or_compute_scope) throw new InvalidOperationException("Cannot nest Render or Compute scopes");
        m_in_render_or_compute_scope = true;

        #endregion

        #region Cmd

        var cmd = new FCmdCompute
        {
            Base = { Type = FCmdType.Compute },
        };

        #endregion

        #region DebugScope

        var debug_scope = false;
        if (Name != null)
        {
            debug_scope = true;
            Scope(Name);
        }
        else if (Name8.Length > 0)
        {
            debug_scope = true;
            Scope(Name8);
        }

        #endregion

        #region EndCmd

        var cmd_index = (uint)Data.Commands.LongLength;
        Data.Commands.Add(new() { Compute = cmd });

        #endregion

        #region Scope

        ComputeScope2 scope = new(this, cmd_index, debug_scope, m_debug_scope_count);

        #endregion

        return scope;
    }

    #endregion
}

#region DebugScope

public struct DebugScope2(GpuRecord self) : IDisposable
{
    private bool m_disposed;
    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(DebugScope2));
        m_disposed = true;
        if (!self.DebugEnabled) return;
        var cmd = new FCmdEndScope
        {
            Base = { Type = FCmdType.EndScope },
        };
        self.Data.Commands.Add(new() { EndScope = cmd });
        self.m_debug_scope_count--;
    }
}

#endregion

#region PipelineContext

internal unsafe struct PipelineContext
{
    internal ShaderPipeline? m_current_pipeline;
    internal ShaderBinding? m_current_binding;

    #region SetPipeline

    public void SetPipeline(GpuRecord self, ShaderPipeline Pipeline)
    {
        m_current_pipeline = Pipeline;
        self.AddObject(m_current_pipeline);
        var cmd = new FCmdSetPipeline
        {
            Base = { Type = FCmdType.SetPipeline },
            Pipeline = Pipeline.Ptr,
        };
        self.Data.Commands.Add(new() { SetPipeline = cmd });
        if (m_current_binding != null && m_current_binding.Layout != Pipeline.BindingLayout) m_current_binding = null;
    }

    #endregion

    #region SetBinding

    public void SetBinding(GpuRecord self, ShaderBinding Binding)
    {
        Binding.AssertSameIsolate(self.Isolate);
        if (m_current_binding == Binding) return;
        if (m_current_pipeline != null)
        {
            if (Binding.Layout != m_current_pipeline.BindingLayout)
                throw new InvalidOperationException("The binding layout is not compatible with the currently set pipeline");
        }
        m_current_binding = Binding;
        var first = self.AddObject(m_current_binding);
        var cmd = new FCmdSetBinding
        {
            Base = { Type = FCmdType.SetBinding },
            Binding = (uint)self.Data.Bindings.LongLength,
            Index = self.Data.NumSetBindings++,
        };
        self.Data.Bindings.Add(
            new()
            {
                Binding = Binding.Ptr
            }
        );
        self.Data.Commands.Add(new() { SetBinding = cmd });
        if (first) self.Data.SumMaxBindSlots += Binding.Data.SumPersistentSlots + Binding.Data.SumTransientSlots;
    }

    #endregion
}

#endregion

#region RenderScope

public unsafe struct RenderScope2(
    GpuRecord self,
    uint info_index,
    uint cmd_index,
    bool debug_scope,
    uint debug_scope_count
) : IDisposable
{
    #region Fields

    PipelineContext m_pipeline_context;
    internal bool m_disposed;

    #endregion

    #region Props

    internal ref FRenderInfo2 Info => ref self.Data.PayloadRenderInfo[info_index];
    internal ref FCmdRender Cmd => ref self.Data.Commands[cmd_index].Render;

    #endregion

    #region Dispose

    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(RenderScope2));
        m_disposed = true;
        if (self.m_debug_scope_count > debug_scope_count)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
        Cmd.CommandCount = (uint)self.m_data->Commands.LongLength - cmd_index;
        self.m_in_render_or_compute_scope = false;
        self.Data.Commands.Add(new() { Type = FCmdType.End });
        if (debug_scope) new DebugScope2(self).Dispose();
    }

    #endregion

    #region Debug

    #region Label

    public void Label(string Label, Color? Color = null) => self.Label(Label, Color);

    public void Label(ReadOnlySpan<byte> Label, Color? Color = null) => self.Label(Label, Color);

    #endregion

    #region Scope

    public DebugScope2 Scope(string Name, Color? Color = null) => self.Scope(Name, Color);

    public DebugScope2 Scope(ReadOnlySpan<byte> Name, Color? Color = null) => self.Scope(Name, Color);

    #endregion

    #endregion

    #region SetPipeline

    public void SetPipeline(ShaderPipeline Pipeline)
    {
        self.AssertNotEnded();
        if (m_pipeline_context.m_current_pipeline == Pipeline) return;
        if (!Pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Pixel))
            throw new ArgumentException("Only shaders with a Pixel stage can be used for rendering");
        m_pipeline_context.SetPipeline(self, Pipeline);
    }

    #endregion

    #region SetBinding

    public void SetBinding(ShaderBinding Binding)
    {
        self.AssertNotEnded();
        m_pipeline_context.SetBinding(self, Binding);
    }

    #endregion

    #region SetViewportScissor

    public void SetViewportScissor(
        ReadOnlySpan<UViewPort> Viewports,
        ReadOnlySpan<URect> Scissors
    )
    {
        self.AssertNotEnded();
        var cmd = new FCmdSetViewportScissor
        {
            Base = { Type = FCmdType.SetViewportScissor },
            ViewportCount = (uint)Viewports.Length,
            ScissorRectCount = (uint)Scissors.Length,
        };
        if (Viewports.Length > 0)
        {
            cmd.ViewportIndex = self.AddViewports(Viewports);
        }
        if (Scissors.Length > 0)
        {
            cmd.ScissorRectIndex = self.AddRects(Scissors);
        }
        self.Data.Commands.Add(new() { SetViewportScissor = cmd });
    }

    #endregion

    #region SetMeshBuffers

    public void SetMeshBuffers(
        MeshLayout MeshLayout,
        uint VertexStartSlot,
        ReadOnlySpan<VertexBufferRange> VertexBuffers
    ) => SetMeshBuffers(MeshLayout, FGraphicsFormat.Unknown, null, VertexStartSlot, VertexBuffers);

    public void SetMeshBuffers(
        MeshLayout MeshLayout,
        ReadOnlySpan<VertexBufferRange> VertexBuffers
    ) => SetMeshBuffers(MeshLayout, FGraphicsFormat.Unknown, null, 0, VertexBuffers);

    public void SetMeshBuffers(
        MeshLayout MeshLayout,
        FGraphicsFormat IndexFormat,
        BufferRange<IIbv>? IndexBuffer,
        uint VertexStartSlot,
        ReadOnlySpan<VertexBufferRange> VertexBuffers
    )
    {
        self.AssertNotEnded();
        self.AddObject(MeshLayout);
        var cmd = new FCmdSetMeshBuffers
        {
            Base = { Type = FCmdType.SetMeshBuffers },
            IndexFormat = IndexFormat,
            VertexStartSlot = VertexStartSlot,
            PayloadIndex = (uint)self.Data.PayloadMeshBuffers.LongLength,
        };
        var buf = new FMeshBuffers2
        {
            MeshLayout = MeshLayout.Ptr,
            VertexBufferCount = (uint)VertexBuffers.Length,
        };
        if (IndexBuffer is { } index_buffer)
        {
            index_buffer.Buffer.AssertSameIsolate(self.Isolate);
            buf.IndexBuffer = new()
            {
                Buffer = self.AddResource(index_buffer.Buffer.Resource),
                Offset = index_buffer.Offset,
                Size = index_buffer.Size == uint.MaxValue ? (uint)index_buffer.Buffer.Size : index_buffer.Size,
            };
        }
        if (VertexBuffers.Length > 0)
        {
            var index = self.Data.PayloadVertexBufferRange.LongLength;
            buf.VertexBuffersIndex = (uint)index;
            var vbs = self.Data.PayloadVertexBufferRange.UnsafeAddRange(VertexBuffers.Length);
            for (var i = 0; i < VertexBuffers.Length; i++)
            {
                var buffer = VertexBuffers[i];
                buffer.Buffer.AssertSameIsolate(self.Isolate);
                ref var dst = ref vbs[i];
                dst = new()
                {
                    Base =
                    {
                        Buffer = self.AddResource(buffer.Buffer.Resource),
                        Offset = buffer.Offset,
                        Size = buffer.Size == uint.MaxValue ? (uint)buffer.Buffer.Size : buffer.Size,
                    },
                    Index = buffer.Index,
                };
            }
        }
        self.Data.PayloadMeshBuffers.Add(buf);
        self.Data.Commands.Add(new() { SetMeshBuffers = cmd });
    }

    #endregion

    #region Draw

    public void Draw(
        uint VertexCount, uint InstanceCount = 1,
        uint FirstVertex = 0, uint FirstInstance = 0,
        ShaderBinding? Binding = null
    ) => Draw(null, false, VertexCount, InstanceCount, FirstVertex, FirstInstance, 0, Binding);

    public void Draw(
        ShaderPipeline? Pipeline,
        uint VertexCount, uint InstanceCount = 1,
        uint FirstVertex = 0, uint FirstInstance = 0,
        ShaderBinding? Binding = null
    ) => Draw(Pipeline, false, VertexCount, InstanceCount, FirstVertex, FirstInstance, 0, Binding);

    public void DrawIndexed(
        uint IndexCount, uint InstanceCount = 1,
        uint FirstIndex = 0, uint FirstInstance = 0, uint VertexOffset = 0,
        ShaderBinding? Binding = null
    ) => Draw(null, true, IndexCount, InstanceCount, FirstIndex, FirstInstance, VertexOffset, Binding);

    public void DrawIndexed(
        ShaderPipeline? Pipeline,
        uint IndexCount, uint InstanceCount = 1,
        uint FirstIndex = 0, uint FirstInstance = 0, uint VertexOffset = 0,
        ShaderBinding? Binding = null
    ) => Draw(Pipeline, true, IndexCount, InstanceCount, FirstIndex, FirstInstance, VertexOffset, Binding);

    public void Draw(
        ShaderPipeline? Pipeline, bool Indexed,
        uint VertexOrIndexCount, uint InstanceCount = 1,
        uint FirstVertexOrIndex = 0, uint FirstInstance = 0, uint VertexOffset = 0,
        ShaderBinding? Binding = null
    )
    {
        self.AssertNotEnded();
        if (Pipeline != null)
        {
            if (!Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Only Vertex shaders can use Draw");
            SetPipeline(Pipeline);
        }
        else
        {
            if (m_pipeline_context.m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_pipeline_context.m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Only shaders with a vertex stage can use Draw");
        }
        if (Binding != null) SetBinding(Binding);
        var cmd = new FCmdDraw
        {
            Base = { Type = FCmdType.Draw },
            VertexOrIndexCount = VertexOrIndexCount,
            InstanceCount = InstanceCount,
            FirstVertexOrIndex = FirstVertexOrIndex,
            FirstInstance = FirstInstance,
            VertexOffset = VertexOffset,
            Indexed = Indexed,
        };
        self.Data.Commands.Add(new() { Draw = cmd });
    }

    #endregion

    #region DispatchMesh

    public void DispatchMesh(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        ShaderBinding? Binding = null
    ) => DispatchMesh(null, GroupCountX, GroupCountY, GroupCountZ, Binding);

    public void DispatchMesh(
        ShaderPipeline? Pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        ShaderBinding? Binding = null
    )
    {
        self.AssertNotEnded();
        if (Pipeline != null)
        {
            if (!Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh))
                throw new ArgumentException("Only Mesh shaders can use DispatchMesh");
            SetPipeline(Pipeline);
        }
        else
        {
            if (m_pipeline_context.m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_pipeline_context.m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh))
                throw new ArgumentException("Only shaders with a mesh stage can use DispatchMesh");
        }
        if (Binding != null) SetBinding(Binding);
        var cmd = new FCmdDispatch
        {
            Base = { Type = FCmdType.Dispatch },
            GroupCountX = GroupCountX,
            GroupCountY = GroupCountY,
            GroupCountZ = GroupCountZ,
            Type = FDispatchType.Mesh,
        };
        self.Data.Commands.Add(new() { Dispatch = cmd });
    }

    #endregion
}

#endregion

#region ComputeScope

public unsafe struct ComputeScope2(
    GpuRecord self,
    uint cmd_index,
    bool debug_scope,
    uint debug_scope_count
) : IDisposable
{
    #region Fields

    PipelineContext m_pipeline_context;
    internal bool m_disposed;

    #endregion

    #region Props

    internal ref FCmdCompute Cmd => ref self.Data.Commands[cmd_index].Compute;

    #endregion

    #region Dispose

    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope2));
        m_disposed = true;
        if (self.m_debug_scope_count > debug_scope_count)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
        Cmd.CommandCount = (uint)self.m_data->Commands.LongLength - cmd_index;
        self.m_in_render_or_compute_scope = false;
        self.Data.Commands.Add(new() { Type = FCmdType.End });
        if (debug_scope) new DebugScope2(self).Dispose();
    }

    #endregion

    #region Debug

    #region Label

    public void Label(string Label, Color? Color = null) => self.Label(Label, Color);

    public void Label(ReadOnlySpan<byte> Label, Color? Color = null) => self.Label(Label, Color);

    #endregion

    #region Scope

    public DebugScope2 Scope(string Name, Color? Color = null) => self.Scope(Name, Color);

    public DebugScope2 Scope(ReadOnlySpan<byte> Name, Color? Color = null) => self.Scope(Name, Color);

    #endregion

    #endregion

    #region SetPipeline

    public void SetPipeline(ShaderPipeline Pipeline)
    {
        self.AssertNotEnded();
        if (m_pipeline_context.m_current_pipeline == Pipeline) return;
        if (!Pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Compute))
            throw new ArgumentException("Only shaders with a Compute stage can be used for computing");
        m_pipeline_context.SetPipeline(self, Pipeline);
    }

    #endregion

    #region SetBinding

    public void SetBinding(ShaderBinding Binding)
    {
        self.AssertNotEnded();
        m_pipeline_context.SetBinding(self, Binding);
    }

    #endregion

    #region Dispatch

    public void Dispatch(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        ShaderBinding? Binding = null
    ) => Dispatch(null, GroupCountX, GroupCountY, GroupCountZ, Binding);

    public void Dispatch(
        ShaderPipeline? Pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        ShaderBinding? Binding = null
    )
    {
        self.AssertNotEnded();
        if (Pipeline != null)
        {
            if (!Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Compute))
                throw new ArgumentException("Only Compute shaders can use Dispatch");
            SetPipeline(Pipeline);
        }
        else
        {
            if (m_pipeline_context.m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_pipeline_context.m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Compute))
                throw new ArgumentException("Only shaders with a Compute stage can use Dispatch");
        }
        if (Binding != null) SetBinding(Binding);
        var cmd = new FCmdDispatch
        {
            Base = { Type = FCmdType.Dispatch },
            GroupCountX = GroupCountX,
            GroupCountY = GroupCountY,
            GroupCountZ = GroupCountZ,
            Type = FDispatchType.Compute,
        };
        self.Data.Commands.Add(new() { Dispatch = cmd });
    }

    #endregion
}

#endregion

#region RenderInfo

[Union]
public partial struct LoadOp
{
    [UnionTemplate]
    private interface Template
    {
        void Load();
        void Discard();
        void NoAccess();
    }

    public static readonly LoadOp Load = MakeLoad();
    public static LoadOp<T> Clear<T>(T Clear) => LoadOp<T>.MakeClear(Clear);
    public static readonly LoadOp Discard = MakeDiscard();
    public static readonly LoadOp NoAccess = MakeNoAccess();
}

[Union]
public partial struct LoadOp<T>
{
    [UnionTemplate]
    private interface Template
    {
        void Load();
        T Clear();
        void Discard();
        void NoAccess();
    }

    public static readonly LoadOp<T> Load = MakeLoad();
    public static implicit operator LoadOp<T>(T Clear) => MakeClear(Clear);
    public static readonly LoadOp<T> Discard = MakeDiscard();
    public static readonly LoadOp<T> NoAccess = MakeNoAccess();

    public static implicit operator LoadOp<T>(LoadOp op) => op.Tag switch
    {
        LoadOp.Tags.Load     => Load,
        LoadOp.Tags.Discard  => Discard,
        LoadOp.Tags.NoAccess => NoAccess,
        _                    => throw new ArgumentOutOfRangeException()
    };
}

[Union]
public partial struct StoreOp
{
    [UnionTemplate]
    private interface Template
    {
        void Store();
        void Discard();
        void NoAccess();
    }

    public static readonly StoreOp Store = MakeStore();
    public static readonly StoreOp Discard = MakeDiscard();
    public static readonly StoreOp NoAccess = MakeNoAccess();
}

public ref struct RenderInfo(ReadOnlySpan<RenderInfo.RtvInfo> Rtvs, RenderInfo.DsvInfo? Dsv = null)
{
    public DsvInfo? Dsv = Dsv;
    public ReadOnlySpan<RtvInfo> Rtvs = Rtvs;

    public RenderInfo() : this([]) { }
    public RenderInfo(DsvInfo Dsv, ReadOnlySpan<RtvInfo> Rtvs = default) : this(Rtvs, Dsv) { }

    public struct DsvInfo(
        IDsv View,
        LoadOp<float> DepthLoad,
        LoadOp<uint> StencilLoad,
        StoreOp DepthStore,
        StoreOp StencilStore
    )
    {
        public IDsv View = View;
        public LoadOp<float> DepthLoad = DepthLoad;
        public LoadOp<uint> StencilLoad = StencilLoad;
        public StoreOp DepthStore = DepthStore;
        public StoreOp StencilStore = StencilStore;

        public DsvInfo(IDsv View) : this(View, LoadOp.Load, LoadOp.Load, StoreOp.Store, StoreOp.Store) { }
        public DsvInfo(IDsv View, LoadOp<float> DepthLoad, LoadOp<uint> StencilLoad)
            : this(View, DepthLoad, StencilLoad, StoreOp.Store, StoreOp.Store) { }
        public DsvInfo(IDsv View, LoadOp<float> DepthLoad)
            : this(View, DepthLoad, LoadOp.Load, StoreOp.Store, StoreOp.Store) { }
    }

    public struct RtvInfo(IRtv View, LoadOp<Color> Load, StoreOp Store)
    {
        public IRtv View = View;
        public LoadOp<Color> Load = Load;
        public StoreOp Store = Store;

        public RtvInfo(IRtv View) : this(View, LoadOp.Load, StoreOp.Store) { }

        public RtvInfo(IRtv View, LoadOp<Color> Load) : this(View, Load, StoreOp.Store) { }
    }
}

#endregion
