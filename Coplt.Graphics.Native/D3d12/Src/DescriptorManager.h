#pragma once

#include "directx/d3dx12.h"

#include <array>
#include <concurrent_queue.h>
#include <mutex>
#include <bit>

#include "Device.h"
#include "../../Api/Include/Ptr.h"

namespace Coplt
{
    struct DescriptorHeap final : Object<DescriptorHeap, FUnknown>
    {
    private:
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12DescriptorHeap> m_heap{};
        u64 const m_id{};
        D3D12_DESCRIPTOR_HEAP_TYPE const m_type{};
        u32 const m_stride{};
        u32 const m_size{};
        bool const m_gpu{};

    public:
        explicit DescriptorHeap(const ComPtr<ID3D12Device2>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 size, bool gpu);

        const ComPtr<ID3D12DescriptorHeap>& Heap() const;

        u64 Id() const;

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const;
        u32 Stride() const;
        bool IsRemote() const;

        u32 Size() const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetLocalHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetRemoteHandle(u32 offset = 0) const;
    };

    struct DescriptorAllocation
    {
        u64 Version{};
        u32 Offset{COPLT_U32_MAX};
        u32 Size{};

        DescriptorAllocation() = default;

        explicit DescriptorAllocation(u64 Version, u32 Offset, u32 Size) : Version(Version), Offset(Offset), Size(Size)
        {
        }

        operator bool() const { return Offset != COPLT_U32_MAX; }

        bool operator==(const DescriptorAllocation& other) const
        {
            return Version == other.Version && Offset == other.Offset && Size == other.Size;
        }
    };

    struct DescriptorPersistentAllocation : DescriptorAllocation
    {
        u64 AllocatorVersion{};

        DescriptorPersistentAllocation() = default;

        explicit DescriptorPersistentAllocation(u64 Version, u32 Offset, u32 Size, u64 AllocatorVersion)
            : DescriptorAllocation(Version, Offset, Size), AllocatorVersion(AllocatorVersion)
        {
        }
    };

    struct DescriptorAllocator final : Object<DescriptorAllocator, FUnknown>
    {
    private:
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorHeap> m_heap{};
        u64 m_version{};
        HashMap<u64, DescriptorPersistentAllocation> m_persist_allocations{};
        D3D12_DESCRIPTOR_HEAP_TYPE const m_type{};
        u32 const m_stride{};
        u32 m_persist_offset{};
        u32 m_transient_offset{};

    public:
        explicit DescriptorAllocator(const ComPtr<ID3D12Device2>& device, D3D12_DESCRIPTOR_HEAP_TYPE type);

        const Rc<DescriptorHeap>& Heap() const;

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const;
        u32 Stride() const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetLocalHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetRemoteHandle(u32 offset = 0) const;

        void InitFrame(u32 GrowthCapacity);

        DescriptorAllocation AllocateTransient(u32 Size);
        DescriptorAllocation AllocatePersistent(NonNull<DescriptorHeap> Heap, bool& IsOld);
        void Upload(const DescriptorAllocation& Al, const Rc<DescriptorHeap>& Heap, u32 Offset = 0) const;
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

        NonNull<DescriptorAllocator> ResourceHeap() const;
        NonNull<DescriptorAllocator> SamplerHeap() const;

        void InitFrame(u32 ResourceCap, u32 SamplerCap);
        void EndFrame();

        std::array<ID3D12DescriptorHeap*, 2> GetDescriptorHeaps() const;
    };
}
