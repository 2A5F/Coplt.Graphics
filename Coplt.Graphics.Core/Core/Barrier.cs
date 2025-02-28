using Coplt.Graphics.Native;
using Coplt.Union;

namespace Coplt.Graphics.Core;

[Union]
public partial struct Barrier
{
    [UnionTemplate]
    private interface Template
    {
        void None();
        GlobalBarrier Global();
        BufferBarrier Buffer();
        ImageBarrier Image();
    }

    public static implicit operator Barrier(GlobalBarrier value) => MakeGlobal(value);

    public static implicit operator Barrier(BufferBarrier value) => MakeBuffer(value);

    public static implicit operator Barrier(ImageBarrier value) => MakeImage(value);
}

public record struct GlobalBarrier(
    ResAccess AccessBefore,
    ResAccess AccessAfter,
    ShaderStageFlags StagesBefore,
    ShaderStageFlags StagesAfter
)
{
    public ResAccess AccessBefore = AccessBefore;
    public ResAccess AccessAfter = AccessAfter;
    public ShaderStageFlags StagesBefore = StagesBefore;
    public ShaderStageFlags StagesAfter = StagesAfter;
}

public record struct BufferBarrier(
    IGpuResource Buffer,
    ulong Offset,
    ulong Size,
    LegacyState LegacyBefore,
    LegacyState LegacyAfter,
    ResAccess AccessBefore,
    ResAccess AccessAfter,
    ShaderStageFlags StagesBefore = ShaderStageFlags.None,
    ShaderStageFlags StagesAfter = ShaderStageFlags.None
)
{
    public IGpuResource Buffer = Buffer;
    public ulong Offset = Offset;
    public ulong Size = Size;
    public LegacyState LegacyBefore = LegacyBefore;
    public LegacyState LegacyAfter = LegacyAfter;
    public ResAccess AccessBefore = AccessBefore;
    public ResAccess AccessAfter = AccessAfter;
    public ShaderStageFlags StagesBefore = StagesBefore;
    public ShaderStageFlags StagesAfter = StagesAfter;

    public BufferBarrier(
        IGpuResource Buffer,
        LegacyState LegacyBefore,
        LegacyState LegacyAfter,
        ResAccess AccessBefore,
        ResAccess AccessAfter,
        ShaderStageFlags StagesBefore = ShaderStageFlags.None,
        ShaderStageFlags StagesAfter = ShaderStageFlags.None
    ) : this(
        Buffer, 0, Buffer.Size,
        LegacyBefore, LegacyAfter, AccessBefore, AccessAfter, StagesBefore, StagesAfter
    ) { }

    public BufferBarrier(
        IGpuResource Buffer,
        LegacyState LegacyBefore,
        LegacyState LegacyAfter
    ) : this(
        Buffer, 0, Buffer.Size,
        LegacyBefore, LegacyAfter, default, default, default, default
    ) { }

    public BufferBarrier(
        IGpuResource Buffer,
        ulong Offset,
        ulong Size,
        LegacyState LegacyBefore,
        LegacyState LegacyAfter
    ) : this(
        Buffer, Offset, Size,
        LegacyBefore, LegacyAfter, default, default, default, default
    ) { }

    public BufferBarrier(
        IGpuResource Buffer,
        ResAccess AccessBefore,
        ResAccess AccessAfter,
        ShaderStageFlags StagesBefore = ShaderStageFlags.None,
        ShaderStageFlags StagesAfter = ShaderStageFlags.None
    ) : this(
        Buffer, 0, Buffer.Size,
        default, default, AccessBefore, AccessAfter, StagesBefore, StagesAfter
    ) { }

    public BufferBarrier(
        IGpuResource Buffer,
        ulong Offset,
        ulong Size,
        ResAccess AccessBefore,
        ResAccess AccessAfter,
        ShaderStageFlags StagesBefore = ShaderStageFlags.None,
        ShaderStageFlags StagesAfter = ShaderStageFlags.None
    ) : this(
        Buffer, Offset, Size,
        default, default, AccessBefore, AccessAfter, StagesBefore, StagesAfter
    ) { }
}

