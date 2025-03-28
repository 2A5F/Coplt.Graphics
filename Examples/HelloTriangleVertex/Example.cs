using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private Shader Shader = null!;
    private MeshLayout MeshLayout = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private GpuBuffer PositionColorBuffer = null!;
    private GpuBuffer UvBuffer = null!;

    public override string Name => "Hello Triangle Vertex";
    protected override async Task LoadResources(GpuRecord cmd)
    {
        var SlotId_Position = Graphics.GetSlotId("Position");
        var SlotId_Color = Graphics.GetSlotId("Color");
        var SlotId_Uv = Graphics.GetSlotId("Uv");
        var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
        Shader = Device.CreateShader(
            modules, null,
            Device.CreateShaderInputLayout(
                [new(SlotId_Position), new(SlotId_Color), new(SlotId_Uv)]
            )
        );
        MeshLayout = Device.CreateMeshLayout(
            [
                new(Stride: sizeof(float) * 7),
                new(Stride: sizeof(float) * 2),
            ],
            [
                new(SlotId_Position, GraphicsFormat.R32G32B32_Float),
                new(SlotId_Color, GraphicsFormat.R32G32B32A32_Float, Offset: sizeof(float) * 3),
                new(SlotId_Uv, GraphicsFormat.R32G32_Float, BufferIndex: 1),
            ],
            Name: "Mesh"
        );
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
            }, MeshLayout, Name: Name
        );
        PositionColorBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.VertexBuffer,
                Size = sizeof(float) * 4 * 6,
            }, "Position and Color"
        );
        UvBuffer = Device.CreateBuffer(
            new()
            {
                Purpose = ResourcePurpose.VertexBuffer,
                Size = sizeof(float) * 2 * 3,
            }, "Uv"
        );
        cmd.Upload(
            PositionColorBuffer, [
                /* position */ 0.0f, 0.5f * 1.7f, 0f, /* color */ 0.71f, 0.22f, 0.21f, 1f,
                /* position */ 0.5f, -0.5f * 1.7f, 0f, /* color */ 0.34f, 0.64f, 0.31f, 1f,
                /* position */ -0.5f, -0.5f * 1.7f, 0f, /* color */ 0.36f, 0.42f, 0.92f, 1f,
            ]
        );
        cmd.Upload(
            UvBuffer, [
                0.0f, 0.0f,
                -1.0f, 2.0f,
                1.0f, 2.0f,
            ]
        );
    }
    protected override void Render(GpuRecord cmd, Time time)
    {
        using var render = cmd.Render([new(Output, new Color(0.83f, 0.8f, 0.97f))], Name: Name);
        // render.SetMeshBuffers(
        //     MeshLayout, [
        //         new(0, PositionColorBuffer),
        //         new(1, UvBuffer),
        //     ]
        // );
        // render.Draw(Pipeline, 3);
    }
}
