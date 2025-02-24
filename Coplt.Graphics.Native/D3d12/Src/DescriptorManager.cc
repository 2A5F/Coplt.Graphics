#include "DescriptorManager.h"

using namespace Coplt;

DescriptorManager::DescriptorManager(D3d12GpuDevice* device) : m_device(device)
{
    m_dx_device = m_device->m_device;

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = m_rtv_cap = RtvDsvInitCap;
        chr | m_dx_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtv_heap));
    }
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        desc.NumDescriptors = m_dsv_cap = RtvDsvInitCap;
        chr | m_dx_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_dsv_heap));
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = m_cbv_srv_uav_cap = CbvSrvUavInitCap;
        chr | m_dx_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_cbv_srv_uav_Cpu_heap));
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        chr | m_dx_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_cbv_srv_uav_Gpu_heap));
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        desc.NumDescriptors = m_sampler_cap = SamplerInitCap;
        chr | m_dx_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_sampler_Cpu_heap));
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        chr | m_dx_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_sampler_Gpu_heap));
    }
}
