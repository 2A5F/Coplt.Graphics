using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Native;
using Coplt.Union;

namespace Coplt.Graphics.Core;

public enum DispatchType : byte
{
    Auto,
    Compute,
    Mesh,
}

/// <summary>
/// 命令列表，不支持多线程访问
/// </summary>
public sealed unsafe class CommandList
{
    #region Fields

    internal readonly GraphicsInstance m_instance;
    internal readonly GpuQueue m_queue;
    internal readonly List<FCommandItem> m_commands = new();
    internal readonly List<FRenderCommandItem> m_render_commands = new();
    internal readonly List<FComputeCommandItem> m_compute_commands = new();
    internal readonly List<FResourceMeta> m_resource_metas = new();
    internal readonly List<FRenderInfo> m_render_infos = new();
    internal readonly List<FComputeInfo> m_compute_infos = new();
    internal readonly List<FResolveInfo> m_resolve_infos = new();
    internal readonly List<FRect> m_rects = new();
    internal readonly List<FViewport> m_viewports = new();
    internal readonly List<FMeshBuffers> m_mesh_buffers = new();
    internal readonly List<FVertexBufferRange> m_vertex_buffer_ranges = new();
    internal readonly List<FBufferCopyRange> m_buffer_copy_ranges = new();
    internal readonly List<FBindItem> m_bind_items = new();
    internal readonly List<FBarrier> m_barriers = new();
    internal readonly List<byte> m_string8 = new();
    internal readonly List<char> m_string16 = new();
    internal readonly Dictionary<object, int> m_resources = new();
    internal readonly HashSet<object> m_objects = new();
    internal GpuOutput? m_current_output;
    internal bool m_in_render_or_compute_scope;
    internal int m_debug_scope_count;
    internal int m_render_debug_scope_count;
    internal int m_compute_debug_scope_count;

    #endregion

    #region Props

    public GpuQueue Queue => m_queue;
    public bool DebugEnabled => m_instance.DebugEnabled;

    #endregion

    #region Ctor

    internal CommandList(GpuQueue queue)
    {
        m_queue = queue;
        m_instance = queue.Device.Instance;
    }

    #endregion

    #region Reset

    internal void Reset()
    {
        m_commands.Clear();
        m_render_commands.Clear();
        m_compute_commands.Clear();
        m_resource_metas.Clear();
        m_render_infos.Clear();
        m_compute_infos.Clear();
        m_resolve_infos.Clear();
        m_rects.Clear();
        m_viewports.Clear();
        m_mesh_buffers.Clear();
        m_vertex_buffer_ranges.Clear();
        m_buffer_copy_ranges.Clear();
        m_bind_items.Clear();
        m_barriers.Clear();
        m_string8.Clear();
        m_string16.Clear();
        m_resources.Clear();
        m_objects.Clear();
        m_current_output = null;
    }

    #endregion

    #region Submit

    internal void BuildSubmitStruct(GpuExecutor Executor, bool NoWait)
    {
        if (m_debug_scope_count != 0)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
        if (m_in_render_or_compute_scope)
            throw new InvalidOperationException(
                "There is still Render or Compute scope not ended, please check whether Dispose is missed."
            );

        fixed (FCommandItem* p_commands = CollectionsMarshal.AsSpan(m_commands))
        fixed (FRenderCommandItem* p_render_commands = CollectionsMarshal.AsSpan(m_render_commands))
        fixed (FComputeCommandItem* p_compute_commands = CollectionsMarshal.AsSpan(m_compute_commands))
        fixed (FResourceMeta* p_resources = CollectionsMarshal.AsSpan(m_resource_metas))
        fixed (FRenderInfo* p_render_infos = CollectionsMarshal.AsSpan(m_render_infos))
        fixed (FComputeInfo* p_compute_infos = CollectionsMarshal.AsSpan(m_compute_infos))
        fixed (FResolveInfo* p_resolve_infos = CollectionsMarshal.AsSpan(m_resolve_infos))
        fixed (FRect* p_rects = CollectionsMarshal.AsSpan(m_rects))
        fixed (FViewport* p_viewports = CollectionsMarshal.AsSpan(m_viewports))
        fixed (FMeshBuffers* p_mesh_buffers = CollectionsMarshal.AsSpan(m_mesh_buffers))
        fixed (FVertexBufferRange* p_vertex_buffer_ranges = CollectionsMarshal.AsSpan(m_vertex_buffer_ranges))
        fixed (FBufferCopyRange* p_buffer_copy_ranges = CollectionsMarshal.AsSpan(m_buffer_copy_ranges))
        fixed (FBindItem* p_bind_items = CollectionsMarshal.AsSpan(m_bind_items))
        fixed (FBarrier* p_barriers = CollectionsMarshal.AsSpan(m_barriers))
        fixed (byte* p_string8 = CollectionsMarshal.AsSpan(m_string8))
        fixed (char* p_string16 = CollectionsMarshal.AsSpan(m_string16))
        {
            FCommandSubmit submit = new()
            {
                Commands = p_commands,
                RenderCommands = p_render_commands,
                ComputeCommands = p_compute_commands,
                Resources = p_resources,
                RenderInfos = p_render_infos,
                ComputeInfos = p_compute_infos,
                ResolveInfos = p_resolve_infos,
                Rects = p_rects,
                Viewports = p_viewports,
                MeshBuffers = p_mesh_buffers,
                VertexBufferRanges = p_vertex_buffer_ranges,
                BufferCopyRanges = p_buffer_copy_ranges,
                BindItems = p_bind_items,
                Barriers = p_barriers,
                Str8 = p_string8,
                Str16 = p_string16,
                CommandCount = (uint)m_commands.Count,
                ResourceCount = (uint)m_resource_metas.Count,
            };
            Queue.ActualSubmit(Executor, &submit, NoWait);
        }
    }

