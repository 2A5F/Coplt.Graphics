#include "Instance.h"

#include "../../Api/Include/Error.h"
#include "Adapter.h"
#include "Device.h"

using namespace Coplt;

D3d12Instance::D3d12Instance(const FInstanceCreateOptions& options)
{
    UINT dxgi_flags = 0;

    if (options.Debug)
    {
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug_controller))))
        {
            m_debug_controller->EnableDebugLayer();
            dxgi_flags |= DXGI_CREATE_FACTORY_DEBUG;
            m_debug_enabled = true;

            if (options.GpuBasedValidation)
            {
                ComPtr<ID3D12Debug1> debug1{};
                if (SUCCEEDED(m_debug_controller->QueryInterface<ID3D12Debug1>(&debug1)))
                {
                    debug1->SetEnableGPUBasedValidation(true);
                    m_gpu_based_validation_enabled = true;
                }
                ComPtr<ID3D12Debug2> debug2{};
                if (SUCCEEDED(m_debug_controller->QueryInterface<ID3D12Debug2>(&debug2)))
                {
                    debug2->SetGPUBasedValidationFlags(D3D12_GPU_BASED_VALIDATION_FLAGS_DISABLE_STATE_TRACKING);
                }
            }

            ComPtr<ID3D12Debug5> debug5{};
            if (SUCCEEDED(m_debug_controller->QueryInterface<ID3D12Debug5>(&debug5)))
            {
                debug5->SetEnableAutoName(true);
            }
        }
        else
            m_debug_controller = nullptr;
    }

    chr | CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&m_factory));

    if (SUCCEEDED(m_factory->QueryInterface(IID_PPV_ARGS(&m_factory6))))
    {
        ComPtr<IDXGIAdapter1> adapter{};
        constexpr auto preference = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;
        for (
            UINT index = 0;
            SUCCEEDED(m_factory6->EnumAdapterByGpuPreference(index, preference, IID_PPV_ARGS(&adapter)));
            ++index
        )
        {
            CreateAdapter(std::move(adapter));
        }
    }
    else
    {
        ComPtr<IDXGIAdapter1> adapter{};
        for (UINT index = 0; SUCCEEDED(m_factory->EnumAdapters1(index, &adapter)); ++index)
        {
            CreateAdapter(std::move(adapter));
        }
    }

    m_f_adapters.reserve(m_adapters.size());
    for (const auto& adapter : m_adapters)
        m_f_adapters.push_back(adapter.get());
}

void D3d12Instance::CreateAdapter(ComPtr<IDXGIAdapter1>&& adapter)
{
    m_adapters.push_back(new D3d12GpuAdapter(this->CloneThis(), std::move(adapter)));
}

FResult D3d12Instance::CreateDevice(const FGpuAutoSelectDeviceCreateOptions& options, FGpuDeviceCreateResult* out) noexcept
{
    return feb([&]
    {
        FResult ret = FResult::None();
        std::vector<FResult> results{};
        for (const auto& adapter : m_adapters)
        {
            if (!adapter->IsSupport(options.Requires))
                continue;
            auto r = feb([&]
            {
                out->Device = new D3d12GpuDevice(adapter.Clone(), options, *out);
            });
            if (!r.IsError())
                goto end;
            results.push_back(r);
        }
        for (auto& result : results)
        {
            Logger().Log(FLogLevel::Error, FLogType::Common, result.msg);
        }
        if (m_adapters.empty())
            Logger().Log(FLogLevel::Error, FLogType::Common, u"No physical gpu device is available.");
        ret = FResult::Error(u"Creation failed. No matching device.");
    end:
        for (auto& result : results)
        {
            result.Drop();
        }
        return ret;
    });
}

FGpuAdapter* const* D3d12Instance::GetAdapters(u32* out_count) noexcept
{
    *out_count = m_f_adapters.size();
    return m_f_adapters.data();
}
