#include "DescriptorManager.h"

#include "Device.h"

using namespace Coplt;

DescriptorAllocation::DescriptorAllocation(DescriptorHeap* heap, const u64 offset)
    : m_heap(heap), m_offset(offset)
{
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocation::GetCpuHandle() const
{
    if (!m_heap) return {};
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heap->m_heap->GetCPUDescriptorHandleForHeapStart(), m_offset, m_heap->m_inc);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorAllocation::GetGpuHandle() const
{
    if (!m_heap) return {};
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heap->m_heap->GetGPUDescriptorHandleForHeapStart(), m_offset, m_heap->m_inc);
}

DescriptorAllocation::operator bool() const
{
    return m_heap != nullptr;
}

DescriptorHeap::DescriptorHeap(NonNull<D3d12GpuDevice> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_size, u32 max_size)
    : m_device(device->m_device), m_half_size(init_size), m_type(type), m_max_size(max_size / 2)
{
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = type;
        desc.NumDescriptors = m_half_size * 2;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));

        m_inc = m_device->GetDescriptorHandleIncrementSize(type);
    }

    {
        D3D12MA::VIRTUAL_BLOCK_DESC desc{};
        D3D12MA::ALLOCATION_CALLBACKS allocation_callbacks{};
        allocation_callbacks.pAllocate = [](auto size, auto align, auto data) { return mi_malloc_aligned(size, align); };
        allocation_callbacks.pFree = [](auto ptr, auto data) { return mi_free(ptr); };
        desc.Size = m_half_size;
        desc.pAllocationCallbacks = &allocation_callbacks;
        chr | CreateVirtualBlock(&desc, &m_virtual_block);
    }
}

void DescriptorHeap::ReadyFrame(const u32 cap)
{
    m_tmp_offset = 0;
    EnsureCapacity(cap);
}

void DescriptorHeap::EnsureCapacity(u32 cap)
{
    const auto new_cap = std::min(std::bit_ceil(cap), m_max_size);
    if (cap > new_cap)
        COPLT_THROW_FMT("Out of memory; The required descriptor heap capacity is {}, but the maximum allowed is {}", cap * 2, m_max_size * 2);
    cap = new_cap;
    if (m_need_grow) cap = std::max(cap, std::min(m_half_size * 2, m_max_size));
    else if (cap <= m_half_size) return;

    m_half_size = cap;
    m_version++;
    m_virtual_block->Clear();
    m_allocations.Clear();

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = m_type;
        desc.NumDescriptors = m_half_size * 2;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));
    }

    {
        D3D12MA::VIRTUAL_BLOCK_DESC desc{};
        D3D12MA::ALLOCATION_CALLBACKS allocation_callbacks{};
        allocation_callbacks.pAllocate = [](auto size, auto align, auto data) { return mi_malloc_aligned(size, align); };
        allocation_callbacks.pFree = [](auto ptr, auto data) { return mi_free(ptr); };
        desc.Size = m_half_size;
        desc.pAllocationCallbacks = &allocation_callbacks;
        chr | CreateVirtualBlock(&desc, &m_virtual_block);
    }
}

DescriptorAllocation DescriptorHeap::Allocate(const u64 ObjectId, const u64 Version, const u32 Size, bool& NeedUpload)
{
    bool exists = false;
    auto& allocation = m_allocations.GetOrAdd(ObjectId, exists, [this, Version, Size](auto& p)
    {
        D3D12MA::VIRTUAL_ALLOCATION_DESC desc{};
        desc.Alignment = 1;
        desc.Size = Size;
        D3D12MA::VirtualAllocation allocation{};
        u64 offset{};
        const auto r = m_virtual_block->Allocate(&desc, &allocation, &offset);
        if (r == E_OUTOFMEMORY)
        {
            const auto a = AllocateTmp(Size);
            m_need_grow = true;
            p.put(Allocation{{}, a.m_offset, Version});
        }
        else
        {
            chr | r;
            p.put(Allocation{allocation, offset, Version});
        }
    });
    if (!exists) NeedUpload = true;
    if (allocation.m_version < Version)
    {
        allocation.m_version = Version;
        NeedUpload = true;
    }
    return DescriptorAllocation(this, allocation.m_offset);
}

DescriptorAllocation DescriptorHeap::AllocateTmp(const u32 Size)
{
    const auto offset = m_tmp_offset;
    m_tmp_offset += Size;
    if (m_tmp_offset > m_half_size)
        COPLT_THROW("Out of descriptor heap");
    const auto inv_offset = m_half_size * 2 - offset - Size;
    return DescriptorAllocation(this, inv_offset);
}

DescriptorManager::DescriptorManager(NonNull<D3d12GpuDevice> device)
{
    m_res = new DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, InitResHeapSize, MaxResHeapSize);
    m_smp = new DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, InitSmpHeapSize, MaxSmpHeapSize);
}

void DescriptorManager::ReadyFrame(const u32 res_cap, const u32 smp_cap) const
{
    m_res->ReadyFrame(res_cap);
    m_smp->ReadyFrame(smp_cap);
}