    #endregion

    #region AddResource

    internal FResourceRef AddResource<T>(T resource) where T : IGpuResource
    {
        if (resource.Queue != m_queue)
            throw new ArgumentException($"Resource {resource} does not belong to queue {m_queue}");
        if (resource is GpuOutput output)
        {
            if (m_current_output != null && output != m_current_output)
                throw new ArgumentException($"Multiple outputs cannot be used at the same time");
            m_current_output = output;
        }
        ref var slot = ref CollectionsMarshal.GetValueRefOrAddDefault(m_resources, resource, out var exists);
        if (!exists)
        {
            slot = m_resource_metas.Count;
            m_resource_metas.Add(resource.GetMeta());
        }
        return new(slot + 1);
    }

    #endregion

    #region AddObject

    /// <summary>
    /// 返回是否首次添加
    /// </summary>
    public bool AddObject(object obj) => m_objects.Add(obj);

    #endregion

    #region Commands

    #region Debug

    #region AddString

    internal uint AddString(string str)
    {
        var index = m_string16.Count;
        CollectionsMarshal.SetCount(m_string16, index + str.Length);
        var dst = CollectionsMarshal.AsSpan(m_string16).Slice(index, str.Length);
        str.CopyTo(dst);
        return (uint)index;
    }

    internal uint AddString(ReadOnlySpan<byte> str)
    {
        var index = m_string8.Count;
        CollectionsMarshal.SetCount(m_string8, index + str.Length);
        var dst = CollectionsMarshal.AsSpan(m_string8).Slice(index, str.Length);
        str.CopyTo(dst);
        return (uint)index;
    }

    #endregion

    #region Label

