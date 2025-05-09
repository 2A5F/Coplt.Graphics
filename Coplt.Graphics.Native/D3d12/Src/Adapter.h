#pragma once

#include <wrl/client.h>
#include "directx/d3dx12.h"

#include "Device.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Adapter.h"

namespace Coplt
{
    struct D3d12GpuAdapter final : Object<D3d12GpuAdapter, FD3d12GpuAdapter>
    {
        Rc<D3d12Instance> m_instance{};
        ComPtr<IDXGIAdapter1> m_adapter{};
        std::wstring m_name_string{};
        std::wstring m_driver_string{};
        CD3DX12FeatureSupport m_feature_support{};

        explicit D3d12GpuAdapter(Rc<D3d12Instance>&& instance, ComPtr<IDXGIAdapter1>&& adapter);

        FResult CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDeviceCreateResult* out) noexcept override;

        b8 IsSupport(const FDeviceRequires& Requires) noexcept override;
    };
}
