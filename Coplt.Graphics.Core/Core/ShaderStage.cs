using Coplt.Graphics.Native;

namespace Coplt.Graphics
{
    public enum ShaderStage : byte
    {
        Compute = 0,
        Pixel = 1,
        Vertex = 2,
        Mesh = 3,
        Task = 4,
    }

    [Flags]
    public enum ShaderStageFlags : uint
    {
        None = 0,
        Compute = 1 << 0,
        Pixel = 1 << 1,
        Vertex = 1 << 2,
        Mesh = 1 << 3,
        Task = 1 << 4,
    }
}

namespace Coplt.Graphics.Core
{
    public static partial class GraphicsExtensions
    {
        public static FShaderStage ToFFI(this ShaderStage value) => (FShaderStage)value;

        public static ShaderStage FromFFI(this FShaderStage value) => (ShaderStage)value;

        public static FShaderStageFlags ToFFI(this ShaderStageFlags value) => (FShaderStageFlags)value;

        public static ShaderStageFlags FromFFI(this FShaderStageFlags value) => (ShaderStageFlags)value;

        public static bool HasAnyFlags(this ShaderStageFlags value, ShaderStageFlags other) => (value & other) != 0;
        public static bool HasFlags(this ShaderStageFlags value, ShaderStageFlags other) => (value & other) == other;
    }
}