public record struct ImageBarrier(
    IGpuResource Image,
    SubResourceRange SubResourceRange,
    LegacyState LegacyBefore,
    LegacyState LegacyAfter,
    ResLayout LayoutBefore,
    ResLayout LayoutAfter,
    ResAccess AccessBefore,
    ResAccess AccessAfter,
    ShaderStageFlags StagesBefore = ShaderStageFlags.None,
    ShaderStageFlags StagesAfter = ShaderStageFlags.None,
    ImageBarrierFlags Flags = ImageBarrierFlags.None
)
{
    public IGpuResource Image = Image;
    public SubResourceRange SubResourceRange = SubResourceRange;
    public LegacyState LegacyBefore = LegacyBefore;
    public LegacyState LegacyAfter = LegacyAfter;
    public ResLayout LayoutBefore = LayoutBefore;
    public ResLayout LayoutAfter = LayoutAfter;
    public ResAccess AccessBefore = AccessBefore;
    public ResAccess AccessAfter = AccessAfter;
    public ShaderStageFlags StagesBefore = StagesBefore;
    public ShaderStageFlags StagesAfter = StagesAfter;
    public ImageBarrierFlags Flags = Flags;

    public ImageBarrier(
        IGpuResource Image,
        LegacyState LegacyBefore,
        LegacyState LegacyAfter,
        ResLayout LayoutBefore,
        ResLayout LayoutAfter,
        ResAccess AccessBefore,
        ResAccess AccessAfter,
        ShaderStageFlags StagesBefore = ShaderStageFlags.None,
        ShaderStageFlags StagesAfter = ShaderStageFlags.None,
        ImageBarrierFlags Flags = ImageBarrierFlags.None
    ) : this(
        Image, new(Image),
        LegacyBefore, LegacyAfter, LayoutBefore, LayoutAfter, AccessBefore, AccessAfter, StagesBefore, StagesAfter,
        Flags
    ) { }

    public ImageBarrier(
        IGpuResource Image,
        SubResourceRange SubResourceRange,
        LegacyState LegacyBefore,
        LegacyState LegacyAfter,
        ImageBarrierFlags Flags = ImageBarrierFlags.None
    ) : this(
        Image, SubResourceRange,
        LegacyBefore, LegacyAfter, default, default, default, default, default, default, Flags
    ) { }

    public ImageBarrier(
        IGpuResource Image,
        LegacyState LegacyBefore,
        LegacyState LegacyAfter,
        ImageBarrierFlags Flags = ImageBarrierFlags.None
    ) : this(
        Image, new(Image),
        LegacyBefore, LegacyAfter, default, default, default, default, default, default, Flags
    ) { }

    public ImageBarrier(
        IGpuResource Image,
        ResLayout LayoutBefore,
        ResLayout LayoutAfter,
        ResAccess AccessBefore,
        ResAccess AccessAfter,
        ShaderStageFlags StagesBefore = ShaderStageFlags.None,
        ShaderStageFlags StagesAfter = ShaderStageFlags.None,
        ImageBarrierFlags Flags = ImageBarrierFlags.None
    ) : this(
        Image, new(Image),
        default, default, LayoutBefore, LayoutAfter, AccessBefore, AccessAfter, StagesBefore, StagesAfter, Flags
    ) { }

    public ImageBarrier(
        IGpuResource Image,
        SubResourceRange SubResourceRange,
        ResLayout LayoutBefore,
        ResLayout LayoutAfter,
        ResAccess AccessBefore,
        ResAccess AccessAfter,
        ShaderStageFlags StagesBefore = ShaderStageFlags.None,
        ShaderStageFlags StagesAfter = ShaderStageFlags.None,
        ImageBarrierFlags Flags = ImageBarrierFlags.None
    ) : this(
        Image, SubResourceRange,
        default, default, LayoutBefore, LayoutAfter, AccessBefore, AccessAfter, StagesBefore, StagesAfter, Flags
    ) { }
}

[Flags]
public enum ImageBarrierFlags : uint
{
    None,
    Discard = 1 << 0,
    CrossQueue = 1 << 1,
}

public record struct SubResourceRange(
    uint IndexOrFirstMipLevel,
    uint NumMipLevels,
    uint FirstArraySlice,
    uint NumArraySlices,
    uint FirstPlane,
    uint NumPlanes
)
{
    public uint IndexOrFirstMipLevel = IndexOrFirstMipLevel;
    public uint NumMipLevels = NumMipLevels;
    public uint FirstArraySlice = FirstArraySlice;
    public uint NumArraySlices = NumArraySlices;
    public uint FirstPlane = FirstPlane;
    public uint NumPlanes = NumPlanes;

    public SubResourceRange(IGpuResource Image)
        : this(0, Image.MipLevels, 0, Image.DepthOrLength, 0, Image.Planes) { }
}