    public void Label(string Label)
    {
        if (!DebugEnabled) return;
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.Label },
            StringIndex = AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str16,
        };
        m_commands.Add(new() { Label = cmd });
    }

    public void Label(ReadOnlySpan<byte> Label)
    {
        if (!DebugEnabled) return;
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.Label },
            StringIndex = AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str8,
        };
        m_commands.Add(new() { Label = cmd });
    }

    #endregion

    #region Scope

    public DebugScope Scope(string Name)
    {
        if (!DebugEnabled) return new(this);
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.BeginScope },
            StringIndex = AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str16,
        };
        m_commands.Add(new() { Label = cmd });
        m_debug_scope_count++;
        return new(this);
    }

    public DebugScope Scope(ReadOnlySpan<byte> Name)
    {
        if (!DebugEnabled) return new(this);
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.BeginScope },
            StringIndex = AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str8,
        };
        m_commands.Add(new() { Label = cmd });
        m_debug_scope_count++;
        return new(this);
    }

    #endregion

    #endregion

    #region Present

    internal void Present(GpuOutput Output)
    {
        var cmd = new FCommandPresent
        {
            Base = { Type = FCommandType.Present },
            Image = AddResource(Output),
        };
        m_commands.Add(new() { Present = cmd });
    }

    #endregion

    #region Barrier

    public void Barrier(params ReadOnlySpan<Barrier> Barriers)
    {
        var index = m_barriers.Count;
        CollectionsMarshal.SetCount(m_barriers, index + Barriers.Length);
        var barriers = CollectionsMarshal.AsSpan(m_barriers).Slice(index, Barriers.Length);
        for (var i = 0; i < Barriers.Length; i++)
        {
            ref readonly var src = ref Barriers[i];
            ref var dst = ref barriers[i];
            FBarrier item = default;
            switch (src.Tag)
            {
            case Core.Barrier.Tags.None:
            {
                item.Type = FBarrierType.None;
                break;
            }
            case Core.Barrier.Tags.Global:
            {
                item.Type = FBarrierType.Global;
                var global = src.Global;
                item.Global = new()
                {
                    AccessBefore = global.AccessBefore.ToFFI(),
                    AccessAfter = global.AccessAfter.ToFFI(),
                    StagesBefore = global.StagesBefore.ToFFI(),
                    StagesAfter = global.StagesAfter.ToFFI(),
                };
                break;
            }
            case Core.Barrier.Tags.Buffer:
            {
                item.Type = FBarrierType.Buffer;
                var buffer = src.Buffer;
                item.Buffer = new()
                {
                    LegacyBefore = buffer.LegacyBefore.ToFFI(),
                    LegacyAfter = buffer.LegacyAfter.ToFFI(),
                    AccessBefore = buffer.AccessBefore.ToFFI(),
                    AccessAfter = buffer.AccessAfter.ToFFI(),
                    StagesBefore = buffer.StagesBefore.ToFFI(),
                    StagesAfter = buffer.StagesAfter.ToFFI(),
                    Buffer = AddResource(buffer.Buffer),
                    Offset = buffer.Offset,
                    Size = buffer.Size,
                };
                break;
            }
            case Core.Barrier.Tags.Image:
            {
                item.Type = FBarrierType.Image;
                var image = src.Image;
                item.Image = new()
                {
                    LegacyBefore = image.LegacyBefore.ToFFI(),
                    LegacyAfter = image.LegacyAfter.ToFFI(),
                    AccessBefore = image.AccessBefore.ToFFI(),
                    AccessAfter = image.AccessAfter.ToFFI(),
                    StagesBefore = image.StagesBefore.ToFFI(),
                    StagesAfter = image.StagesAfter.ToFFI(),
                    LayoutBefore = image.LayoutBefore.ToFFI(),
                    LayoutAfter = image.LayoutAfter.ToFFI(),
                    Image = AddResource(image.Image),
                    SubResourceRange = new()
                    {
                        IndexOrFirstMipLevel = image.SubResourceRange.IndexOrFirstMipLevel,
                        NumMipLevels = image.SubResourceRange.NumMipLevels,
                        FirstArraySlice = image.SubResourceRange.FirstArraySlice,
                        NumArraySlices = image.SubResourceRange.NumArraySlices,
                        FirstPlane = image.SubResourceRange.FirstPlane,
                        NumPlanes = image.SubResourceRange.NumPlanes,
                    },
                    Flags = (FImageBarrierFlags)image.Flags,
                };
                break;
            }
            default:
                throw new ArgumentOutOfRangeException();
            }
            dst = item;
        }
        var cmd = new FCommandBarrier
        {
            Base = { Type = FCommandType.Barrier },
            BarrierCount = (uint)Barriers.Length,
            BarrierIndex = (uint)index,
        };
        m_commands.Add(new() { Barrier = cmd });
    }

    #endregion

    #region ClearColor

    /// <summary>
    /// 使用 (0, 0, 0, 1) 清空 Rtv
    /// </summary>
    public void ClearColor<Rtv>(
        Rtv Image, ReadOnlySpan<URect> Rects = default
    ) where Rtv : IRtv => ClearColor(Image, new(0, 0, 0, 1), Rects);

    /// <summary>
    /// 使用指定的颜色清空 Rtv
    /// </summary>
    public void ClearColor<Rtv>(
        Rtv Image, float4 Color, ReadOnlySpan<URect> Rects = default
    ) where Rtv : IRtv
    {
        if (!Image.TryRtv()) throw new ArgumentException($"Resource {Image} cannot be used as Rtv");

        var cmd = new FCommandClearColor
        {
            Base = { Type = FCommandType.ClearColor },
            RectCount = (uint)Rects.Length,
            Image = AddResource(Image.Resource),
            Color = Unsafe.BitCast<float4, FCommandClearColor._Color_e__FixedBuffer>(Color),
        };
        if (Rects.Length > 0)
        {
            cmd.RectIndex = (uint)m_rects.Count;
            m_rects.AddRange(MemoryMarshal.Cast<URect, FRect>(Rects));
        }
        m_commands.Add(new() { ClearColor = cmd });
    }

    #endregion

    #region ClearDepthStencil

    /// <summary>
    /// 使用指定的 1 深度 和 0 模板 清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image,
        ReadOnlySpan<URect> Rects = default
    ) where Dsv : IDsv => ClearDepthStencil(Image, 1, 0, DsvClear.All, Rects);

    /// <summary>
    /// 使用指定的深度清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image, float Depth,
        ReadOnlySpan<URect> Rects = default
    ) where Dsv : IDsv => ClearDepthStencil(Image, Depth, 0, DsvClear.Depth, Rects);

    /// <summary>
    /// 使用指定的模板清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image, byte Stencil,
        ReadOnlySpan<URect> Rects = default
    ) where Dsv : IDsv => ClearDepthStencil(Image, 1, Stencil, DsvClear.Stencil, Rects);

    /// <summary>
    /// 使用指定的深度和模板清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image, float Depth, byte Stencil, DsvClear Clear = DsvClear.All,
        ReadOnlySpan<URect> Rects = default
    ) where Dsv : IDsv
    {
        if (!Image.TryDsv()) throw new ArgumentException($"Resource {Image} cannot be used as Dsv");

        var cmd = new FCommandClearDepthStencil
        {
            Base = { Type = FCommandType.ClearDepthStencil },
            RectCount = (uint)Rects.Length,
            Image = AddResource(Image.Resource),
            Depth = Depth,
            Stencil = Stencil,
            Clear = (FDepthStencilClearFlags)Clear,
        };
        if (Rects.Length > 0)
        {
            cmd.RectIndex = (uint)m_rects.Count;
            m_rects.AddRange(MemoryMarshal.Cast<URect, FRect>(Rects));
        }
        m_commands.Add(new() { ClearDepthStencil = cmd });
    }

    #endregion

    #region Bind

    public void Bind(
        ShaderBinding Binding, ReadOnlySpan<ShaderBindingSetItem> Items
    )
    {
        AddObject(Binding);
        var cmd = new FCommandBind
        {
            Base = { Type = FCommandType.Bind },
            Binding = Binding.m_ptr,
            ItemCount = (uint)Items.Length,
            ItemsIndex = 0
        };
        if (Items.Length > 0)
        {
            var index = m_bind_items.Count;
            cmd.ItemsIndex = (uint)index;
            CollectionsMarshal.SetCount(m_bind_items, m_bind_items.Count + Items.Length);
            var dst = CollectionsMarshal.AsSpan(m_bind_items).Slice(index, Items.Length);
            var defines = Binding.Layout.NativeDefines;
            var views = Binding.MutViews;
            for (var i = 0; i < Items.Length; i++)
            {
                var src = Items[i];
                ref readonly var define = ref defines[(int)src.Index];
                define.CheckCompatible(src.View, (int)src.Index);
                dst[i] = new()
                {
                    Index = src.Index,
                    View = src.View.ToFFI(),
                };
                views[(int)src.Index] = src.View;
            }
        }
        m_commands.Add(new() { Bind = cmd });
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
        if (Size != ulong.MaxValue)
        {
            if (DstOffset + Size > Dst.Size) throw new ArgumentException("The copy range exceeds the buffer range");
            if (SrcOffset + Size > Src.Size) throw new ArgumentException("The copy range exceeds the buffer range");
        }
        else if (Dst == Src || Dst.Size != Src.Size) Size = Math.Min(Dst.Size, Src.Size);
        var cmd = new FCommandBufferCopy
        {
            Base = { Type = FCommandType.BufferCopy },
            RangeIndex = (uint)m_buffer_copy_ranges.Count,
            Dst = { Buffer = AddResource(Dst) },
            DstType = FBufferRefType.Buffer,
            Src = { Buffer = AddResource(Src) },
            SrcType = FBufferRefType.Buffer,
        };
        m_buffer_copy_ranges.Add(
            new()
            {
                Size = Size,
                DstOffset = DstOffset,
                SrcOffset = SrcOffset,
            }
        );
        m_commands.Add(new() { BufferCopy = cmd });
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
        if (Data.Length == 0) return;
        var Size = Math.Min(Dst.Size, (uint)Data.Length);
        var src = m_queue.WriteToUpload(Data);
        var cmd = new FCommandBufferCopy
        {
            Base = { Type = FCommandType.BufferCopy },
            RangeIndex = (uint)m_buffer_copy_ranges.Count,
            Dst = { Buffer = AddResource(Dst) },
            DstType = FBufferRefType.Buffer,
            Src = { Upload = src },
            SrcType = FBufferRefType.Upload,
        };
        m_buffer_copy_ranges.Add(
            new()
            {
                Size = Size,
                DstOffset = DstOffset,
                SrcOffset = src.Offset,
            }
        );
        m_commands.Add(new() { BufferCopy = cmd });
    }

    public void Upload(
        GpuBuffer Dst,
        UploadLoc Loc,
        ulong DstOffset = 0
    )
    {
        if (Loc.SubmitId != m_queue.SubmitId)
            throw new ArgumentException("An attempt was made to use an expired upload location");
        var Size = Math.Min(Dst.Size, Loc.Size);
        var cmd = new FCommandBufferCopy
        {
            Base = { Type = FCommandType.BufferCopy },
            RangeIndex = (uint)m_buffer_copy_ranges.Count,
            Dst = { Buffer = AddResource(Dst) },
            DstType = FBufferRefType.Buffer,
            Src = { Upload = Loc },
            SrcType = FBufferRefType.Upload,
        };
        m_buffer_copy_ranges.Add(
            new()
            {
                Size = Size,
                DstOffset = DstOffset,
                SrcOffset = Loc.Offset,
            }
        );
        m_commands.Add(new() { BufferCopy = cmd });
    }

    #endregion

    #region Render

    public RenderScope Render(
        ReadOnlySpan<RenderInfo.RtvInfo> Rtvs, RenderInfo.DsvInfo? Dsv = null,
        bool AutoViewportScissor = true,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    ) => Render(new(Rtvs, Dsv), AutoViewportScissor, Name, Name8);
    public RenderScope Render(
        RenderInfo.DsvInfo Dsv, ReadOnlySpan<RenderInfo.RtvInfo> Rtvs = default,
        bool AutoViewportScissor = true,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    ) => Render(new(Rtvs, Dsv), AutoViewportScissor, Name, Name8);
    public RenderScope Render(
        in RenderInfo Info,
        bool AutoViewportScissor = true,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        #region Check Nested

        if (m_in_render_or_compute_scope) throw new InvalidOperationException("Cannot nest Render or Compute scopes");
        m_in_render_or_compute_scope = true;

        #endregion

        #region Check Args

        if (Info.Dsv == null && Info.Rtvs.Length == 0) throw new ArgumentException("No any rtv or dsv provided");
        if (Info.Rtvs.Length > 8) throw new ArgumentException("Too many rtvs provided, it can't be more than 8");

        #endregion

        #region Cmd

        var info_index = m_render_infos.Count;
        var cmd = new FCommandRender
        {
            Base = { Type = FCommandType.Render },
            InfoIndex = (uint)info_index,
            CommandStartIndex = (uint)m_render_commands.Count,
        };

        #endregion

        #region Init

        var has_dsv = Info.Dsv.HasValue;
        var dsv = Info.Dsv ?? default;

        #endregion

        #region Info

        FRenderInfo info = new()
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

        var rt_size = Info.Dsv?.View.Size2d ?? Info.Rtvs[0].View.Size2d;
        for (var i = 0; i < Info.Rtvs.Length; i++)
        {
            ref readonly var rtv = ref Info.Rtvs[i];
            if (!rtv.View.Size.Equals(rt_size))
                throw new ArgumentException("RenderTargets And DepthStencil must be the same size");
            info.Rtv[i] = AddResource(rtv.View.Resource);
            info.ResolveInfoIndex[i] = uint.MaxValue; // todo
            if (rtv.Load is { IsClear: true, Clear: var cc })
                Unsafe.As<float, float4>(ref info.Color[i * 4]) = cc;
            info.RtvLoadOp[i] = rtv.Load.Tag switch
            {
                LoadOp<float4>.Tags.Load     => FLoadOp.Load,
                LoadOp<float4>.Tags.Clear    => FLoadOp.Clear,
                LoadOp<float4>.Tags.Discard  => FLoadOp.Discard,
                LoadOp<float4>.Tags.NoAccess => FLoadOp.NoAccess,
                _                            => throw new ArgumentOutOfRangeException()
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

        #region EndInfo

        m_render_infos.Add(info);

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

        #region Add

        m_commands.Add(new() { Render = cmd });

        #endregion

        #region Scope

        RenderScope scope = new(this, info_index, m_render_commands.Count, m_render_commands, debug_scope);

        #endregion

        #region AutoViewportScissor

        if (AutoViewportScissor)
        {
            scope.SetViewportScissor(
                [new UViewPort { Width = rt_size.x, Height = rt_size.y }],
                [new URect { Right = rt_size.x, Bottom = rt_size.y }]
            );
        }

        #endregion

        return scope;
    }

    #endregion

    #region Compute

    public ComputeScope Compute(string? Name = null, ReadOnlySpan<byte> Name8 = default)
    {
        #region Check Nested

        if (m_in_render_or_compute_scope) throw new InvalidOperationException("Cannot nest Render or Compute scopes");
        m_in_render_or_compute_scope = true;

        #endregion

        #region Debug

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

        #region Cmd and Scope

        var info_index = m_compute_infos.Count;
        m_compute_infos.Add(new());

        var cmd = new FCommandCompute
        {
            Base = { Type = FCommandType.Compute },
            InfoIndex = (uint)info_index,
            CommandStartIndex = (uint)m_compute_commands.Count,
        };
        m_commands.Add(new() { Compute = cmd });
        return new(this, info_index, m_compute_commands.Count, m_compute_commands, debug_scope);

        #endregion
    }

    #endregion

    #endregion
}

#region DebugScope

public struct DebugScope(CommandList self) : IDisposable
{
    private bool m_disposed;
    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope));
        m_disposed = true;
        if (!self.DebugEnabled) return;
        var cmd = new FCommandEndScope
        {
            Base = { Type = FCommandType.EndScope },
        };
        self.m_commands.Add(new() { EndScope = cmd });
        self.m_debug_scope_count--;
    }
}

