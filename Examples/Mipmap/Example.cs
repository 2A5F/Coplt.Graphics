using System.Runtime.InteropServices;
using Coplt.Graphics;
using Coplt.Graphics.Core;
using Coplt.Graphics.States;
using Coplt.Mathematics;

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
                new()
                {
                    Id = 0,
                    Slot = 0,
                    Stage = ShaderStage.Compute,
                    View = ShaderLayoutItemView.Constants,
                    Count = 4,
                    Type = ShaderLayoutItemType.ConstantBuffer,
                },
                new()
                {
                    Id = 1,
                    Slot = 0,
                    Count = uint.MaxValue,
                    Stage = ShaderStage.Compute,
                    View = ShaderLayoutItemView.Uav,
                    Type = ShaderLayoutItemType.Texture2D,
                    UavAccess = ResourceAccess.ReadWrite,
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
                            Stages = ShaderStageFlags.Compute,
                            View = ShaderLayoutItemView.Constants,
                            Count = 4,
                            Type = ShaderLayoutItemType.ConstantBuffer,
                        },
                        new()
                        {
                            Id = 1,
                            Count = uint.MaxValue,
                            Stages = ShaderStageFlags.Compute,
                            View = ShaderLayoutItemView.Uav,
                            Type = ShaderLayoutItemType.Texture2D,
                        }
                    ],
                    Usage: BindGroupUsage.Dynamic
                )
            ], Name: Name
        );
        Shader = Device.CreateShader(modules, ShaderLayout);
        Pipeline = Device.CreateComputeShaderPipeline(
            Shader, BindingLayout, Name: Name
        );
        BindGroup = Isolate.CreateBindGroup(BindGroupLayout, []);
        Binding = Isolate.CreateBinding(BindingLayout, [new(0, BindGroup)]);

        using var image_data = await LoadImage("./pattern.png");
        var upload_memory = cmd.AllocImageUploadMemory2D(4, (uint)image_data.Width, (uint)image_data.Height);
        for (var row = 0u; row < upload_memory.RowCount; row++)
        {
            var row_span = image_data.Frames[0].PixelBuffer.DangerousGetRowSpan((int)row);
            MemoryMarshal.AsBytes(row_span).CopyTo(upload_memory[row]);
        }
        var mip_levels = 1 + (uint)math.log2((double)math.max(image_data.Width, image_data.Height));
        var test_image = Isolate.CreateImage(
            new()
            {
                Purpose = ResourcePurpose.ShaderResource,
                Format = GraphicsFormat.R8G8B8A8_UNorm,
                Width = (uint)image_data.Width,
                Height = (uint)image_data.Height,
                MipLevels = mip_levels,
            },
            Name: "Test Image"
        );
        cmd.Upload(test_image, upload_memory);

        GenMipmaps(cmd, test_image);
    }

    private void GenMipmaps(GpuRecord cmd, GpuImage image)
    {
        using var compute = cmd.Compute();
        compute.SetBinding(Binding);
        compute.SetDynSize(0, 4);
        compute.SetConstants(0, 0, [image.Width, image.Height, 4]);
        compute.SetDynItem(
            0, [
                new(1, 0, image.View2D(0, 1)),
                new(1, 1, image.View2D(1, 1)),
                new(1, 2, image.View2D(2, 1)),
                new(1, 3, image.View2D(3, 1)),
            ]
        );
        compute.Dispatch(Pipeline, 1, 1);
    }

    protected override void Render(GpuRecord cmd, Time time)
    {
        cmd.ClearColor(Output, new Color(0.83f, 0.8f, 0.97f));
    }
}
