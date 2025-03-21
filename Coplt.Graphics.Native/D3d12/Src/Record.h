#pragma once

#include "Context.h"
#include "Isolate.h"
#include "../../Api/FFI/Record.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    struct I3d12BarrierAnalyzer;
    struct D3d12BarrierAnalyzer;
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

        ResUseType GetUseType(D3D12_BARRIER_ACCESS access, ResUseType UavUse = ResUseType::ReadWrite);

        struct ResUse
        {
            u32 ResIndex{};
            D3D12_BARRIER_LAYOUT Layout{};
            D3D12_BARRIER_ACCESS Access{};
            D3D12_BARRIER_SYNC Sync{};
            ResUseType Type{};
        };
    }

    COPLT_INTERFACE_DEFINE(ID3d12GpuRecord, "57a9c7f9-1ec0-4d78-89b9-e547667c50b3", FGpuRecord)
    {
        using ResUseType = Recording::ResUseType;

        Rc<I3d12BarrierAnalyzer> m_barrier_analyzer{};
        std::vector<Rc<FUnknown>> m_resources_owner{};
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

        void WaitAndRecycle(HANDLE event) override;
        void Recycle() override;

        FResult End() noexcept override;
        void EnsureEnd() override;
        void DoEnd();

        FCmdRes& GetRes(const FCmdResRef& ref);

        void Analyze();
        void Interpret(const CmdListPack& clp, u32 cmd_index, u32 cmd_count);

        void Interpret_ClearColor(const CmdListPack& clp, u32 i, const FCmdClearColor& cmd);
        void Interpret_ClearDepthStencil(const CmdListPack& clp, u32 i, const FCmdClearDepthStencil& cmd);
    };

    COPLT_INTERFACE_DEFINE(I3d12BarrierAnalyzer, "80086589-a583-41de-9aec-205afc022cd9", FUnknown)
    {
        using ResUseType = Recording::ResUseType;
        using ResUse = Recording::ResUse;

        std::vector<ResUse> m_res_inputs{};
        std::vector<ResUse> m_res_outputs{};

        virtual void Recycle() = 0;
        virtual void Analyzer(NonNull<D3d12GpuRecord> record) = 0;
        virtual void Interpret(NonNull<D3d12GpuRecord> record, CmdListPack& cmd) = 0;
    };

    struct D3d12BarrierAnalyzer final : Object<D3d12BarrierAnalyzer, I3d12BarrierAnalyzer>
    {
    private:
        struct ResInfo
        {
            u32 CurrentBarrierPart{COPLT_U32_MAX};
            D3D12_BARRIER_LAYOUT Layout{};
            D3D12_BARRIER_ACCESS Access{};
            D3D12_BARRIER_SYNC Sync{};
            ResUseType Type{};
            FCmdResType ResType{};
            bool WasDefInput{};
            bool First{true};

            ResInfo() = default;
            explicit ResInfo(FCmdResType ResType);

            bool IsReadCompatible(D3D12_BARRIER_LAYOUT Layout) const;
            // 返回是否改变
            bool MergeRead(D3D12_BARRIER_ACCESS Access, D3D12_BARRIER_SYNC Sync);
        };

        enum class PartType : u32
        {
            Cmd,
            BarrierBegin,
            BarrierEnd,
        };

        struct BarrierPart
        {
            PartType Type{};
            u32 GlobalIndex{COPLT_U32_MAX};
            u32 GlobalCount{};
            u32 TextureIndex{COPLT_U32_MAX};
            u32 TextureCount{};
            u32 BufferIndex{COPLT_U32_MAX};
            u32 BufferCount{};
        };

        struct CmdPart
        {
            PartType Type{PartType::Cmd};
            u32 Index{};
            u32 Count{};
        };

        union Part
        {
            PartType Type{};
            BarrierPart Barrier;
            CmdPart Cmd;
        };

        NonNull<D3d12GpuRecord> m_record;
        std::vector<ResInfo> m_resources{};
        std::vector<Part> m_parts{};
        u32 m_current_barrier_begin_part{};
        u32 m_current_cmd_part{};
        std::vector<D3D12_GLOBAL_BARRIER> m_begin_global_barriers{};
        std::vector<D3D12_TEXTURE_BARRIER> m_begin_texture_barriers{};
        std::vector<D3D12_BUFFER_BARRIER> m_begin_buffer_barriers{};
        std::vector<D3D12_GLOBAL_BARRIER> m_end_global_barriers{};
        std::vector<D3D12_TEXTURE_BARRIER> m_end_texture_barriers{};
        std::vector<D3D12_BUFFER_BARRIER> m_end_buffer_barriers{};

    public:
        explicit D3d12BarrierAnalyzer();

        void Recycle() override;
        void Analyzer(NonNull<D3d12GpuRecord> record) override;

    private:
        ResInfo& GetInfo(const FCmdResRef& ref);
        std::pair<NonNull<FCmdRes>, NonNull<ResInfo>> Get(const FCmdResRef& ref);
        NonNull<ID3D12Resource> GetResource(u32 index) const;
        NonNull<FGpuBufferData> GetBufferData(u32 index) const;
        NonNull<FGpuImageData> GetImageData(u32 index) const;

        void MarkUse(
            ResInfo& Info,
            D3D12_BARRIER_ACCESS Access, D3D12_BARRIER_LAYOUT Layout, D3D12_BARRIER_SYNC Sync,
            ResUseType UavUse = ResUseType::ReadWrite
        );
        void CmdNext();

        void BuildInputOutput();

        void Analyze_ClearColor(u32 i, const FCmdClearColor& cmd);
        void Analyze_ClearDepthStencil(u32 i, const FCmdClearDepthStencil& cmd);

    public:
        void Interpret(NonNull<D3d12GpuRecord> record, CmdListPack& cmd) override;
    };

    NonNull<ID3D12Resource> GetResource(const FCmdRes& res);
    NonNull<FGpuBufferData> GetBufferData(const FCmdRes& res);
    NonNull<FGpuImageData> GetImageData(const FCmdRes& res);
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv(const FCmdRes& res);
    CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsv(const FCmdRes& res);
}
