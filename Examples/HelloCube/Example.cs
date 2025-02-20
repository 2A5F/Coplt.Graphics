using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;
using Coplt.Mathematics;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private Shader Shader = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private GpuBuffer MeshBuffer = null!;
    private GpuBuffer TestBuffer = null!;

    public override string Name => "HelloCube";
    protected override async Task LoadResources(CommandList cmd)
    {
        var shader_name = "HelloTriangle";
        var modules = await LoadShaderModules(shader_name, [ShaderStage.Vertex, ShaderStage.Pixel]);
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
            }, Name: shader_name
        );
        MeshBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.VertexBuffer,
                Size = sizeof(float) * 4 * 3 * 2,
            }, "Cube Mesh"
        );
        TestBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.VertexBuffer,
                Size = sizeof(float) * 4 * 3 * 2,
            }, "Test Buffer"
        );
        cmd.BufferCopy(MeshBuffer, TestBuffer);
    }
    protected override void Render(CommandList cmd)
    {
        cmd.SetRenderTargets([Output]);
        cmd.ClearColor(Output, new float4(0.83f, 0.8f, 0.97f, 1f));
        cmd.Draw(Pipeline, 3);
    }
}
