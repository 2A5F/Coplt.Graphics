using System.Diagnostics;
using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.States;

namespace Coplt.Graphics.Core;

public enum StaticSamplerBorderColor : byte
{
    Transparent,
    Black,
    White,
    BlackUInt,
    WhiteUInt,
}

public record struct StaticSamplerInfo()
{
    public uint MaxAnisotropy;
    public float MipLodBias;
    public float MinLod;
    public float MaxLod = 3.402823466E38f;
    public CmpFunc Cmp;
    public FilterMode Mag;
    public FilterMode Min;
    public FilterMode Mipmap;
    public AddressMode U;
    public AddressMode V;
    public AddressMode W;
    public StaticSamplerBorderColor BorderColor;

    public static readonly StaticSamplerInfo PointRepeat = new()
    {
        Mag = FilterMode.Linear,
        Min = FilterMode.Linear,
        Mipmap = FilterMode.Point,
    };

    public static readonly StaticSamplerInfo LinearRepeat = new()
    {
        Mag = FilterMode.Linear,
        Min = FilterMode.Linear,
        Mipmap = FilterMode.Point,
    };

    public static readonly StaticSamplerInfo TrilinearRepeat = new()
    {
        Mag = FilterMode.Linear,
        Min = FilterMode.Linear,
        Mipmap = FilterMode.Linear,
    };

    public static readonly StaticSamplerInfo PointClamp = new()
    {
        Mag = FilterMode.Linear,
        Min = FilterMode.Linear,
        Mipmap = FilterMode.Point,
        U = AddressMode.Clamp,
        V = AddressMode.Clamp,
        W = AddressMode.Clamp,
    };

    public static readonly StaticSamplerInfo LinearClamp = new()
    {
        Mag = FilterMode.Linear,
        Min = FilterMode.Linear,
        Mipmap = FilterMode.Point,
        U = AddressMode.Clamp,
        V = AddressMode.Clamp,
        W = AddressMode.Clamp,
    };

    public static readonly StaticSamplerInfo TrilinearClamp = new()
    {
        Mag = FilterMode.Linear,
        Min = FilterMode.Linear,
        Mipmap = FilterMode.Linear,
        U = AddressMode.Clamp,
        V = AddressMode.Clamp,
        W = AddressMode.Clamp,
    };
}

public enum BindGroupUsage : byte
{
    /// <summary>
    /// 一般组
    /// </summary>
    Common = 0,
    /// <summary>
    /// 提示组的使用是频繁更改的，d3d12 将会尽可能使用根描述符
    /// </summary>
    Dynamic = 1,
}

public unsafe record struct BindGroupItem()
{
    /// <inheritdoc cref="ShaderLayoutItem.Id"/>
    public required ulong Id;
    /// <inheritdoc cref="ShaderLayoutItem.Scope"/>
    public ulong Scope;
    /// <summary>
    /// 数量
    /// </summary>
    public uint Count = 1;
    /// <summary>
    /// StaticSamplers 中的 Index
    /// </summary>
    public uint StaticSamplerIndex;
    public GraphicsFormat Format;
    public required ShaderStageFlags Stages;
    public required ShaderLayoutItemView View;
    public required ShaderLayoutItemType Type;
    public ResourceAccess UavAccess = ResourceAccess.Unknown;

    static BindGroupItem()
    {
        Debug.Assert(sizeof(FBindGroupItem) == sizeof(BindGroupItem));
    }
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBindGroupLayout : DeviceChild
{
    #region Fields

    internal FBindGroupLayoutData* m_data;

    #endregion

    #region Props

    public new FBindGroupLayout* Ptr => (FBindGroupLayout*)m_ptr;
    public ref readonly FBindGroupLayoutData Data => ref *m_data;
    public ReadOnlySpan<BindGroupItem> Items => new(Data.Items, (int)Data.NumItems);
    public ReadOnlySpan<StaticSamplerInfo> StaticSamplers => new(Data.StaticSamplers, (int)Data.NumStaticSamplers);
    public BindGroupUsage Usage => (BindGroupUsage)Data.Usage;

    #endregion

    #region Ctor

    internal ShaderBindGroupLayout(FBindGroupLayoutCreateResult result, string? name, GpuDevice device) : base((FGpuObject*)result.Layout, name, device)
    {
        if (Ptr == null) return;
        m_data = result.Data;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion
}
