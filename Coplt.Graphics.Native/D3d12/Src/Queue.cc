#include "Queue.h"

#include "Device.h"
#include "Isolate.h"
#include "Output.h"
#include "../../Api/Include/Error.h"

using namespace Coplt;

QueueWaitPoint::QueueWaitPoint(Rc<ID3d12GpuQueue>&& queue, const u64 fence_value) : m_queue(std::move(queue)), m_fence_value(fence_value)
{
}

void QueueWaitPoint::Wait(const HANDLE event) const
{
    m_queue->WaitFenceValue(m_fence_value, event);
}

D3d12GpuQueue2::D3d12GpuQueue2(const NonNull<D3d12GpuIsolate> isolate, const FGpuQueueType type)
    : FGpuQueueData()
{
    m_device = isolate->m_device;
    QueueType = type;
    const NonNull device = m_device->m_device.Get();

    constexpr UINT node_mask = 0;

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.NodeMask = node_mask;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = ToDx(type);
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

    chr | device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue));

    chr | device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    m_fence_value = 1;
}

FGpuQueueData* D3d12GpuQueue2::GpuQueueData() noexcept
{
    return this;
}

FResult D3d12GpuQueue2::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        if (name.is_null()) return;
        chr | m_queue >> SetNameEx(name);
        if (name.is8())
        {
            const auto str = fmt::format("[{}]::Fence", name.str8);
            chr | m_fence >> SetNameEx(FStr8or16(str));
        }
        else
        {
            const auto str = fmt::format(L"[{}]::Fence", reinterpret_cast<const wchar_t*>(name.str16));
            chr | m_fence >> SetNameEx(FStr8or16(str));
        }
    });
}

void* D3d12GpuQueue2::GetRawQueue() noexcept
{
    return m_queue.Get();
}

u64 D3d12GpuQueue2::Signal()
{
    std::lock_guard lock(m_mutex);
    return SignalNoLock();
}

QueueWaitPoint D3d12GpuQueue2::SignalWaitPointer()
{
    return QueueWaitPoint(this->CloneThis(), Signal());
}

u64 D3d12GpuQueue2::SignalNoLock()
{
    const auto fence_value = m_fence_value;
    chr | m_queue->Signal(m_fence.Get(), fence_value);
    m_fence_value++;
    return fence_value;
}

void D3d12GpuQueue2::WaitFenceValue(const u64 fence_value, const HANDLE event)
{
    if (m_fence->GetCompletedValue() < fence_value)
    {
        chr | m_fence->SetEventOnCompletion(fence_value, event);
        WaitForSingleObject(event, INFINITE);
    }
}

void D3d12GpuQueue2::Wait(D3d12GpuQueue2& other, const u64 fence_value)
{
    chr | m_queue->Wait(other.m_fence.Get(), fence_value);
}
