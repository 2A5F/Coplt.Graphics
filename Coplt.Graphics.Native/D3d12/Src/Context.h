#pragma once

#include <directx/d3d12video.h>

#include "../../Api/FFI/Context.h"
#include "../../Api/Include/Object.h"
#include "CmdListPack.h"
#include "Device.h"
#include "Isolate.h"
#include "Queue.h"
#include "Resource.h"

namespace Coplt
{
    struct D3d12FrameContext final : GpuObject<D3d12FrameContext, FFrameContext>
    {
        constexpr static u64 InitUploadBufferSize = 1024 * 1024;

        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        Rc<D3d12GpuQueue> m_queue{};
        ComPtr<ID3D12CommandAllocator> m_command_allocator{};
        Rc<DescriptorContext> m_descriptors{};
        std::vector<BufferPack> m_upload_buffers{};
        usize m_cur_upload_buffer_index{};
        usize m_cur_upload_buffer_size{};

        explicit D3d12FrameContext(Rc<D3d12GpuQueue>&& queue);

        FResult SetName(const FStr8or16& name) noexcept override;

        FResult GrowUploadBuffer(u64 min_required_size) noexcept override;

        // 回收帧使用的资源，重复使用，需要在队列中等待帧完成后调用
        void Recycle();
    };

    struct D3d12CommandList;
    struct D3d12RentedCommandList;
    struct D3d12RentedCommandAllocator;

    struct D3d12RecordContext final : GpuObject<D3d12RecordContext, FRecordContext>
    {
        constexpr static u64 InitUploadBufferSize = 1024 * 1024;

        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12CommandListPoolCluster> m_cmd_alloc_pool{};
        std::vector<D3d12RentedCommandAllocator> m_recycled_command_allocators{};
        std::vector<BufferPack> m_upload_buffers{};
        usize m_cur_upload_buffer_index{};
        usize m_cur_upload_buffer_size{};

        explicit D3d12RecordContext(NonNull<D3d12GpuIsolate> isolate);

        FResult SetName(const FStr8or16& name) noexcept override;

        FResult GrowUploadBuffer(u64 min_required_size) noexcept override;

        // 回收帧使用的资源，重复使用，需要在队列中等待帧完成后调用
        void Recycle();
    };

    struct D3d12CommandPool final : Object<D3d12CommandPool, FUnknown>
    {
        using CommandAllocatorConcurrentQueue = moodycamel::ConcurrentQueue<ComPtr<ID3D12CommandAllocator>>;
        using CommandListConcurrentQueue = moodycamel::ConcurrentQueue<Rc<D3d12CommandList>>;

        Rc<D3d12GpuDevice> m_device{};
        Box<CommandAllocatorConcurrentQueue> m_command_allocator_pool{};
        Box<CommandListConcurrentQueue> m_command_list_pool{};
        D3D12_COMMAND_LIST_TYPE m_type{};

        explicit D3d12CommandPool(const Rc<D3d12GpuDevice>& device, D3D12_COMMAND_LIST_TYPE type);

        D3d12RentedCommandAllocator RentCommandAllocator();
        void ReturnCommandAllocator(ComPtr<ID3D12CommandAllocator>&& allocator) const;
        void ReturnCommandList(Rc<D3d12CommandList>&& list) const;
    };

    struct D3d12CommandListPoolCluster final : Object<D3d12CommandListPoolCluster, FUnknown>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12CommandPool> m_direct{};
        Rc<D3d12CommandPool> m_compute{};
        Rc<D3d12CommandPool> m_copy{};
        Rc<D3d12CommandPool> m_video_decode{};
        Rc<D3d12CommandPool> m_video_process{};
        Rc<D3d12CommandPool> m_video_encode{};

        explicit D3d12CommandListPoolCluster(const Rc<D3d12GpuDevice>& device);

