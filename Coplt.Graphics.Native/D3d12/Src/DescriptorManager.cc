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
}

CpuDescriptorAllocator::CpuDescriptorAllocator(
    ComPtr<ID3D12Device2> device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 init_cap
) : m_device(std::move(device)), m_type(type)
{
    m_heap = new DescriptorHeap(m_device, type, init_cap, false);
}

GpuDescriptorAllocator::GpuDescriptorAllocator(
    ComPtr<ID3D12Device2> device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 init_cap
) : m_device(std::move(device)), m_type(type), m_cap((init_cap + BlockSize - 1) / BlockSize)
{
    m_heap = new DescriptorHeap(m_device, type, m_cap * BlockSize, true);
}

DescriptorManager::DescriptorManager(D3d12GpuDevice* device) : m_device(device)
{
    m_dx_device = m_device->m_device;

    m_rtv_HEAP = new DescriptorHeap(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RtvDsvInitCap, false);
    m_dsv_HEAP = new DescriptorHeap(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, RtvDsvInitCap, false);
    m_cbv_srv_uav_CPU = box<CpuDescriptorAllocator>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CbvSrvUavInitCap);
    m_sampler_CPU = box<CpuDescriptorAllocator>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, SamplerInitCap);
    m_cbv_srv_uav_GPU = box<GpuDescriptorAllocator>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CbvSrvUavInitCap);
    m_sampler_GPU = box<GpuDescriptorAllocator>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, SamplerInitCap);
}
