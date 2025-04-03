#include "Device.h"

#include "Binding.h"

#include "../../Api/Src/Shader.h"
#include "Adapter.h"
#include "DescriptorManager.h"
#include "Buffer.h"
#include "GraphicsPipeline.h"
#include "Image.h"
#include "Isolate.h"
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

D3d12GpuDevice::D3d12GpuDevice(Rc<D3d12GpuAdapter>&& adapter, const FGpuDeviceCreateOptions& options, FGpuDeviceCreateResult& out):
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

    if (m_adapter->m_feature_support.CacheCoherentUMA())
    {
        D3D12MA::POOL_DESC pool_desc{};
        pool_desc.Flags = D3D12MA::POOL_FLAG_MSAA_TEXTURES_ALWAYS_COMMITTED;
        pool_desc.HeapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
        pool_desc.HeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        pool_desc.HeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        chr | m_gpu_allocator->CreatePool(&pool_desc, &m_uma_pool);
    }

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

    FGpuIsolateCreateOptions isolate_options{};
    isolate_options.Type = FGpuIsolateType::Main;
    out.MainIsolate = CreateIsolate(isolate_options);
    Rc main_isolate = out.MainIsolate.Isolate;
    isolate_options.Type = FGpuIsolateType::Copy;
    out.CopyIsolate = CreateIsolate(isolate_options);
    main_isolate.leak();
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

const Rc<D3d12ShaderLayout>& D3d12GpuDevice::GetEmptyShaderLayout(FShaderLayoutFlags flags)
{
    if (!m_empty_layouts)
        m_empty_layouts = std::make_unique<EmptyShaderLayouts>();
    return m_empty_layouts->GetOrAdd(flags, [&](auto& p)
    {
        FShaderLayoutCreateOptions options{};
        options.Flags = flags;
        p = Rc(new D3d12ShaderLayout(options));
    });
}

const Rc<D3d12BindGroupLayout>& D3d12GpuDevice::GetEmptyBindGroupLayout()
{
    if (m_empty_bind_group_layout == nullptr)
    {
        constexpr FBindGroupLayoutCreateOptions options{};
        m_empty_bind_group_layout = Rc(new D3d12BindGroupLayout(options));
    }
    return m_empty_bind_group_layout;
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

const Rc<D3d12BindingLayout>& D3d12GpuDevice::GetEmptyBindingLayout(FShaderLayoutFlags flags)
{
    if (!m_empty_binding_layout)
        m_empty_binding_layout = std::make_unique<EmptyBindingLayouts>();
    return m_empty_binding_layout->GetOrAdd(flags, [&](auto& p)
    {
        const Rc shader_layout = GetEmptyShaderLayout(flags);
        std::wstring name{};
        FBindingLayoutCreateOptions options{};
        if (Debug())
        {
            name = fmt::format(L"Empty Binding Layout {}", static_cast<u32>(flags));
            options.Name = FStr8or16(name);
        }
        options.ShaderLayout = shader_layout.get();
        p = Rc(new D3d12BindingLayout(this->CloneThis(), options));
    });
}

FResult D3d12GpuDevice::CreateIsolate(const FGpuIsolateCreateOptions& options, FGpuIsolateCreateResult& out) noexcept
{
    return feb([&]
    {
        out = CreateIsolate(options);
    });
}

FGpuIsolateCreateResult D3d12GpuDevice::CreateIsolate(const FGpuIsolateCreateOptions& options)
{
    FGpuIsolateCreateResult out;
    const auto ptr = new D3d12GpuIsolate(this->CloneThis(), options);
    out.Isolate = ptr;
    out.Data = ptr;
    return out;
}

FResult D3d12GpuDevice::CreateShaderLayout(const FShaderLayoutCreateOptions& options, FShaderLayoutCreateResult* out) noexcept
{
    return feb([&]
    {
        const auto ptr = new D3d12ShaderLayout(options);
        out->Layout = ptr;
        out->Data = ptr;
    });
}

FResult D3d12GpuDevice::CreateBindGroupLayout(const FBindGroupLayoutCreateOptions& options, FBindGroupLayoutCreateResult* out) noexcept
{
    return feb([&]
    {
        const auto ptr = new D3d12BindGroupLayout(options);
        out->Layout = ptr;
        out->Data = ptr;
    });
}

FResult D3d12GpuDevice::CreateBindingLayout(const FBindingLayoutCreateOptions& options, FBindingLayout** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12BindingLayout(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::GetEmptyShaderLayout(const FGetEmptyShaderLayoutOptions& options, FShaderLayoutCreateResult* out) noexcept
{
    return feb([&]
    {
        Rc r = GetEmptyShaderLayout(options.Flags);
        const auto ptr = r.leak();
        out->Layout = ptr;
        out->Data = ptr;
    });
}

FResult D3d12GpuDevice::GetEmptyBindGroupLayout(FBindGroupLayoutCreateResult* out) noexcept
{
    return feb([&]
    {
        Rc r = GetEmptyBindGroupLayout();
        const auto ptr = r.leak();
        out->Layout = ptr;
        out->Data = ptr;
    });
}

FResult D3d12GpuDevice::GetEmptyBindingLayout(const FGetEmptyBindingLayoutOptions& options, FBindingLayout** out) noexcept
{
    return feb([&]
    {
        Rc r = GetEmptyBindingLayout(options.Flags);
        *out = r.leak();
    });
}

FResult D3d12GpuDevice::CreateShaderInputLayout(const FShaderInputLayoutCreateOptions& options, FShaderInputLayout** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12ShaderInputLayout(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateMeshLayout(const FMeshLayoutCreateOptions& options, FMeshLayout** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12MeshLayout(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateShaderModule(const FShaderModuleCreateOptions& options, FShaderModuleCreateResult* out) noexcept
{
    return feb([&]
    {
        const auto ptr = ShaderModule::Create(options);
        out->ShaderModule = ptr;
        out->Data = ptr;
    });
}

FResult D3d12GpuDevice::CreateShader(const FShaderCreateOptions& options, FShaderCreateResult* out) noexcept
{
    return feb([&]
    {
        const auto ptr = new Shader(this->CloneThis(), options);
        out->Shader = ptr;
        out->Data = ptr;
    });
}

FResult D3d12GpuDevice::CreateBindGroup(const FShaderBindGroupCreateOptions& options, FShaderBindGroup** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12ShaderBindGroup(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateBinding(const FShaderBindingCreateOptions& options, FShaderBinding** out) noexcept
{
    return feb([&]
    {
        *out = new D3d12ShaderBinding(this->CloneThis(), options);
    });
}

FResult D3d12GpuDevice::CreateGraphicsPipeline(const FGraphicsShaderPipelineCreateOptions& options, FGraphicsShaderPipeline** out) noexcept
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
