#pragma once

#include <variant>

#include "../../Api/FFI/States.h"
#include "../../Api/FFI/Command.h"

#include "../Include/Utils.h"
#include "../Include/States.h"

#include "CmdListPack.h"

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
        struct ResState
        {
            ID3D12Resource* Resource{};
            D3D12_RESOURCE_STATES LastState{};
            D3D12_RESOURCE_STATES CurrentState{};
            u32 CurrentBarrierIndex{COPLT_U32_MAX};
            // 记录最后使用的项块
            u32 CurrentItemsSize{COPLT_U32_MAX};
            FResourceRefType Type{};

            bool HasBarrier() const { return CurrentBarrierIndex != COPLT_U32_MAX; }
        };

        enum class ItemType
        {
            Command,
            Barrier,
        };

        struct Item
        {
            u32 CommandIndex{};
            u32 CommandCount{};
            u32 BarrierIndex{};
            u32 BarrierCount{};
        };

        struct Context
        {
            // 以下在屏障阶段和转译阶段都会使用

            FShaderPipeline* Pipeline{};
            FD3d12PipelineState* D3dPipeline{};
            FD3d12ShaderLayout* Layout{};
            // 如果不是图形管线将不会设置
            FD3d12GraphicsShaderPipeline* GPipeline{};

            // 以下仅在屏障阶段使用

            FResourceRef Rtv[8];
            u32 NumRtv{};
            FResourceRef Dsv{};
            FResourceRef Ibv{};
            FResourceRef Vbv[31];
            u32 NumVbv{};

            void ResetPipeline();
            void Reset();
        };

        std::vector<ResState> m_states{};
        std::vector<D3D12_RESOURCE_BARRIER> m_barriers{};
        std::vector<u32> m_barrier_resources{};
        std::vector<Item> m_items{};
        u32 m_last_barrier_index{};
        Context m_context{};

    public:
        explicit D3d12CommandInterpreter(D3d12GpuQueue* queue);

        void Interpret(const FCommandSubmit& submit);

        void CmdNext();
        void ReqState(FResourceRef ResSrc, FResourceState ReqState);
        void MarkResUse(FResourceRef ResSrc);
        void AddBarrier(ResState& state, FResourceRef ResSrc);
        void BarNext();

    private:
        void Init();
        void Reset();

        void InitStates(const FCommandSubmit& submit);

        void CollectBarrier(const FCommandSubmit& submit);

        void Translate(const FCommandSubmit& submit);

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
