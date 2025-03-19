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
public sealed unsafe partial class GpuOutput : GpuExecutor, IGpuResource, IRtv, ISrv
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
    public FCmdRes IntoCmd() => throw new NotImplementedException();

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

    public GpuIsolate Isolate => throw new NotImplementedException();
}

[Dropping(Unmanaged = true)]
public abstract unsafe partial class GpuOutput2 : IsolateChild, IGpuResource, IRtv, ISrv
{
    #region Fields

    internal FGpuOutputData* m_data;

    #endregion

    #region Props

    public new FGpuOutput2* Ptr => (FGpuOutput2*)m_ptr;
    internal ref readonly FGpuOutputData Data => ref *m_data;
    GpuResourceType IGpuResource.Type => GpuResourceType.Image;
    IGpuResource IGpuView.Resource => this;
    public GraphicsFormat Format => Data.Format.FromFFI();
    ulong IGpuView.Size => (ulong)Width * Height;
    public uint Width => Data.Width;
    public uint Height => Data.Height;
    uint IGpuView.DepthOrLength => 1;
    uint IGpuView.MipLevels => 1;
    uint IGpuView.Planes => 1;
    uint IGpuView.Count => 0;
    uint IGpuView.Stride => 0;
    public PresentMode PresentMode => (PresentMode)Data.PresentMode;

    #endregion

    #region Ctor

    internal GpuOutput2(FGpuOutput2* ptr, FGpuOutputData* data, string? name, GpuIsolate isolate) : base((FGpuObject*)ptr, name, isolate)
    {
        m_data = data;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion

    #region Cmd

    FCmdRes IGpuResource.IntoCmd() => new()
    {
        Type = FCmdResType.Output,
        Output = Ptr,
    };

    #endregion

    #region Views

    public bool TrySrv() => true;
    public bool TryRtv() => true;

    #endregion

    #region Methods

    public void Resize(uint Width, uint Height) => Ptr->Resize(Width, Height).TryThrow();

    public void Present() => Ptr->Present().TryThrow();
    public void PresentNoWait() => Ptr->PresentNoWait().TryThrow();
    public void Wait() => Ptr->Wait().TryThrow();

    #endregion

    public FResourceMeta GetMeta() => throw new NotSupportedException();
    public ResState State
    {
        get => throw new NotSupportedException();
        set => throw new NotSupportedException();
    }
    public ref FResState NativeState => throw new NotSupportedException();
    public GpuQueue Queue => throw new NotSupportedException();
}

public record struct GpuSwapChainCreateOptions()
{
    public required uint Width;
    public required uint Height;
    public GraphicsFormat Format = GraphicsFormat.R8G8B8A8_UNorm;
    public PresentMode PresentMode = PresentMode.TripleBuffer;
    public OutputAlphaMode AlphaMode = OutputAlphaMode.Opaque;
    public bool VSync = false;
    public GpuOutputFormatSelector FormatSelector;
}

public sealed unsafe class GpuSwapChain : GpuOutput2
{
    #region Props

    public new FGpuSwapChain* Ptr => (FGpuSwapChain*)m_ptr;
    internal new ref readonly FGpuSwapChainData Data => ref *(FGpuSwapChainData*)m_data;

    public OutputAlphaMode AlphaMode => (OutputAlphaMode)Data.AlphaMode;
    public bool VSync
    {
        get => Data.VSync;
        set => SetVSync(value);
    }

    #endregion

    #region Ctor

    internal GpuSwapChain(FGpuSwapChain* ptr, FGpuSwapChainData* data, string? name, GpuIsolate isolate)
        : base((FGpuOutput2*)ptr, (FGpuOutputData*)data, name, isolate) { }

    #endregion

    #region Methods

    public void SetVSync(bool Enable) => Ptr->SetVSync(Enable).TryThrow();

    #endregion
}
