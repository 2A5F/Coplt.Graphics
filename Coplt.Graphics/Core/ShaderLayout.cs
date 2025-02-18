using System.Diagnostics;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

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

public enum ShaderLayoutItemUsage : byte
{
    /// <summary>
    /// 一般改变频率的变量，可能每帧都会改变
    /// </summary>
    Common,
    /// <summary>
    /// 不太改变的变量，例如材质参数，一般很少改变，可以进行一定的静态优化，建议将所有材质绑定放到单独的 space 中区分
    /// </summary>
    Material,
    /// <summary>
    /// 经常改变的变量，例如每次绘制调用都会改变, dx 后端将直接在根签名内，类型是 Sampler 时表示是静态采样器
    /// </summary>
    Instant,
}

public unsafe record struct ShaderLayoutItemDefine
{
    /// <summary>
    /// dx 后端无论什么时候都是 register, vk 后端一般情况是 binding，类型为 Constants 时是 push const 的 offset，为字节偏移
    /// </summary>
    public uint Slot;
    public uint Space;
    /// <summary>
    /// 类型是 Sampler并且 Usage 是 Static 时是静态采样器描述的索引；其他类型表示数量
    /// </summary>
    public uint CountOrIndex;
    public ShaderStage Stage;
    public ShaderLayoutItemView View;
    public ShaderLayoutItemType Type;
    public ShaderLayoutItemUsage Usage;

    static ShaderLayoutItemDefine()
    {
        Debug.Assert(sizeof(FShaderLayoutItemDefine) == sizeof(ShaderLayoutItemDefine));
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
public sealed unsafe partial class ShaderLayout
{
    #region Fields

    internal FShaderLayout* m_ptr;
    internal string? m_name;
    internal readonly FShaderLayoutItemDefine* m_native_defines;
    internal readonly uint m_native_defines_count;

    #endregion

    #region Props

    public FShaderLayout* Ptr => m_ptr;
    public ReadOnlySpan<FShaderLayoutItemDefine> NativeElements => new(m_native_defines, (int)m_native_defines_count);

    #endregion

    #region Ctor

    internal ShaderLayout(FShaderLayout* ptr, string? name)
    {
        m_ptr = ptr;
        m_name = name;
        if (m_ptr != null)
        {
            uint count;
            m_native_defines = m_ptr->GetItemDefines(&count);
            m_native_defines_count = count;
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
            ? $"{nameof(ShaderLayout)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(ShaderLayout)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
