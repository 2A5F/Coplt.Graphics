# Coplt.Graphics

[![Build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

Render Hardware Interface designed for c#
- Use com style object
- Utf16 string support
- Try to avoid string passing
- Minimize native calls and use memory to transfer data in batches to reduce GC transitions
- Avoid pointer nesting as much as possible, as it will lead to difficult memory fixation and temporary heap allocation.（Name and criticize vulkan）
- Support new graphics features as much as possible, but CPU parallelism is not the focus. Heavy command filling should obviously not occur, and GPU-driven should be used

# Examples

|Preview|Name|Description|
|-|-|-|
|[<img src="./Examples/HelloTriangle/HelloTriangle.png" width="128"/>](./Examples/HelloTriangle/HelloTriangle.md)|[Hello Triangle](./Examples/HelloTriangle/HelloTriangle.md)|The most basic usage, basic swap chain creation, pipeline creation.<br/>No any resources, including vertex buffers.|
|[<img src="./Examples/HelloTriangle/HelloTriangle.png" width="128"/>](./Examples/HelloTriangleVertex/HelloTriangleVertex.md)|[Hello Triangle Vertex](./Examples/HelloTriangleVertex/HelloTriangleVertex.md)|Demonstrates the use of vertex buffers.|
|[<img src="./Examples/Colorful/Colorful.webp" width="128"/>](./Examples/Colorful/Colorful.md)|[Colorful](./Examples/Colorful/Colorful.md)|Demonstrates the use of shader binding.|
|[<img src="./Examples/Image/Image.png" width="128"/>](./Examples/Image/Image.md)|[Image](./Examples/Image/Image.md)|Demonstrates the use of image load, upload and sampler.|
|[<img src="./Examples/Mipmap/Mipmap.png" width="128"/>](./Examples/Mipmap/Mipmap.md)|[Mipmap](./Examples/Mipmap/Mipmap.md)|Demonstrate the use of compute shaders and dynamic groups.|

# Build

- Requires .NET 9
- Requires Ninja
- Requires Clang 19, does not support msvc, gcc
