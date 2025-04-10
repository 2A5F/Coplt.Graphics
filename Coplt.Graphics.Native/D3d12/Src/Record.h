#pragma once

#include <vector>
#include <directx/d3dx12.h>

#include "../../Api/Include/GpuObject.h"
#include "../../Api/FFI/Record.h"
#include "Barrier.h"
#include "Binding.h"
#include "Buffer.h"
#include "Context.h"
#include "Image.h"
#include "Isolate.h"
#include "Layout.h"
#include "Output.h"

namespace Coplt
{
    struct ResourceInfo
    {
        Rc<FGpuObject> Resource{};

        union
        {
            ID3d12GpuOutput2* Output;
            ID3d12GpuBuffer* Buffer;
            ID3d12GpuImage* Image;
        };

        FCmdResType Type{};
        u32 Index{};

        ResourceInfo() = default;
        explicit ResourceInfo(Rc<FGpuObject>&& resource, const FCmdRes& res, u32 index);
        explicit ResourceInfo(Rc<FGpuObject>&& resource, const View& view, u32 index);

        bool IsImage() const;

        NonNull<ID3D12Resource> GetResource() const;
        NonNull<FGpuBufferData> GetBufferData() const;
        NonNull<FGpuImageData> GetImageData() const;
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv() const;
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsv() const;
        NonNull<LayoutState> GetState() const;
    };

    COPLT_INTERFACE_DEFINE(ID3d12GpuRecord, "57a9c7f9-1ec0-4d78-89b9-e547667c50b3", FGpuRecord)
    {
        virtual FGpuRecordData* Data() noexcept = 0;
        virtual const FGpuRecordData* Data() const noexcept = 0;

        virtual std::span<ResourceInfo> ResourceInfos() noexcept = 0;

        virtual const Rc<D3d12RecordContext>& Context() const noexcept = 0;

        // 可空
        virtual const D3d12RentedCommandList& ResultList() const noexcept = 0;

        virtual void RegisterWaitPoint(QueueWaitPoint&& wait_point) = 0;
        virtual void WaitAndRecycle(HANDLE event) = 0;
        virtual void Recycle() = 0;

        virtual void EnsureEnd() = 0;

        virtual const Rc<ID3d12BarrierAnalyzer>& BarrierAnalyzer() = 0;

        virtual void BeforeInterpret(const D3d12RentedCommandList& list) = 0;
        virtual void Interpret(const D3d12RentedCommandList& list, u32 offset, u32 count) = 0;
        virtual void AfterInterpret(const D3d12RentedCommandList& list) = 0;

        virtual void AfterSubmit() = 0;
    };

    struct ID3d12ShaderPipeline;
    struct ID3d12GraphicsShaderPipeline;

    struct D3d12GpuRecord final : GpuObject<D3d12GpuRecord, ID3d12GpuRecord>, FGpuRecordData
    {
        using CmdList = D3d12RentedCommandList;

        enum class RecordState : u8
        {
            Main,
            Render,
            Compute,
        };

        struct RenderState
        {
            u32 StartIndex{};
            FCmdRender Cmd{};
        };

        struct ComputeState
        {
            u32 StartIndex{};
            FCmdCompute Cmd{};
        };

        struct PipelineContext
        {
            Ptr<ID3d12ShaderPipeline> Pipeline{};
            Ptr<ID3d12BindingLayout> Layout{};
            // 如果不是图形管线将不会设置
            Ptr<ID3d12GraphicsShaderPipeline> GPipeline{};

            Ptr<ID3d12ShaderBinding> Binding{};
            u32 SetBindingIndex{};

            bool PipelineChanged{};
            bool BindingChanged{};

            void Reset();
            void SetPipeline(NonNull<FShaderPipeline> pipeline, u32 i);
            void SetBinding(NonNull<FShaderBinding> binding, u32 i, const FCmdSetBinding& cmd);
        };

        struct BindingInfo
        {
            u64 BindingVersion{};
            u32 AllocationIndex{COPLT_U32_MAX};
            u32 BindItemIndex{COPLT_U32_MAX};
        };

        struct SetBindingInfo
        {
            ID3d12BindingLayout* Layout{};
            u32 AllocationIndex{COPLT_U32_MAX};
            u32 BindItemIndex{COPLT_U32_MAX};
        };

        struct BindItem
        {
            union
            {
                // todo 直接资源和常量
            };
        };

