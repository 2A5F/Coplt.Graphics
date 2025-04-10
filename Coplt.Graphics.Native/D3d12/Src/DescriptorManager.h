#pragma once

#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include "D3D12MemAlloc.h"
#include "../../Api/Include/HashMap.h"
#include "../../Api/Include/Ptr.h"
#include "../../Api/Include/Utils.h"

#include "../Include/Utils.h"

namespace Coplt
{
    struct D3d12GpuDevice;
    struct DescriptorHeap;

    // 分配只能在帧内使用
    struct DescriptorAllocation
    {
        Rc<DescriptorHeap> m_heap{};
        u64 m_offset{};

        explicit DescriptorAllocation(DescriptorHeap& heap, u64 offset);

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const;
    };

    struct DescriptorHeap final : Object<DescriptorHeap, FUnknown>
    {
        struct Allocation
        {
            D3D12MA::VirtualAllocation m_allocation{};
            u64 m_offset{};
        };

        // 堆版本，每次扩容增加版本，每次扩容所有持久分配都会失效
        u64 m_version{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};
        ComPtr<D3D12MA::VirtualBlock> m_virtual_block{};
        HashMap<u64, Allocation> m_allocations{}; // todo 释放绑组时排队到隔离，释放分配
        // 描述符半容量，永远保证一半容量可以完整容纳每帧需求
        u32 m_half_size{};
        // 临时分配的偏移
        u32 m_tmp_offset{};
        // heap 类型的描述符增量
        u32 m_inc{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        // 下帧是否需要扩容，即使半容量足够
        bool m_need_grow{};

        explicit DescriptorHeap(NonNull<D3d12GpuDevice> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_size);

        void ReadyFrame(u32 cap);
        void EnsureCapacity(u32 cap);

        DescriptorAllocation Allocate(u64 ObjectId, u32 Size, bool& IsOld);
        DescriptorAllocation AllocateTmp(u32 Size);
    };

    struct DescriptorManager final
    {
        constexpr static u32 InitResHeapSize = 1024;
        constexpr static u32 InitSmpHeapSize = 64;

        Rc<DescriptorHeap> m_res{};
        Rc<DescriptorHeap> m_smp{};

        DescriptorManager() = default;

        explicit DescriptorManager(NonNull<D3d12GpuDevice> device);

        void ReadyFrame(u32 res_cap, u32 smp_cap) const;
    };
}
