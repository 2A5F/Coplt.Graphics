using System.Diagnostics;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public enum ResourceAccess : byte
{
    Unknown,
    ReadOnly,
    WriteOnly,
    ReadWrite,
}

public enum ShaderLayoutItemView : byte
{
    Cbv,
    Srv,
    Uav,
    /// <summary>
    /// Type 必须是 Sampler
    /// </summary>
    Sampler,
    /// <summary>
    /// 将使用 Push Const / Root Const, 忽略 Usage，Type 必须是 ConstantBuffer
    /// </summary>
    Constants,
}

public enum ShaderLayoutItemType : byte
{
    ConstantBuffer,
    Buffer,
    RawBuffer,
    StructureBuffer,
    StructureBufferWithCounter,
    Texture1D,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    Texture2DMultisample,
    Texture2DArrayMultisample,
    Texture3D,
    TextureCube,
    TextureCubeArray,
    Sampler,
    RayTracingAccelerationStructure,
}

public unsafe record struct ShaderLayoutItem()
{
    /// <summary>
    /// 绑定点的 Id，建议从名称缓存自增 id, 不同 stage 不同 scope 的 id 可以重复
    /// </summary>
    public required ulong Id;
    /// <summary>
    /// 绑定点所属范围，和 Id 共同组成唯一定位
    /// </summary>
    public ulong Scope;
    /// <summary>
    /// dx 后端无论什么时候都是 register, vk 后端一般情况是 binding，类型为 Constants 时是 push const 的 offset，为字节偏移
    /// </summary>
    public required uint Slot;
    /// <summary>
    /// dx 的 space，vk 的 set，vk 建议尽可能多的拆分 set，dx 的 space 写不写都一样
    /// </summary>
    public uint Space;
    /// <summary>
    /// 数量，View 是 StaticSampler 时必须是 1，其他必须最少是 1, View 是 Constants 是 32 位值的数量，而不是 byte 的数量
    /// </summary>
    public uint Count = 1;
    public GraphicsFormat Format;
    public required ShaderStage Stage;
    public required ShaderLayoutItemView View;
    public required ShaderLayoutItemType Type;
    public ResourceAccess UavAccess = ResourceAccess.Unknown;

    static ShaderLayoutItem()
    {
        Debug.Assert(sizeof(FShaderLayoutItem) == sizeof(ShaderLayoutItem));
    }
}

[Flags]
public enum ShaderLayoutFlags : byte
{
    None = 0,
    /// <summary>
    /// 是否启用无绑定
    /// </summary>
    BindLess = 1 << 0,
    /// <summary>
    /// 是否启用输入组装
    /// </summary>
    InputAssembler = 1 << 1,
    /// <summary>
    /// 是否启用流输出
    /// </summary>
    StreamOutput = 1 << 2,
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderLayout : DeviceChild
{
    #region Fields

    internal FShaderLayoutData* m_data;
    internal ShaderBindingLayout? m_empty_binding_layout;

    #endregion

    #region Props

    public new FShaderLayout* Ptr => (FShaderLayout*)m_ptr;
    public ref readonly FShaderLayoutData Data => ref *m_data;
    public ReadOnlySpan<ShaderLayoutItem> NativeItems => new(Data.Items, (int)Data.NumItems);
    public ShaderLayoutFlags Flags => (ShaderLayoutFlags)Data.Flags;

    #endregion

    #region Ctor

    internal ShaderLayout(FShaderLayoutCreateResult result, string? name, GpuDevice device) : base((FGpuObject*)result.Layout, name, device)
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

    #region GetEmptyBindingLayout

    public ShaderBindingLayout GetEmptyBindingLayout() => m_empty_binding_layout ??= Device.CreateBindingLayout(this, [], $"Empty Binding Layout of [{this}]");

    #endregion
}
