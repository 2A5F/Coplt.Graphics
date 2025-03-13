using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private ShaderLayout Layout = null!;
    private Shader Shader = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private ShaderBinding ShaderBinding = null!;
    private GpuBuffer ArgBuffer = null!;

    public override string Name => "Colorful";
    protected override async Task LoadResources(CommandList cmd)
    {
        var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
        Layout = Device.CreateShaderLayout(
            [
                new()
                {
                    Slot = 0,
                    Stage = ShaderStage.Pixel,
                    View = ShaderLayoutItemView.Cbv,
                    Type = ShaderLayoutItemType.ConstantBuffer,
                    Usage = ShaderLayoutItemUsage.Persist,
                }
            ],
            Name: Name
        );
        Shader = Device.CreateShader(modules, Layout);
        ShaderBinding = Device.CreateShaderBinding(Layout, Name: Name);
        Pipeline = Device.CreateGraphicsShaderPipeline(
            Shader, new()
            {
                DsvFormat = GraphicsFormat.Unknown,
                BlendState =
                {
                    Rt0 =
                    {
                        Src = BlendType.SrcAlpha,
                        Dst = BlendType.InvSrcAlpha,
                        Op = BlendOp.Add,
                    }
                },
                Topology = PrimitiveTopologyType.TriangleStrip,
            }, Name: Name
        );
        ArgBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.ConstantBuffer,
                Size = sizeof(float),
            },
            Name: "Args"
        );
        cmd.Bind(ShaderBinding, [new(0, ArgBuffer)]);
    }
    protected override void Render(CommandList cmd, Time time)
    {
        cmd.Upload(ArgBuffer, [(float)time.Total.TotalSeconds]);
        using var render = cmd.Render([new(Output, new Color(0.83f, 0.8f, 0.97f))]);
        render.Draw(Pipeline, 4, Binding: ShaderBinding);
    }
}