public struct RenderDebugScope(CommandList self) : IDisposable
{
    private bool m_disposed;
    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope));
        m_disposed = true;
        if (!self.DebugEnabled) return;
        var cmd = new FCommandEndScope
        {
            Base = { Type = FCommandType.EndScope },
        };
        self.m_render_commands.Add(new() { EndScope = cmd });
        self.m_render_debug_scope_count--;
    }
}

public struct ComputeDebugScope(CommandList self) : IDisposable
{
    private bool m_disposed;
    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope));
        m_disposed = true;
        if (!self.DebugEnabled) return;
        var cmd = new FCommandEndScope
        {
            Base = { Type = FCommandType.EndScope },
        };
        self.m_compute_commands.Add(new() { EndScope = cmd });
        self.m_compute_debug_scope_count--;
    }
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

    public struct RtvInfo(IRtv View, LoadOp<float4> Load, StoreOp Store)
    {
        public IRtv View = View;
        public LoadOp<float4> Load = Load;
        public StoreOp Store = Store;

        public RtvInfo(IRtv View) : this(View, LoadOp.Load, StoreOp.Store) { }

        public RtvInfo(IRtv View, LoadOp<float4> Load) : this(View, Load, StoreOp.Store) { }
    }
}

#endregion

#region RenderScope

