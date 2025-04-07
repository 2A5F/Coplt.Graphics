using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private ShaderLayout ShaderLayout = null!;
    private ShaderBindGroupLayout BindGroupLayout = null!;
    private ShaderBindingLayout BindingLayout = null!;
    private Shader Shader = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private GpuBuffer ArgBuffer = null!;
    private ShaderBindGroup BindGroup = null!;
    private ShaderBinding Binding = null!;

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
                BindGroupLayout = Device.CreateBindGroupLayout(
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
        ArgBuffer = Isolate.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.ConstantBuffer,
                Size = sizeof(float),
            },
            Name: "Args"
        );
        BindGroup = Isolate.CreateBindGroup(BindGroupLayout, [new(0, ArgBuffer)]);
        Binding = Isolate.CreateBinding(BindingLayout, [new(0, BindGroup)]);
    }
    protected override void Render(GpuRecord cmd, Time time)
    {
        cmd.Upload(ArgBuffer, [(float)time.Total.TotalSeconds]);
        using var render = cmd.Render([new(Output, LoadOp.Discard)]);
        render.Draw(Pipeline, 4, Binding: Binding);
    }
}
