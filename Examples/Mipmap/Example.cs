using System.Runtime.CompilerServices;
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
                Purpose = ResourcePurpose.ShaderResource | ResourcePurpose.UnorderedAccess,
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

    [InlineArray(5)]
    private struct SetShaderBindItemArr
    {
        private SetShaderBindItem _;
    }

    private void GenMipmaps(GpuRecord cmd, GpuImage image)
    {
        // Not handling non-power-of-2 cases
        using var compute = cmd.Compute(Name: "GenMipmaps");
        compute.SetBinding(Binding);
        var size = new uint2(image.Width, image.Height);
        var i = 0u;
        var l = (image.MipLevels - 1u) % 4;
        if (l == 0) l = 4;
        for (; i < image.MipLevels; i += l, size >>= (int)l, l = 4)
        {
            var mip_levels = math.min(image.MipLevels - i - 1, 4);
            var arr = math.min(l, image.MipLevels - i - 1);
            if (arr == 0) break;
            var half_size = size >> 1;
            var groups = (half_size + 7) >> 3;
            compute.SetDynArraySize(BindGroup, arr + 1);
            compute.SetConstants(BindGroup, 0, [size.x, size.y, mip_levels, arr]);
            Span<SetShaderBindItem> set_items =
            [
                new(1, 0, image.View2D((byte)(i + 0))),
                new(1, 1, image.View2D((byte)(i + 1))),
                new(1, 2, image.View2D((byte)(i + 2))),
                new(1, 3, image.View2D((byte)(i + 3))),
                new(1, 4, image.View2D((byte)(i + 4))),
            ];
            compute.SetBindItem(BindGroup, set_items[..((int)arr + 1)]);
            compute.Dispatch(Pipeline, groups.x, groups.y);
        }
    }

    protected override void Render(GpuRecord cmd, Time time)
    {
        cmd.ClearColor(Output, new Color(0.83f, 0.8f, 0.97f));
    }
}
