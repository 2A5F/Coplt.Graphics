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
Layout = Device.CreateShaderLayout(
    [
        new()
        {
            Slot = 0,
            Stage = ShaderStage.Pixel,
            View = ShaderLayoutItemView.Cbv,
            Type = ShaderLayoutItemType.ConstantBuffer,
            Usage = ShaderLayoutItemUsage.Persist,
        }
    ],
    Name: Name
);

Shader = Device.CreateShader(modules, Layout);
Pipeline = Device.CreateGraphicsShaderPipeline(... Omitted here);

// Shader Binding instance, maybe it's similar to webgpu's bindgroup, but allows dynamic changes
ShaderBinding = Device.CreateShaderBinding(Layout, Name: Name);

// Create argument buffer
ArgBuffer = Device.CreateBuffer(
    new()
    {
        Purpose = ResourcePurpose.ConstantBuffer,
        Size = sizeof(float),
    },
    Name: "Args"
);
// Bind it
cmd.Bind(ShaderBinding, [new(0, ArgBuffer)]);
```

## 3 Render
```cs
// Write time data
cmd.Upload(ArgBuffer, [(float)time.Total.TotalSeconds]);
using var render = cmd.Render([new(Output, new float4(0.83f, 0.8f, 0.97f, 1f))]);
render.Draw(Pipeline, 4, Binding: ShaderBinding);
```
