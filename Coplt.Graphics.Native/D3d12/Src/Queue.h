#pragma once

#include <mutex>
#include <directx/d3d12.h>

#include "concurrentqueue.h"

#include "Device.h"
#include "../Include/Utils.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Queue.h"

namespace Coplt
{
    struct D3d12GpuIsolate;
    struct D3d12FrameContext;

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

    struct QueueWaitPoint final
    {
        Rc<ID3d12GpuQueue> m_queue{};
        u64 m_fence_value{};

        QueueWaitPoint() = default;
        explicit QueueWaitPoint(Rc<ID3d12GpuQueue>&& queue, u64 fence_value);

        void Wait(HANDLE event) const;
    };

    struct D3d12GpuQueue2;

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
        virtual void Wait(D3d12GpuQueue2& other, u64 fence_value) = 0;
    };

    struct D3d12GpuQueue2 final : GpuObject<D3d12GpuQueue2, ID3d12GpuQueue>, FGpuQueueData
    {
        explicit D3d12GpuQueue2(NonNull<D3d12GpuIsolate> isolate, FGpuQueueType type);

        FGpuQueueData* GpuQueueData() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;
        void* GetRawQueue() noexcept override;

        u64 Signal() override;
        QueueWaitPoint SignalWaitPointer() override;
        u64 SignalNoLock() override;

        void WaitFenceValue(u64 fence_value, HANDLE event) override;
        void Wait(D3d12GpuQueue2& other, u64 fence_value) override;
    };
} // Coplt
