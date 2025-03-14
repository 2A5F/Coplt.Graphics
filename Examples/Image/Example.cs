using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
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

    public override string Name => "Image";
    protected override async Task LoadResources(CommandList cmd)
    {
        var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
        Layout = Device.CreateShaderLayout(
            [
                new()
                {
                    Slot = 0,
                    Stage = ShaderStage.Pixel,
                    View = ShaderLayoutItemView.Srv,
                    Type = ShaderLayoutItemType.Texture2D,
                    Usage = ShaderLayoutItemUsage.Persist,
                },
                new()
                {
                    Slot = 0,
                    Stage = ShaderStage.Pixel,
                    View = ShaderLayoutItemView.Sampler,
                    Type = ShaderLayoutItemType.Sampler,
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

        using var image_data = await LoadImage("./Image.png");
        var upload_memory = Device.MainQueue.AllocImageUploadMemory(4, (uint)image_data.Width, (uint)image_data.Height, 1, out var loc);
        for (var row = 0u; row < upload_memory.RowCount; row++)
        {
            var row_span = image_data.Frames[0].PixelBuffer.DangerousGetRowSpan((int)row);
            MemoryMarshal.AsBytes(row_span).CopyTo(upload_memory[row]);
        }
        var test_buffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.None,
                Size = (uint)upload_memory.RawSpan.Length,
            }
        );
        cmd.Upload(test_buffer, loc);
        // var test_image = Device.CreateImage(
        //     new()
        //     {
        //         Purpose = ResourcePurpose.ShaderResource,
        //         Format = GraphicsFormat.R8G8B8A8_UNorm_sRGB,
        //         Width = (uint)image_data.Width,
        //         Height = (uint)image_data.Height,
        //     }
        // );
        // cmd.Upload(test_buffer, loc);
    }
    protected override void Render(CommandList cmd, Time time)
    {
        using var render = cmd.Render([new(Output, new Color(0.83f, 0.8f, 0.97f))]);
        render.Draw(Pipeline, 4, Binding: ShaderBinding);
    }
}
