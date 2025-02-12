#pragma once
#include <directx/d3d12.h>

#include "../../Api/FFI/Shader.h"

namespace Coplt
{
    inline D3D12_SHADER_VISIBILITY ToDx(FShaderStage const value)
    {
        switch (value)
        {
        case FShaderStage::Pixel:
            return D3D12_SHADER_VISIBILITY_PIXEL;
        case FShaderStage::Vertex:
            return D3D12_SHADER_VISIBILITY_VERTEX;
        case FShaderStage::Mesh:
            return D3D12_SHADER_VISIBILITY_MESH;
        case FShaderStage::Task:
            return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
        default:
            return D3D12_SHADER_VISIBILITY_ALL;
        }
    };
}
