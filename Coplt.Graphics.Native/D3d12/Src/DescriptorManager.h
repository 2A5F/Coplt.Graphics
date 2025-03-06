#pragma once

#include <concurrent_queue.h>
#include <mutex>

#include "Device.h"

namespace Coplt
{
    struct DescriptorHeap final : Object<DescriptorHeap, FUnknown>
    {
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        u32 m_size{};
        bool m_gpu{};
        volatile bool m_discarded{};

        bool IsDiscarded() const { return m_discarded; }

        explicit DescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 size, bool gpu);
    };

    struct CpuDescriptorAllocator
    {
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        u32 m_used{};

        explicit CpuDescriptorAllocator(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_cap);

        // todo
    };

    struct GpuDescriptorAllocator
    {
        constexpr static u32 BlockSize = 8;

        RwLock m_grow_lock{};
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        u32 m_used{};
        u32 m_cap{};

        explicit GpuDescriptorAllocator(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_cap);
    };

    struct DescriptorManager final
    {
        constexpr static u32 RtvDsvInitCap = 8;
        constexpr static u32 SamplerInitCap = 8;
        constexpr static u32 CbvSrvUavInitCap = 1024;

        D3d12GpuDevice* m_device;
        ComPtr<ID3D12Device2> m_dx_device{};

        Rc<DescriptorHeap> m_rtv_HEAP{};
        Rc<DescriptorHeap> m_dsv_HEAP{};
        Box<CpuDescriptorAllocator> m_cbv_srv_uav_CPU{};
        Box<CpuDescriptorAllocator> m_sampler_CPU{};
        Box<GpuDescriptorAllocator> m_cbv_srv_uav_GPU{};
        Box<GpuDescriptorAllocator> m_sampler_GPU{};

        explicit DescriptorManager(D3d12GpuDevice* device);
    };
}
