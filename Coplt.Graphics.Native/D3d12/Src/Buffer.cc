#include "Buffer.h"

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
    if (HasAnyFlags(m_purpose, FResourcePurpose::UnorderedAccess))
        m_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    m_resource = box<ResourcePack>(m_device.get(), m_allocator.Get(), m_cpu_access, m_state, &m_desc, nullptr);

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

FResult D3d12GpuBuffer::GetCurrentResourcePtr(void* out) const noexcept
{
    // todo 瞬态
    *static_cast<ID3D12Resource**>(out) = m_resource->m_resource.Get();
    return FResult::None();
}
