#include "Isolate.h"

#include "Record.h"
#include "SwapChain.h"
#include "../../Api/FFI/Record.h"

using namespace Coplt;

D3d12GpuIsolate::D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options)
    : FGpuIsolateData()
{
    m_device = std::move(device);
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

FGpuQueueCreateResult* D3d12GpuIsolate::GetQueues(u32* OutNumQueues) noexcept
{
    *OutNumQueues = m_queues.size();
    return m_queues.data();
}

FGpuIsolateData* D3d12GpuIsolate::GpuIsolateData() noexcept
{
    return this;
}

FResult D3d12GpuIsolate::CreateRecord(const FGpuRecordCreateOptions& options, FGpuRecordCreateResult& out) noexcept
{
    return feb([&]
    {
        const auto ptr = new D3d12GpuRecord(this, options);
        out.Record = ptr;
        out.Data = ptr;
    });
}

FResult D3d12GpuIsolate::CreateSwapChainFromExists(const FGpuSwapChainFromExistsCreateOptions& options, FGpuSwapChainCreateResult& out) noexcept
{
    return feb([&]
    {
        const auto ptr = new D3d12GpuSwapChain(this, options);
        out.SwapChain = ptr;
        out.Data = ptr;
    });
}

FResult D3d12GpuIsolate::CreateSwapChainForHwnd(const FGpuSwapChainCreateOptions& options, void* hwnd, FGpuSwapChainCreateResult& out) noexcept
{
    return feb([&]
    {
        const auto ptr = new D3d12GpuSwapChain(this, options, static_cast<HWND>(hwnd));
        out.SwapChain = ptr;
        out.Data = ptr;
    });
}
