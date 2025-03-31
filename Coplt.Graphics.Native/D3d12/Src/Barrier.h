#pragma once

#include "Context.h"
#include "Device.h"
#include "Isolate.h"
#include "../../Api/Include/Object.h"
#include "../../Api/FFI/Cmd.h"
#include "../Include/ResState.h"

namespace Coplt
{
    struct IOResState
    {
        u32 ResIndex{};
        u32 LstGroup{};
        ResState State{};
    };

    struct ID3d12BarrierAnalyzer;
    struct ID3d12BarrierCombiner;

    COPLT_INTERFACE_DEFINE(ID3d12BarrierMarshal, "792f273c-376d-4027-af99-0d14d3d71302", FUnknown)
    {
        virtual Rc<ID3d12BarrierAnalyzer> CreateAnalyzer() = 0;
        virtual Rc<ID3d12BarrierCombiner> CreateCombiner() = 0;
    };

    COPLT_INTERFACE_DEFINE(ID3d12BarrierAnalyzer, "115741ea-2138-4c92-9f4f-94f91d254587", FUnknown)
    {
        virtual std::span<const IOResState> Inputs() const = 0;
        virtual std::span<const IOResState> Outputs() const = 0;

        virtual void Clear() = 0;
        virtual void StartAnalyze(std::span<FCmdRes> resources) = 0;
        virtual void EndAnalyze() = 0;

        virtual void OnUse(FCmdResRef ResRef, ResAccess Access, ResUsage Usage, ResLayout Layout) = 0;
        virtual void OnUse(FCmdResRef ResRef) = 0;
        virtual void OnCmd() = 0;
        // 每个命令都需要调用一次
        virtual void CmdNext() = 0;

        virtual void Interpret(const D3d12RentedCommandList& list, ID3d12GpuRecord& record) = 0;
    };

    COPLT_INTERFACE_DEFINE(ID3d12BarrierCombiner, "e1e15444-6369-4d8a-90b3-153672abca39", FUnknown)
    {
        // 返回是否有实际提交，如果没有实际提交可以直接回收录制，不然就需要等待提交执行完成
        virtual bool Submit(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) = 0;
    };

    struct AD3d12BarrierCombiner : Object<AD3d12BarrierCombiner, ID3d12BarrierCombiner>
    {
        struct ResInfo
        {
            FCmdRes Res{};
            NonNull<LayoutState> State;
        };

        struct ListNode
        {
            u32 DepIndex{};
            u32 DepCount{};
            u32 ListIndex{};
            u32 ListCount{};
            FGpuQueueType Queue{};
        };

        Rc<D3d12GpuDevice> m_device{};
        std::vector<ID3D12CommandList*> m_lists{};
        std::vector<D3d12RentedCommandList> m_rented_lists{};
        std::vector<ListNode> m_list_node{};
        std::vector<FGpuQueueType> m_queue_deps{};
        FGpuQueueFlags m_used_queues{};
        HashMap<u64, ResInfo> m_submit_resources{};

        virtual void StartSubmit();
        virtual void EndSubmit();

        // 返回是否有实际提交，如果没有实际提交可以直接回收录制，不然就需要等待提交执行完成
        bool Submit(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) override;
        virtual void Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) = 0;
    };

    COPLT_INTERFACE_DEFINE(ID3d12EnhancedBarrierAnalyzer, "09e37529-9ad3-4b69-a9e5-cf57659a48c9", ID3d12BarrierAnalyzer)
    {
        enum class InfoState : u8
        {
            Unused,
            Input,
            Used,
        };

        // todo 优化内存分配
        struct Group
        {
            std::vector<D3D12_BUFFER_BARRIER> m_buffer_barriers{};
            std::vector<D3D12_TEXTURE_BARRIER> m_texture_barriers{};
            u32 m_cmd_index{};
            u32 m_cmd_count{};

            void Push(D3D12_BUFFER_BARRIER barrier);
            void Push(D3D12_TEXTURE_BARRIER barrier);

            void Clear();
            bool IsEmpty() const;
        };

        struct ResInfo
        {
            NonNull<const FCmdRes> Res;
            ResState State{};
            ResState OldState{};
            u32 PreGroup{COPLT_U32_MAX};
            u32 LstGroup{COPLT_U32_MAX};
            u32 CurGroup{COPLT_U32_MAX};
            InfoState InfoState{};

            explicit ResInfo(NonNull<const FCmdRes> Res);

            void SetNewState(ResState state);
        };

        virtual std::span<Group> Groups() = 0;
    };

    namespace Enhanced
    {
        struct EnhancedBarrierMarshal final : Object<EnhancedBarrierMarshal, ID3d12BarrierMarshal>
        {
            SRc<FGpuIsolateConfig> m_isolate_config{};
            Rc<D3d12GpuDevice> m_device;

            explicit EnhancedBarrierMarshal(const D3d12GpuIsolate& isolate);

            Rc<ID3d12BarrierAnalyzer> CreateAnalyzer() override;
            Rc<ID3d12BarrierCombiner> CreateCombiner() override;
        };

        struct EnhancedBarrierAnalyzer final : Object<EnhancedBarrierAnalyzer, ID3d12EnhancedBarrierAnalyzer>
        {
            SRc<FGpuIsolateConfig> m_isolate_config{};
            Rc<D3d12GpuDevice> m_device{};
            std::vector<ResInfo> m_resources{};
            std::vector<IOResState> m_inputs{};
            std::vector<IOResState> m_outputs{};
            std::vector<Group> m_groups{};
            u32 m_last_cmd_count{};
            b8 m_use_split_barrier{};

            explicit EnhancedBarrierAnalyzer(const EnhancedBarrierMarshal& marshal);

            std::span<const IOResState> Inputs() const override;
            std::span<const IOResState> Outputs() const override;

            std::span<Group> Groups() override;
            Group& CurGroup();
            const Group& CurGroup() const;
            u32 CurGroupIndex() const;

            void Clear() override;
            void StartAnalyze(std::span<FCmdRes> resources) override;
            void EndAnalyze() override;
            void OnUse(FCmdResRef ResRef, ResAccess Access, ResUsage Usage, ResLayout Layout) override;
            void OnUse(FCmdResRef ResRef) override;
            void OnCmd() override;
            void CmdNext() override;

            void Split();
            void CreateBarrier(const ResInfo& info);

            void Interpret(const D3d12RentedCommandList& list, ID3d12GpuRecord& record) override;
            static void SubmitBarrier(const D3d12RentedCommandList& list, Group& group);
        };

        struct EnhancedBarrierCombiner final : Object<EnhancedBarrierCombiner, AD3d12BarrierCombiner>
        {
            explicit EnhancedBarrierCombiner(const Rc<D3d12GpuDevice>& device);

            ID3d12EnhancedBarrierAnalyzer::Group m_tmp_group{};

            void EndSubmit() override;

            void Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) override;
        };
    }
}
