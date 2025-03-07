#pragma once

#include "directx/d3dx12.h"

#include <concurrent_queue.h>
#include <mutex>
#include <bit>

#include "Device.h"

namespace Coplt
{
    struct DescriptorHeap final : Object<DescriptorHeap, FUnknown>
    {
        u64 m_heap_ver{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        u32 m_stride{};
        u32 m_size{};
        bool m_gpu{};
        volatile bool m_discarded{};

        bool IsDiscarded() const { return m_discarded; }

        explicit DescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 size, bool gpu);

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(u32 offset = 0) const;
    };

    enum class DescriptorAllocationType : u8
    {
        Persist,
        Transient,
    };

    struct DescriptorPersistAllocation final : Object<DescriptorHeap, FUnknown>
    {
        // 版本，每次使用时改变
        u64 Version{};
        // 上次使时 gpu 堆的版本
        u64 GpuHeapVersion{};
        // 所使用的 cpu 堆
        Rc<DescriptorHeap> CpuHeap{};
        // 当前在描述符堆内的索引，每次使用都可能变
        u32 Index{};
        // 分配类型
        DescriptorAllocationType Type{};
        // 是否存活，持久分配如果长时间不用将会进入死亡状态
        bool Living{};
    };

    struct DescriptorAllocation final
    {
        // 仅持久时，瞬态时为 null
        Rc<DescriptorPersistAllocation> Persist{};
        u64 Version{};
        u32 IndexOrOffset{COPLT_U32_MAX};
        u32 Size{};
        DescriptorAllocationType Type{};

        operator bool() const & { return IndexOrOffset != COPLT_U32_MAX; }
    };

    struct DescriptorAllocator final : Object<DescriptorAllocator, FUnknown>
    {
        u64 m_ver{};
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorHeap> m_transient_cpu_heap{};
        Rc<DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        // 持久分配
        std::vector<Rc<DescriptorPersistAllocation>> m_persist_allocations{};
        u32 m_max_cap{};
        u32 m_persist_total_size{};
        // 当前偏移
        u32 m_transient_offset{};
        u32 m_transient_start{};

        explicit DescriptorAllocator(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_cap, u32 max_cap);

        u32 Stride() const;
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetTransientCpuHandle(u32 offset = 0) const;
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(u32 offset = 0) const;
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(const DescriptorAllocation& allocation) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle(const DescriptorAllocation& allocation) const;

        void Start(u32 growth_capacity);
        // void UploadTransient(const DescriptorAllocation& allocation) const;

        DescriptorAllocation Allocate(u32 Size, DescriptorAllocationType Type);

        // todo 复活死亡的持久分配
    };

    struct DescriptorManager final
    {
        constexpr static u32 RtvDsvCap = 8;
        constexpr static u32 SamplerInitCap = 8;
        constexpr static u32 CbvSrvUavInitCap = 1024;

        D3d12GpuDevice* m_device;
        ComPtr<ID3D12Device2> m_dx_device{};

        Rc<DescriptorHeap> m_rtv{};
        Rc<DescriptorHeap> m_dsv{};
        Rc<DescriptorAllocator> m_cbv_srv_uav{};
        Rc<DescriptorAllocator> m_sampler{};

        explicit DescriptorManager(D3d12GpuDevice* device);
    };
}
