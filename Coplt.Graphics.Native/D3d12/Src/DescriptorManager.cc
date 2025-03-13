#include "DescriptorManager.h"

#include "../../Api/Include/AllocObjectId.h"

using namespace Coplt;

DescriptorHeap::DescriptorHeap(const ComPtr<ID3D12Device2>& device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 size, const bool gpu)
    : m_device(device), m_id(AllocObjectId()), m_type(type),
      m_stride(m_device->GetDescriptorHandleIncrementSize(type)), m_size(size), m_gpu(gpu)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = type;
    desc.NumDescriptors = size;
    if (gpu) desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));
}

const ComPtr<ID3D12DescriptorHeap>& DescriptorHeap::Heap() const
{
    return m_heap;
}

u64 DescriptorHeap::Id() const
{
    return m_id;
}

u64 DescriptorHeap::Version() const
{
    return m_version;
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::Type() const
{
    return m_type;
}

u32 DescriptorHeap::Stride() const
{
    return m_stride;
}

bool DescriptorHeap::IsRemote() const
{
    return m_gpu;
}

u32 DescriptorHeap::Size() const
{
    return m_size;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHeap::GetLocalHandle(const u32 offset) const
{
    return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_heap->GetCPUDescriptorHandleForHeapStart(), static_cast<i32>(offset), m_stride);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetRemoteHandle(const u32 offset) const
{
    return CD3DX12_GPU_DESCRIPTOR_HANDLE(m_heap->GetGPUDescriptorHandleForHeapStart(), static_cast<i32>(offset), m_stride);
}

void DescriptorHeap::IncVersion()
{
    m_version++;
}

DescriptorAllocator::DescriptorAllocator(const ComPtr<ID3D12Device2>& device, const D3D12_DESCRIPTOR_HEAP_TYPE type)
    : m_device(device), m_type(type), m_stride(device->GetDescriptorHandleIncrementSize(type))
{
}

const Rc<DescriptorHeap>& DescriptorAllocator::Heap() const
{
    return m_heap;
}

const Rc<DescriptorHeap>& DescriptorAllocator::TmpHeap() const
{
    return m_tmp_heap;
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorAllocator::Type() const
{
    return m_type;
}

u32 DescriptorAllocator::Stride() const
{
    return m_stride;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetTmpLocalHandle(const u32 offset) const
{
    return m_tmp_heap->GetLocalHandle(offset);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetLocalHandle(const u32 offset) const
{
    return m_heap->GetLocalHandle(offset);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetRemoteHandle(const u32 offset) const
{
    return m_heap->GetRemoteHandle(offset);
}

void DescriptorAllocator::InitFrame(const u32 GrowthCapacity)
{
    if (m_heap == nullptr || m_persist_offset < GrowthCapacity)
    {
        const auto cap_req = std::max(8u, (m_heap == nullptr ? 0 : m_heap->Size() - m_persist_offset) + GrowthCapacity);
        m_heap = new DescriptorHeap(m_device, m_type, std::bit_ceil(cap_req), true);
        m_persist_offset = m_heap->Size();
        m_persist_allocations.Clear();
    }
    if (GrowthCapacity > 0 && (m_tmp_heap == nullptr || m_tmp_heap->Size() < GrowthCapacity))
    {
        m_tmp_heap = new DescriptorHeap(m_device, m_type, std::bit_ceil(GrowthCapacity), false);
    }
    m_dynamic_offset = 0;
    m_version++;
}

void DescriptorAllocator::AllocateTransient(u32 Size, DescriptorAllocation*& al)
{
    if (Size == 0)
    {
        *al = DescriptorAllocation(0, 0);
        return;
    }
    if (!m_heap)
        COPLT_THROW("Descriptor heap is null");
    const auto offset = m_dynamic_offset;
    const auto new_offset = offset + Size;
    if (new_offset >= m_persist_offset)
        COPLT_THROW("Out of descriptor heap");
    m_dynamic_offset = new_offset;
    *al = DescriptorAllocation(offset, Size);
}

void DescriptorAllocator::AllocatePersistent(NonNull<DescriptorHeap> Heap, PersistDescriptorAllocation*& al, bool& IsOld)
{
    if (Heap->Size() == 0)
    {
        IsOld = true;
        *al = PersistDescriptorAllocation(0, 0, 0);
        return;
    }
    if (!m_heap)
        COPLT_THROW("Descriptor heap is null");
    auto& ref = m_persist_allocations.GetOrAdd(Heap->Id(), IsOld, [&](auto& p)
    {
        const auto offset = m_persist_offset;
        const auto new_offset = offset - Heap->Size();
        if (new_offset < m_dynamic_offset)
            COPLT_THROW("Out of descriptor heap");
        m_persist_offset = new_offset;
        p = PersistDescriptorAllocation_Internal(0, new_offset, Heap->Size(), m_version);
    });
    ref.AllocatorVersion = m_version;
    al = &ref;
}

void DescriptorAllocator::Upload(const DescriptorAllocation& Al, const Rc<DescriptorHeap>& Heap, const u32 Offset) const
{
    if (Al.Size == 0) return;
    m_device->CopyDescriptorsSimple(
        Al.Size,
        GetLocalHandle(Al.Offset),
        Heap->GetLocalHandle(Offset),
        m_type
    );
}

// void DescriptorAllocator::StoreTmp(const DescriptorAllocation& Al, const Rc<DescriptorHeap>& Heap, const u32 Offset) const
// {
//     if (Al.Size == 0) return;
//     m_device->CopyDescriptorsSimple(
//         Al.Size,
//         GetTmpLocalHandle(Al.Offset),
//         Heap->GetLocalHandle(Offset),
//         m_type
//     );
// }
//
// void DescriptorAllocator::SyncTmp() const
// {
//     if (m_dynamic_offset == 0) return;
//     m_device->CopyDescriptorsSimple(
//         m_dynamic_offset,
//         GetLocalHandle(),
//         GetTmpLocalHandle(),
//         m_type
//     );
// }

DescriptorContext::DescriptorContext(const ComPtr<ID3D12Device2>& device) : m_device(device)
{
    m_resource = new DescriptorAllocator(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_sampler = new DescriptorAllocator(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
}

NonNull<DescriptorAllocator> DescriptorContext::ResourceHeap() const
{
    if (!m_in_frame) [[unlikely]]
        COPLT_THROW("Frame not started");
    return m_resource.get();
}

NonNull<DescriptorAllocator> DescriptorContext::SamplerHeap() const
{
    if (!m_in_frame) [[unlikely]]
        COPLT_THROW("Frame not started");
    return m_sampler.get();
}

void DescriptorContext::InitFrame(const u32 ResourceCap, const u32 SamplerCap)
{
    m_resource->InitFrame(ResourceCap);
    m_sampler->InitFrame(SamplerCap);
    m_in_frame = true;
}

void DescriptorContext::EndFrame()
{
    m_in_frame = false;
}

// void DescriptorContext::SyncTmp() const
// {
//     m_resource->SyncTmp();
//     m_sampler->SyncTmp();
// }

std::array<ID3D12DescriptorHeap*, 2> DescriptorContext::GetDescriptorHeaps() const
{
    return {m_resource->Heap()->Heap().Get(), m_sampler->Heap()->Heap().Get()};
}
