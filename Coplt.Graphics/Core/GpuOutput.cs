using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics;

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
    public TextureFormat Format = TextureFormat.R8G8B8A8_UNorm;
    public PresentMode PresentMode = PresentMode.TripleBuffer;
    public OutputAlphaMode AlphaMode = OutputAlphaMode.Opaque;
    public bool VSync = false;
    public GpuOutputFormatSelector FormatSelector;
}

#endregion

[Dropping(Unmanaged = true)]
public sealed unsafe partial class GpuOutput
{
    #region Fields

    internal FGpuOutput* m_ptr;

    #endregion

    #region Props

    public FGpuOutput* Ptr => m_ptr;

    #endregion

    #region Ctor

    public GpuOutput(FGpuOutput* ptr) => m_ptr = ptr;

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
    }

    #endregion

    #region Present

    /// <summary>
    /// 提交命令并等待下帧可用
    /// </summary>
    public void Present() => m_ptr->Present().TryThrow();

    /// <summary>
    /// 提交命令
    /// </summary>
    public void PresentNoWait() => m_ptr->PresentNoWait().TryThrow();

    /// <summary>
    /// 等待下帧可用
    /// </summary>
    public void WaitNextFrame() => m_ptr->WaitNextFrame().TryThrow();

    #endregion

    #region Resize

    public void Resize(uint width, uint height) => m_ptr->Resize(width, height).TryThrow();

    #endregion
}