public unsafe struct RenderScope(
    CommandList self,
    int info_index,
    int cmd_index,
    List<FRenderCommandItem> m_commands,
    bool debug_scope
) : IDisposable
{
    #region Fields

    private ShaderPipeline? m_current_pipeline;
    private ShaderBinding? m_current_binding;
    private bool m_disposed;

    #endregion

    #region Props

    internal ref FRenderInfo Info => ref CollectionsMarshal.AsSpan(self.m_render_infos)[info_index];

    #endregion

    #region Dispose

    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope));
        m_disposed = true;
        if (self.m_render_debug_scope_count != 0)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
        Info.CommandCount = (uint)m_commands.Count - (uint)cmd_index;
        self.m_render_commands.Add(new() { Type = FCommandType.End });
        self.m_in_render_or_compute_scope = false;
        if (debug_scope) new DebugScope(self).Dispose();
    }

    #endregion

    #region Debug

    #region Label

    public void Label(string Label)
    {
        if (!self.DebugEnabled) return;
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.Label },
            StringIndex = self.AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str16,
        };
        m_commands.Add(new() { Label = cmd });
    }

    public void Label(ReadOnlySpan<byte> Label)
    {
        if (!self.DebugEnabled) return;
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.EndScope },
            StringIndex = self.AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str8,
        };
        m_commands.Add(new() { Label = cmd });
    }

    #endregion

    #region Scope

    public RenderDebugScope Scope(string Name)
    {
        if (!self.DebugEnabled) return new(self);
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.EndScope },
            StringIndex = self.AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str16,
        };
        m_commands.Add(new() { Label = cmd });
        self.m_render_debug_scope_count++;
        return new(self);
    }

    public RenderDebugScope Scope(ReadOnlySpan<byte> Name)
    {
        if (!self.DebugEnabled) return new(self);
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.EndScope },
            StringIndex = self.AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str8,
        };
        m_commands.Add(new() { Label = cmd });
        self.m_render_debug_scope_count++;
        return new(self);
    }

    #endregion

    #endregion

    #region SetPipeline

    public void SetPipeline(ShaderPipeline Pipeline)
    {
        if (m_current_pipeline == Pipeline) return;
        self.AddObject(m_current_pipeline = Pipeline);
        if (m_current_binding != null)
        {
            if (Pipeline.Shader.Layout != m_current_binding.Layout)
                m_current_binding = null;
        }
        var cmd = new FCommandSetPipeline
        {
            Base = { Type = FCommandType.SetPipeline },
            Pipeline = Pipeline.m_ptr,
        };
        m_commands.Add(new() { SetPipeline = cmd });
    }

    #endregion

    #region SetBinding

    public void SetBinding(ShaderBinding Binding)
    {
        if (m_current_binding == Binding) return;
        if (m_current_pipeline != null)
        {
            if (m_current_pipeline.Shader.Layout != Binding.Layout)
                throw new ArgumentException("This binding is incompatible with the current pipeline");
        }
        self.AddObject(m_current_binding = Binding);
        var cmd = new FCommandSetBinding
        {
            Base = { Type = FCommandType.SetBinding },
            Binding = Binding.m_ptr,
        };
        m_commands.Add(new() { SetBinding = cmd });
    }

    #endregion

    #region SetViewportScissor

    public void SetViewportScissor(
        ReadOnlySpan<UViewPort> Viewports,
        ReadOnlySpan<URect> Scissors
    )
    {
        var cmd = new FCommandSetViewportScissor
        {
            Base = { Type = FCommandType.SetViewportScissor },
            ViewportCount = (uint)Viewports.Length,
            ScissorRectCount = (uint)Scissors.Length,
        };
        if (Viewports.Length > 0)
        {
            cmd.ViewportIndex = (uint)self.m_viewports.Count;
            self.m_viewports.AddRange(MemoryMarshal.Cast<UViewPort, FViewport>(Viewports));
        }
        if (Scissors.Length > 0)
        {
            cmd.ScissorRectIndex = (uint)self.m_rects.Count;
            self.m_rects.AddRange(MemoryMarshal.Cast<URect, FRect>(Scissors));
        }
        m_commands.Add(new() { SetViewportScissor = cmd });
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
        self.AddObject(MeshLayout);
        var cmd = new FCommandSetMeshBuffers
        {
            Base = { Type = FCommandType.SetMeshBuffers },
            IndexFormat = IndexFormat,
            VertexStartSlot = VertexStartSlot,
            PayloadIndex = (uint)self.m_mesh_buffers.Count,
        };
        var buf = new FMeshBuffers
        {
            MeshLayout = MeshLayout.m_ptr,
            IndexBuffer = IndexBuffer is { } index_buffer
                ? new()
                {
                    Buffer = self.AddResource(index_buffer.Buffer.Resource),
                    Offset = index_buffer.Offset,
                    Size = index_buffer.Size == uint.MaxValue ? (uint)index_buffer.Buffer.Size : index_buffer.Size,
                }
                : new()
                {
                    Buffer = new(uint.MaxValue),
                },
            VertexBufferCount = (uint)VertexBuffers.Length,
        };
        if (VertexBuffers.Length > 0)
        {
            var index = self.m_vertex_buffer_ranges.Count;
            buf.VertexBuffersIndex = (uint)index;
            CollectionsMarshal.SetCount(
                self.m_vertex_buffer_ranges, self.m_vertex_buffer_ranges.Count + VertexBuffers.Length
            );
            var vbs = CollectionsMarshal.AsSpan(self.m_vertex_buffer_ranges).Slice(index, VertexBuffers.Length);
            for (var i = 0; i < VertexBuffers.Length; i++)
            {
                var buffer = VertexBuffers[i];
                vbs[i] = new()
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
        self.m_mesh_buffers.Add(buf);
        m_commands.Add(new() { SetMeshBuffers = cmd });
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
        if (Pipeline != null)
        {
            if (!Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Only Vertex shaders can use Draw");
            SetPipeline(Pipeline);
        }
        else
        {
            if (m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Only Vertex shaders can use Draw");
        }
        if (Binding != null) SetBinding(Binding);
        var cmd = new FCommandDraw
        {
            Base = { Type = FCommandType.Draw },
            VertexOrIndexCount = VertexOrIndexCount,
            InstanceCount = InstanceCount,
            FirstVertexOrIndex = FirstVertexOrIndex,
            FirstInstance = FirstInstance,
            VertexOffset = VertexOffset,
            Indexed = Indexed,
        };
        m_commands.Add(new() { Draw = cmd });
    }

    #endregion

    #region DispatchMesh

    public void DispatchMesh(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    ) => DispatchMesh(null, GroupCountX, GroupCountY, GroupCountZ);

    public void DispatchMesh(
        ShaderPipeline? Pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    )
    {
        if (Pipeline != null)
        {
            if (!Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh))
                throw new ArgumentException("Only Mesh shaders can use DispatchMesh");
            SetPipeline(Pipeline);
        }
        else
        {
            if (m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh))
                throw new ArgumentException("Only Mesh shaders can use DispatchMesh");
        }
        var cmd = new FCommandDispatch
        {
            Base = { Type = FCommandType.Draw },
            GroupCountX = GroupCountX,
            GroupCountY = GroupCountY,
            GroupCountZ = GroupCountZ,
            Type = FDispatchType.Mesh,
        };
        m_commands.Add(new() { Dispatch = cmd });
    }

    #endregion
}

#endregion

#region ComputeScope

public unsafe struct ComputeScope(
    CommandList self,
    int info_index,
    int cmd_index,
    List<FComputeCommandItem> m_commands,
    bool debug_scope
) : IDisposable
{
    #region Fields

    private ShaderPipeline? m_current_pipeline;
    private ShaderBinding? m_current_binding;
    private bool m_disposed;

    #endregion

    #region Props

    internal ref FComputeInfo Info => ref CollectionsMarshal.AsSpan(self.m_compute_infos)[info_index];

    #endregion

    #region Dispose

    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope));
        m_disposed = true;
        if (self.m_compute_debug_scope_count != 0)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
        Info.CommandCount = (uint)m_commands.Count - (uint)cmd_index;
        self.m_compute_commands.Add(new() { Type = FCommandType.End });
        self.m_in_render_or_compute_scope = false;
        if (debug_scope) new DebugScope(self).Dispose();
    }

    #endregion

    #region Debug

    #region Label

    public void Label(string Label)
    {
        if (!self.DebugEnabled) return;
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.Label },
            StringIndex = self.AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str16,
        };
        m_commands.Add(new() { Label = cmd });
    }

    public void Label(ReadOnlySpan<byte> Label)
    {
        if (!self.DebugEnabled) return;
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.EndScope },
            StringIndex = self.AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str8,
        };
        m_commands.Add(new() { Label = cmd });
    }

    #endregion

    #region Scope

    public ComputeDebugScope Scope(string Name)
    {
        if (!self.DebugEnabled) return new(self);
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.EndScope },
            StringIndex = self.AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str16,
        };
        m_commands.Add(new() { Label = cmd });
        self.m_compute_debug_scope_count++;
        return new(self);
    }

    public ComputeDebugScope Scope(ReadOnlySpan<byte> Name)
    {
        if (!self.DebugEnabled) return new(self);
        var cmd = new FCommandLabel
        {
            Base = { Type = FCommandType.EndScope },
            StringIndex = self.AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str8,
        };
        m_commands.Add(new() { Label = cmd });
        self.m_compute_debug_scope_count++;
        return new(self);
    }

    #endregion

    #endregion

    #region SetPipeline

    public void SetPipeline(ShaderPipeline Pipeline)
    {
        if (m_current_pipeline == Pipeline) return;
        self.AddObject(m_current_pipeline = Pipeline);
        if (m_current_binding != null)
        {
            if (Pipeline.Shader.Layout != m_current_binding.Layout)
                m_current_binding = null;
        }
        var cmd = new FCommandSetPipeline
        {
            Base = { Type = FCommandType.SetPipeline },
            Pipeline = Pipeline.m_ptr,
        };
        m_commands.Add(new() { SetPipeline = cmd });
    }

    #endregion

    #region SetBinding

    public void SetBinding(ShaderBinding Binding)
    {
        if (m_current_binding == Binding) return;
        if (m_current_pipeline != null)
        {
            if (m_current_pipeline.Shader.Layout != Binding.Layout)
                throw new ArgumentException("This binding is incompatible with the current pipeline");
        }
        self.AddObject(m_current_binding = Binding);
        var cmd = new FCommandSetBinding
        {
            Base = { Type = FCommandType.SetBinding },
            Binding = Binding.m_ptr,
        };
        m_commands.Add(new() { SetBinding = cmd });
    }

    #endregion

    #region Dispatch

    public void Dispatch(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    ) => Dispatch(null, DispatchType.Compute, GroupCountX, GroupCountY, GroupCountZ);

    public void DispatchMesh(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    ) => Dispatch(null, DispatchType.Mesh, GroupCountX, GroupCountY, GroupCountZ);

    public void Dispatch(
        ShaderPipeline Pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    ) => Dispatch(Pipeline, DispatchType.Auto, GroupCountX, GroupCountY, GroupCountZ);

    public void DispatchMesh(
        ShaderPipeline Pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    ) => Dispatch(Pipeline, DispatchType.Mesh, GroupCountX, GroupCountY, GroupCountZ);

    public void Dispatch(
        ShaderPipeline? Pipeline, DispatchType Type,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1
    )
    {
        if (Pipeline != null)
        {
            if (!Pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Compute | ShaderStageFlags.Mesh))
                throw new ArgumentException("Only Mesh and Compute shaders can use Dispatch/DispatchMesh");
            SetPipeline(Pipeline);
        }
        else
        {
            if (m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_current_pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Compute | ShaderStageFlags.Mesh))
                throw new ArgumentException("Only Mesh and Compute shaders can use Dispatch/DispatchMesh");
        }
        if (Type == DispatchType.Auto)
        {
            if (m_current_pipeline!.Shader.Stages.HasFlags(ShaderStageFlags.Compute)) Type = DispatchType.Compute;
            else if (m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh)) Type = DispatchType.Mesh;
            else throw new UnreachableException();
        }
        else if (Type == DispatchType.Mesh)
        {
            if (!m_current_pipeline!.Shader.Stages.HasFlags(ShaderStageFlags.Mesh))
                throw new ArgumentException("Only Mesh shaders can use DispatchMesh");
        }
        else if (Type == DispatchType.Compute)
        {
            if (!m_current_pipeline!.Shader.Stages.HasFlags(ShaderStageFlags.Compute))
                throw new ArgumentException("Only Compute shaders can use Dispatch");
        }
        var cmd = new FCommandDispatch
        {
            Base = { Type = FCommandType.SetBinding },
            GroupCountX = GroupCountX,
            GroupCountY = GroupCountY,
            GroupCountZ = GroupCountZ,
            Type = Type switch
            {
                DispatchType.Auto    => throw new UnreachableException(),
                DispatchType.Compute => FDispatchType.Compute,
                DispatchType.Mesh    => FDispatchType.Mesh,
                _                    => throw new ArgumentOutOfRangeException(nameof(Type), Type, null)
            },
        };
        m_commands.Add(new() { Dispatch = cmd });
    }

    #endregion
}

#endregion
