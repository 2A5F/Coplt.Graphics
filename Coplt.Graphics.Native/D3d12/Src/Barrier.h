#pragma once

#include "Device.h"
#include "RecordStorage.h"
#include "../../Api/Include/Object.h"
#include "../../Api/FFI/Cmd.h"
#include "../Include/ResState.h"

namespace Coplt
{
    struct InputResState
    {
        u32 ResIndex{};
        u32 BeforeUseListIndex{};
        ResState State{};
    };

    struct ID3d12BarrierRecorder;
    struct ID3d12BarrierCombiner;

    COPLT_INTERFACE_DEFINE(ID3d12BarrierAnalyzer, "792f273c-376d-4027-af99-0d14d3d71302", FUnknown)
    {
        virtual Rc<ID3d12BarrierRecorder> CreateRecorder(const Rc<ID3d12RecordStorage>& storage) = 0;
        virtual Rc<ID3d12BarrierCombiner> CreateCombiner() = 0;
    };

    COPLT_INTERFACE_DEFINE(ID3d12BarrierRecorder, "115741ea-2138-4c92-9f4f-94f91d254587", FUnknown)
    {
        virtual std::span<const InputResState> Inputs() const = 0;

        virtual void Clear() = 0;
        virtual void StartRecord(std::span<FCmdRes> resources) = 0;
        virtual void EndRecord() = 0;

        virtual void OnUse(FCmdResRef ResRef, ResAccess Access, ResUsage Usage, ResLayout Layout) = 0;
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

        struct ListRange
        {
            u32 DepIndex{};
            u32 DepCount{};
            u32 ListIndex{};
            u32 ListCount{};
            FGpuQueueType Queue{};
        };

        Rc<D3d12GpuDevice> m_device{};
        std::vector<ID3D12CommandList*> m_submit_lists{};
        std::vector<ListRange> m_list_ranges{};
        std::vector<FGpuQueueType> m_queue_deps{};
        FGpuQueueFlags m_used_queues{};
        HashMap<u64, ResInfo> m_submit_resources{};

        virtual void StartSubmit();
        virtual void EndSubmit();

        // 返回是否有实际提交，如果没有实际提交可以直接回收录制，不然就需要等待提交执行完成
        bool Submit(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) override;
        virtual void Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) = 0;
    };

    namespace Enhanced
    {
        struct EnhancedBarrierAnalyzer final : Object<EnhancedBarrierAnalyzer, ID3d12BarrierAnalyzer>
        {
            Rc<D3d12GpuDevice> m_device;

            explicit EnhancedBarrierAnalyzer(const Rc<D3d12GpuDevice>& device);

            Rc<ID3d12BarrierRecorder> CreateRecorder(const Rc<ID3d12RecordStorage>& storage) override;
            Rc<ID3d12BarrierCombiner> CreateCombiner() override;
        };

        struct EnhancedBarrierRecorder final : Object<EnhancedBarrierRecorder, ID3d12BarrierRecorder>
        {
            enum class InfoState : u8
            {
                Unused,
                Input,
                Used,
            };

            struct ResInfo
            {
                ResState State{};
                // 在使用前的，可以插入屏障的 list 的 index
                u32 CurrentBeforeUseListIndex{};
                u32 CurrentBeginBarrierGroupIndex{};
                u32 CurrentEndBarrierGroupIndex{};
                const FCmdResType ResType{};
                InfoState InfoState{};

                explicit ResInfo(FCmdResType ResType);
            };

            struct BarrierGroupRange
            {
                u32 BufferIndex{};
                u32 BufferCount{};
                u32 TextureIndex{};
                u32 TextureCount{};
            };

            Rc<D3d12GpuDevice> m_device{};
            Rc<ID3d12RecordStorage> m_storage{};
            std::vector<ResInfo> m_resources{};
            std::vector<InputResState> m_inputs{};
            std::vector<BarrierGroupRange> m_groups{};
            std::vector<D3D12_BUFFER_BARRIER> m_begin_buffer_barriers{};
            std::vector<D3D12_TEXTURE_BARRIER> m_begin_texture_barriers{};
            std::vector<D3D12_BUFFER_BARRIER> m_end_buffer_barriers{};
            std::vector<D3D12_TEXTURE_BARRIER> m_end_texture_barriers{};
            u32 m_current_begin_barrier_group_index{};
            u32 m_current_end_barrier_group_index{};

            explicit EnhancedBarrierRecorder(const Rc<D3d12GpuDevice>& device, const Rc<ID3d12RecordStorage>& storage);

            std::span<const InputResState> Inputs() const override;

            void Clear() override;
            void StartRecord(std::span<FCmdRes> resources) override;
            void EndRecord() override;
            void OnUse(FCmdResRef ResRef, ResAccess Access, ResUsage Usage, ResLayout Layout) override;
        };

        struct EnhancedBarrierCombiner final : Object<EnhancedBarrierCombiner, AD3d12BarrierCombiner>
        {
            explicit EnhancedBarrierCombiner(const Rc<D3d12GpuDevice>& device);

            std::vector<D3D12_BUFFER_BARRIER> m_buffer_barriers{};
            std::vector<D3D12_TEXTURE_BARRIER> m_texture_barriers{};

            void EndSubmit() override;

            void Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records) override;
        };
    }
}
