#pragma once

#include <variant>

#include "CmdListPack.h"
#include "../../Api/Include/HashMap.h"

#include "../Include/Utils.h"
#include "../../Api/FFI/States.h"
#include "../../Api/FFI/Command.h"

namespace Coplt
{
    struct D3d12GpuQueue;

    struct D3d12CommandInterpreter final
    {
        D3d12GpuQueue* m_queue{};

    private:
        struct ResState
        {
            ID3D12Resource* Resource{};
            FResourceState LastState{};
            FResourceState CurrentState{};
            u32 CurrentBarrierIndex{COPLT_U32_MAX};
            FResourceRefType Type{};
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

        FShaderPipeline* m_current_pipeline{};
        std::vector<ResState> m_states{};
        std::vector<D3D12_RESOURCE_BARRIER> m_barriers{};
        std::vector<u32> m_barrier_resources{};
        std::vector<Item> m_items{};
        u32 m_last_barrier_index{};

    public:
        explicit D3d12CommandInterpreter(D3d12GpuQueue* queue);

        void Interpret(const FCommandSubmit& submit);

        void CmdNext();
        void ReqState(u32 CmdIndex, FResourceRef ResSrc, FResourceState ReqState);
        void AddBarrier(ResState& state, const FResourceRef ResSrc);
        void BarNext();

    private:
        void Init();
        void Reset();

        void InitStates(const FCommandSubmit& submit);

        void CollectBarrier(const FCommandSubmit& submit);

        void Translate(const FCommandSubmit& submit);

        void SetPipeline(
            const CmdListPack& cmd_pack, FShaderPipeline* pipeline, u32 i
        );

        static ID3D12Resource* GetResource(const FResourceMeta& meta);

        static ID3D12Resource* GetResource(FUnknown* object, FResourceRefType type);

        static D3D12_CPU_DESCRIPTOR_HANDLE GetRtv(const FResourceMeta& meta);

        static D3D12_RESOURCE_STATES ToDx(FResourceState state);
    };
}
