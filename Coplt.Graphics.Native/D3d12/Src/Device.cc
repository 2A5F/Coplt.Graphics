#include "Device.h"

#include "dxgi1_6.h"
#include "GraphicsPipeline.h"
#include "Layout.h"
#include "Queue.h"
#include "../../Api/Src/Shader.h"

using namespace Coplt;

namespace
{
    void GetAdapter(
        IDXGIFactory4* factory, IDXGIAdapter1** out_adapter,
        D3D_FEATURE_LEVEL feature_level, const FGpuDeviceCreateOptions& options
    )
    {
        ComPtr<IDXGIFactory6> factory6{};
        ComPtr<IDXGIAdapter1> adapter{};
        if (SUCCEEDED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
        {
            const DXGI_GPU_PREFERENCE preference =
                options.Preference == FGpuPreference::HighPerformance
                    ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE
                    : options.Preference == FGpuPreference::MinimumPower
                    ? DXGI_GPU_PREFERENCE_MINIMUM_POWER
                    : DXGI_GPU_PREFERENCE_UNSPECIFIED;
            for (
                UINT index = 0;
                SUCCEEDED(factory6->EnumAdapterByGpuPreference(index, preference, IID_PPV_ARGS(&adapter)));
                ++index)
            {
                DXGI_ADAPTER_DESC1 desc{};
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), feature_level, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        if (adapter.Get() == nullptr)
        {
            for (UINT index = 0; SUCCEEDED(factory->EnumAdapters1(index, &adapter)); ++index)
            {
                DXGI_ADAPTER_DESC1 desc{};
                adapter->GetDesc1(&desc);

                if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
                {
                    continue;
                }

                if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), feature_level, _uuidof(ID3D12Device), nullptr)))
                {
                    break;
                }
            }
        }

        *out_adapter = adapter.Detach();
    }

    void debug_callback(
        D3D12_MESSAGE_CATEGORY Category,
        D3D12_MESSAGE_SEVERITY Severity,
        D3D12_MESSAGE_ID ID,
        LPCSTR pDescription,
        void* pContext
    )
    {
        D3d12GpuDevice* self = static_cast<D3d12GpuDevice*>(pContext);

        FLogLevel level = FLogLevel::Debug;
        if (Severity <= D3D12_MESSAGE_SEVERITY_ERROR)
        {
            level = FLogLevel::Error;
        }
        else if (Severity == D3D12_MESSAGE_SEVERITY_WARNING)
        {
            level = FLogLevel::Warn;
        }
        else if (Severity == D3D12_MESSAGE_SEVERITY_INFO)
        {
            level = FLogLevel::Info;
        }

        self->Logger().Log(level, FLogType::DirectX, pDescription);
    }
}

D3d12GpuDevice::D3d12GpuDevice(
    Rc<D3d12Instance> instance,
    const FGpuDeviceCreateOptions& options
) : m_instance(std::move(instance))
{
    UINT dxgi_flags = 0;

    if (options.Debug)
    {
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug_controller))))
        {
            m_debug_controller->EnableDebugLayer();
            dxgi_flags |= DXGI_CREATE_FACTORY_DEBUG;
        }
        else m_debug_controller = nullptr;
    }

    chr | CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&m_factory));

    const auto feature_level =
        options.D3dFeatureLevel == FD3dFeatureLevel::Unset
            ? D3D_FEATURE_LEVEL_12_2
            : static_cast<D3D_FEATURE_LEVEL>(options.D3dFeatureLevel);

    GetAdapter(m_factory.Get(), &m_adapter, feature_level, options);
    if (!m_adapter)
        throw WRuntimeException(L"No matching devices found");

    chr | D3D12CreateDevice(m_adapter.Get(), feature_level, IID_PPV_ARGS(&m_device));

    if (Debug())
    {
        chr | m_device >> SetNameEx(options.Name);

        if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(&m_info_queue))))
        {
            if (!SUCCEEDED(
                m_info_queue->RegisterMessageCallback(
                    debug_callback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, this, & m_callback_cookie)
            ))
            {
                Logger().Log(FLogLevel::Warn, u"Register message callback failed.");
            }
        }
    }

    chr | m_device->QueryInterface(IID_PPV_ARGS(&m_device0));
}

D3d12GpuDevice::~D3d12GpuDevice()
{
    if (m_info_queue && m_callback_cookie != 0)
    {
        chr | m_info_queue->UnregisterMessageCallback(m_callback_cookie);
    }
}

FResult D3d12GpuDevice::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        chr | m_device >> SetNameEx(name);
    });
}

void* D3d12GpuDevice::GetRawDevice() noexcept
{
    return m_device0.Get();
}

FResult D3d12GpuDevice::CreateMainQueue(const FMainQueueCreateOptions& options, FGpuQueue** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12GpuQueue(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateShaderModule(const FShaderModuleCreateOptions& options, FShaderModule** out) noexcept
{
    return feb([&]
    {
        *out = ShaderModule::Create(options);
    });
}

FResult D3d12GpuDevice::CreateShaderLayout(const FShaderLayoutCreateOptions& options, FShaderLayout** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12ShaderLayout(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateShaderInputLayout(const FShaderInputLayoutCreateOptions& options,
                                                FShaderInputLayout** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12ShaderInputLayout(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateShader(const FShaderCreateOptions& options, FShader** out) noexcept
{
    return feb([&]
    {
        *out = new Shader(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateMeshLayout(const FMeshLayoutCreateOptions& options, FMeshLayout** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12MeshLayout(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateGraphicsPipeline(
    const FGraphicsShaderPipelineCreateOptions& options, FGraphicsShaderPipeline** out
) noexcept
{
    return feb([&]
    {
        *out = new D3d12GraphicsShaderPipeline(this->CloneThis(), options);
    });
}
