﻿using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;
using Coplt.Mathematics;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height) : ExampleBase(Handle, Width, Height)
{
    private Shader Shader = null!;
    private GraphicsShaderPipeline Pipeline = null!;
    private ShaderBinding ShaderBinding = null!;

    public override string Name => "Colorful";
    protected override async Task LoadResources(CommandList cmd)
    {
        var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
        Shader = Device.CreateShader(
            modules, Device.CreateShaderLayout([]), Device.CreateShaderInputLayout([])
        );
        ShaderBinding = Device.CreateShaderBinding(Shader.Layout!, Name: Name);
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
    }
    protected override void Render(CommandList cmd, Time time)
    {
        cmd.SetRenderTargets([Output]);
        cmd.ClearColor(Output, new float4(0.83f, 0.8f, 0.97f, 1f));
        cmd.Draw(Pipeline, 4);
    }
}
