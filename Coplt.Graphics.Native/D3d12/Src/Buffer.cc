#include "Buffer.h"

#include "../../Api/Include/AllocObjectId.h"

using namespace Coplt;

D3d12GpuBuffer::D3d12GpuBuffer(Rc<D3d12GpuDevice>&& device, const FGpuBufferCreateOptions& options)
    : m_device(std::move(device))
{
    m_allocator = m_device->m_gpu_allocator;

    m_state = FResState::BufferCommon();
    m_purpose = options.Purpose;
    m_cpu_access = options.CpuAccess;
    m_size = options.Size;
    m_count = options.Count;
    m_stride = options.Stride;
    m_usage = options.Usage;

    if (options.Usage == FBufferUsage::Raw)
    {
        m_size = (m_size + 255) / 256 * 256;
    }

    if (HasAnyFlags(m_purpose, FResourcePurpose::DepthStencil | FResourcePurpose::ShadingRate))
        COPLT_THROW("Buffer does not support this purpose combination.");

    m_desc = CD3DX12_RESOURCE_DESC1::Buffer(m_size, D3D12_RESOURCE_FLAG_NONE);
    if (HasAnyFlags(m_purpose, FResourcePurpose::RayTracing))
        m_desc.Flags |= D3D12_RESOURCE_FLAG_RAYTRACING_ACCELERATION_STRUCTURE;
    else
    {
        if (HasAnyFlags(m_purpose, FResourcePurpose::RenderTarget))
            m_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
        if (HasAnyFlags(m_purpose, FResourcePurpose::UnorderedAccess))
            m_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        if (!HasAnyFlags(m_purpose, FResourcePurpose::ShaderResource))
            m_desc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }

    m_resource = box<ResourcePack>(m_device.get(), m_allocator.Get(), m_cpu_access, m_state, &m_desc, nullptr, false);

    if (m_device->Debug())
    {
        chr | m_resource->m_resource >> SetNameEx(options.Name);
    }
}

FResult D3d12GpuBuffer::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        chr | m_resource->m_resource >> SetNameEx(name);
    });
}

ResourceType D3d12GpuBuffer::GetResourceType() noexcept
{
    return ResourceType::Buffer;
}

FGpuViewableData* D3d12GpuBuffer::GpuViewableData() noexcept
{
    return this;
}

FGpuResourceData* D3d12GpuBuffer::GpuResourceData() noexcept
{
    return this;
}

FGpuBufferData* D3d12GpuBuffer::GpuBufferData() noexcept
{
    return this;
}

NonNull<FGpuBufferData> D3d12GpuBuffer::Data()
{
    return this;
}

NonNull<LayoutState> D3d12GpuBuffer::State()
{
    return &m_layout_state;
}

void* D3d12GpuBuffer::GetRawResourcePtr() noexcept
{
    return GetResourcePtr();
}

NonNull<ID3D12Resource> D3d12GpuBuffer::GetResourcePtr()
{
    return m_resource->m_resource.Get();
}

FResult D3d12GpuBuffer::Map(void** ptr, const b8 Discard) noexcept
{
    return feb([&]
    {
        constexpr D3D12_RANGE range = {0, 0};
        chr | m_resource->m_resource->Map(0, Discard ? &range : nullptr, ptr);
    });
}

FResult D3d12GpuBuffer::Unmap(const b8 Discard) noexcept
{
    return feb([&]
    {
        constexpr D3D12_RANGE range = {0, 0};
        m_resource->m_resource->Unmap(0, Discard ? &range : nullptr);
    });
}
