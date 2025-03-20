#pragma once

#include "Context.h"
#include "Isolate.h"
#include "../../Api/FFI/Record.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    struct D3d12GpuRecord;

    namespace Recording
    {
        enum class ResUseType : u8
        {
            None,
            Read,
            Write,
            ReadWrite,
        };

        struct ResUse
        {
            u32 LastUse{};
            u32 ResIndex{};
            u32 CmdIndex{};
            D3D12_BARRIER_LAYOUT Layout{};
            D3D12_BARRIER_ACCESS Access{};
            FShaderStageFlags Stages{};
            ResUseType Type{};
        };

        ResUseType GetUseType(D3D12_BARRIER_ACCESS access, ResUseType UavUse = ResUseType::ReadWrite);

        struct ResInfo
        {
            Rc<FUnknown> m_owner{};
            u32 m_res_index{};

            u32 m_last_use{COPLT_U32_MAX};
            u32 m_last_write_use{COPLT_U32_MAX};
            u32 m_last_read_use_after_write{COPLT_U32_MAX};

            ResInfo() = default;

            explicit ResInfo(Rc<FUnknown>&& resource, u32 res_index);

            void MarkUse(
                NonNull<D3d12GpuRecord> self, u32 index,
                D3D12_BARRIER_ACCESS Access, D3D12_BARRIER_LAYOUT Layout,
                FShaderStageFlags Stages, ResUseType UavUse = ResUseType::ReadWrite
            );
        };
    }

    COPLT_INTERFACE_DEFINE(ID3d12GpuRecord, "57a9c7f9-1ec0-4d78-89b9-e547667c50b3", FGpuRecord)
    {
        using ResUseType = Recording::ResUseType;
        using ResUse = Recording::ResUse;
        using ResInfo = Recording::ResInfo;

        std::vector<ResInfo> m_resources{};
        std::vector<u32> m_outputs{};
        std::vector<ResUse> m_res_use{};
        std::vector<QueueWaitPoint> m_queue_wait_points{};

        virtual FGpuRecordData* Data() noexcept = 0;
        virtual const FGpuRecordData* Data() const noexcept = 0;

        virtual void WaitAndRecycle(HANDLE event) = 0;
        virtual void Recycle() = 0;

        virtual void EnsureEnd() = 0;
    };

    struct D3d12GpuRecord final : GpuObject<D3d12GpuRecord, ID3d12GpuRecord>, FGpuRecordData
    {
        u64 m_isolate_id{};
        u64 m_record_id{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12RecordContext> m_context{};

        explicit D3d12GpuRecord(NonNull<D3d12GpuIsolate> isolate);

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuRecordData* GpuFGpuRecordData() noexcept override;
        FGpuRecordData* Data() noexcept override;
        const FGpuRecordData* Data() const noexcept override;

        std::pair<NonNull<FCmdRes>, NonNull<ResInfo>> Get(const FCmdResRef& ref);

        void WaitAndRecycle(HANDLE event) override;
        void Recycle() override;

        FResult End() noexcept override;
        void EnsureEnd() override;
        void DoEnd();

        void Analyze();
        void Analyze_ClearColor(u32 i, const FCmdClearColor& cmd);
    };
}
