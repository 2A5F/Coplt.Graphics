using System.Runtime.InteropServices;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

#region Enums

public enum PresentMode : byte
{
    NoBuffer,
    DoubleBuffer,
    TripleBuffer,
}

public enum OutputAlphaMode : byte
{
    Opaque,
    PrePremultiplied,
    PostPremultiplied,
}

public enum HdrType : byte
{
    None,
    UNorm10,
    Float16,
}

#endregion

#region GpuOutputOptions

/// <summary>
/// 输出格式选择器，按选择器选择，不符合将回退，保证成功；指定格式不保证
/// </summary>
public record struct GpuOutputFormatSelector
{
    /// <summary>
    /// 指定格式，忽略选择器
    /// </summary>
    public bool Specify;
    /// <summary>
    /// 如果可能，使用 srgb 格式
    /// </summary>
    public bool Srgb;
    /// <summary>
    /// 如果可能，使用 hdr 格式，和 srgb 冲突，并且优先级更高
    /// </summary>
    public HdrType Hdr;
}

public record struct GpuOutputOptions()
{
    public required uint Width;
    public required uint Height;
    public GraphicsFormat Format = GraphicsFormat.R8G8B8A8_UNorm;
    public PresentMode PresentMode = PresentMode.TripleBuffer;
    public OutputAlphaMode AlphaMode = OutputAlphaMode.Opaque;
    public bool VSync = false;
    public GpuOutputFormatSelector FormatSelector;
}

#endregion

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuOutput : GpuExecutor, IQueueOwned, IGpuResource, IRtv, ISrv
{
    #region Fields

    internal new FGpuOutput* m_ptr;

    #endregion

    #region Props

    public new FGpuOutput* Ptr => m_ptr;
    public GraphicsFormat Format => m_ptr->m_format.FromFFI();
    public ResState State
    {
        get => NativeState.FromFFI();
        set => NativeState = value.ToFFI();
    }
    public ref FResState NativeState => ref m_ptr->m_state;
    public USize2d Size2d => new(m_ptr->m_width, m_ptr->m_height);
    public USize3d Size3d => new(m_ptr->m_width, m_ptr->m_height, 1);
    ulong IGpuView.Size => m_ptr->m_width;
    public uint Width => m_ptr->m_width;
    public uint Height => m_ptr->m_height;
    public uint DepthOrLength => 1;
    public uint MipLevels => 1;
    public uint Planes => 1;
    uint IGpuView.Count => 0;
    uint IGpuView.Stride => 0;
    GpuResourceType IGpuResource.Type => GpuResourceType.Image;
    IGpuResource IGpuView.Resource => this;
    FResourceMeta IGpuResource.GetMeta() => new()
    {
        Type = FResourceRefType.Output,
        Output = m_ptr,
    };

    #endregion

    #region Ctor

    internal GpuOutput(FGpuOutput* ptr, string? name, GpuQueue queue) : base((FGpuExecutor*)ptr, name, queue)
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
            ? $"{nameof(GpuOutput)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuOutput)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region Present

    public void Present(bool NoWait = false) => Queue.Submit(this, NoWait);

    #endregion

    #region Views

    public bool TrySrv() => true;
    public bool TryRtv() => true;

    #endregion

    #region Resize

    public void Resize(uint width, uint height) => m_ptr->Resize(width, height).TryThrow();

    #endregion
}