        u64 m_isolate_id{};
        u64 m_record_id{};
        SRc<FGpuIsolateConfig> m_isolate_config{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12RecordContext> m_context{};
        Rc<ID3d12BarrierAnalyzer> m_barrier_analyzer{};
        HashMap<u64, u64> m_resource_map{}; // id -> index
        std::vector<ResourceInfo> m_resource_infos{};
        std::vector<BindingInfo> m_binding_infos{};
        std::vector<SetBindingInfo> m_set_binding_infos{};
        std::vector<DescriptorAllocation> m_allocations{};
        std::vector<BindItem> m_bind_items{};
        std::vector<QueueWaitPoint> m_queue_wait_points{};
        std::vector<ReadGuard> m_tmp_locks{};
        RenderState m_cur_render{};
        ComputeState m_cur_compute{};
        PipelineContext m_pipeline_context{};
        RecordState m_state{};
        D3d12RentedCommandList m_result_list{};

        explicit D3d12GpuRecord(NonNull<D3d12GpuIsolate> isolate);

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuRecordData* GpuFGpuRecordData() noexcept override;
        FGpuRecordData* Data() noexcept override;
        const FGpuRecordData* Data() const noexcept override;
        std::span<ResourceInfo> ResourceInfos() noexcept override;
        const Rc<D3d12RecordContext>& Context() const noexcept override;
        const D3d12RentedCommandList& ResultList() const noexcept override;

        const Rc<ID3d12BarrierAnalyzer>& BarrierAnalyzer() override;

        void RegisterWaitPoint(QueueWaitPoint&& wait_point) override;
        void WaitAndRecycle(HANDLE event) override;
        void Recycle() override;

        FResult End() noexcept override;
        void EnsureEnd() override;
        void DoEnd();

        void AfterSubmit() override;

        ResourceInfo& GetRes(const FCmdResRef& ref);
        const ResourceInfo& GetRes(const FCmdResRef& ref) const;

        void ResetState();

        FResIndex AddResource(const FCmdRes& res);
        FResIndex AddResource(const View& view);
        void ReadyResource();
        // 返回帧所需的最小描述符堆大小
        void ReadyBindings(u32& MinResHeapSize, u32& MinSmpHeapSize);

        void Analyze();
        void Analyze_PreparePresent(u32 i, const FCmdPreparePresent& cmd) const;
        void Analyze_ClearColor(u32 i, const FCmdClearColor& cmd) const;
        void Analyze_ClearDepthStencil(u32 i, const FCmdClearDepthStencil& cmd) const;
        void Analyze_BufferCopy(u32 i, const FCmdBufferCopy& cmd) const;
        void Analyze_Render(u32 i, const FCmdRender& cmd);
        void Analyze_RenderEnd(u32 i, const FCmdRender& cmd);
        void Analyze_Compute(u32 i, const FCmdCompute& cmd);
        void Analyze_ComputeEnd(u32 i, const FCmdCompute& cmd);
        void Analyze_SetPipeline(u32 i, const FCmdSetPipeline& cmd);
        void Analyze_SetBinding(u32 i, const FCmdSetBinding& cmd);
        void Analyze_SetMeshBuffers(u32 i, const FCmdSetMeshBuffers& cmd);
        void Analyze_Dispatch(u32 i, const FCmdDispatch& cmd) const;

        void BeforeInterpret(const D3d12RentedCommandList& list) override;
        void AfterInterpret(const D3d12RentedCommandList& list) override;
        void Interpret(const D3d12RentedCommandList& list, u32 offset, u32 count) override;
        void Interpret_Label(const CmdList& list, u32 i, const FCmdLabel& cmd) const;
        void Interpret_BeginScope(const CmdList& list, u32 i, const FCmdBeginScope& cmd) const;
        void Interpret_EndScope(const CmdList& list, u32 i, const FCmdEndScope& cmd) const;
        void Interpret_ClearColor(const CmdList& list, u32 i, const FCmdClearColor& cmd);
        void Interpret_ClearDepthStencil(const CmdList& list, u32 i, const FCmdClearDepthStencil& cmd);
        void Interpret_BufferCopy(const CmdList& list, u32 i, const FCmdBufferCopy& cmd);
        void Interpret_Render(const CmdList& list, u32 i, const FCmdRender& cmd);
        void Interpret_RenderEnd(const CmdList& list, u32 i, const FCmdRender& cmd);
        void Interpret_Compute(const CmdList& list, u32 i, const FCmdCompute& cmd);
        void Interpret_ComputeEnd(const CmdList& list, u32 i, const FCmdCompute& cmd);
        void Interpret_SetPipeline(const CmdList& list, u32 i, const FCmdSetPipeline& cmd);
        void Interpret_SetBinding(const CmdList& list, u32 i, const FCmdSetBinding& cmd);
        void Interpret_SetViewportScissor(const CmdList& list, u32 i, const FCmdSetViewportScissor& cmd) const;
        void Interpret_SetMeshBuffers(const CmdList& list, u32 i, const FCmdSetMeshBuffers& cmd) const;
        void Interpret_Draw(const CmdList& list, u32 i, const FCmdDraw& cmd) const;
        void Interpret_Dispatch(const CmdList& list, u32 i, const FCmdDispatch& cmd) const;

        void SetPipeline(NonNull<FShaderPipeline> pipeline, u32 i);
        // 返回是否改变
        bool SetBinding(NonNull<FShaderBinding> binding, u32 i, const FCmdSetBinding& cmd);
        void SetPipeline(const CmdList& list, NonNull<FShaderPipeline> pipeline, u32 i);
    };

    D3D12_COMMAND_LIST_TYPE GetType(FGpuRecordMode Mode);
}
