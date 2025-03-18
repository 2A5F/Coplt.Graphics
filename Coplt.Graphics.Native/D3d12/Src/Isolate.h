#pragma once

#include "Device.h"
#include "Queue.h"
#include "../../Api/FFI/Isolate.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuIsolate, "1c507c7f-140a-4717-809e-88096b475fea", FGpuIsolate)
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12GpuQueue2> m_main_queue{};
        Rc<D3d12GpuQueue2> m_back_queue{};
        Rc<D3d12GpuQueue2> m_copy_queue{};
        std::array<FGpuQueueCreateResult, 3> m_queues{};
        std::mutex m_mutex{};
    };

    struct D3d12GpuIsolate final : GpuObject<D3d12GpuIsolate, ID3d12GpuIsolate>, FGpuIsolateData
    {
        explicit D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuQueueCreateResult* GetQueues(u32* OutNumQueues) noexcept override;
        FGpuIsolateData* GpuIsolateData() noexcept override;

        FResult CreateRecord(const FGpuRecordCreateOptions& options, FGpuRecordCreateResult& out) noexcept override;

        FResult CreateSwapChainFromExists(const FGpuSwapChainFromExistsCreateOptions& options, FGpuSwapChainCreateResult& out) noexcept override;
        FResult CreateSwapChainForHwnd(const FGpuSwapChainCreateOptions& options, void* hwnd, FGpuSwapChainCreateResult& out) noexcept override;
    };
}
