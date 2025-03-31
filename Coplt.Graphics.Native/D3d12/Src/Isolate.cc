#include "Isolate.h"

#include "Record.h"
#include "SwapChain.h"
#include "Context.h"
#include "../../Api/FFI/Record.h"
#include "Barrier.h"

using namespace Coplt;

D3d12GpuIsolate::D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options)
    : FGpuIsolateData()
{
    m_waiting_record = box<RecordQueue>();
    m_record_pool = box<RecordQueue>();
    m_device = std::move(device);

    #pragma region 创建队列

    m_main_queue = new D3d12GpuQueue2(this, FGpuQueueType::Direct);
    m_compute_queue = new D3d12GpuQueue2(this, FGpuQueueType::Compute);
    m_copy_queue = new D3d12GpuQueue2(this, FGpuQueueType::Copy);

    #pragma endregion

    m_barrier_marshal = new Enhanced::EnhancedBarrierMarshal(m_device);
    m_barrier_combiner = m_barrier_marshal->CreateCombiner();

    m_cmd_alloc_pool = new D3d12CommandListPoolCluster(m_device);

    m_event = CreateEventW(nullptr, false, false, nullptr);
    if (m_event == nullptr) chr | HRESULT_FROM_WIN32(GetLastError());

    m_wait_thread = std::thread([this]
    {
        while (!m_exited)
        {
            m_waiting_signal.acquire();
            feb(m_device->m_instance->m_logger, [&]
            {
                Rc<ID3d12GpuRecord> records[8]{};
            re:
                if (const auto count = m_waiting_record->try_dequeue_bulk(records, 8); count > 0)
                {
                    for (auto i = 0; i < count; ++i)
                    {
                        records[i]->WaitAndRecycle(m_event);
                        m_record_pool->enqueue(std::move(records[i]));
                    }
                    goto re;
                }
            });
        }
    });
}

D3d12GpuIsolate::~D3d12GpuIsolate()
{
    m_exited = true;
    m_waiting_signal.release(1);
    m_wait_thread.join();
    CloseHandle(m_event);
}

FResult D3d12GpuIsolate::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FGpuIsolateData* D3d12GpuIsolate::GpuIsolateData() noexcept
{
    return this;
}

const Rc<D3d12GpuQueue2>& D3d12GpuIsolate::GetQueue(const FGpuQueueType type)
{
    switch (type)
    {
    case FGpuQueueType::Direct:
        return m_main_queue;
    case FGpuQueueType::Compute:
        return m_compute_queue;
    case FGpuQueueType::Copy:
        return m_copy_queue;
    default:
        break;
    }
    return m_main_queue;
}

FResult D3d12GpuIsolate::RentRecords(const u32 NumRecords, FGpuRecordCreateResult* OutRecords) noexcept
{
    return feb([&]
    {
        const std::span out(OutRecords, NumRecords);
        RentRecords(out);
    });
}

FResult D3d12GpuIsolate::ReturnRecords(const u32 NumRecords, FGpuRecord** Records) noexcept
{
    return feb([&]
    {
        const std::span records(Records, NumRecords);
        ReturnRecords(records);
    });
}

FResult D3d12GpuIsolate::Submit(const u32 NumRecords, FGpuRecord** Records, FGpuRecordCreateResult* OutRecords) noexcept
{
    return feb([&]
    {
        const std::span records(Records, NumRecords);
        const std::span out(OutRecords, NumRecords);
        Submit(records, out);
    });
}

