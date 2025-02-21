using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum CommandFlags : uint
{
    None = 0,
    DontTransition = 1 << 0,
}

public enum DispatchType : byte
{
    Auto,
    Compute,
    Mesh,
}

public sealed unsafe class CommandList
{
    #region Fields

    internal readonly GpuQueue m_queue;
    internal readonly List<FCommandItem> m_commands = new();
    internal readonly List<FResourceMeta> m_resource_metas = new();
    internal readonly Dictionary<object, int> m_resources = new();
    internal readonly HashSet<object> m_objects = new();
    internal readonly List<byte> m_payload = new();
    internal GpuOutput? m_current_output;
    internal ShaderPipeline? m_current_pipeline;

    #endregion

    #region Props

    public GpuQueue Queue => m_queue;

    #endregion

    #region Ctor

    internal CommandList(GpuQueue mQueue)
    {
        m_queue = mQueue;
    }

    #endregion

    #region Reset

    internal void Reset()
    {
        m_commands.Clear();
        m_resource_metas.Clear();
        m_resources.Clear();
        m_objects.Clear();
        m_payload.Clear();
        m_current_output = null;
    }

    #endregion

    #region AddResource

    private int AddResource<T>(T resource) where T : IView
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
        return slot;
    }

    #endregion

    #region AddObject

    /// <summary>
    /// 返回是否首次添加
    /// </summary>
    public bool AddObject(object obj) => m_objects.Add(obj);

    #endregion

    #region Commands

    #region Present

    internal void Present(GpuOutput Output, CommandFlags Flags = CommandFlags.None)
    {
        var index = AddResource(Output);
        var cmd = new FCommandPresent
        {
            Image = new(index),
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.Present,
                Flags = (FCommandFlags)Flags,
                Present = cmd,
            }
        );
        Output.UnsafeChangeState(FResourceState.Present);
    }

    #endregion

    #region ClearColor

    /// <summary>
    /// 使用 (0, 0, 0, 1) 清空 Rtv
    /// </summary>
    public void ClearColor<Rtv>(
        Rtv Image, ReadOnlySpan<URect> Rects = default,
        CommandFlags Flags = CommandFlags.None
    ) where Rtv : IRtv => ClearColor(Image, new(0, 0, 0, 1), Rects, Flags);

    /// <summary>
    /// 使用指定的颜色清空 Rtv
    /// </summary>
    public void ClearColor<Rtv>(
        Rtv Image, float4 Color, ReadOnlySpan<URect> Rects = default,
        CommandFlags Flags = CommandFlags.None
    ) where Rtv : IRtv
    {
        if (!Image.TryRtv()) throw new ArgumentException($"Resource {Image} cannot be used as Rtv");

        var index = AddResource(Image);

        var cmd = new FCommandClearColor
        {
            RectCount = (uint)Rects.Length,
            Image = new(index),
        };
        cmd.Color[0] = Color.r;
        cmd.Color[1] = Color.g;
        cmd.Color[2] = Color.b;
        cmd.Color[3] = Color.a;
        if (Rects.Length > 0)
        {
            cmd.RectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(Rects));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.ClearColor,
                Flags = (FCommandFlags)Flags,
                ClearColor = cmd,
            }
        );
        Image.UnsafeChangeState(FResourceState.RenderTarget);
    }

    #endregion

    #region ClearDepthStencil

    /// <summary>
    /// 使用指定的 1 深度 和 0 模板 清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image,
        ReadOnlySpan<URect> Rects = default,
        CommandFlags Flags = CommandFlags.None
    ) where Dsv : IDsv => ClearDepthStencil(Image, 1, 0, DsvClear.All, Rects, Flags);

    /// <summary>
    /// 使用指定的深度清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image, float Depth,
        ReadOnlySpan<URect> Rects = default,
        CommandFlags Flags = CommandFlags.None
    ) where Dsv : IDsv => ClearDepthStencil(Image, Depth, 0, DsvClear.Depth, Rects, Flags);

    /// <summary>
    /// 使用指定的模板清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image, byte Stencil,
        ReadOnlySpan<URect> Rects = default,
        CommandFlags Flags = CommandFlags.None
    ) where Dsv : IDsv => ClearDepthStencil(Image, 1, Stencil, DsvClear.Stencil, Rects, Flags);

    /// <summary>
    /// 使用指定的深度和模板清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv Image, float Depth, byte Stencil, DsvClear Clear = DsvClear.All,
        ReadOnlySpan<URect> Rects = default,
        CommandFlags Flags = CommandFlags.None
    ) where Dsv : IDsv
    {
        if (!Image.TryDsv()) throw new ArgumentException($"Resource {Image} cannot be used as Dsv");

        var index = AddResource(Image);

        var cmd = new FCommandClearDepthStencil
        {
            RectCount = (uint)Rects.Length,
            Image = new(index),
            Depth = Depth,
            Stencil = Stencil,
            Clear = (FDepthStencilClearFlags)Clear,
        };
        if (Rects.Length > 0)
        {
            cmd.RectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(Rects));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.ClearDepthStencil,
                Flags = (FCommandFlags)Flags,
                ClearDepthStencil = cmd,
            }
        );
        Image.UnsafeChangeState(FResourceState.DepthWrite);
    }

    #endregion

    #region SetRenderTargets

    public void SetRenderTargets(
        ReadOnlySpan<IRtv> Rtvs, IDsv? Dsv = null,
        bool AutoViewportScissor = true,
        CommandFlags Flags = CommandFlags.None
    )
    {
        if (Rtvs.Length == 0 && Dsv == null) throw new ArgumentException("No Rtvs or Dsv");
        var num_rtv = Math.Min(Rtvs.Length, 8);
        for (var i = 0; i < num_rtv; i++)
        {
            var rtv = Rtvs[i];
            if (!rtv.TryRtv()) throw new ArgumentException($"Resource {rtv} cannot be used as Rtv");
        }
        if (Dsv != null && !Dsv.TryDsv()) throw new ArgumentException($"Resource {Dsv} cannot be used as Dsv");

        var cmd = new FCommandSetRenderTargets
        {
            Dsv = Dsv == null ? new(uint.MaxValue) : new(AddResource(Dsv)),
            NumRtv = (uint)num_rtv,
        };
        var rt_size = Dsv?.Size ?? Rtvs[0].Size;
        for (var i = 0; i < num_rtv; i++)
        {
            cmd.Rtv[i] = new(AddResource(Rtvs[i]));
            if (!Rtvs[i].Size.Equals(rt_size))
                throw new ArgumentException($"RenderTargets And DepthStencil must be the same size");
        }
        if (AutoViewportScissor)
        {
            cmd.ViewportCount = 1;
            cmd.ScissorRectCount = 1;
            var viewport = new UViewPort
            {
                Width = rt_size.x,
                Height = rt_size.y,
            };
            var scissor = new URect
            {
                Right = rt_size.x,
                Bottom = rt_size.y,
            };
            cmd.ViewportIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(new ReadOnlySpan<UViewPort>(in viewport)));
            cmd.ScissorRectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(new ReadOnlySpan<URect>(in scissor)));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.SetRenderTargets,
                Flags = (FCommandFlags)Flags,
                SetRenderTargets = cmd,
            }
        );
        for (var i = 0; i < num_rtv; i++)
        {
            Rtvs[i].UnsafeChangeState(FResourceState.RenderTarget);
        }
        Dsv?.UnsafeChangeState(FResourceState.DepthWrite);
    }

    #endregion

    #region SetViewportScissor

    public void SetViewportScissor(
        ReadOnlySpan<UViewPort> Viewports,
        ReadOnlySpan<URect> Scissors,
        CommandFlags Flags = CommandFlags.None
    )
    {
        var cmd = new FCommandSetViewportScissor
        {
            ViewportCount = (uint)Viewports.Length,
            ScissorRectCount = (uint)Scissors.Length,
        };
        if (Viewports.Length > 0)
        {
            cmd.ViewportIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(Viewports));
        }
        if (Scissors.Length > 0)
        {
            cmd.ScissorRectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(Scissors));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.SetViewportScissor,
                Flags = (FCommandFlags)Flags,
                SetViewportScissor = cmd,
            }
        );
    }

    #endregion

    #region SetPipeline

    public void SetPipeline(ShaderPipeline Pipeline, CommandFlags Flags = CommandFlags.None)
    {
        AddObject(m_current_pipeline = Pipeline);
        var cmd = new FCommandSetPipeline
        {
            Pipeline = Pipeline.m_ptr,
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.SetPipeline,
                Flags = (FCommandFlags)Flags,
                SetPipeline = cmd,
            }
        );
    }

    #endregion

    #region SetMeshBuffers

    public void SetMeshBuffers(
        MeshLayout MeshLayout,
        uint VertexStartSlot,
        ReadOnlySpan<VertexBufferRange> VertexBuffers,
        CommandFlags Flags = CommandFlags.None
    ) => SetMeshBuffers(MeshLayout, FGraphicsFormat.Unknown, null, VertexStartSlot, VertexBuffers, Flags);

    public void SetMeshBuffers(
        MeshLayout MeshLayout,
        ReadOnlySpan<VertexBufferRange> VertexBuffers,
        CommandFlags Flags = CommandFlags.None
    ) => SetMeshBuffers(MeshLayout, FGraphicsFormat.Unknown, null, 0, VertexBuffers, Flags);

    public void SetMeshBuffers(
        MeshLayout MeshLayout,
        FGraphicsFormat IndexFormat,
        BufferRange<IIbv>? IndexBuffer,
        uint VertexStartSlot,
        ReadOnlySpan<VertexBufferRange> VertexBuffers,
        CommandFlags Flags = CommandFlags.None
    )
    {
        AddObject(MeshLayout);
        var cmd = new FCommandSetMeshBuffers
        {
            MeshLayout = MeshLayout.m_ptr,
            IndexFormat = IndexFormat,
            IndexBuffer = IndexBuffer is { } index_buffer
                ? new()
                {
                    Buffer = new(AddResource(index_buffer.Buffer)),
                    Offset = index_buffer.Offset,
                    Size = index_buffer.Size == uint.MaxValue ? (uint)index_buffer.Buffer.LongSize : index_buffer.Size,
                }
                : new()
                {
                    Buffer = new(uint.MaxValue),
                },
            VertexStartSlot = VertexStartSlot,
            VertexBufferCount = (uint)VertexBuffers.Length,
        };
        if (VertexBuffers.Length > 0)
        {
            cmd.VertexBuffersIndex = (uint)m_payload.Count;
            Span<FVertexBufferRange> vbs = stackalloc FVertexBufferRange[VertexBuffers.Length];
            for (var i = 0; i < VertexBuffers.Length; i++)
            {
                var buffer = VertexBuffers[i];
                vbs[i] = new()
                {
                    Base =
                    {
                        Buffer = new(AddResource(buffer.Buffer)),
                        Offset = buffer.Offset,
                        Size = buffer.Size == uint.MaxValue ? (uint)buffer.Buffer.LongSize : buffer.Size,
                    },
                    Index = buffer.Index,
                };
            }
            m_payload.AddRange(MemoryMarshal.AsBytes(vbs));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.SetMeshBuffers,
                Flags = (FCommandFlags)Flags,
                SetMeshBuffers = cmd,
            }
        );
        if (IndexBuffer.HasValue)
        {
            IndexBuffer.Value.Buffer.UnsafeChangeState(FResourceState.IndexBuffer);
        }
        foreach (var buffer in VertexBuffers)
        {
            buffer.Buffer.UnsafeChangeState(FResourceState.VertexBuffer);
        }
    }

    #endregion

    #region Draw

    public void Draw(
        uint VertexCount, uint InstanceCount = 1,
        uint FirstVertex = 0, uint FirstInstance = 0,
        CommandFlags Flags = CommandFlags.None
    ) => Draw(null, false, VertexCount, InstanceCount, FirstVertex, FirstInstance, 0, Flags);

    public void Draw(
        ShaderPipeline? Pipeline,
        uint VertexCount, uint InstanceCount = 1,
        uint FirstVertex = 0, uint FirstInstance = 0,
        CommandFlags Flags = CommandFlags.None
    ) => Draw(Pipeline, false, VertexCount, InstanceCount, FirstVertex, FirstInstance, 0, Flags);

    public void DrawIndexed(
        uint IndexCount, uint InstanceCount = 1,
        uint FirstIndex = 0, uint FirstInstance = 0, uint VertexOffset = 0,
        CommandFlags Flags = CommandFlags.None
    ) => Draw(null, true, IndexCount, InstanceCount, FirstIndex, FirstInstance, VertexOffset, Flags);

    public void DrawIndexed(
        ShaderPipeline? Pipeline,
        uint IndexCount, uint InstanceCount = 1,
        uint FirstIndex = 0, uint FirstInstance = 0, uint VertexOffset = 0,
        CommandFlags Flags = CommandFlags.None
    ) => Draw(Pipeline, true, IndexCount, InstanceCount, FirstIndex, FirstInstance, VertexOffset, Flags);

    public void Draw(
        ShaderPipeline? Pipeline, bool Indexed,
        uint VertexOrIndexCount, uint InstanceCount = 1,
        uint FirstVertexOrIndex = 0, uint FirstInstance = 0, uint VertexOffset = 0,
        CommandFlags Flags = CommandFlags.None
    )
    {
        if (Pipeline != null)
        {
            m_current_pipeline = Pipeline;
            if (!Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Non Vertex pipelines cannot use Draw");
            AddObject(Pipeline);
        }
        else
        {
            if (m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Non Vertex pipelines cannot use Draw");
        }
        var cmd = new FCommandDraw
        {
            Pipeline = Pipeline == null ? null : Pipeline.m_ptr,
            VertexOrIndexCount = VertexOrIndexCount,
            InstanceCount = InstanceCount,
            FirstVertexOrIndex = FirstVertexOrIndex,
            FirstInstance = FirstInstance,
            VertexOffset = VertexOffset,
            Indexed = Indexed,
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.Draw,
                Flags = (FCommandFlags)Flags,
                Draw = cmd,
            }
        );
    }

    #endregion

    #region Dispatch

    public void Dispatch(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags Flags = CommandFlags.None
    ) => Dispatch(null, DispatchType.Compute);

    public void DispatchMesh(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags Flags = CommandFlags.None
    ) => Dispatch(null, DispatchType.Mesh);

    public void Dispatch(
        ShaderPipeline Pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags Flags = CommandFlags.None
    ) => Dispatch(Pipeline, DispatchType.Auto);

    public void Dispatch(
        ShaderPipeline? Pipeline, DispatchType Type,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags Flags = CommandFlags.None
    )
    {
        if (Pipeline != null)
        {
            m_current_pipeline = Pipeline;
            if (!Pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Compute | ShaderStageFlags.Mesh))
                throw new ArgumentException("Non Mesh and Compute pipelines cannot use Dispatch");
            AddObject(Pipeline);
            if (Type == DispatchType.Auto)
            {
                if (Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Compute)) Type = DispatchType.Compute;
                else if (Pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh)) Type = DispatchType.Mesh;
                else throw new UnreachableException();
            }
        }
        else
        {
            if (m_current_pipeline == null) throw new InvalidOperationException("Pipeline is not set");
            if (!m_current_pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Compute | ShaderStageFlags.Mesh))
                throw new ArgumentException("Non Mesh and Compute pipelines cannot use Dispatch");
            if (Type == DispatchType.Auto)
            {
                if (m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Compute)) Type = DispatchType.Compute;
                else if (m_current_pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Mesh)) Type = DispatchType.Mesh;
                else throw new UnreachableException();
            }
        }
        var cmd = new FCommandDispatch
        {
            Pipeline = Pipeline == null ? null : Pipeline.m_ptr,
            GroupCountX = GroupCountX,
            GroupCountY = GroupCountY,
            GroupCountZ = GroupCountZ,
            Type = Type switch
            {
                DispatchType.Auto => throw new UnreachableException(),
                DispatchType.Compute => FDispatchType.Compute,
                DispatchType.Mesh    => FDispatchType.Mesh,
                _                    => throw new ArgumentOutOfRangeException(nameof(Type), Type, null)
            },
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.Dispatch,
                Flags = (FCommandFlags)Flags,
                Dispatch = cmd,
            }
        );
    }

    #endregion

    #region BufferCopy

    public void Copy(
        GpuBuffer Dst,
        GpuBuffer Src,
        ulong DstOffset = 0,
        ulong SrcOffset = 0,
        ulong Size = ulong.MaxValue,
        CommandFlags Flags = CommandFlags.None
    )
    {
        if (Size != ulong.MaxValue)
        {
            if (DstOffset + Size > Dst.Size) throw new ArgumentException("The copy range exceeds the buffer range");
            if (SrcOffset + Size > Src.Size) throw new ArgumentException("The copy range exceeds the buffer range");
        }
        else if (Dst == Src || Dst.Size != Src.Size) Size = Math.Min(Dst.Size, Src.Size);
        var dst = AddResource(Dst);
        var src = AddResource(Src);
        var cmd = new FCommandBufferCopy
        {
            Size = Size,
            DstOffset = DstOffset,
            SrcOffset = SrcOffset,
            DstType = FBufferRefType.Buffer,
            SrcType = FBufferRefType.Buffer,
        };
        cmd.Dst.Buffer = new(dst);
        cmd.Src.Buffer = new(src);
        m_commands.Add(
            new()
            {
                Type = FCommandType.BufferCopy,
                Flags = (FCommandFlags)Flags,
                BufferCopy = cmd,
            }
        );
        Dst.UnsafeChangeState(FResourceState.CopyDst);
        Src.UnsafeChangeState(FResourceState.CopySrc);
    }

    #endregion

    #region BufferUpload

    public void Upload<T>(
        GpuBuffer Dst,
        ReadOnlySpan<T> Data,
        ulong DstOffset = 0,
        CommandFlags Flags = CommandFlags.None
    ) where T : unmanaged => Upload(Dst, MemoryMarshal.AsBytes(Data), DstOffset, Flags);

    public void Upload(
        GpuBuffer Dst,
        ReadOnlySpan<byte> Data,
        ulong DstOffset = 0,
        CommandFlags Flags = CommandFlags.None
    )
    {
        if (Data.Length == 0) return;
        var Size = Math.Min(Dst.Size, (uint)Data.Length);
        var dst = AddResource(Dst);
        var src = m_queue.WriteToUpload(Data);
        var cmd = new FCommandBufferCopy
        {
            Size = Size,
            DstOffset = DstOffset,
            SrcOffset = src.Offset,
            DstType = FBufferRefType.Buffer,
            SrcType = FBufferRefType.Upload,
        };
        cmd.Dst.Buffer = new(dst);
        cmd.Src.Upload = new(src);
        m_commands.Add(
            new()
            {
                Type = FCommandType.BufferCopy,
                Flags = (FCommandFlags)Flags,
                BufferCopy = cmd,
            }
        );
        Dst.UnsafeChangeState(FResourceState.CopyDst);
    }

    #endregion

    #endregion
}
