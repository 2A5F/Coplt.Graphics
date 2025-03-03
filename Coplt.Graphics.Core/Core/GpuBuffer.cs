using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct GpuBufferCreateOptions()
{
    public required ResourcePurpose Purpose;
    public CpuAccess CpuAccess = CpuAccess.None;
    public required ulong Size;
    public uint Count;
    public uint Stride;
    public BufferUsage Usage = BufferUsage.Raw;
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuBuffer : GpuResource, IGpuResource, ICbv, ISrv, IUav, IRtv, IIbv, IVbv
{
    #region Fields

    internal new FGpuBuffer* m_ptr;

    #endregion

    #region Props

    public new FGpuBuffer* Ptr => m_ptr;
    public ulong Size => m_ptr->m_size;
    GraphicsFormat IGpuView.Format => GraphicsFormat.Unknown;
    uint IGpuView.Width => (uint)Size;
    uint IGpuView.Height => 1;
    uint IGpuView.DepthOrLength => 1;
    uint IGpuView.MipLevels => 1;
    uint IGpuView.Planes => 1;
    public uint Count => m_ptr->m_count;
    public uint Stride => m_ptr->m_stride;
    public BufferUsage Usage => (BufferUsage)m_ptr->m_usage;
    GpuResourceType IGpuResource.Type => GpuResourceType.Buffer;
    IGpuResource IGpuView.Resource => this;
    FResourceMeta IGpuResource.GetMeta() => new()
    {
        Type = FResourceRefType.Buffer,
        Buffer = m_ptr,
    };

    #endregion

    #region Ctor

    internal GpuBuffer(FGpuBuffer* ptr, string? name, GpuQueue queue) : base(&ptr->Base, name, queue)
    {
        m_ptr = ptr;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_ptr = null;
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuBuffer)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuBuffer)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region View

    public bool TryVbv() => Purpose.HasFlags(ResourcePurpose.VertexBuffer);
    public bool TryIbv() => Purpose.HasFlags(ResourcePurpose.IndexBuffer);
    public bool TryRtv() => Purpose.HasFlags(ResourcePurpose.RenderTarget);
    public bool TryUav() => Purpose.HasFlags(ResourcePurpose.UnorderedAccess);
    public bool TrySrv() => Purpose.HasFlags(ResourcePurpose.ShaderResource);
    public bool TryCbv() => Purpose.HasFlags(ResourcePurpose.ConstantBuffer);

    #endregion
}