FResult D3d12GpuIsolate::SubmitReturn(const u32 NumRecords, FGpuRecord** Records) noexcept
{
    return feb([&]
    {
        const std::span records(Records, NumRecords);
        SubmitReturn(records);
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

void D3d12GpuIsolate::RentRecords(const std::span<FGpuRecordCreateResult> out)
{
    const auto f = [&](const std::span<Rc<ID3d12GpuRecord>> items)
    {
        const auto count = m_record_pool->try_dequeue_bulk(items.data(), out.size());
        for (u32 i = 0; i < out.size(); ++i)
        {
            ID3d12GpuRecord* ptr;
            if (i < count) ptr = items[i].leak();
            else ptr = new D3d12GpuRecord(this);
            out[i].Record = ptr;
            out[i].Data = ptr->GpuFGpuRecordData();
        }
    };
    if (out.size() < 16)
    {
        Rc<ID3d12GpuRecord> items[out.size()];
        for (u32 i = 0; i < out.size(); ++i) items[i] = {};
        f(std::span(items, out.size()));
    }
    else
    {
        std::vector<Rc<ID3d12GpuRecord>> items(out.size(), {});
        f(std::span(items));
    }
}

void D3d12GpuIsolate::RentRecords(std::span<Rc<ID3d12GpuRecord>> out)
{
    const auto count = m_record_pool->try_dequeue_bulk(out.data(), out.size());
    for (u32 i = count; i < out.size(); ++i)
    {
        out[i] = new D3d12GpuRecord(this);
    }
}

void D3d12GpuIsolate::ReturnRecords(const std::span<FGpuRecord*> records)
{
    const auto f = [&](const std::span<Rc<ID3d12GpuRecord>> items)
    {
        for (u32 i = 0; i < items.size(); ++i)
        {
            items[i] = records[i]->QueryInterface<ID3d12GpuRecord>();
        }
        for (u32 i = 0; i < items.size(); ++i)
        {
            if (items[i] == nullptr)
                COPLT_THROW("Record from different backends");
            items[i]->Recycle();
        }
        m_record_pool->enqueue_bulk(items.data(), items.size());
    };
    if (records.size() < 16)
    {
        Rc<ID3d12GpuRecord> items[records.size()];
        for (u32 i = 0; i < records.size(); ++i) items[i] = {};
        f(std::span(items, records.size()));
    }
    else
    {
        std::vector<Rc<ID3d12GpuRecord>> items(records.size(), {});
        f(std::span(items));
    }
}

void D3d12GpuIsolate::ReturnRecords(std::span<Rc<ID3d12GpuRecord>> records)
{
    for (const auto& item : records)
    {
        item->Recycle();
    }
    m_record_pool->enqueue_bulk(records.data(), records.size());
}

void D3d12GpuIsolate::Submit(std::span<FGpuRecord*> records, std::span<FGpuRecordCreateResult> out)
{
    SubmitReturn(records);
    RentRecords(out);
}

void D3d12GpuIsolate::SubmitReturn(std::span<FGpuRecord*> records)
{
    const auto f = [&](const std::span<Rc<ID3d12GpuRecord>> items)
    {
        for (u32 i = 0; i < items.size(); ++i)
        {
            items[i] = records[i]->QueryInterface<ID3d12GpuRecord>();
        }
        for (u32 i = 0; i < items.size(); ++i)
        {
            if (items[i] == nullptr)
                COPLT_THROW("Record from different backends");
        }
        SubmitReturn(items);
    };
    if (records.size() < 16)
    {
        Rc<ID3d12GpuRecord> items[records.size()];
        for (u32 i = 0; i < records.size(); ++i) items[i] = {};
        f(std::span(items, records.size()));
    }
    else
    {
        std::vector<Rc<ID3d12GpuRecord>> items(records.size(), {});
        f(std::span(items));
    }
}

void D3d12GpuIsolate::SubmitReturn(std::span<Rc<ID3d12GpuRecord>> records)
{
    std::lock_guard lock(m_mutex);
    SubmitNoLock(records);
}

void D3d12GpuIsolate::SubmitNoLock(std::span<Rc<ID3d12GpuRecord>> records)
{
    for (const auto& record : records)
    {
        record->EnsureEnd();
    }
    m_barrier_combiner->Submit(this, records);
    m_waiting_record->enqueue_bulk(records.data(), records.size());
    m_waiting_signal.release(1);
}
