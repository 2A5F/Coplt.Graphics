﻿using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

public record struct ShaderInputLayoutElement()
{
    public ShaderInputLayoutElement(
        SlotId SlotId, uint SlotIndex = 0, uint Location = uint.MaxValue
    ) : this()
    {
        this.SlotId = SlotId;
        this.SlotIndex = SlotIndex;
        this.Location = Location;
    }

    public ShaderInputLayoutElement(
        string? SlotName, uint SlotIndex = 0, uint Location = uint.MaxValue, SlotId? SlotId = default
    ) : this()
    {
        this.SlotName = SlotName;
        this.SlotIndex = SlotIndex;
        this.Location = Location;
        this.SlotId = SlotId;
    }

    public ShaderInputLayoutElement(
        String8? SlotName8, uint SlotIndex = 0, uint Location = uint.MaxValue, SlotId? SlotId = default
    ) : this()
    {
        this.SlotName8 = SlotName8;
        this.SlotIndex = SlotIndex;
        this.Location = Location;
        this.SlotId = SlotId;
    }

    public static implicit operator ShaderInputLayoutElement(string SlotName) => new(SlotName);

    /// <summary>
    /// dx 后端为语义名称，必须有；其他后端可选，和 <see cref="SlotName"/> 同时存在时优先选择 <see cref="SlotName8"/>
    /// </summary>
    public String8? SlotName8;
    /// <summary>
    /// dx 后端为语义名称，必须有；其他后端可选，和 <see cref="SlotName8"/> 同时存在时优先选择 <see cref="SlotName8"/>
    /// </summary>
    public string? SlotName;
    /// <summary>
    /// 与 <see cref="MeshBufferElement"/> 对应的 Id，相同 <see cref="SlotName"/>/<see cref="SlotName8"/> 的 <see cref="SlotId"/> 必须也相同
    /// </summary>
    public SlotId? SlotId;
    /// <summary>
    /// 是 Slot 中第几个项，对应 dx 的 SemanticIndex
    /// </summary>
    public uint SlotIndex;
    /// <summary>
    /// 仅 vk, 用于指定位置；dx 将忽略; -1 将使用 FShaderInputLayoutElement 索引顺序
    /// </summary>
    public uint Location = uint.MaxValue;
}

public record struct ShaderInputLayoutElementMeta
{
    /// <summary>
    /// dx 后端为语义名称，必须有；其他后端可选
    /// </summary>
    public String8? SlotName8;
    /// <summary>
    /// 与 <see cref="MeshBufferElement"/> 对应的 Id，相同 <see cref="SlotName8"/> 的 <see cref="SlotId"/> 必须也相同
    /// </summary>
    public uint SlotId;
    /// <summary>
    /// 是 Slot 中第几个项，对应 dx 的 SemanticIndex
    /// </summary>
    public uint SlotIndex;
    /// <summary>
    /// 仅 vk, 用于指定位置；dx 将忽略; -1 将使用 FShaderInputLayoutElement 索引顺序
    /// </summary>
    public uint Location;
}

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderInputLayout
{
    #region Fields

    internal FShaderInputLayout* m_ptr;
    internal string? m_name;
    internal readonly ShaderInputLayoutElementMeta[] m_elements;
    internal readonly FShaderInputLayoutElement* m_native_elements;
    internal readonly uint m_native_elements_count;

    #endregion

    #region Props

    public FShaderInputLayout* Ptr => m_ptr;
    public ReadOnlySpan<ShaderInputLayoutElementMeta> Elements => m_elements;
    public ReadOnlySpan<FShaderInputLayoutElement> NativeElements =>
        new(m_native_elements, (int)m_native_elements_count);

    #endregion

    #region Ctor

    internal ShaderInputLayout(FShaderInputLayout* ptr, ShaderInputLayoutElementMeta[] elements, string? name)
    {
        m_ptr = ptr;
        m_name = name;
        m_elements = elements;
        if (m_ptr != null)
        {
            uint count;
            m_native_elements = m_ptr->GetElements(&count);
            m_native_elements_count = count;
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
            FStr8or16 str = new(ptr, name.Length);
            m_ptr->SetName(&str).TryThrow();
        }
    }

    public void SetName(ReadOnlySpan<byte> name)
    {
        m_name = null;
        fixed (byte* ptr = name)
        {
            FStr8or16 str = new(ptr, name.Length);
            m_ptr->SetName(&str).TryThrow();
        }
    }

    #endregion

    #region ToString

    public override string ToString() =>
        m_name is null
            ? $"{nameof(ShaderInputLayout)}(0x{(nuint)m_ptr:X})"
            : $"{nameof(ShaderInputLayout)}(0x{(nuint)m_ptr:X} \"{m_name}\")";

    #endregion
}
