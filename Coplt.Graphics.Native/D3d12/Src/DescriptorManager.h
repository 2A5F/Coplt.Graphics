#pragma once
#include "Device.h"

namespace Coplt
{
    struct DescriptorManager final
    {
        constexpr static u32 RtvDsvInitCap = 128;
        constexpr static u32 SamplerInitCap = 128;
        constexpr static u32 CbvSrvUavInitCap = 1024;

        D3d12GpuDevice* m_device;
        ComPtr<ID3D12Device2> m_dx_device{};

        ComPtr<ID3D12DescriptorHeap> m_rtv_heap{};
        ComPtr<ID3D12DescriptorHeap> m_dsv_heap{};

        ComPtr<ID3D12DescriptorHeap> m_cbv_srv_uav_Cpu_heap{};
        ComPtr<ID3D12DescriptorHeap> m_sampler_Cpu_heap{};

        ComPtr<ID3D12DescriptorHeap> m_cbv_srv_uav_Gpu_heap{};
        ComPtr<ID3D12DescriptorHeap> m_sampler_Gpu_heap{};

        u32 m_rtv_used{};
        u32 m_rtv_cap{};

        u32 m_dsv_used{};
        u32 m_dsv_cap{};

        u32 m_cbv_srv_uav_used{};
        u32 m_cbv_srv_uav_cap{};

        u32 m_sampler_used{};
        u32 m_sampler_cap{};

        explicit DescriptorManager(D3d12GpuDevice* device);
    };
}
