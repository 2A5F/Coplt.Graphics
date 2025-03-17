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
        GpuImage Image();
        Sampler Sampler();
    }

    public static View None => default;

    public static implicit operator View(GpuBuffer buffer) => MakeBuffer(buffer);
    public static implicit operator View(GpuImage image) => MakeImage(image);
    public static implicit operator View(Sampler sampler) => MakeSampler(sampler);

    public FView ToFFI() => Tag switch
    {
        Tags.None    => new FView { Type = FViewType.None },
        Tags.Buffer  => new FView { Type = FViewType.Buffer, Buffer = Buffer.Ptr },
        Tags.Image   => new FView { Type = FViewType.Image, Image = Image.Ptr },
        Tags.Sampler => new FView { Type = FViewType.Sampler, Sampler = Sampler.Ptr },
        _            => throw new ArgumentOutOfRangeException()
    };
}
