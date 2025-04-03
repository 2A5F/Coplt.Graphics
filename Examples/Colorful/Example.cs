using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private ShaderLayout ShaderLayout = null!;
    private BindingLayout BindingLayout = null!;
    private Shader Shader = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private ShaderBinding ShaderBinding = null!;
    private GpuBuffer ArgBuffer = null!;

    public override string Name => "Colorful";
    protected override async Task LoadResources(GpuRecord cmd)
    {
        var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
        ShaderLayout = Device.CreateShaderLayout(
            [
                new()
                {
                    Id = 0,
                    Slot = 0,
                    Stage = ShaderStage.Pixel,
                    View = ShaderLayoutItemView.Cbv,
                    Type = ShaderLayoutItemType.ConstantBuffer,
                }
            ]
        );
        BindingLayout = Device.CreateBindingLayout(
            ShaderLayout, [
                Device.CreateBindGroupLayout(
                    [
                        new()
                        {
                            Id = 0,
                            Stages = ShaderStageFlags.Pixel,
                            View = ShaderLayoutItemView.Cbv,
                            Type = ShaderLayoutItemType.ConstantBuffer,
                        }
                    ]
                )
            ], Name: Name
        );
        Shader = Device.CreateShader(modules, ShaderLayout);
        // ShaderBinding = Device.CreateShaderBinding(Layout, Name: Name);
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
            }, BindingLayout, Name: Name
        );
        ArgBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.ConstantBuffer,
                Size = sizeof(float),
            },
            Name: "Args"
        );
        // cmd.Bind(ShaderBinding, [new(0, ArgBuffer)]);
    }
    protected override void Render(GpuRecord cmd, Time time)
    {
        cmd.Upload(ArgBuffer, [(float)time.Total.TotalSeconds]);
        using var render = cmd.Render([new(Output, LoadOp.Discard)]);
        // render.Draw(Pipeline, 4, Binding: ShaderBinding);
        render.Draw(Pipeline, 4);
    }
}
