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

public record struct GpuOutputOptions()
{
    public required uint Width;
    public required uint Height;
    /// <summary>
    /// 是否使用 srgb，实现可以选择忽略
    /// </summary>
    public bool Srgb;
    /// <summary>
    /// 是否使用 hdr，实现可以选择忽略
    /// </summary>
    public HdrType Hdr;
    public PresentMode PresentMode = PresentMode.TripleBuffer;
    public OutputAlphaMode AlphaMode = OutputAlphaMode.Opaque;
    public bool VSync = false;
}

#endregion

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
}

public record struct GpuSwapChainCreateOptions()
{
    public required uint Width;
    public required uint Height;
    /// <summary>
    /// <para>是否使用 <c>sRGB</c>，实现可以选择忽略</para>
    /// <para>是指整个渲染管线是不是使用 <c>sRGB</c>，而不是交换链是不是使用，具体交换链格式请使用 <see cref="GpuOutput2.Format"/> 获取</para>
    /// <para>设置为 <c>false</c> 时需要注意保证顶点缓冲区和 cbuffer 中传入的颜色是线性的，以及使用 <c>sRGB</c> 视图转换纹理到线性</para>
    /// <para><c>true</c> 时将原样传输像素数据进行呈现，<c>false</c> 将在呈现时对输出执行 线性 -> <c>sRGB</c> 转换</para>
    /// </summary>
    public bool Srgb = true;
    /// <summary>
    /// 是否使用 hdr，实现可以选择忽略
    /// </summary>
    public HdrType Hdr;
    public PresentMode PresentMode = PresentMode.TripleBuffer;
    public OutputAlphaMode AlphaMode = OutputAlphaMode.Opaque;
    public bool VSync = false;
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
    
    public void GetRawPtr() => Ptr->GetRawPtr();

    #endregion
}
