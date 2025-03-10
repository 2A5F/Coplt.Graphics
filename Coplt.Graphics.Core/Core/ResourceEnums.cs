﻿using Coplt.Graphics.Native;

namespace Coplt.Graphics
{
    [Flags]
    public enum ResourcePurpose : uint
    {
        None = 0,
        ConstantBuffer = 1 << 0,
        ShaderResource = 1 << 1,
        UnorderedAccess = 1 << 2,
        RenderTarget = 1 << 3,
        DepthStencil = 1 << 4,
        IndexBuffer = 1 << 5,
        VertexBuffer = 1 << 6,
        StreamOutput = 1 << 7,
        RayTracing = 1 << 8,
        ShadingRate = 1 << 9,
        IndirectDrawArgs = 1 << 10,
    }

    public enum CpuAccess : byte
    {
        None = 0,
        Write = 1,
        Read = 2,
    }
}

public enum BufferUsage : byte
{
    /// <summary>
    /// 未定义用法
    /// </summary>
    Undefined,
    /// <summary>
    /// 按结构化缓冲区使用
    /// </summary>
    Structured,
    /// <summary>
    /// 按原始缓冲区使用
    /// </summary>
    Raw,
}

namespace Coplt.Graphics.Core
{
    public static partial class GraphicsExtensions
    {
        public static FResourcePurpose ToFFI(this ResourcePurpose value) => (FResourcePurpose)value;
        public static bool HasFlags(this ResourcePurpose purpose, ResourcePurpose flag) => (purpose & flag) == flag;
        public static bool HasAnyFlags(this ResourcePurpose purpose, ResourcePurpose flag) => (purpose & flag) != 0;
        public static FCpuAccess ToFFI(this CpuAccess value) => value switch
        {
            CpuAccess.None  => FCpuAccess.None,
            CpuAccess.Write => FCpuAccess.Write,
            CpuAccess.Read  => FCpuAccess.Read,
            _               => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };
        public static FBufferUsage ToFFI(this BufferUsage value) => value switch
        {
            BufferUsage.Undefined  => FBufferUsage.Undefined,
            BufferUsage.Structured => FBufferUsage.Structured,
            BufferUsage.Raw        => FBufferUsage.Raw,
            _                      => throw new ArgumentOutOfRangeException(nameof(value), value, null)
        };
    }
}
