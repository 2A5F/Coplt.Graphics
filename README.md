# Coplt.Graphics

[![Build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

Render Hardware Interface designed for c#
- Use com style object
- Utf16 string support
- Try to avoid string passing
- Minimize native calls and use memory to transfer data in batches to reduce GC transitions
- Avoid pointer nesting as much as possible, as it will lead to difficult memory fixation and temporary heap allocation.（Name and criticize vulkan）
- Support new graphics features as much as possible, but CPU parallelism is not the focus. Heavy command filling should obviously not occur, and GPU scene should be used

# Examples

|Preview|Name|Description|
|-|-|-|
|<img src="./Examples/HelloTriangle/HelloTriangle.png" width="128"/>|[HelloTriangle](./Examples/HelloTriangle/HelloTriangle.md)|The most basic usage, basic swap chain creation, pipeline creation.<br/>No any resources, including vertex buffers.|

# Build

- Requires .NET 9
- Requires Ninja
- Requires Clang 19, does not support msvc, gcc
