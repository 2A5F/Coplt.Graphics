#include "Device.h"

#include "Binding.h"

#include "../../Api/Src/Shader.h"
#include "Adapter.h"
#include "DescriptorManager.h"
#include "Buffer.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "Layout.h"
#include "Queue.h"
#include "Sampler.h"
#include "../../Api/Include/AllocObjectId.h"

using namespace Coplt;

namespace
{
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

D3d12GpuDevice::D3d12GpuDevice(Rc<D3d12GpuAdapter>&& adapter, const FGpuDeviceCreateOptions& options):
    m_adapter(std::move(adapter))
{
    m_instance = m_adapter->m_instance;

    const auto feature_level = static_cast<D3D_FEATURE_LEVEL>(m_adapter->m_d3d_feature_level);
    chr | D3D12CreateDevice(m_adapter->m_adapter.Get(), feature_level, IID_PPV_ARGS(&m_device));

    D3D12MA::ALLOCATOR_DESC allocator_desc{};
    D3D12MA::ALLOCATION_CALLBACKS allocation_callbacks{};
    allocation_callbacks.pAllocate = [](auto size, auto align, auto data) { return mi_malloc_aligned(size, align); };
    allocation_callbacks.pFree = [](auto ptr, auto data) { return mi_free(ptr); };
    allocator_desc.pDevice = m_device.Get();
    allocator_desc.pAdapter = m_adapter->m_adapter.Get();
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
}

D3d12GpuDevice::~D3d12GpuDevice()
{
    if (m_info_queue && m_callback_cookie != 0)
    {
        chr | m_info_queue->UnregisterMessageCallback(m_callback_cookie);
    }
}

const Logger& D3d12GpuDevice::Logger() const noexcept
{
    return m_instance->Logger();
}

bool D3d12GpuDevice::Debug() const noexcept
{
    return m_instance->Debug();
}

FResult D3d12GpuDevice::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!Debug())
            return;
        chr | m_device >> SetNameEx(name);
    });
}

FGpuAdapter* D3d12GpuDevice::GetAdapter() noexcept
{
    return m_adapter.get();
}

void* D3d12GpuDevice::GetRawDevice() noexcept
{
    return m_device0.Get();
}


const Rc<D3d12ShaderLayout>& D3d12GpuDevice::GetEmptyLayout(FShaderLayoutFlags flags)
{
    if (!m_empty_layouts)
        m_empty_layouts = std::make_unique<EmptyLayouts>();
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

FResult D3d12GpuDevice::CreateImage(const FGpuImageCreateOptions& options, FGpuImage** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12GpuImage(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateSampler(const FGpuSamplerCreateOptions& options, FGpuSampler** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12GpuSampler(options);
    });
}
