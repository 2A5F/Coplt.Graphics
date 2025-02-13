#pragma once

#include "../../Api/FFI/Layout.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FD3d12ShaderLayout, "384259cf-c392-4903-8d2c-ec959c287912", FShaderLayout)
    {
        // 返回 ID3D12RootSignature*
        virtual void* GetRootSignaturePtr() noexcept = 0;
    };

    COPLT_INTERFACE_DEFINE(FD3d12ShaderInputLayout, "3dfc8de2-bca2-48ae-b869-850cb11b3ee9", FShaderInputLayout)
    {
    };
}
