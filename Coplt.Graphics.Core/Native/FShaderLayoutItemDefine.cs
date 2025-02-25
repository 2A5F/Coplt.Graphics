using Coplt.Graphics.Core;

namespace Coplt.Graphics.Native;

public partial struct FShaderLayoutItemDefine
{
    /// <summary>
    /// 检查兼容性，不兼容将 throw
    /// </summary>
    public readonly void CheckCompatible(in View view, int index)
    {
        switch (view.Tag)
        {
        case Core.View.Tags.None:
            break;
        case Core.View.Tags.Buffer:
            if (Type is not (FShaderLayoutItemType.ConstantBuffer or FShaderLayoutItemType.RawBuffer
                or FShaderLayoutItemType.StructureBuffer or FShaderLayoutItemType.StructureBufferWithCounter))
                throw new ArgumentException($"Binding slot {index} does not supports buffers");
            break;
        default:
            throw new ArgumentOutOfRangeException();
        }
    }
}
