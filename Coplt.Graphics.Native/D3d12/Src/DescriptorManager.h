#pragma once

#include <directx/d3d12.h>
#include <directx/d3dx12.h>
#include "D3D12MemAlloc.h"
#include "../../Api/Include/Ptr.h"
#include "../../Api/Include/Utils.h"

#include "../Include/Utils.h"

namespace Coplt
{
    struct D3d12GpuDevice;
    struct DescriptorHeap;

    struct DescriptorAllocation
    {
        Rc<DescriptorHeap> m_heap{};
        u64 m_version{};
        D3D12MA::VirtualAllocation m_allocation{};
        u64 m_offset{};

        explicit DescriptorAllocation(DescriptorHeap& heap, D3D12MA::VirtualAllocation allocation, u64 offset);
        ~DescriptorAllocation();

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const;
    };

    struct DescriptorHeap final : Object<DescriptorHeap, FUnknown>
    {
        u64 m_version{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};
        ComPtr<D3D12MA::VirtualBlock> m_virtual_block{};
        u32 m_inc{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};

        explicit DescriptorHeap(NonNull<D3d12GpuDevice> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 init_size);

        DescriptorAllocation Allocate(u32 size);
    };

    struct DescriptorManager final
    {
        constexpr static u32 InitResHeapSize = 1024;
        constexpr static u32 InitSmpHeapSize = 64;

        Rc<DescriptorHeap> m_res{};
        Rc<DescriptorHeap> m_smp{};

        DescriptorManager() = default;

        explicit DescriptorManager(NonNull<D3d12GpuDevice> device);

        void EnsureCapacity();
    };
}
