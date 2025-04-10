# Colorful

Demonstrates the use of shader binding

![](./Colorful.webp)

## The Pixel Shader

```hlsl
cbuffer Args : register(b0)
{
    float time;
} 

[shader("pixel")]
float4 Pixel(Varying input) : SV_Target
{
    return float4(input.Uv, (cos(time) + 1) * 0.5, 1);
}
```

## 2 LoadResources
```cs
var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);

// Defines the shader layout. 
// This rhi does not provide shader compilation and reflection related features, and needs to be provided by the user.
ShaderLayout = Device.CreateShaderLayout(
    [
        new()
        {
            Id = 0,                                         // The ID of the binding point. different stages and scopes can be repeated. 
            Scope = 0,                                      // The Id Scope
            Slot = 0,                                       // Dx: hlsl register; Vk: binding or push const offset when View is Constants
            Space = 0,                                      // Dx: hlsl space; Vk: binding set
            Count = 1,                                      // The size of the array, must >= 1, or == 1 when View is StaticSampler, or num of 32 bit value when view is Constants
            Format = GraphicsFormat.Unknown,                // The format
            Stage = ShaderStage.Pixel,                      // Can only be bound to a single shader stage
            View = ShaderLayoutItemView.Cbv,                // View type, Cbv Srv Uav Sampler Constants
            Type = ShaderLayoutItemType.ConstantBuffer,     // Resource type, use hlsl style, ConstantBuffer StructureBuffer Texture2D etc
            UavAccess = ResourceAccess.Unknown,             // Used to distinguish read and write behaviors during uav access
        }
    ]
);
// Define the bind group layout and binding layout
BindingLayout = Device.CreateBindingLayout(
    ShaderLayout, [
        // A binding layout has multiple binding group layouts, and a binding group can be shared among multiple bindings.
        // A typical usage is to extract shared binding groups for bindings shared by all materials,
        // and separate binding groups for bindings unique to each material, and split static and dynamic bindings into different binding groups.
        BindGroupLayout = Device.CreateBindGroupLayout(
            [
                new()
                {
                    Id = 0,                                         // Same to Id in ShaderLayout
                    Scope = 0,                                      // Same to Scope in ShaderLayout
                    Count = 1,                                      // Must be >= Count in ShaderLayout
                    StaticSamplerIndex = 0,                         // Index if View is StaticSampler
                    Format = GraphicsFormat.Unknown,                // Must same to Format in ShaderLayout
                    Stages = ShaderStageFlags.Pixel,                // Allow multiple stages
                    View = ShaderLayoutItemView.Cbv,                // Must same to View in ShaderLayout
                    Type = ShaderLayoutItemType.ConstantBuffer,     // Must same to Type in ShaderLayout
                    UavAccess = ResourceAccess.Unknown,             // If in shader layout is ResourceAccess.Unknown, then use this
                }
            ]
        )
    ], Name: Name
);

Shader = Device.CreateShader(modules, ShaderLayout);
Pipeline = Device.CreateGraphicsShaderPipeline(Shader, new() { .. Omitted here }, BindingLayout, Name: Name);

// Create argument buffer
ArgBuffer = Device.CreateBuffer(
    new()
    {
        Purpose = ResourcePurpose.ConstantBuffer,
        Size = sizeof(float),
    },
    Name: "Args"
);

// Create Binding
BindGroup = Isolate.CreateBindGroup(BindGroupLayout, [new(0, ArgBuffer)]);
Binding = Isolate.CreateBinding(BindingLayout, [new(0, BindGroup)]);
```

## 3 Render
```cs
// Write time data
cmd.Upload(ArgBuffer, [(float)time.Total.TotalSeconds]);
using var render = cmd.Render([new(Output, new Color(0.83f, 0.8f, 0.97f, 1f))]);
render.Draw(Pipeline, 4, Binding: Binding);
```