        D3d12RentedCommandAllocator RentCommandAllocator(D3D12_COMMAND_LIST_TYPE type) const;
    };

    struct D3d12CommandList final : Object<D3d12CommandList, FUnknown>
    {
        D3D12_COMMAND_LIST_TYPE type{};

        ComPtr<ID3D12GraphicsCommandList10> g10{};
        ComPtr<ID3D12GraphicsCommandList9> g9{};
        ComPtr<ID3D12GraphicsCommandList8> g8{};
        ComPtr<ID3D12GraphicsCommandList7> g7{};
        ComPtr<ID3D12GraphicsCommandList6> g6{};
        ComPtr<ID3D12GraphicsCommandList5> g5{};
        ComPtr<ID3D12GraphicsCommandList4> g4{};
        ComPtr<ID3D12GraphicsCommandList3> g3{};
        ComPtr<ID3D12GraphicsCommandList2> g2{};
        ComPtr<ID3D12GraphicsCommandList1> g1{};
        ComPtr<ID3D12GraphicsCommandList> g0{};

        ComPtr<ID3D12VideoDecodeCommandList3> vd3{};
        ComPtr<ID3D12VideoDecodeCommandList2> vd2{};
        ComPtr<ID3D12VideoDecodeCommandList1> vd1{};
        ComPtr<ID3D12VideoDecodeCommandList> vd0{};

        ComPtr<ID3D12VideoEncodeCommandList4> ve4{};
        ComPtr<ID3D12VideoEncodeCommandList3> ve3{};
        ComPtr<ID3D12VideoEncodeCommandList2> ve2{};
        ComPtr<ID3D12VideoEncodeCommandList1> ve1{};
        ComPtr<ID3D12VideoEncodeCommandList> ve0{};

        ComPtr<ID3D12VideoProcessCommandList3> vp3{};
        ComPtr<ID3D12VideoProcessCommandList2> vp2{};
        ComPtr<ID3D12VideoProcessCommandList1> vp1{};
        ComPtr<ID3D12VideoProcessCommandList> vp0{};

        explicit D3d12CommandList(const D3d12RentedCommandAllocator& allocator);

        void Reset(const D3d12RentedCommandAllocator& allocator) const;
        void Close() const;
    };

    struct D3d12RentedCommandList final
    {
        Rc<D3d12CommandPool> m_pool{};
        Rc<D3d12CommandList> m_list{};

        D3d12RentedCommandList() = default;
        explicit D3d12RentedCommandList(const Rc<D3d12CommandPool>& pool, Rc<D3d12CommandList>&& list);
        ~D3d12RentedCommandList();

        D3d12RentedCommandList(D3d12RentedCommandList&&) = default;
        D3d12RentedCommandList& operator=(D3d12RentedCommandList&&) = default;

        D3d12RentedCommandList(const D3d12RentedCommandList&) = delete;
        D3d12RentedCommandList& operator=(const D3d12RentedCommandList&) = delete;

        D3d12CommandList* operator->() const noexcept;
        D3d12CommandList& operator*() const noexcept;

        operator bool() const noexcept;
    };

    // 必须等待队列 fence 完成才能析构，或者可以 move 走
    struct D3d12RentedCommandAllocator final
    {
        Rc<D3d12CommandPool> m_pool{};
        ComPtr<ID3D12CommandAllocator> m_allocator{};

        explicit D3d12RentedCommandAllocator(const Rc<D3d12CommandPool>& pool, ComPtr<ID3D12CommandAllocator>&& allocator);
        ~D3d12RentedCommandAllocator();

        D3d12RentedCommandAllocator(D3d12RentedCommandAllocator&& other) = default;
        D3d12RentedCommandAllocator& operator=(D3d12RentedCommandAllocator&& other) = default;

        D3d12RentedCommandAllocator(const D3d12RentedCommandAllocator&) = delete;
        D3d12RentedCommandAllocator& operator=(const D3d12RentedCommandAllocator&) = delete;

        D3d12RentedCommandList RentCommandList() const;
    };
}
