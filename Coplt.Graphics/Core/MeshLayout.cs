using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct MeshBufferElement
{
    /// <summary>
    /// 属于 mesh 中的第几个 buffer
    /// </summary>
    public uint BufferIndex;
    /// <summary>
    /// 与 <see cref="ShaderInputLayoutElement"/> 对应的 Id
    /// </summary>
    public uint SlotId;
    /// <summary>
    /// 是 Slot 中第几个项，对应 dx 的 SemanticIndex
    /// </summary>
    public uint SlotIndex;
    /// <summary>
    /// 元素格式
    /// </summary>
    public GraphicsFormat Format;
    /// <summary>
    /// 元素在间隔中的偏移
    /// </summary>
    public uint Offset;
    /// <summary>
    /// 每次实例数据可重复几次，对应 dx 的 InstanceDataStepRate；对于实例(<see cref="MeshBufferElementRate.Instance"/>) 0 相当于 1；仅 dx 支持
    /// </summary>
    public uint InstanceRepeat;
}

public record struct MeshBufferDefine
{
    /// <summary>
    /// 元素间隔
    /// </summary>
    public uint Stride;
    /// <summary>
    /// 元素频率，指示是按顶点还是按实例
    /// </summary>
    public MeshBufferElementRate Rate;
}

/// <summary>
/// 元素频率，指示是按顶点还是按实例
/// </summary>
public enum MeshBufferElementRate : byte
{
    Vertex,
    Instance,
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class MeshLayout
{
    #region Fields

    internal FMeshLayout* m_ptr;
    internal string? m_name;
    internal readonly FMeshBufferDefine* m_native_buffers;
    internal readonly FMeshBufferElement* m_native_elements;
    internal readonly uint m_native_buffers_count;
    internal readonly uint m_native_elements_count;

    #endregion

    #region Props

    public FMeshLayout* Ptr => m_ptr;
    public ReadOnlySpan<MeshBufferDefine> Buffers => new(m_native_buffers, (int)m_native_buffers_count);
    public ReadOnlySpan<MeshBufferElement> Elements => new(m_native_elements, (int)m_native_elements_count);

    #endregion

    #region Ctor

    internal MeshLayout(FMeshLayout* ptr, string? name)
    {
        m_ptr = ptr;
        m_name = name;
        if (m_ptr != null)
        {
            {
                uint count;
                m_native_buffers = m_ptr->GetBuffers(&count);
                m_native_buffers_count = count;
            }
            {
                uint count;
                m_native_elements = m_ptr->GetElements(&count);
                m_native_elements_count = count;
            }
        }
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
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
            Str8or16 str = new() { str16 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        fixed (byte* ptr = name)
        {
            Str8or16 str = new() { str8 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
        m_name = null;
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(MeshLayout)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(MeshLayout)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
