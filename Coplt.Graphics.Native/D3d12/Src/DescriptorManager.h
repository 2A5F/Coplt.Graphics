#pragma once

#include "directx/d3dx12.h"

#include <array>
#include <concurrent_queue.h>
#include <mutex>
#include <bit>

#include "Device.h"

namespace Coplt
{
    struct DescriptorHeap final : Object<DescriptorHeap, FUnknown>
    {
    private:
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE m_type{};
        u32 m_stride{};
        u32 m_size{};
        bool m_gpu{};

    public:
        explicit DescriptorHeap(ComPtr<ID3D12Device2> device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 size, bool gpu);

        const ComPtr<ID3D12DescriptorHeap>& Heap() const;

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const;
        u32 Stride() const;
        bool IsRemote() const;

        u32 Size() const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetLocalHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetRemoteHandle(u32 offset = 0) const;
    };

    struct DescriptorPersistAllocation
    {
        // todo
    };

    struct DescriptorTransientAllocation
    {
        u32 Offset{};
        u32 Size{};
    };

    struct DescriptorAllocator final : Object<DescriptorAllocator, FUnknown>
    {
    private:
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorHeap> m_heap{};
        D3D12_DESCRIPTOR_HEAP_TYPE const m_type{};
        u32 const m_stride{};
        u32 m_transient_offset{};

    public:
        explicit DescriptorAllocator(const ComPtr<ID3D12Device2>& device, D3D12_DESCRIPTOR_HEAP_TYPE type);

        const Rc<DescriptorHeap>& Heap() const;

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const;
        u32 Stride() const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetLocalHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetRemoteHandle(u32 offset = 0) const;

        void InitFrame(u32 TransientCapacity);

        DescriptorTransientAllocation AllocateTransient(u32 Size);
    };

    struct DescriptorContext final : Object<DescriptorContext, FUnknown>
    {
    private:
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorAllocator> m_resource{};
        Rc<DescriptorAllocator> m_sampler{};
        bool m_in_frame{};

    public:
        explicit DescriptorContext(const ComPtr<ID3D12Device2>& device);

        DescriptorAllocator* ResourceHeap() const;
        DescriptorAllocator* SamplerHeap() const;

        void InitFrame(u32 ResourceCap, u32 SamplerCap);
        void EndFrame();

        std::array<ID3D12DescriptorHeap*, 2> GetDescriptorHeaps() const;
    };
}
