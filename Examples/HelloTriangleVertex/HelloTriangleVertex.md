# HelloTriangleVertex

Based on [HelloTriangle](../HelloTriangle/HelloTriangle.md)

Demonstrates the use of vertex buffers

![](../HelloTriangle/HelloTriangle.png)

## 2 LoadResources
```cs
// Convert semantic names to integer ids for easy comparison
var SlotId_Position = Graphics.GetSlotId("Position");
var SlotId_Color = Graphics.GetSlotId("Color");
var SlotId_Uv = Graphics.GetSlotId("Uv");
        
var modules = await LoadShaderModules("Shader", [ShaderStage.Vertex, ShaderStage.Pixel]);
Shader = Device.CreateShader(
    modules, null,
    // Provide input layout
    Device.CreateShaderInputLayout(
        [new(SlotId_Position), new(SlotId_Color), new(SlotId_Uv)]
    )
);

MeshLayout = Device.CreateMeshLayout(
    /* Buffers: */ [
        new(Stride: sizeof(float) * 7),
        new(Stride: sizeof(float) * 2),
    ],
    /* Elements: */ [
        new(SlotId_Position, GraphicsFormat.R32G32B32_Float),
        new(SlotId_Color, GraphicsFormat.R32G32B32A32_Float, Offset: sizeof(float) * 3),
        new(SlotId_Uv, GraphicsFormat.R32G32_Float, BufferIndex: 1),
    ],
    Name: "Mesh"
);

Pipeline = Device.CreateGraphicsShaderPipeline(
    // The input layout determines shader-related definitions
    Shader, 
    new()
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
    }, 
    // And the mesh layout determines buffer-related definitions
    MeshLayout, 
    Name: "HelloTriangleVertex"
);

// Creating vertex buffers
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

// Upload vertex data
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
```

## 3 Render
```cs
using var render = cmd.Render([new(Output, new float4(0.83f, 0.8f, 0.97f, 1f))]);
render.SetMeshBuffers(
    MeshLayout, [
        new(0, PositionColorBuffer),
        new(1, UvBuffer),
    ]
);
render.Draw(Pipeline, 3);
```
