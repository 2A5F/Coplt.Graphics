#pragma once

#include <mutex>
#include <directx/d3d12.h>

#include "concurrentqueue.h"

#include "CmdListPack.h"
#include "Device.h"
#include "../Include/Utils.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Queue.h"
#include "Command.h"
#include "DescriptorManager.h"
#include "Executor.h"

namespace Coplt
{
    struct D3d12GpuIsolate;
    struct D3d12FrameContext;

    struct D3d12GpuQueue final : GpuObject<D3d12GpuQueue, FD3d12GpuQueue>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12CommandQueue> m_queue{};
        Rc<D3d12FrameContext> m_frame_context{};
        CmdListPack m_cmd{};
        std::mutex m_mutex{};

        D3d12CommandInterpreter m_command_interpreter;

        explicit D3d12GpuQueue(Rc<D3d12GpuDevice>&& device, const FMainQueueCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        void* GetRawQueue() noexcept override;

        FResult CreateOutputFromRawSwapchain(
            const FGpuOutputFromSwapChainCreateOptions& options,
            void* swapchain,
            FGpuOutput** out
        ) noexcept override;

        FResult CreateOutputForHwnd(
            const FGpuOutputCreateOptions& options,
            void* hwnd,
            FGpuOutput** out
        ) noexcept override;

        ~D3d12GpuQueue() noexcept override;

        // 提交命令，并互换帧上下文，参数提供的帧上下文必须可用
        void Submit(Rc<D3d12FrameContext>& frame_context, /* 可选 */ const FCommandSubmit* submit);
        // 提交命令，并互换帧上下文，但是无锁，需要外部手动锁，参数提供的帧上下文必须可用
        void SubmitNoLock(Rc<D3d12FrameContext>& frame_context, /* 可选 */ const FCommandSubmit* submit);

        FResult Submit(FGpuExecutor* Executor, const FCommandSubmit* submit) noexcept override;
        FResult SubmitNoWait(FGpuExecutor* Executor, const FCommandSubmit* submit) noexcept override;
    };

    inline D3D12_COMMAND_LIST_TYPE ToDx(const FGpuQueueType value)
    {
        switch (value)
        {
        case FGpuQueueType::Direct:
            return D3D12_COMMAND_LIST_TYPE_DIRECT;
        case FGpuQueueType::Compute:
            return D3D12_COMMAND_LIST_TYPE_COMPUTE;
        case FGpuQueueType::Copy:
            return D3D12_COMMAND_LIST_TYPE_COPY;
        case FGpuQueueType::VideoEncode:
            return D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE;
        case FGpuQueueType::VideoDecode:
            return D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE;
        case FGpuQueueType::VideoProcess:
            return D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS;
        }
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    }

    struct ID3d12GpuQueue;
    struct D3d12GpuQueue2;

    // 必须等待队列 fence 完成才能析构，或者可以 move 走
    struct RentedCommandAllocator final
    {
        Rc<D3d12GpuQueue2> m_queue{};
        ComPtr<ID3D12CommandAllocator> m_command_allocator{};

        RentedCommandAllocator() = default;
        explicit RentedCommandAllocator(Rc<D3d12GpuQueue2>&& queue, ComPtr<ID3D12CommandAllocator>&& command_allocator);

        ~RentedCommandAllocator();
    };

    struct QueueWaitPoint final
    {
        Rc<ID3d12GpuQueue> m_queue{};
        u64 m_fence_value{};

        QueueWaitPoint() = default;
        explicit QueueWaitPoint(Rc<ID3d12GpuQueue>&& queue, u64 fence_value);

        void Wait(HANDLE event) const;
    };

    COPLT_INTERFACE_DEFINE(ID3d12GpuQueue, "a60df5da-ecff-4ae4-a38b-6ddef7db5922", FD3d12GpuQueue2)
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12CommandQueue> m_queue{};
        ComPtr<ID3D12Fence> m_fence{};
        u64 m_fence_value{};
        std::mutex m_mutex{};

        virtual u64 Signal() = 0;
        virtual QueueWaitPoint SignalWaitPointer() = 0;
        virtual u64 SignalNoLock() = 0;

        virtual void WaitFenceValue(u64 fence_value, HANDLE event) = 0;
        virtual RentedCommandAllocator RentCommandAllocator() = 0;
        virtual void ReturnCommandAllocator(ComPtr<ID3D12CommandAllocator>&& command_allocator) = 0;
    };

    struct D3d12GpuQueue2 final : GpuObject<D3d12GpuQueue2, ID3d12GpuQueue>, FGpuQueueData
    {
        using CommandAllocatorConcurrentQueue = moodycamel::ConcurrentQueue<ComPtr<ID3D12CommandAllocator>>;

        Box<CommandAllocatorConcurrentQueue> m_command_allocator_pool{};

        explicit D3d12GpuQueue2(NonNull<D3d12GpuIsolate> isolate, FGpuQueueType type);

        FGpuQueueData* GpuQueueData() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;
        void* GetRawQueue() noexcept override;

        u64 Signal() override;
        QueueWaitPoint SignalWaitPointer() override;
        u64 SignalNoLock() override;

        void WaitFenceValue(u64 fence_value, HANDLE event) override;

        RentedCommandAllocator RentCommandAllocator() override;
        void ReturnCommandAllocator(ComPtr<ID3D12CommandAllocator>&& command_allocator) override;
    };
} // Coplt
