using System.Diagnostics;
using Coplt.Graphics.Core;

namespace Coplt.Graphics.Native;

public partial struct FShaderLayoutItemDefine
{
    /// <summary>
    /// 检查兼容性，不兼容将 throw
    /// </summary>
    public readonly void CheckCompatible(in View view, int index)
    {
        if (View is FShaderLayoutItemView.Constants)
            throw new ArgumentException(
                $"Binding slot {index} required [{View}], [{View}] slot cannot bind resources."
            );
        switch (view.Tag)
        {
        case Core.View.Tags.None:
            break;
        case Core.View.Tags.Buffer:
        {
            var buffer = view.Buffer;
            switch (Type)
            {
            case FShaderLayoutItemType.RayTracingAccelerationStructure:
            case FShaderLayoutItemType.ConstantBuffer:
            case FShaderLayoutItemType.RawBuffer:
                if (buffer.Usage is not BufferUsage.Raw)
                    throw new ArgumentException(
                        $"Binding slot [{index}] required [{Type} (Usage: {BufferUsage.Raw})] but usage of [{buffer}] is a [{buffer.Usage}]."
                    );
                break;
            case FShaderLayoutItemType.StructureBuffer:
            case FShaderLayoutItemType.StructureBufferWithCounter:
                if (buffer.Usage is not BufferUsage.Raw)
                    throw new ArgumentException(
                        $"Binding slot [{index}] required [{Type} (Usage: {BufferUsage.Structured})] but usage of [{buffer}] is a [{buffer.Usage}]."
                    );
                break;
            case FShaderLayoutItemType.Texture1D:
            case FShaderLayoutItemType.Texture1DArray:
            case FShaderLayoutItemType.Texture2D:
            case FShaderLayoutItemType.Texture2DArray:
            case FShaderLayoutItemType.Texture2DMultisample:
            case FShaderLayoutItemType.Texture2DArrayMultisample:
            case FShaderLayoutItemType.Texture3D:
            case FShaderLayoutItemType.TextureCube:
            case FShaderLayoutItemType.TextureCubeArray:
            case FShaderLayoutItemType.Sampler:
                throw new ArgumentException(
                    $"Binding slot [{index}] required [{Type}] but [{buffer}] is a {nameof(GpuBuffer)}."
                );
            default:
                throw new ArgumentOutOfRangeException();
            }
            switch (View)
            {
            case FShaderLayoutItemView.Cbv:
                if (!buffer.TryCbv())
                    throw new ArgumentException(
                        $"Binding slot [{index}] required [{View}], but [{buffer}] cannot be used as [{View}]."
                    );
                break;
            case FShaderLayoutItemView.Srv:
                if (!buffer.TrySrv())
                    throw new ArgumentException(
                        $"Binding slot [{index}] required [{View}], but [{buffer}] cannot be used as [{View}]."
                    );
                break;
            case FShaderLayoutItemView.Uav:
                if (!buffer.TryUav())
                    throw new ArgumentException(
                        $"Binding slot [{index}] required [{View}], but [{buffer}] cannot be used as [{View}]."
                    );
                break;
            case FShaderLayoutItemView.Sampler:
            case FShaderLayoutItemView.Constants:
                throw new UnreachableException();
            default:
                throw new ArgumentOutOfRangeException();
            }
            break;
        }
        default:
            throw new ArgumentOutOfRangeException();
        }
    }
}
