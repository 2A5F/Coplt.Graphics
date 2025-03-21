using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private Shader Shader = null!;
    private GraphicsShaderPipeline Pipeline = null!;

    public override string Name => "Hello Triangle";
    protected override async Task LoadResources(GpuRecord cmd)
    {
        var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
        Shader = Device.CreateShader(modules, null, Device.CreateShaderInputLayout([]));
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
                }
            }, Name: Name
        );
    }
    protected override void Render(GpuRecord cmd, Time time)
    {
        cmd.ClearColor(Output, new Color(0.83f, 0.8f, 0.97f));
        // using var render = cmd.Render([new(Output, new Color(0.83f, 0.8f, 0.97f))]);
        // render.Draw(Pipeline, 3);
    }
}
