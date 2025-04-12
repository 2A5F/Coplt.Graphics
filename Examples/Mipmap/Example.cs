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
    private ComputeShaderPipeline Pipeline = null!;
    private GpuBuffer ArgBuffer = null!;
    private ShaderBindGroup BindGroup = null!;
    private ShaderBinding Binding = null!;

    public override string Name => "Mipmap";
    protected override async Task LoadResources(GpuRecord cmd)
    {
        var modules = await LoadShaderModules("Shader", [ShaderStage.Compute]);
        ShaderLayout = Device.CreateShaderLayout(
            [
                // new()
                // {
                //     Id = 0,
                //     Slot = 0,
                //     Stage = ShaderStage.Pixel,
                //     View = ShaderLayoutItemView.Cbv,
                //     Type = ShaderLayoutItemType.ConstantBuffer,
                // }
            ]
        );
        BindingLayout = Device.CreateBindingLayout(
            ShaderLayout, [
                BindGroupLayout = Device.CreateBindGroupLayout(
                    [
                        // new()
                        // {
                        //     Id = 0,
                        //     Stages = ShaderStageFlags.Pixel,
                        //     View = ShaderLayoutItemView.Cbv,
                        //     Type = ShaderLayoutItemType.ConstantBuffer,
                        // }
                    ]
                )
            ], Name: Name
        );
        Shader = Device.CreateShader(modules, ShaderLayout);
        Pipeline = Device.CreateComputeShaderPipeline(
            Shader, BindingLayout, Name: Name
        );
        
        using var image_data = await LoadImage("./pattern.png");
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

        GenMipmaps(cmd);
    }

    private void GenMipmaps(GpuRecord cmd)
    {
        using var compute = cmd.Compute();
        compute.Dispatch(Pipeline, 1, 1, 1);
    }

    protected override void Render(GpuRecord cmd, Time time)
    {
        cmd.ClearColor(Output, new Color(0.83f, 0.8f, 0.97f));
    }
}
