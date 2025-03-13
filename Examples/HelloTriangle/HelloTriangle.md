# HelloTriangle

The most basic usage, basic swap chain creation, pipeline creation.  
No any resources, including vertex buffers.  

![](./HelloTriangle.png)

## 1 InitGraphics

```cs
// Load an instance of the specified backend
Graphics = GraphicsInstance.LoadD3d12();
// Creating a Graphics Device
Device = Graphics.CreateDevice(Debug: true, Name: "Main Device");
// Creating a swap chain
Output = Device.MainQueue.CreateOutputForHwnd(new() { Width = Width, Height = Height }, Hwnd);
```

## 2 LoadResources
```cs
// See the source code for details
// This shader module is not a vk module. A module can only have one shader stage with one entrypoint.
// The main purpose is to use native memory to store data to avoid complex memory pinning.
var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
// To create a shader, you need to provide the shader stages (shader module)
// and the binding layout, input layout (if have vertex shader).
Shader = Device.CreateShader(modules, null, Device.CreateShaderInputLayout([]));
// Then create a pipeline using the graphics state
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
    }, Name: "HelloTriangle"
);
```

## 3 Render
```cs
var cmd = Device.MainCommandList;
while (!IsClosed)
{
   Render(cmd, CalcTime());
   Output.Present();
}

void Render(CommandList cmd, Time time)
{
    using var render = cmd.Render([new(Output, new float4(0.83f, 0.8f, 0.97f, 1f))]);
    render.Draw(Pipeline, 3);
}
```
