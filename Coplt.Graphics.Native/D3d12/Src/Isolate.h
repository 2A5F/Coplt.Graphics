#pragma once

#include "Device.h"
#include "Queue.h"
#include "../FFI/Isolate.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuIsolate, "1c507c7f-140a-4717-809e-88096b475fea", FD3d12GpuIsolate)
    {
        virtual FGpuIsolateData& Data() noexcept = 0;
        virtual const FGpuIsolateData& Data() const noexcept = 0;
        virtual const Rc<D3d12GpuQueue2>& MainQueue() const noexcept = 0;
        virtual const Rc<D3d12GpuQueue2>& BackQueue() const noexcept = 0;
        virtual const Rc<D3d12GpuQueue2>& CopyQueue() const noexcept = 0;
    };

    struct D3d12GpuIsolate final : GpuObject<D3d12GpuIsolate, ID3d12GpuIsolate>, FGpuIsolateData
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12GpuQueue2> m_main_queue{};
        Rc<D3d12GpuQueue2> m_back_queue{};
        Rc<D3d12GpuQueue2> m_copy_queue{};
        std::array<FGpuQueueCreateResult, 3> m_queues{};

        explicit D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;
        const Rc<D3d12GpuQueue2>& MainQueue() const noexcept override;
        const Rc<D3d12GpuQueue2>& BackQueue() const noexcept override;
        const Rc<D3d12GpuQueue2>& CopyQueue() const noexcept override;
        FGpuQueueCreateResult* GetQueues(u32* OutNumQueues) noexcept override;
        FGpuIsolateData* GpuIsolateData() noexcept override;
        FGpuIsolateData& Data() noexcept override;
        const FGpuIsolateData& Data() const noexcept override;
    };
}
