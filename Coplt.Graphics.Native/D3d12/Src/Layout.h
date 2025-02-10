#pragma once

#include <vector>

#include "Device.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Layout.h"

namespace Coplt
{
    struct D3d12ShaderLayout final : Object<D3d12ShaderLayout, FD3d12ShaderLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12RootSignature> m_root_signature{};
        std::vector<FShaderLayoutItemDefine> m_layout_item_defines{};

        explicit D3d12ShaderLayout(Rc<D3d12GpuDevice>&& device, const FShaderLayoutCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        void* GetRootSignaturePtr() noexcept override;

        FShaderLayoutItemDefine* GetItemDefines(u32* out_count) noexcept override;
    };
}
