using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum CommandFlags : uint
{
    None = 0,
    DontTransition = 1 << 0,
}

public sealed class CommandList
{
    #region Fields

    internal readonly GpuQueue m_queue;
    internal readonly List<FCommandItem> m_commands = new();
    internal readonly List<FResourceMeta> m_resource_metas = new();
    internal readonly Dictionary<object, int> m_resources = new();
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
        m_payload.Clear();
        m_current_output = null;
    }

    #endregion

    #region AddResource

    private int AddResource<T>(T resource) where T : IView
    {
        if (resource.Queue != m_queue) throw new ArgumentException($"Resource {resource} does not belong to queue {m_queue}");
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

    #region Commands

    #region Present

    internal void Present(GpuOutput output)
    {
        var index = AddResource(output);
        var cmd = new FCommandPresent
        {
            Image = new(index),
        };
        m_commands.Add(new()
        {
            Type = FCommandType.Present,
            Flags = FCommandFlags.None,
            Present = cmd,
        });
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
        m_commands.Add(new()
        {
            Type = FCommandType.ClearColor,
            Flags = (FCommandFlags)flags,
            ClearColor = cmd,
        });
        image.UnsafeChangeState(FResourceState.RenderTarget);
    }

    #endregion

    #endregion
}
