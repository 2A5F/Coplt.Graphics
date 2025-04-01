using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct MeshBufferElement(
    SlotId SlotId,
    GraphicsFormat Format,
    uint BufferIndex = 0,
    uint SlotIndex = 0,
    uint Offset = 0,
    uint InstanceRepeat = 0
)
{
    /// <summary>
    /// 属于 mesh 中的第几个 buffer
    /// </summary>
    public uint BufferIndex = BufferIndex;
    /// <summary>
    /// 与 <see cref="ShaderInputLayoutElement"/> 对应的 Id
    /// </summary>
    public SlotId SlotId = SlotId;
    /// <summary>
    /// 是 Slot 中第几个项，对应 dx 的 SemanticIndex
    /// </summary>
    public uint SlotIndex = SlotIndex;
    /// <summary>
    /// 元素格式
    /// </summary>
    public GraphicsFormat Format = Format;
    /// <summary>
    /// 元素在间隔中的偏移
    /// </summary>
    public uint Offset = Offset;
    /// <summary>
    /// 每次实例数据可重复几次，对应 dx 的 InstanceDataStepRate；对于实例(<see cref="MeshBufferElementRate.Instance"/>) 0 相当于 1；仅 dx 支持
    /// </summary>
    public uint InstanceRepeat = InstanceRepeat;
}

public record struct MeshBufferDefine(uint Stride, MeshBufferElementRate Rate = MeshBufferElementRate.Vertex)
{
    /// <summary>
    /// 元素间隔
    /// </summary>
    public uint Stride = Stride;
    /// <summary>
    /// 元素频率，指示是按顶点还是按实例
    /// </summary>
    public MeshBufferElementRate Rate = Rate;
}

/// <summary>
/// 元素频率，指示是按顶点还是按实例
/// </summary>
public enum MeshBufferElementRate : byte
{
    Vertex,
    Instance,
}

public sealed unsafe class MeshLayout : DeviceChild
{
    #region Fields

    internal readonly FMeshBufferDefine* m_native_buffers;
    internal readonly FMeshBufferElement* m_native_elements;
    internal readonly uint m_native_buffers_count;
    internal readonly uint m_native_elements_count;

    #endregion

    #region Props

    public new FMeshLayout* Ptr => (FMeshLayout*)m_ptr;
    public ReadOnlySpan<MeshBufferDefine> Buffers => new(m_native_buffers, (int)m_native_buffers_count);
    public ReadOnlySpan<MeshBufferElement> Elements => new(m_native_elements, (int)m_native_elements_count);

    #endregion

    #region Ctor

    internal MeshLayout(FMeshLayout* ptr, string? name, GpuDevice device) : base((FGpuObject*)ptr, name, device)
    {
        if (m_ptr != null)
        {
            {
                uint count;
                m_native_buffers = ptr->GetBuffers(&count);
                m_native_buffers_count = count;
            }
            {
                uint count;
                m_native_elements = ptr->GetElements(&count);
                m_native_elements_count = count;
            }
        }
    }

    #endregion
}
