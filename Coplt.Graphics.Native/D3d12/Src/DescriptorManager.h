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
        u64 m_version{};
        D3D12_DESCRIPTOR_HEAP_TYPE const m_type{};
        u32 const m_stride{};
        u32 const m_size{};
        bool const m_gpu{};

    public:
        explicit DescriptorHeap(const ComPtr<ID3D12Device2>& device, D3D12_DESCRIPTOR_HEAP_TYPE type, u32 size, bool gpu);

        const ComPtr<ID3D12DescriptorHeap>& Heap() const;

        u64 Id() const;
        u64 Version() const;

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const;
        u32 Stride() const;
        bool IsRemote() const;

        u32 Size() const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetLocalHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetRemoteHandle(u32 offset = 0) const;

        void IncVersion();
    };

    struct DescriptorAllocation
    {
        u32 Offset{COPLT_U32_MAX};
        u32 Size{};

        DescriptorAllocation() = default;

        explicit DescriptorAllocation(u32 Offset, u32 Size) : Offset(Offset), Size(Size)
        {
        }

        operator bool() const { return Offset != COPLT_U32_MAX; }
    };

    struct PersistDescriptorAllocation : DescriptorAllocation
    {
        u64 Version{};
        PersistDescriptorAllocation() = default;

        explicit PersistDescriptorAllocation(u64 Version, u32 Offset, u32 Size)
            : DescriptorAllocation(Offset, Size), Version(Version)
        {
        }
    };

    struct PersistDescriptorAllocation_Internal : PersistDescriptorAllocation
    {
        u64 AllocatorVersion{};

        PersistDescriptorAllocation_Internal() = default;

        explicit PersistDescriptorAllocation_Internal(u64 Version, u32 Offset, u32 Size, u64 AllocatorVersion)
            : PersistDescriptorAllocation(Version, Offset, Size), AllocatorVersion(AllocatorVersion)
        {
        }
    };

    struct DescriptorAllocator final : Object<DescriptorAllocator, FUnknown>
    {
    private:
        ComPtr<ID3D12Device2> m_device{};
        Rc<DescriptorHeap> m_tmp_heap{};
        Rc<DescriptorHeap> m_heap{};
        u64 m_version{};
        HashMap<u64, PersistDescriptorAllocation_Internal> m_persist_allocations{};
        D3D12_DESCRIPTOR_HEAP_TYPE const m_type{};
        u32 const m_stride{};
        u32 m_persist_offset{};
        u32 m_dynamic_offset{};

    public:
        explicit DescriptorAllocator(const ComPtr<ID3D12Device2>& device, D3D12_DESCRIPTOR_HEAP_TYPE type);

        const Rc<DescriptorHeap>& Heap() const;
        const Rc<DescriptorHeap>& TmpHeap() const;

        D3D12_DESCRIPTOR_HEAP_TYPE Type() const;
        u32 Stride() const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetTmpLocalHandle(u32 offset = 0) const;

        CD3DX12_CPU_DESCRIPTOR_HANDLE GetLocalHandle(u32 offset = 0) const;
        CD3DX12_GPU_DESCRIPTOR_HANDLE GetRemoteHandle(u32 offset = 0) const;

        void InitFrame(u32 GrowthCapacity);

        void AllocateTransient(u32 Size, DescriptorAllocation*& al);
        void AllocatePersistent(NonNull<DescriptorHeap> Heap, PersistDescriptorAllocation*& al, bool& IsOld);
        void Upload(const DescriptorAllocation& Al, const Rc<DescriptorHeap>& Heap, u32 Offset = 0) const;
        // void StoreTmp(const DescriptorAllocation& Al, const Rc<DescriptorHeap>& Heap, u32 Offset = 0) const;
        //
        // // 将临时堆复制到 gpu 可见堆
        // void SyncTmp() const;
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

        // // 将临时堆复制到 gpu 可见堆
        // void SyncTmp() const;

        std::array<ID3D12DescriptorHeap*, 2> GetDescriptorHeaps() const;
    };
}
