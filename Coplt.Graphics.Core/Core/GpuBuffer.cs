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

    private volatile void* m_mapped_ptr;

    #endregion

    #region Props

    public new FGpuBuffer* Ptr => (FGpuBuffer*)m_ptr;
    public new FGpuBufferData* Data => (FGpuBufferData*)m_data;
    public void* MappedPtr => m_mapped_ptr;
    public ulong Size => Data->m_size;
    GraphicsFormat IGpuView.Format => GraphicsFormat.Unknown;
    uint IGpuView.Width => (uint)Size;
    uint IGpuView.Height => 1;
    uint IGpuView.DepthOrLength => 1;
    uint IGpuView.MipLevels => 1;
    uint IGpuView.Planes => 1;
    public uint Count => Data->m_count;
    public uint Stride => Data->m_stride;
    public BufferUsage Usage => (BufferUsage)Data->m_usage;
    GpuResourceType IGpuResource.Type => GpuResourceType.Buffer;
    IGpuResource IGpuView.Resource => this;
    FResourceMeta IGpuResource.GetMeta() => new()
    {
        Type = FResourceRefType.Buffer,
        Buffer = Ptr,
    };

    #endregion

    #region Ctor

    internal GpuBuffer(FGpuBuffer* ptr, string? name, GpuQueue queue) : this(ptr, ptr->GpuBufferData(), name, queue) { }
    private GpuBuffer(FGpuBuffer* ptr, FGpuBufferData* data, string? name, GpuQueue queue)
        : base((FGpuResource*)ptr, (FGpuResourceData*)data, name, queue) { }

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

    #region Map UnMap

    public void* Map() => Map(CpuAccess != CpuAccess.Read);
    private void* Map(bool discard)
    {
        if (CpuAccess == CpuAccess.None)
            throw new InvalidOperationException("Unable to map a buffer that is not accessible to the cpu");
        if (m_mapped_ptr != null) return m_mapped_ptr;
        void* p;
        Ptr->Map(&p, discard).TryThrow();
        m_mapped_ptr = p;
        return p;
    }

    public void UnMap() => UnMap(CpuAccess != CpuAccess.Write);
    private void UnMap(bool discard)
    {
        if (CpuAccess == CpuAccess.None)
            throw new InvalidOperationException("Unable to unmap a buffer that is not accessible to the cpu");
        m_mapped_ptr = null;
        Ptr->Unmap(discard).TryThrow();
    }

    public Span<byte> EnsureMappedMemory()
    {
        if (m_mapped_ptr == null) Map();
        return new(m_mapped_ptr, (int)(uint)Size);
    }

    #endregion
}
