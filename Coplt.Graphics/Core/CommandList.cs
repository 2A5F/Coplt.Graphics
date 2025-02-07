using System.Runtime.InteropServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics;

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
    }

    #endregion

    #region AddResource

    private int AddResource<T>(T resource) where T : IView
    {
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
    }

    #endregion

    #endregion
}
