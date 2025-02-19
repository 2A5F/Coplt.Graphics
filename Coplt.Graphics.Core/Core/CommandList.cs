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

    internal void Present(GpuOutput output, CommandFlags flags = CommandFlags.None)
    {
        var index = AddResource(output);
        var cmd = new FCommandPresent
        {
            Image = new(index),
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.Present,
                Flags = (FCommandFlags)flags,
                Present = cmd,
            }
        );
        output.UnsafeChangeState(FResourceState.Present);
    }

    #endregion

    #region ClearColor

    /// <summary>
    /// 使用 (0, 0, 0, 1) 清空 Rtv
    /// </summary>
    public void ClearColor<Rtv>(
        Rtv image, ReadOnlySpan<URect> rects = default,
        CommandFlags flags = CommandFlags.None
    ) where Rtv : IRtv => ClearColor(image, new(0, 0, 0, 1), rects, flags);

    /// <summary>
    /// 使用指定的颜色清空 Rtv
    /// </summary>
    public void ClearColor<Rtv>(
        Rtv image, float4 color, ReadOnlySpan<URect> rects = default,
        CommandFlags flags = CommandFlags.None
    ) where Rtv : IRtv
    {
        if (!image.TryRtv()) throw new ArgumentException($"Resource {image} cannot be used as Rtv");

        var index = AddResource(image);

        var cmd = new FCommandClearColor
        {
            RectCount = (uint)rects.Length,
            Image = new(index),
        };
        cmd.Color[0] = color.r;
        cmd.Color[1] = color.g;
        cmd.Color[2] = color.b;
        cmd.Color[3] = color.a;
        if (rects.Length > 0)
        {
            cmd.RectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(rects));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.ClearColor,
                Flags = (FCommandFlags)flags,
                ClearColor = cmd,
            }
        );
        image.UnsafeChangeState(FResourceState.RenderTarget);
    }

    #endregion

    #region ClearDepthStencil

    /// <summary>
    /// 使用指定的 1 深度 和 0 模板 清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv image,
        ReadOnlySpan<URect> rects = default, CommandFlags flags = CommandFlags.None
    ) where Dsv : IDsv => ClearDepthStencil(image, 1, 0, DsvClear.All, rects, flags);

    /// <summary>
    /// 使用指定的深度清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv image, float depth,
        ReadOnlySpan<URect> rects = default, CommandFlags flags = CommandFlags.None
    ) where Dsv : IDsv => ClearDepthStencil(image, depth, 0, DsvClear.Depth, rects, flags);

    /// <summary>
    /// 使用指定的模板清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv image, byte stencil,
        ReadOnlySpan<URect> rects = default, CommandFlags flags = CommandFlags.None
    ) where Dsv : IDsv => ClearDepthStencil(image, 1, stencil, DsvClear.Stencil, rects, flags);

    /// <summary>
    /// 使用指定的深度和模板清空 Dsv
    /// </summary>
    public void ClearDepthStencil<Dsv>(
        Dsv image, float depth, byte stencil, DsvClear clear = DsvClear.All,
        ReadOnlySpan<URect> rects = default, CommandFlags flags = CommandFlags.None
    ) where Dsv : IDsv
    {
        if (!image.TryDsv()) throw new ArgumentException($"Resource {image} cannot be used as Dsv");

        var index = AddResource(image);

        var cmd = new FCommandClearDepthStencil
        {
            RectCount = (uint)rects.Length,
            Image = new(index),
            Depth = depth,
            Stencil = stencil,
            Clear = (FDepthStencilClearFlags)clear,
        };
        if (rects.Length > 0)
        {
            cmd.RectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(rects));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.ClearDepthStencil,
                Flags = (FCommandFlags)flags,
                ClearDepthStencil = cmd,
            }
        );
        image.UnsafeChangeState(FResourceState.DepthWrite);
    }

    #endregion

    #region SetRenderTargets

    public void SetRenderTargets(
        ReadOnlySpan<IRtv> rtvs, IDsv? dsv = null,
        bool auto_viewport_scissor = true,
        CommandFlags flags = CommandFlags.None
    )
    {
        if (rtvs.Length == 0 && dsv == null) throw new ArgumentException("No Rtvs or Dsv");
        var num_rtv = Math.Min(rtvs.Length, 8);
        for (var i = 0; i < num_rtv; i++)
        {
            var rtv = rtvs[i];
            if (!rtv.TryRtv()) throw new ArgumentException($"Resource {rtv} cannot be used as Rtv");
        }
        if (dsv != null && !dsv.TryDsv()) throw new ArgumentException($"Resource {dsv} cannot be used as Dsv");

        var cmd = new FCommandSetRenderTargets
        {
            Dsv = dsv == null ? new(uint.MaxValue) : new(AddResource(dsv)),
            NumRtv = (uint)num_rtv,
        };
        var rt_size = dsv?.Size ?? rtvs[0].Size;
        for (var i = 0; i < num_rtv; i++)
        {
            cmd.Rtv[i] = new(AddResource(rtvs[i]));
            if (!rtvs[i].Size.Equals(rt_size))
                throw new ArgumentException($"RenderTargets And DepthStencil must be the same size");
        }
        if (auto_viewport_scissor)
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
                Flags = (FCommandFlags)flags,
                SetRenderTargets = cmd,
            }
        );
        for (var i = 0; i < num_rtv; i++)
        {
            rtvs[i].UnsafeChangeState(FResourceState.RenderTarget);
        }
        dsv?.UnsafeChangeState(FResourceState.DepthWrite);
    }

    #endregion

    #region SetViewportScissor

    public void SetViewportScissor(
        ReadOnlySpan<UViewPort> viewports,
        ReadOnlySpan<URect> scissors,
        CommandFlags flags = CommandFlags.None
    )
    {
        var cmd = new FCommandSetViewportScissor
        {
            ViewportCount = (uint)viewports.Length,
            ScissorRectCount = (uint)scissors.Length,
        };
        if (viewports.Length > 0)
        {
            cmd.ViewportIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(viewports));
        }
        if (scissors.Length > 0)
        {
            cmd.ScissorRectIndex = (uint)m_payload.Count;
            m_payload.AddRange(MemoryMarshal.AsBytes(scissors));
        }
        m_commands.Add(
            new()
            {
                Type = FCommandType.SetViewportScissor,
                Flags = (FCommandFlags)flags,
                SetViewportScissor = cmd,
            }
        );
    }

    #endregion

    #region SetPipeline

    public void SetPipeline(ShaderPipeline pipeline, CommandFlags flags = CommandFlags.None)
    {
        AddObject(pipeline);
        var cmd = new FCommandSetPipeline
        {
            Pipeline = pipeline.m_ptr,
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.SetPipeline,
                Flags = (FCommandFlags)flags,
                SetPipeline = cmd,
            }
        );
    }

    #endregion

    #region Draw

    public void Draw(
        uint vertex_count, uint instance_count = 1,
        uint first_vertex = 0, uint first_instance = 0,
        CommandFlags flags = CommandFlags.None
    ) => Draw(null, false, vertex_count, instance_count, first_vertex, first_instance, 0, flags);

    public void Draw(
        ShaderPipeline? pipeline,
        uint vertex_count, uint instance_count = 1,
        uint first_vertex = 0, uint first_instance = 0,
        CommandFlags flags = CommandFlags.None
    ) => Draw(pipeline, false, vertex_count, instance_count, first_vertex, first_instance, 0, flags);

    public void DrawIndexed(
        uint index_count, uint instance_count = 1,
        uint first_index = 0, uint first_instance = 0, uint vertex_offset = 0,
        CommandFlags flags = CommandFlags.None
    ) => Draw(null, true, index_count, instance_count, first_index, first_instance, vertex_offset, flags);

    public void DrawIndexed(
        ShaderPipeline? pipeline,
        uint index_count, uint instance_count = 1,
        uint first_index = 0, uint first_instance = 0, uint vertex_offset = 0,
        CommandFlags flags = CommandFlags.None
    ) => Draw(pipeline, true, index_count, instance_count, first_index, first_instance, vertex_offset, flags);

    public void Draw(
        ShaderPipeline? pipeline, bool indexed,
        uint vertex_or_index_count, uint instance_count = 1,
        uint first_vertex_or_index = 0, uint first_instance = 0, uint vertex_offset = 0,
        CommandFlags flags = CommandFlags.None
    )
    {
        if (pipeline != null) AddObject(pipeline);
        if (pipeline != null)
        {
            if (!pipeline.Shader.Stages.HasFlags(ShaderStageFlags.Vertex))
                throw new ArgumentException("Non Vertex pipelines cannot use Draw");
            AddObject(pipeline);
        }
        var cmd = new FCommandDraw
        {
            Pipeline = pipeline == null ? null : pipeline.m_ptr,
            VertexOrIndexCount = vertex_or_index_count,
            InstanceCount = instance_count,
            FirstVertexOrIndex = first_vertex_or_index,
            FirstInstance = first_instance,
            VertexOffset = vertex_offset,
            Indexed = indexed,
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.Draw,
                Flags = (FCommandFlags)flags,
                Draw = cmd,
            }
        );
    }

    #endregion

    #region Dispatch

    public void Dispatch(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags flags = CommandFlags.None
    ) => Dispatch(null, DispatchType.Compute);

    public void DispatchMesh(
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags flags = CommandFlags.None
    ) => Dispatch(null, DispatchType.Mesh);

    public void Dispatch(
        ShaderPipeline pipeline,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags flags = CommandFlags.None
    ) => Dispatch(pipeline, DispatchType.Auto);

    public void Dispatch(
        ShaderPipeline? pipeline, DispatchType type,
        uint GroupCountX = 1, uint GroupCountY = 1, uint GroupCountZ = 1,
        CommandFlags flags = CommandFlags.None
    )
    {
        if (pipeline != null)
        {
            if (!pipeline.Shader.Stages.HasAnyFlags(ShaderStageFlags.Compute | ShaderStageFlags.Mesh))
                throw new ArgumentException("Non Mesh and Compute pipelines cannot use Dispatch");
            AddObject(pipeline);
        }
        var cmd = new FCommandDispatch
        {
            Pipeline = pipeline == null ? null : pipeline.m_ptr,
            GroupCountX = GroupCountX,
            GroupCountY = GroupCountY,
            GroupCountZ = GroupCountZ,
            Type = (FDispatchType)type,
        };
        m_commands.Add(
            new()
            {
                Type = FCommandType.Dispatch,
                Flags = (FCommandFlags)flags,
                Dispatch = cmd,
            }
        );
    }

    #endregion

    #region BufferCopy

    public void BufferCopy(
        GpuBuffer Dst,
        ulong DstOffset,
        GpuBuffer Src,
        ulong SrcOffset,
        ulong Size,
        CommandFlags flags = CommandFlags.None
    )
    {
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
                Flags = (FCommandFlags)flags,
                BufferCopy = cmd,
            }
        );
        Dst.UnsafeChangeState(FResourceState.CopyDst);
        Src.UnsafeChangeState(FResourceState.CopySrc);
    }

    #endregion

    #endregion
}
