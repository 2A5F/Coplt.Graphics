#include "Queue.h"

#include "Device.h"
#include "Output.h"
#include "../../Api/Include/Error.h"

using namespace Coplt;

D3d12GpuQueue::D3d12GpuQueue(
    Rc<D3d12GpuDevice>&& device,
    const FMainQueueCreateOptions& options
) : m_device(std::move(device))
{
    m_queue_type = FGpuQueueType::Direct;

    m_dx_device = m_device->m_device;

    constexpr UINT node_mask = 0;

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.NodeMask = node_mask;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = 0;

    chr | m_dx_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue));

    chr | m_dx_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator));

    chr | m_dx_device->CreateCommandList(
        node_mask, D3D12_COMMAND_LIST_TYPE_DIRECT, m_command_allocator.Get(),
        nullptr, IID_PPV_ARGS(&m_command_list)
    );

    chr | m_command_list->Close();

    if (m_device->Debug())
    {
        chr | m_queue >> SetNameEx(options.Name);
    }
}

FResult D3d12GpuQueue::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        chr | m_queue >> SetNameEx(name);
    });
}

void* D3d12GpuQueue::GetRawQueue() noexcept
{
    return m_queue.Get();
}

FResult D3d12GpuQueue::CreateOutputForHwnd(
    const FGpuOutputCreateOptions& options,
    void* hwnd,
    FGpuOutput** out
) noexcept
{
    return feb([&]
    {
#ifdef _WINDOWS
        *out = new D3d12GpuSwapChainOutput(this->CloneThis(), options, static_cast<HWND>(hwnd));
#else
        return FResult::Error(u"Calling Windows platform-specific APIs on non-Windows platforms");
#endif
    });
}
