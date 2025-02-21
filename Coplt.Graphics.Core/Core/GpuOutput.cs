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
public sealed unsafe partial class GpuOutput : IRtv, ISrv
{
    #region Fields

    internal FGpuOutput* m_ptr;
    internal string? m_name;
    internal readonly GpuQueue m_queue;

    #endregion

    #region Props

    public FGpuOutput* Ptr => m_ptr;

    public uint3 Size => new(m_ptr->m_width, m_ptr->m_height, 1);
    public GpuQueue Queue => m_queue;

    #endregion

    #region Ctor

    internal GpuOutput(GpuQueue queue, FGpuOutput* ptr, string? name)
    {
        m_queue = queue;
        m_ptr = ptr;
        m_name = name;
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
        m_name = null;
        fixed (byte* ptr = name)
        {
            Str8or16 str = new() { str8 = ptr, len = name.Length };
            m_ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(GpuOutput)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(GpuOutput)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion

    #region Views

    FResourceMeta IView.GetMeta() => new()
    {
        CurrentState = m_ptr->m_state,
        Type = FResourceRefType.Output,
        Output = m_ptr,
    };
    void IView.UnsafeChangeState(FResourceState state) => UnsafeChangeState(state);
    internal void UnsafeChangeState(FResourceState state) => m_ptr->m_state.ChangeState(state);

    public bool TrySrv() => true;
    public bool TryRtv() => true;

    #endregion

    #region Present

    /// <summary>
    /// 提交命令并等待下帧可用
    /// </summary>
    public void Present()
    {
        using var _ = Queue.m_lock.EnterScope();
        var cmd = Queue.m_cmd;
        if (cmd.m_current_output != null && cmd.m_current_output != this)
            throw new ArgumentException(
                $"The command is already used by another output and cannot be presented in this output"
            );
        cmd.Present(this);
        fixed (FResourceMeta* p_resources = CollectionsMarshal.AsSpan(cmd.m_resource_metas))
        fixed (FCommandItem* p_commands = CollectionsMarshal.AsSpan(cmd.m_commands))
        fixed (byte* p_payload = CollectionsMarshal.AsSpan(cmd.m_payload))
        {
            FCommandSubmit submit = new()
            {
                CommandCount = (uint)cmd.m_commands.Count,
                ResourceCount = (uint)cmd.m_resource_metas.Count,
                Commands = p_commands,
                Resources = p_resources,
                Payload = p_payload,
            };
            try
            {
                m_ptr->Present(&submit).TryThrow();
            }
            finally
            {
                cmd.Reset();
            }
        }
    }

    /// <summary>
    /// 提交命令
    /// </summary>
    public void PresentNoWait()
    {
        using var _ = Queue.m_lock.EnterScope();
        var cmd = Queue.m_cmd;
        if (cmd.m_current_output != null && cmd.m_current_output != this)
            throw new ArgumentException(
                $"The command is already used by another output and cannot be presented in this output"
            );
        cmd.Present(this);
        fixed (FResourceMeta* p_resources = CollectionsMarshal.AsSpan(cmd.m_resource_metas))
        fixed (FCommandItem* p_commands = CollectionsMarshal.AsSpan(cmd.m_commands))
        fixed (byte* p_payload = CollectionsMarshal.AsSpan(cmd.m_payload))
        {
            FCommandSubmit submit = new()
            {
                CommandCount = (uint)cmd.m_commands.Count,
                ResourceCount = (uint)cmd.m_resource_metas.Count,
                Commands = p_commands,
                Resources = p_resources,
                Payload = p_payload,
            };
            try
            {
                m_ptr->PresentNoWait(&submit).TryThrow();
            }
            finally
            {
                cmd.Reset();
            }
        }
    }

    /// <summary>
    /// 等待下帧可用
    /// </summary>
    public void WaitNextFrame()
    {
        using var _ = Queue.m_lock.EnterScope();
        m_ptr->WaitNextFrame().TryThrow();
    }

    #endregion

    #region Resize

    public void Resize(uint width, uint height) => m_ptr->Resize(width, height).TryThrow();

    #endregion
}
