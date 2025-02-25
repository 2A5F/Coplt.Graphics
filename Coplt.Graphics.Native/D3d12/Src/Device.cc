#include "Device.h"

#include "Binding.h"
#include "dxgi1_6.h"

#include "DescriptorManager.h"
#include "../../Api/Src/Shader.h"
#include "Buffer.h"
#include "GraphicsPipeline.h"
#include "Layout.h"
#include "Queue.h"

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

            if (options.GpuBasedValidation)
            {
                ComPtr<ID3D12Debug1> debug1{};
                if (SUCCEEDED(m_debug_controller->QueryInterface<ID3D12Debug1>(&debug1)))
                {
                    debug1->SetEnableGPUBasedValidation(true);
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
        else m_debug_controller = nullptr;
    }

    chr | CreateDXGIFactory2(dxgi_flags, IID_PPV_ARGS(&m_factory));

    const auto feature_level =
        options.D3dFeatureLevel == FD3dFeatureLevel::Unset
            ? D3D_FEATURE_LEVEL_12_2
            : static_cast<D3D_FEATURE_LEVEL>(options.D3dFeatureLevel);

    GetAdapter(m_factory.Get(), &m_adapter, feature_level, options);
    if (!m_adapter)
        COPLT_THROW("No matching devices found");

    chr | D3D12CreateDevice(m_adapter.Get(), feature_level, IID_PPV_ARGS(&m_device));

    D3D12MA::ALLOCATOR_DESC allocator_desc{};
    D3D12MA::ALLOCATION_CALLBACKS allocation_callbacks{};
    allocation_callbacks.pAllocate = [](auto size, auto align, auto data) { return mi_malloc_aligned(size, align); };
    allocation_callbacks.pFree = [](auto ptr, auto data) { return mi_free(ptr); };
    allocator_desc.pDevice = m_device.Get();
    allocator_desc.pAdapter = m_adapter.Get();
    allocator_desc.pAllocationCallbacks = &allocation_callbacks;
    chr | CreateAllocator(&allocator_desc, &m_gpu_allocator);

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

    m_descriptor_manager = box<DescriptorManager>(this);
}

D3d12GpuDevice::~D3d12GpuDevice()
{
    if (m_info_queue && m_callback_cookie != 0)
    {
        chr | m_info_queue->UnregisterMessageCallback(m_callback_cookie);
    }
}

FResult D3d12GpuDevice::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!Debug()) return;
        chr | m_device >> SetNameEx(name);
    });
}

void* D3d12GpuDevice::GetRawDevice() noexcept
{
    return m_device0.Get();
}


const Rc<D3d12ShaderLayout>& D3d12GpuDevice::GetEmptyLayout(FShaderLayoutFlags flags)
{
    if (!m_empty_layouts) m_empty_layouts = std::make_unique<EmptyLayouts>();
    return m_empty_layouts->GetOrAdd(flags, [&](auto& p)
    {
        const auto name = fmt::format(L"Empty Layout {}", static_cast<u32>(flags));
        FShaderLayoutCreateOptions options{};
        options.Name = FStr8or16(name);
        options.Flags = flags;
        p = Rc(new D3d12ShaderLayout(this->CloneThis(), options));
    });
}

const Rc<D3d12MeshLayout>& D3d12GpuDevice::GetEmptyMeshLayout()
{
    if (m_empty_mesh_layout == nullptr)
    {
        FMeshLayoutCreateOptions options{};
        options.Name = FStr8or16(L"Empty Mesh Layout");
        m_empty_mesh_layout = Rc(new D3d12MeshLayout(this->CloneThis(), options));
    }
    return m_empty_mesh_layout;
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

FResult D3d12GpuDevice::GetEmptyShaderLayout(const FGetEmptyShaderLayoutOptions& options, FShaderLayout** out) noexcept
{
    return feb([&]
    {
        Rc r = GetEmptyLayout(options.Flags);
        *out = r.leak();
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

FResult D3d12GpuDevice::CreateShaderBinding(const FShaderBindingCreateOptions& options, FShaderBinding** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12ShaderBinding(this->CloneThis(), options);
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

FResult D3d12GpuDevice::CreateBuffer(const FGpuBufferCreateOptions& options, FGpuBuffer** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12GpuBuffer(this->CloneThis(), options);
    });
}
