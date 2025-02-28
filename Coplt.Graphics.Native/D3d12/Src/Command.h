#pragma once

#include <variant>

#include "../../Api/FFI/States.h"
#include "../../Api/FFI/Command.h"

#include "../Include/Utils.h"
#include "../Include/States.h"

#include "CmdListPack.h"
#include "../../Api/Include/ChunkedVector.h"

namespace Coplt
{
    struct FD3d12GraphicsShaderPipeline;
    struct FD3d12ShaderLayout;
    struct FD3d12PipelineState;
    struct D3d12GpuQueue;

    struct D3d12CommandInterpreter final
    {
        D3d12GpuQueue* m_queue{};

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
            FShaderPipeline* Pipeline{};
            FD3d12PipelineState* D3dPipeline{};
            FD3d12ShaderLayout* Layout{};
            // 如果不是图形管线将不会设置
            FD3d12GraphicsShaderPipeline* GPipeline{};

            void Reset();
        };

        // std::vector<ResState> m_states{};
        // std::vector<D3D12_RESOURCE_BARRIER> m_barriers{};
        // std::vector<u32> m_barrier_resources{};
        // std::vector<Item> m_items{};
        // u32 m_last_barrier_index{};
        BarrierContext m_barrier_context{};
        Context m_context{};

    public:
        explicit D3d12CommandInterpreter(D3d12GpuQueue* queue);

        void Interpret(const FCommandSubmit& submit);

    private:
        void Reset();

        void Translate(const FCommandSubmit& submit);

        void Barrier(const FCommandSubmit& submit, u32 i, const FCommandBarrier& cmd);
        void Barrier(const FCommandSubmit& submit, const FGlobalBarrier& item);
        void Barrier(const FCommandSubmit& submit, const FBufferBarrier& item);
        void Barrier(const FCommandSubmit& submit, const FImageBarrier& item);
        void ClearColor(const FCommandSubmit& submit, u32 i, const FCommandClearColor& cmd) const;
        void ClearDepthStencil(const FCommandSubmit& submit, u32 i, const FCommandClearDepthStencil& cmd) const;
        void BufferCopy(const FCommandSubmit& submit, u32 i, const FCommandBufferCopy& cmd) const;

        void SetPipelineContext(
            FShaderPipeline* pipeline, u32 i
        );
        void SetPipeline(
            const CmdListPack& cmd_pack, FShaderPipeline* pipeline, u32 i
        );

        static ID3D12Resource* GetResource(const FResourceMeta& meta);
        static ID3D12Resource* GetResource(FUnknown* object, FResourceRefType type);
        static D3D12_CPU_DESCRIPTOR_HANDLE GetRtv(const FResourceMeta& meta);
    };
}
