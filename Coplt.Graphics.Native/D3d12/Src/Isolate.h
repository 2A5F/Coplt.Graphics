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
        Rc<D3d12GpuDevice> m_device{};
        std::mutex m_mutex{};
    };

    struct ID3d12GpuRecord;
    struct ID3d12BarrierMarshal;
    struct ID3d12BarrierCombiner;

    struct D3d12GpuIsolate final : GpuObject<D3d12GpuIsolate, ID3d12GpuIsolate>, FGpuIsolateData
    {
        using RecordQueue = moodycamel::ConcurrentQueue<Rc<ID3d12GpuRecord>>;

        SRc<FGpuIsolateConfig> m_config{};
        DescriptorManager m_descriptor_manager{};
        Rc<D3d12GpuQueue2> m_main_queue{};
        Rc<D3d12GpuQueue2> m_compute_queue{};
        Rc<D3d12GpuQueue2> m_copy_queue{};
        Rc<ID3d12BarrierMarshal> m_barrier_marshal{};
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
        FGpuIsolateData* GpuIsolateData() noexcept override;

        const Rc<D3d12GpuQueue2>& GetQueue(FGpuQueueType type);

        FResult RentRecords(u32 NumRecords, FGpuRecordCreateResult* OutRecords) noexcept override;
        FResult ReturnRecords(u32 NumRecords, FGpuRecord** Records) noexcept override;
        FResult Submit(u32 NumRecords, FGpuRecord** Records, FGpuRecordCreateResult* OutRecords) noexcept override;
        FResult SubmitReturn(u32 NumRecords, FGpuRecord** Records) noexcept override;

        FResult CreateSwapChainFromExists(const FGpuSwapChainFromExistsCreateOptions& options, FGpuSwapChainCreateResult& out) noexcept override;
        FResult CreateSwapChainForHwnd(const FGpuSwapChainCreateOptions& options, void* hwnd, FGpuSwapChainCreateResult& out) noexcept override;

        void RentRecords(std::span<FGpuRecordCreateResult> out);
        void RentRecords(std::span<Rc<ID3d12GpuRecord>> out);
        void ReturnRecords(std::span<FGpuRecord*> records);
        void ReturnRecords(std::span<Rc<ID3d12GpuRecord>> records);
        void Submit(std::span<FGpuRecord*> records, std::span<FGpuRecordCreateResult> out);
        void SubmitReturn(std::span<FGpuRecord*> records);
        void SubmitReturn(std::span<Rc<ID3d12GpuRecord>> records);

        void SubmitNoLock(std::span<Rc<ID3d12GpuRecord>> records);
    };
}
