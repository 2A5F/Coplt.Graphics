#pragma once

#include <variant>

#include "Binding.h"
#include "../../Api/FFI/States.h"
#include "../../Api/FFI/Command.h"

#include "../Include/Utils.h"
#include "../Include/States.h"

#include "CmdListPack.h"
#include "DescriptorManager.h"
#include "../../Api/Include/ChunkedVector.h"
#include "../../Api/Include/ObjectKey.h"
#include "../../Api/Include/Ptr.h"

namespace Coplt
{
    struct FD3d12GraphicsShaderPipeline;
    struct FD3d12ShaderLayout;
    struct FD3d12PipelineState;
    struct D3d12GpuQueue;
    struct DescriptorContext;
    struct DescriptorAllocator;

    struct D3d12CommandInterpreter final
    {
        NonNull<D3d12GpuQueue> m_queue;

    private:
        struct BarrierContext
        {
            D3D12_BARRIER_GROUP m_barrier_groups[3]{};
            i32 m_groups_count = 0;
            std::vector<D3D12_GLOBAL_BARRIER> m_global_barriers{};
            std::vector<D3D12_TEXTURE_BARRIER> m_texture_barriers{};
            std::vector<D3D12_BUFFER_BARRIER> m_buffer_barriers{};

            void Reset();

            void Add(const D3D12_GLOBAL_BARRIER& barrier);
            void Add(const D3D12_TEXTURE_BARRIER& barrier);
            void Add(const D3D12_BUFFER_BARRIER& barrier);

            void BuildBarriers();
        };

        struct Context
        {
            Ptr<FD3d12PipelineState> Pipeline{};
            Ptr<FD3d12ShaderLayout> Layout{};
            // 如果不是图形管线将不会设置
            Ptr<FD3d12GraphicsShaderPipeline> GPipeline{};

            Ptr<ID3d12ShaderBinding> Binding{};

            bool PipelineChanged{};
            bool BindingChanged{};

            void Reset();
        };

        struct BindingContext
        {
            u32 Index{COPLT_U32_MAX};
            u32 Count{};

            operator bool() const { return Index != COPLT_U32_MAX; }
        };

        struct BindingItem
        {
            u32 c{};
            u32 g{};
            DescriptorAllocation al{};
        };

        BarrierContext m_barrier_context{};
        Context m_context{};
        Ptr<DescriptorContext> m_descriptors;
        std::vector<BindingContext> m_bindings_contexts{};
        std::vector<BindingItem> m_bindings_items{};

    public:
        explicit D3d12CommandInterpreter(const NonNull<D3d12GpuQueue>& queue);

        void Interpret(const FCommandSubmit& submit);

    private:
        void Reset();

        void Analyze(const FCommandSubmit& submit);

        void Bind(const FCommandSubmit& submit, u32 i, const FCommandBind& cmd) const;
        void AnalyzeRender(const FCommandSubmit& submit, u32 i, const FCommandRender& cmd);
        void AnalyzeCompute(const FCommandSubmit& submit, u32 i, const FCommandCompute& cmd);
        void AnalyzeSyncBinding(const FCommandSubmit& submit, u32 i, const FCommandSyncBinding& cmd);
        void AllocBindings(NonNull<ID3d12ShaderBinding> binding);
        DescriptorAllocation AllocBindingGroup(
            const ID3d12ShaderLayout::TableGroup& groups,
            NonNull<DescriptorAllocator> da,
            const Rc<DescriptorHeap>& heap
        );

        void Translate(const FCommandSubmit& submit);

        void Label(const FCommandSubmit& submit, const FCommandLabel& cmd) const;
        void BeginScope(const FCommandSubmit& submit, const FCommandBeginScope& cmd) const;
        void EndScope(const FCommandSubmit& submit, const FCommandEndScope& cmd) const;
        void Barrier(const FCommandSubmit& submit, u32 i, const FCommandBarrier& cmd);
        void Barrier(const FCommandSubmit& submit, const FGlobalBarrier& item);
        void Barrier(const FCommandSubmit& submit, const FBufferBarrier& item);
        void Barrier(const FCommandSubmit& submit, const FImageBarrier& item);
        void ClearColor(const FCommandSubmit& submit, u32 i, const FCommandClearColor& cmd) const;
        void ClearDepthStencil(const FCommandSubmit& submit, u32 i, const FCommandClearDepthStencil& cmd) const;
        void BufferCopy(const FCommandSubmit& submit, u32 i, const FCommandBufferCopy& cmd) const;
        void BufferImageCopy(const FCommandSubmit& submit, u32 i, const FCommandBufferImageCopy& cmd) const;
        void Render(const FCommandSubmit& submit, u32 i, const FCommandRender& cmd);
        void RenderDraw(const FCommandSubmit& submit, u32 i, const FCommandDraw& cmd) const;
        void RenderDispatch(const FCommandSubmit& submit, u32 i, const FCommandDispatch& cmd) const;
        void RenderSetViewportScissor(const FCommandSubmit& submit, u32 i, const FCommandSetViewportScissor& cmd) const;
        void RenderSetMeshBuffers(const FCommandSubmit& submit, u32 i, const FCommandSetMeshBuffers& cmd) const;
        void Compute(const FCommandSubmit& submit, u32 i, const FCommandCompute& cmd);
        void ComputeDispatch(const FCommandSubmit& submit, u32 i, const FCommandDispatch& cmd) const;

        void SetPipelineContext(NonNull<FShaderPipeline> pipeline, u32 i);
        void SetPipeline(const CmdListPack& cmd_pack, NonNull<FShaderPipeline> pipeline, u32 i);

        void SetBindingContext(NonNull<FShaderBinding> binding, u32 i);

        void SyncBinding(const FCommandSubmit& submit, u32 i, const FCommandSyncBinding& cmd);
        void UseBinding(NonNull<ID3d12ShaderBinding> binding, std::span<BindingItem> items);

        static NonNull<ID3D12Resource> GetResource(const FResourceMeta& meta);
        static NonNull<ID3D12Resource> GetResource(NonNull<FUnknown> object, FResourceRefType type);
        static D3D12_CPU_DESCRIPTOR_HANDLE GetRtv(const FResourceMeta& meta);
        static D3D12_CPU_DESCRIPTOR_HANDLE GetDsv(const FResourceMeta& meta);
    };
}
