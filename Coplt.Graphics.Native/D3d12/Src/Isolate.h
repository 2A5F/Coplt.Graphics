#pragma once

#include <semaphore>

#include "concurrentqueue.h"

#include "Device.h"
#include "Queue.h"
#include "../../Api/FFI/Isolate.h"
#include "../../Api/Include/GpuObject.h"
#include "../../Api/FFI/Cmd.h"

namespace Coplt
{
    struct D3d12CommandListPoolCluster;
    COPLT_INTERFACE_DEFINE(ID3d12GpuIsolate, "1c507c7f-140a-4717-809e-88096b475fea", FGpuIsolate)
    {
        SRc<FQueueConfig> m_queue_config{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12GpuQueue2> m_main_queue{};
        std::vector<Rc<D3d12GpuQueue2>> m_direct_queues{};
        std::vector<Rc<D3d12GpuQueue2>> m_compute_queues{};
        std::vector<Rc<D3d12GpuQueue2>> m_copy_queues{};
        std::vector<Rc<D3d12GpuQueue2>> m_video_encode_queues{};
        std::vector<Rc<D3d12GpuQueue2>> m_video_decode_queues{};
        std::vector<Rc<D3d12GpuQueue2>> m_video_process_queues{};
        std::vector<FGpuQueueCreateResult> m_queues{};
        std::mutex m_mutex{};
    };

    struct ID3d12GpuRecord;
    struct ID3d12BarrierAnalyzer;
    struct ID3d12BarrierCombiner;

    struct QueueAt
    {
        u32 Index{};
        FGpuQueueType Type{};

        QueueAt() = default;

        explicit QueueAt(u32 index, FGpuQueueType type);

        bool IsMain() const;
    };

    struct D3d12GpuIsolate final : GpuObject<D3d12GpuIsolate, ID3d12GpuIsolate>, FGpuIsolateData
    {
        using RecordQueue = moodycamel::ConcurrentQueue<Rc<ID3d12GpuRecord>>;

        Rc<ID3d12BarrierAnalyzer> m_barrier_analyzer{};
        Rc<ID3d12BarrierCombiner> m_barrier_combiner{};
        HANDLE m_event{};
        Box<RecordQueue> m_waiting_record{};
        Box<RecordQueue> m_record_pool{};
        Rc<D3d12CommandListPoolCluster> m_cmd_alloc_pool{};
        std::binary_semaphore m_waiting_signal{0};
        std::thread m_wait_thread{};
        std::atomic_bool m_exited{false};
        std::atomic<u64> m_record_inc{0};

        explicit D3d12GpuIsolate(Rc<D3d12GpuDevice> device, const FGpuIsolateCreateOptions& options);

        ~D3d12GpuIsolate() override;

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuQueueCreateResult* GetQueues(u32* OutNumQueues) noexcept override;
        FGpuIsolateData* GpuIsolateData() noexcept override;

        const Rc<D3d12GpuQueue2>& GetQueue(QueueAt at);

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
        void SubmitReturn(std::span<Rc<ID3d12GpuRecord>> records);

        void SubmitNoLock(std::span<Rc<ID3d12GpuRecord>> records);
    };
}
