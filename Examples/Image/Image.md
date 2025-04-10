# Image

Demonstrates the use of image load, upload and sampler

![](./Image.png)

## 2 LoadResources
```cs
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
Pipeline = Device.CreateGraphicsShaderPipeline(.. Omitted here);

Sampler = Device.CreateSampler(SamplerInfo.LinearRepeat);
// or
new SamplerInfo() {
    Mag = FilterMode.Linear,
    Min = FilterMode.Linear,
    Mipmap = FilterMode.Point,
};

// Load iamge use SixLabors.ImageSharp for demo
// SixLabors.ImageSharp is not required, you can use any image loading library
using var image_data = await LoadImage("./Image.png");

// Allocate memory for image upload. Due to the 256 row alignment requirement, 
// the memory cannot be simply calculated and cannot be used as a whole block of memory.
// This rhi does not provide a high-level abstraction for uploading images, as any image loading library can be used
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
```

## 3 Render
```cs
using var render = cmd.Render([new(Output, LoadOp.Discard)]);
render.Draw(Pipeline, 4, Binding: Binding);
```
