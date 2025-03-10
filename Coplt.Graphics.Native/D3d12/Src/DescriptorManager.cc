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

const ComPtr<ID3D12DescriptorHeap>& DescriptorHeap::Heap() const
{
    return m_heap;
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

DescriptorAllocator::DescriptorAllocator(const ComPtr<ID3D12Device2>& device, const D3D12_DESCRIPTOR_HEAP_TYPE type)
    : m_device(device), m_type(type), m_stride(device->GetDescriptorHandleIncrementSize(type))
{
}

const Rc<DescriptorHeap>& DescriptorAllocator::Heap() const
{
    return m_heap;
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorAllocator::Type() const
{
    return m_type;
}

u32 DescriptorAllocator::Stride() const
{
    return m_stride;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetLocalHandle(const u32 offset) const
{
    return m_heap->GetLocalHandle(offset);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorAllocator::GetRemoteHandle(const u32 offset) const
{
    return m_heap->GetRemoteHandle(offset);
}

void DescriptorAllocator::InitFrame(const u32 TransientCapacity)
{
    if (m_heap == nullptr || m_heap->Size() < TransientCapacity)
    {
        m_heap = new DescriptorHeap(m_device, m_type, std::bit_ceil(std::max(8u, TransientCapacity)), true);
    }
    m_transient_offset = 0;
}

DescriptorTransientAllocation DescriptorAllocator::AllocateTransient(const u32 Size)
{
    if (!m_heap)
        COPLT_THROW("Descriptor heap is null");
    const auto offset = m_transient_offset;
    const auto new_offset = offset + Size;
    if (new_offset > m_heap->Size())
        COPLT_THROW("Out of descriptor heap");
    m_transient_offset = new_offset;
    return {.Offset = offset, .Size = Size};
}

DescriptorContext::DescriptorContext(const ComPtr<ID3D12Device2>& device) : m_device(device)
{
    m_resource = new DescriptorAllocator(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_sampler = new DescriptorAllocator(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
}

DescriptorAllocator* DescriptorContext::ResourceHeap() const
{
    if (!m_in_frame)
        COPLT_THROW("Frame not started");
    return m_resource.get();
}

DescriptorAllocator* DescriptorContext::SamplerHeap() const
{
    if (!m_in_frame)
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

std::array<ID3D12DescriptorHeap*, 2> DescriptorContext::GetDescriptorHeaps() const
{
    return {m_resource->Heap()->Heap().Get(), m_sampler->Heap()->Heap().Get()};
}
