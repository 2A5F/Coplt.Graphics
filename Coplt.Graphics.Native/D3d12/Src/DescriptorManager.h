#pragma once

#include <concurrent_queue.h>

#include "Device.h"

namespace Coplt
{
    struct CpuDescriptorSet
    {
        ComPtr<ID3D12Device2> m_device{};

        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};

        u32 m_used{};
        u32 m_cap{};

        explicit CpuDescriptorSet(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_cap);
    };

    struct GpuDescriptorSet
    {
        ComPtr<ID3D12Device2> m_device{};

        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        ComPtr<ID3D12DescriptorHeap> m_Cpu_heap{};
        ComPtr<ID3D12DescriptorHeap> m_Gpu_heap{};

        u32 m_used{};
        u32 m_cap{};

        explicit GpuDescriptorSet(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_cap);
    };

    struct DescriptorManager final
    {
        constexpr static u32 RtvDsvInitCap = 128;
        constexpr static u32 SamplerInitCap = 128;
        constexpr static u32 CbvSrvUavInitCap = 1024;

        D3d12GpuDevice* m_device;
        ComPtr<ID3D12Device2> m_dx_device{};

        Box<CpuDescriptorSet> m_rtv_set{};
        Box<CpuDescriptorSet> m_dsv_set{};
        Box<GpuDescriptorSet> m_cbv_srv_uav_set{};
        Box<GpuDescriptorSet> m_sampler_set{};

        explicit DescriptorManager(D3d12GpuDevice* device);
    };
}
