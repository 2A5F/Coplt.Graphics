#pragma once

#include <semaphore>

#include "concurrentqueue.h"

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

    struct ID3d12GpuRecord;

    struct D3d12GpuIsolate final : GpuObject<D3d12GpuIsolate, ID3d12GpuIsolate>, FGpuIsolateData
    {
        using RecordQueue = moodycamel::ConcurrentQueue<Rc<ID3d12GpuRecord>>;

        Box<RecordQueue> m_waiting_ctx{};
        Box<RecordQueue> m_record_pool{};
        std::binary_semaphore m_waiting_signal{0};
        std::jthread m_wait_thread{};
        std::atomic_bool m_exited{false};
        std::atomic<u64> m_record_inc{0};

        explicit D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options);

        ~D3d12GpuIsolate() override;

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuQueueCreateResult* GetQueues(u32* OutNumQueues) noexcept override;
        FGpuIsolateData* GpuIsolateData() noexcept override;

        FResult RentRecords(u32 NumRecords, FGpuRecordCreateResult* OutRecords) noexcept override;
        FResult ReturnRecords(u32 NumRecords, FGpuRecord** Records) noexcept override;
        FResult Submit(u32 NumRecords, FGpuRecord** Records, FGpuRecordCreateResult* OutRecords) noexcept override;
        FResult SubmitReturn(u32 NumRecords, FGpuRecord** Records) noexcept override;

        FResult CreateSwapChainFromExists(const FGpuSwapChainFromExistsCreateOptions& options, FGpuSwapChainCreateResult& out) noexcept override;
        FResult CreateSwapChainForHwnd(const FGpuSwapChainCreateOptions& options, void* hwnd, FGpuSwapChainCreateResult& out) noexcept override;

        void RentRecords(std::span<FGpuRecordCreateResult> out);
        void ReturnRecords(std::span<FGpuRecord*> records);
        void Submit(std::span<FGpuRecord*> records, std::span<FGpuRecordCreateResult> out);
        void SubmitReturn(std::span<FGpuRecord*> records);
    };
}
