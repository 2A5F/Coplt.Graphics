using Coplt.Graphics.Native;
using Coplt.Union;

namespace Coplt.Graphics.Core;

[Union]
public unsafe partial struct View
{
    [UnionTemplate]
    private interface Template
    {
        void None();
        GpuBuffer Buffer();
    }

    public static View None => default;

    public static implicit operator View(GpuBuffer buffer) => MakeBuffer(buffer);

    public FView ToFFI() => Tag switch
    {
        Tags.None   => new FView { Type = FViewType.None },
        Tags.Buffer => new FView { Type = FViewType.Buffer, Buffer = Buffer.Ptr },
        _           => throw new ArgumentOutOfRangeException()
    };
}
