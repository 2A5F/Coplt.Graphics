#pragma once

#include "Device.h"
#include "Layout.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Binding.h"
#include "../FFI/Layout.h"
#include "../Include/View.h"

namespace Coplt
{
    struct D3d12ShaderBinding final : Object<D3d12ShaderBinding, FD3d12ShaderBinding>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        Rc<ID3d12ShaderLayout> m_layout{};
        std::vector<FView> m_f_views{};
        std::vector<View> m_views{};
        std::vector<u32> m_changed{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        FView* GetViews(u32* out_size) noexcept override;

        FResult Set(u32 count, const FShaderBindingBatchSet* bindings) noexcept override;

        void Set(std::span<const FShaderBindingBatchSet> bindings);
    };
}
