using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
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
    private Sampler Sampler = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private ShaderBindGroup BindGroup = null!;
    private ShaderBinding Binding = null!;

    public override string Name => "Image";
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
                    View = ShaderLayoutItemView.Srv,
                    Type = ShaderLayoutItemType.Texture2D,
                },
                new()
                {
                    Id = 1,
                    Slot = 0,
                    Stage = ShaderStage.Pixel,
                    View = ShaderLayoutItemView.Sampler,
                    Type = ShaderLayoutItemType.Sampler,
                }
            ],
            Name: Name
        );
        BindingLayout = Device.CreateBindingLayout(
            ShaderLayout, [
                BindGroupLayout = Device.CreateBindGroupLayout(
                    [
                        new()
                        {
                            Id = 0,
                            Stages = ShaderStageFlags.Pixel,
                            View = ShaderLayoutItemView.Srv,
                            Type = ShaderLayoutItemType.Texture2D,
                        },
                        new()
                        {
                            Id = 1,
                            Stages = ShaderStageFlags.Pixel,
                            View = ShaderLayoutItemView.Sampler,
                            Type = ShaderLayoutItemType.Sampler,
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
        Sampler = Device.CreateSampler(SamplerInfo.LinearRepeat);
        using var image_data = await LoadImage("./Image.png");
        var upload_memory = cmd.AllocImageUploadMemory2D(4, (uint)image_data.Width, (uint)image_data.Height);
        for (var row = 0u; row < upload_memory.RowCount; row++)
        {
            var row_span = image_data.Frames[0].PixelBuffer.DangerousGetRowSpan((int)row);
            MemoryMarshal.AsBytes(row_span).CopyTo(upload_memory[row]);
        }
        var test_image = Isolate.CreateImage(
            new()
            {
                Purpose = ResourcePurpose.ShaderResource,
                Format = GraphicsFormat.R8G8B8A8_UNorm,
                Width = (uint)image_data.Width,
                Height = (uint)image_data.Height,
            },
            Name: "Test Image"
        );
        cmd.Upload(test_image, upload_memory);
        BindGroup = Isolate.CreateBindGroup(BindGroupLayout, [new(0, test_image), new(1, Sampler)]);
        Binding = Isolate.CreateBinding(BindingLayout, [new(0, BindGroup)]);
    }
    protected override void Render(GpuRecord cmd, Time time)
    {
        using var render = cmd.Render([new(Output, LoadOp.Discard)]);
        render.Draw(Pipeline, 4, Binding: Binding);
    }
}
