#pragma once

#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <directx/d3d12.h>

#include "../../Api/Include/Object.h"
#include "../../Api/FFI/Instance.h"
#include "../../Api/Include/InstanceBase.h"
#include "../FFI/Instance.h"
#include "../Include/Utils.h"

namespace Coplt
{
    struct D3d12GpuAdapter;

    struct D3d12Instance final : InstanceBase<Object<D3d12Instance, FD3d12Instance>>
    {
        ComPtr<ID3D12Debug> m_debug_controller{};
        ComPtr<IDXGIFactory4> m_factory{};
        ComPtr<IDXGIFactory6> m_factory6{};
        std::vector<Rc<D3d12GpuAdapter>> m_adapters{};
        std::vector<FGpuAdapter*> m_f_adapters{};

        explicit D3d12Instance(const FInstanceCreateOptions& options);

        void CreateAdapter(ComPtr<IDXGIAdapter1>&& adapter);

        FResult CreateDevice(const FGpuAutoSelectDeviceCreateOptions& options, FGpuDeviceCreateResult* out) noexcept override;

        FGpuAdapter* const* GetAdapters(u32* out_count) noexcept override;

        bool Debug() const noexcept { return m_debug_enabled; }
    };
}
