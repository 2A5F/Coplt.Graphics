#pragma once

#include "../../Api/FFI/Layout.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FD3d12ShaderLayout, "384259cf-c392-4903-8d2c-ec959c287912", FShaderLayout)
    {
        // 返回 ID3D12RootSignature*
        virtual void* GetRootSignaturePtr() noexcept = 0;
    };
}
