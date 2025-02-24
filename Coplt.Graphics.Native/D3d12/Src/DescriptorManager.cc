#include "DescriptorManager.h"

using namespace Coplt;

CpuDescriptorSet::CpuDescriptorSet(
    ComPtr<ID3D12Device2> device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 init_cap
) : m_device(std::move(device)), m_type(type), m_cap(init_cap)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = type;
    desc.NumDescriptors = init_cap;
    chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_heap));
}

GpuDescriptorSet::GpuDescriptorSet(
    ComPtr<ID3D12Device2> device, const D3D12_DESCRIPTOR_HEAP_TYPE type, const u32 init_cap
) : m_device(std::move(device)), m_type(type), m_cap(init_cap)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc{};
    desc.Type = type;
    desc.NumDescriptors = init_cap;
    chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Cpu_heap));
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    chr | m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_Gpu_heap));
}

DescriptorManager::DescriptorManager(D3d12GpuDevice* device) : m_device(device)
{
    m_dx_device = m_device->m_device;

    m_rtv_set = box<CpuDescriptorSet>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, RtvDsvInitCap);
    m_dsv_set = box<CpuDescriptorSet>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, RtvDsvInitCap);
    m_cbv_srv_uav_set = box<GpuDescriptorSet>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, CbvSrvUavInitCap);
    m_sampler_set = box<GpuDescriptorSet>(m_dx_device, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, SamplerInitCap);
}
