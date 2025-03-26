#pragma once

#include "Barrier.h"
#include "Context.h"
#include "Isolate.h"
#include "RecordStorage.h"
#include "../../Api/FFI/Record.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuRecord, "57a9c7f9-1ec0-4d78-89b9-e547667c50b3", FGpuRecord)
    {
        virtual FGpuRecordData* Data() noexcept = 0;
        virtual const FGpuRecordData* Data() const noexcept = 0;

        virtual void RegisterWaitPoint(QueueWaitPoint&& wait_point) = 0;
        virtual void WaitAndRecycle(HANDLE event) = 0;
        virtual void Recycle() = 0;

        virtual void EnsureEnd() = 0;

        virtual const Rc<D3d12RecordStorage>& Storage() = 0;
        virtual const Rc<ID3d12BarrierRecorder>& BarrierRecorder() = 0;
    };

    struct D3d12GpuRecord final : GpuObject<D3d12GpuRecord, ID3d12GpuRecord>, FGpuRecordData
    {
        u64 m_isolate_id{};
        u64 m_record_id{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12RecordContext> m_context{};
        Rc<D3d12RecordStorage> m_storage{};
        Rc<ID3d12BarrierRecorder> m_barrier_recorder{};
        std::vector<Rc<FGpuObject>> m_resources_owner{};
        std::vector<QueueWaitPoint> m_queue_wait_points{};

        explicit D3d12GpuRecord(NonNull<D3d12GpuIsolate> isolate);

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuRecordData* GpuFGpuRecordData() noexcept override;
        FGpuRecordData* Data() noexcept override;
        const FGpuRecordData* Data() const noexcept override;

        const Rc<D3d12RecordStorage>& Storage() override;
        const Rc<ID3d12BarrierRecorder>& BarrierRecorder() override;

        void RegisterWaitPoint(QueueWaitPoint&& wait_point) override;
        void WaitAndRecycle(HANDLE event) override;
        void Recycle() override;

        FResult End() noexcept override;
        void EnsureEnd() override;
        void DoEnd();

        FCmdRes& GetRes(const FCmdResRef& ref);

        D3d12RentedCommandList& CurList() const;

        void ReadyResource();
        void Interpret();
        void Interpret_PreparePresent(u32 i, const FCmdPreparePresent& cmd) const;
        void Interpret_ClearColor(u32 i, const FCmdClearColor& cmd);
        void Interpret_ClearDepthStencil(u32 i, const FCmdClearDepthStencil& cmd);
    };

    ResQueue ToResQueue(FGpuRecordMode mode);
    NonNull<ID3D12Resource> GetResource(const FCmdRes& res);
    NonNull<FGpuBufferData> GetBufferData(const FCmdRes& res);
    NonNull<FGpuImageData> GetImageData(const FCmdRes& res);
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv(const FCmdRes& res);
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsv(const FCmdRes& res);
    NonNull<LayoutState> GetState(const FCmdRes& res);
}
