#include "Queue.h"

#include "Device.h"
#include "Isolate.h"
#include "Output.h"
#include "../../Api/Include/Error.h"

using namespace Coplt;

D3d12GpuQueue::D3d12GpuQueue(
    Rc<D3d12GpuDevice>&& device,
    const FMainQueueCreateOptions& options
) : m_device(std::move(device)), m_command_interpreter(this)
{
    m_queue_type = FGpuQueueType::Direct;

    m_dx_device = m_device->m_device;

    constexpr UINT node_mask = 0;

    D3D12_COMMAND_QUEUE_DESC desc{};
    desc.NodeMask = node_mask;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.Type = ToDx(m_queue_type);
    desc.Priority = 0;

    chr | m_dx_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_queue));

    m_frame_context = new D3d12FrameContext(CloneThis());
    m_context = m_frame_context.get();

    ComPtr<ID3D12GraphicsCommandList> command_list{};
    chr | m_dx_device->CreateCommandList(
        node_mask, desc.Type, m_frame_context->m_command_allocator.Get(),
        nullptr, IID_PPV_ARGS(&command_list)
    );
    m_cmd = CmdListPack(std::move(command_list));

    if (m_device->Debug())
    {
        chr | m_queue >> SetNameEx(options.Name);
    }
}

FResult D3d12GpuQueue::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        chr | m_queue >> SetNameEx(name);
    });
}

void* D3d12GpuQueue::GetRawQueue() noexcept
{
    return m_queue.Get();
}

FResult D3d12GpuQueue::CreateOutputFromRawSwapchain(
    const FGpuOutputFromSwapChainCreateOptions& options,
    void* swapchain,
    FGpuOutput** out
) noexcept
{
    return feb([&]
    {
        *out = new D3d12GpuSwapChainOutput(this->CloneThis(), options, static_cast<IDXGISwapChain3*>(swapchain));
    });
}

FResult D3d12GpuQueue::CreateOutputForHwnd(
    const FGpuOutputCreateOptions& options,
    void* hwnd,
    FGpuOutput** out
) noexcept
{
    return feb([&]
    {
        *out = new D3d12GpuSwapChainOutput(this->CloneThis(), options, static_cast<HWND>(hwnd));
    });
}

D3d12GpuQueue::~D3d12GpuQueue() noexcept
{
    // ReSharper disable once CppFunctionResultShouldBeUsed
    m_cmd->Close();
}

void D3d12GpuQueue::Submit(
    Rc<D3d12FrameContext>& frame_context, /* 可选 */ const FCommandSubmit* submit
)
{
    std::lock_guard lock(m_mutex);
    SubmitNoLock(frame_context, submit);
}

void D3d12GpuQueue::SubmitNoLock(
    Rc<D3d12FrameContext>& frame_context, /* 可选 */ const FCommandSubmit* submit
)
{
    if (submit == nullptr)
    {
        chr | m_cmd->Close();
    }
    else
    {
        m_command_interpreter.Interpret(*submit);
        chr | m_cmd->Close();
        ID3D12CommandList* command_lists[1] = {m_cmd.m_list0.Get()};
        m_queue->ExecuteCommandLists(1, command_lists);
    }

    std::swap(m_frame_context, frame_context);
    chr | m_cmd->Reset(m_frame_context->m_command_allocator.Get(), nullptr);
    m_context = m_frame_context.get();
    m_submit_id++;
}

FResult D3d12GpuQueue::Submit(FGpuExecutor* Executor, const FCommandSubmit* submit) noexcept
{
    return feb([&]
    {
        const auto executor = Executor->QueryInterface<ID3d12GpuExecutor>();
        if (executor == nullptr)
            COPLT_THROW("Executors from different backends");
        executor->Submit(this, submit);
    });
}

FResult D3d12GpuQueue::SubmitNoWait(FGpuExecutor* Executor, const FCommandSubmit* submit) noexcept
{
    return feb([&]
    {
        const auto executor = Executor->QueryInterface<ID3d12GpuExecutor>();
        if (executor == nullptr)
            COPLT_THROW("Executors from different backends");
        executor->SubmitNoWait(this, submit);
    });
}

RentedCommandAllocator::RentedCommandAllocator(Rc<D3d12GpuQueue2>&& queue, ComPtr<ID3D12CommandAllocator>&& command_allocator)
    : m_queue(std::move(queue)), m_command_allocator(std::move(command_allocator))
{
}

RentedCommandAllocator::~RentedCommandAllocator()
{
    if (!m_queue) return;
    m_queue->ReturnCommandAllocator(std::move(m_command_allocator));
}

D3d12GpuQueue2::D3d12GpuQueue2(const NonNull<D3d12GpuIsolate> isolate, const FGpuQueueType type)
    : FGpuQueueData()
{
    m_command_allocator_pool = box<CommandAllocatorConcurrentQueue>();

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

u64 D3d12GpuQueue2::SignalNoLock()
{
    const auto fence_value = m_fence_value;
    chr | m_queue->Signal(m_fence.Get(), fence_value);
    m_fence_value++;
    return fence_value;
}

void D3d12GpuQueue2::WaitFenceValue(u64 fence_value, HANDLE event)
{
    if (m_fence->GetCompletedValue() < fence_value)
    {
        chr | m_fence->SetEventOnCompletion(fence_value, event);
        WaitForSingleObject(event, INFINITE);
    }
}

RentedCommandAllocator D3d12GpuQueue2::RentCommandAllocator()
{
    ComPtr<ID3D12CommandAllocator> command_allocator{};
    if (!m_command_allocator_pool->try_dequeue(command_allocator))
    {
        chr | m_device->m_device->CreateCommandAllocator(
            ToDx(this->QueueType), IID_PPV_ARGS(&command_allocator)
        );
    }
    return RentedCommandAllocator(this->CloneThis(), std::move(command_allocator));
}

void D3d12GpuQueue2::ReturnCommandAllocator(ComPtr<ID3D12CommandAllocator>&& command_allocator)
{
    chr | command_allocator->Reset();
    m_command_allocator_pool->enqueue(std::move(command_allocator));
}
