#include "DescriptorManager.h"

using namespace Coplt;

DescriptorHeap::DescriptorHeap(ComPtr<ID3D12Device2> device, const D3D12_DESCRIPTOR_HEAP_TYPE type, u32 size, bool gpu)
    : m_device(std::move(device)), m_type(type), m_size(size), m_gpu(gpu)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = type;
    desc.NumDescriptors = size;
    if (gpu) desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));
    m_stride = m_device->GetDescriptorHandleIncrementSize(type);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetCpuHandle(const u32 offset) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heap->GetCPUDescriptorHandleForHeapStart(), static_cast<i32>(offset), m_stride);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetGpuHandle(const u32 offset) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heap->GetGPUDescriptorHandleForHeapStart(), static_cast<i32>(offset), m_stride);
}

DescriptorAllocator::DescriptorAllocator(
    ComPtr<ID3D12Device2> device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 init_cap, const u32 max_cap
) : m_device(std::move(device)), m_type(type), m_max_cap(max_cap)
{
    m_transient_cpu_heap = new DescriptorHeap(m_device, type, init_cap, false);
    m_heap = new DescriptorHeap(m_device, type, init_cap, true);
}

u32 DescriptorAllocator::Stride() const
{
    return m_heap->m_stride;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetTransientCpuHandle(const u32 offset) const
{
    return m_transient_cpu_heap->GetCpuHandle(offset);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetCpuHandle(u32 offset) const
{
    return m_heap->GetCpuHandle(offset);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetGpuHandle(const u32 offset) const
{
    return m_heap->GetGpuHandle(offset);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetCpuHandle(const DescriptorAllocation& allocation) const
{
    if (allocation.Type == DescriptorAllocationType::Persist)
    {
        COPLT_THROW("TODO");
    }
    else
    {
        return GetCpuHandle(allocation.IndexOrOffset);
    }
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetGpuHandle(const DescriptorAllocation& allocation) const
{
    if (allocation.Type == DescriptorAllocationType::Persist)
    {
        COPLT_THROW("TODO");
    }
    else
    {
        return GetGpuHandle(allocation.IndexOrOffset);
    }
}

void DescriptorAllocator::Start(const u32 growth_capacity)
{
    m_ver++;
    if (m_transient_cpu_heap->m_size < growth_capacity)
    {
        const auto old_ver = m_transient_cpu_heap->m_heap_ver;
        m_transient_cpu_heap->m_discarded = true;
        m_transient_cpu_heap = new DescriptorHeap(m_device, m_type, std::bit_ceil(growth_capacity), false);
        m_transient_cpu_heap->m_heap_ver = old_ver + 1;
    }
    if (m_heap->m_size < m_persist_total_size + growth_capacity)
    {
        const auto old_ver = m_heap->m_heap_ver;
        m_heap->m_discarded = true;
        m_heap = new DescriptorHeap(m_device, m_type, std::bit_ceil(m_persist_total_size + growth_capacity), true);
        m_heap->m_heap_ver = old_ver + 1;
    }
    // todo 整理持久分配
    m_transient_offset = 0;
    m_transient_start = 0;
}

// void DescriptorAllocator::UploadTransient(const DescriptorAllocation& allocation) const
// {
//     m_device->CopyDescriptorsSimple(
//         allocation.Size,
//         GetCpuHandle(allocation.IndexOrOffset),
//         m_transient_cpu_heap->GetCpuHandle(allocation.IndexOrOffset),
//         m_type
//     );
// }

DescriptorAllocation DescriptorAllocator::Allocate(u32 Size, DescriptorAllocationType Type)
{
    if (Type == DescriptorAllocationType::Persist)
    {
        COPLT_THROW("TODO");
    }
    else
    {
        if (m_transient_offset + Size > m_heap->m_size)
            COPLT_THROW("Out of descriptor heap");
        auto r = DescriptorAllocation{
            .Version = m_ver,
            .IndexOrOffset = m_transient_offset,
            .Size = Size,
            .Type = Type,
        };
        m_transient_offset += Size;
        return r;
    }
}

DescriptorManager::DescriptorManager(D3d12GpuDevice* device) : m_device(device)
{
    m_dx_device = m_device->m_device;

    m_rtv = new DescriptorHeap(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RtvDsvCap, false);
    m_dsv = new DescriptorHeap(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, RtvDsvCap, false);
    m_cbv_srv_uav = new DescriptorAllocator(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CbvSrvUavInitCap, COPLT_U32_MAX);
    m_sampler = new DescriptorAllocator(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, SamplerInitCap, 2048);
}
