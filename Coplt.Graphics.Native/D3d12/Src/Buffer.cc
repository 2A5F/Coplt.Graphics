#include "Buffer.h"

using namespace Coplt;

D3d12GpuBuffer::D3d12GpuBuffer(Rc<D3d12GpuDevice>&& device, const FGpuBufferCreateOptions& options)
    : m_device(std::move(device))
{
    m_allocator = m_device->m_gpu_allocator;

    m_state = FResourceState::Common;
    m_purpose = options.Purpose;
    m_cpu_access = options.CpuAccess;
    m_life_time = options.LifeTime;
    m_size = options.Size;
    m_count = options.Count;
    m_stride = options.Stride;
    m_usage = options.Usage;

    if (HasAnyFlags(m_purpose, FResourcePurpose::DepthStencil | FResourcePurpose::ShadingRate))
        throw WRuntimeException(L"Buffer does not support this purpose combination.");

    m_desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    m_desc.Alignment = 0;
    m_desc.Width = options.Size;
    m_desc.Height = 1;
    m_desc.DepthOrArraySize = 1;
    m_desc.MipLevels = 1;
    m_desc.Format = DXGI_FORMAT_UNKNOWN;
    m_desc.SampleDesc.Count = 1;
    m_desc.SampleDesc.Quality = 0;
    m_desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    m_desc.Flags = D3D12_RESOURCE_FLAG_NONE;
    if (HasAnyFlags(m_purpose, FResourcePurpose::UnorderedAccess))
        m_desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    // 瞬态资源不会立即创建
    if (m_life_time != FLifeTime::Transient)
    {
        m_resource = box<ResourcePack>(m_allocator.Get(), m_cpu_access, m_state, &m_desc, nullptr);

        if (m_device->Debug())
        {
            chr | m_resource->m_resource >> SetNameEx(options.Name);
        }
    }
}

FResult D3d12GpuBuffer::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        if (m_life_time == FLifeTime::Transient) return;
        chr | m_resource->m_resource >> SetNameEx(name);
    });
}

FResult D3d12GpuBuffer::GetCurrentResourcePtr(void* out) const noexcept
{
    // todo 瞬态
    *static_cast<ID3D12Resource**>(out) = m_resource->m_resource.Get();
    return FResult::None();
}
