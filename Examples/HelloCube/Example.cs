using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;
using Coplt.Mathematics;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private Shader Shader = null!;
    private MeshLayout MeshLayout = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private GpuBuffer MeshBuffer = null!;

    public override string Name => "HelloCube";
    protected override async Task LoadResources(CommandList cmd)
    {
        var shader_name = "HelloCube";
        var SlotId_Position = Graphics.GetSlotId("Position");
        var modules = await LoadShaderModules(shader_name, [ShaderStage.Vertex, ShaderStage.Pixel]);
        Shader = Device.CreateShader(
            modules, null,
            Device.CreateShaderInputLayout([new("Position", SlotId: SlotId_Position)])
        );
        MeshLayout = Device.CreateMeshLayout(
            [new(sizeof(float) * 4)],
            [new(SlotId_Position, GraphicsFormat.R32G32B32A32_Float)],
            Name: "Cube"
        );
        Pipeline = Device.CreateGraphicsShaderPipeline(
            Shader, new() { DsvFormat = GraphicsFormat.Unknown, }, MeshLayout, Name: shader_name
        );
        MeshBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.VertexBuffer,
                Size = sizeof(float) * 4 * 3 * 2,
            }, "Cube Mesh"
        );
        cmd.Upload(
            MeshBuffer, [
                new float4(0.0f, 0.5f * 1.7f, 0f, 1f),
                new float4(0.5f, -0.5f * 1.7f, 0f, 1f),
                new float4(-0.5f, -0.5f * 1.7f, 0f, 1f),
            ]
        );
    }
    protected override void Render(CommandList cmd)
    {
        cmd.SetRenderTargets([Output]);
        cmd.ClearColor(Output, new float4(0.83f, 0.8f, 0.97f, 1f));
        cmd.SetMeshBuffers(MeshLayout, [new(0, MeshBuffer)]);
        cmd.Draw(Pipeline, 3);
    }
}
