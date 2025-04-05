#include "DescriptorManager.h"

#include "Device.h"

using namespace Coplt;

DescriptorAllocation::DescriptorAllocation(DescriptorHeap& heap, const D3D12MA::VirtualAllocation allocation, const u64 offset)
    : m_heap(heap.CloneThis()), m_version(heap.m_version), m_allocation(allocation), m_offset(offset)
{
}

DescriptorAllocation::~DescriptorAllocation()
{
    if (!m_heap) return;
    if (m_heap->m_version != m_version) return;
    m_heap->m_virtual_block->FreeAllocation(m_allocation);
    m_heap = {};
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocation::GetCpuHandle() const
{
    if (m_heap->m_version != m_version)
        COPLT_THROW("This allocation has expired");
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heap->m_heap->GetCPUDescriptorHandleForHeapStart(), m_offset, m_heap->m_inc);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorAllocation::GetGpuHandle() const
{
    if (m_heap->m_version != m_version)
        COPLT_THROW("This allocation has expired");
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heap->m_heap->GetGPUDescriptorHandleForHeapStart(), m_offset, m_heap->m_inc);
}

DescriptorHeap::DescriptorHeap(NonNull<D3d12GpuDevice> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_size)
    : m_device(device->m_device), m_type(type)
{
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = type;
        desc.NumDescriptors = init_size;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));

        m_inc = m_device->GetDescriptorHandleIncrementSize(type);
    }

    {
        D3D12MA::VIRTUAL_BLOCK_DESC desc{};
        D3D12MA::ALLOCATION_CALLBACKS allocation_callbacks{};
        allocation_callbacks.pAllocate = [](auto size, auto align, auto data) { return mi_malloc_aligned(size, align); };
        allocation_callbacks.pFree = [](auto ptr, auto data) { return mi_free(ptr); };
        desc.Size = init_size;
        desc.pAllocationCallbacks = &allocation_callbacks;
        chr | CreateVirtualBlock(&desc, &m_virtual_block);
    }
}

DescriptorAllocation DescriptorHeap::Allocate(const u32 size)
{
    D3D12MA::VIRTUAL_ALLOCATION_DESC desc{};
    desc.Alignment = 1;
    desc.Size = size;
    D3D12MA::VirtualAllocation allocation{};
    u64 offset{};
    chr | m_virtual_block->Allocate(&desc, &allocation, &offset);
    return DescriptorAllocation(*this, allocation, offset);
}

DescriptorManager::DescriptorManager(NonNull<D3d12GpuDevice> device)
{
    m_res = new DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, InitResHeapSize);
    m_smp = new DescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, InitSmpHeapSize);
}
