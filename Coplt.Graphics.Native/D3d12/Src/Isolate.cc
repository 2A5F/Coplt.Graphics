#include "Isolate.h"

using namespace Coplt;

D3d12GpuIsolate::D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options)
    : m_device(std::move(device))
{
    m_main_queue = new D3d12GpuQueue2(this, FGpuQueueType::Direct);
    m_back_queue = new D3d12GpuQueue2(this, FGpuQueueType::Compute);
    m_copy_queue = new D3d12GpuQueue2(this, FGpuQueueType::Copy);
    m_queues = {
        FGpuQueueCreateResult{m_main_queue.get(), m_main_queue.get()},
        FGpuQueueCreateResult{m_back_queue.get(), m_back_queue.get()},
        FGpuQueueCreateResult{m_copy_queue.get(), m_copy_queue.get()},
    };
}

FResult D3d12GpuIsolate::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

const Rc<D3d12GpuQueue2>& D3d12GpuIsolate::MainQueue() const noexcept
{
    return m_main_queue;
}

const Rc<D3d12GpuQueue2>& D3d12GpuIsolate::BackQueue() const noexcept
{
    return m_back_queue;
}

const Rc<D3d12GpuQueue2>& D3d12GpuIsolate::CopyQueue() const noexcept
{
    return m_copy_queue;
}

FGpuQueueCreateResult* D3d12GpuIsolate::GetQueues(u32* OutNumQueues) noexcept
{
    *OutNumQueues = m_queues.size();
    return m_queues.data();
}

FGpuIsolateData* D3d12GpuIsolate::GpuIsolateData() noexcept
{
    return this;
}

FGpuIsolateData& D3d12GpuIsolate::Data() noexcept
{
    return *this;
}

const FGpuIsolateData& D3d12GpuIsolate::Data() const noexcept
{
    return *this;
}
