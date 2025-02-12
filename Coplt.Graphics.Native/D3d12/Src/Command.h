#pragma once

#include <variant>

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

        explicit D3d12CommandInterpreter(D3d12GpuQueue* queue);

        struct StatePair
        {
            FResourceState StartState{};
            FResourceState FinalState{};
            FResourceRefType Type{};

            explicit StatePair() = default;

            explicit StatePair(FResourceState StartState, FResourceState FinalState, FResourceRefType Type);
        };

        struct CommandBarrierPart
        {
            HashMap<FUnknown*, StatePair> m_states{};
        };

        struct CommandItemPart
        {
            u32 index{};
            u32 count{};
        };

        using CommandItem = std::variant<CommandItemPart, std::unique_ptr<CommandBarrierPart>>;

        struct InterpreterContext
        {
            const FCommandSubmit* m_submit;
            std::vector<CommandItem> m_items{};
            usize m_current_barrier{0};
            usize m_current_command{0};

            explicit InterpreterContext(const FCommandSubmit& submit);

            CommandBarrierPart& GetCurrentBarrier() const;
            CommandBarrierPart& AddBarrier();

            CommandItemPart& GetCurrentCommand();

            void ReqState(FResourceSrc res_src, FResourceState req_state);
            void CmdNext();
        };

        void Interpret(const FCommandSubmit& submit) const;

    private:
        static void CollectBarrier(InterpreterContext& context, const FCommandSubmit& submit);

        void Interpret(InterpreterContext& context, const FCommandSubmit& submit) const;

        static ID3D12Resource* GetResource(const FResourceMeta& meta);

        static ID3D12Resource* GetResource(FUnknown* object, FResourceRefType type);

        static D3D12_CPU_DESCRIPTOR_HANDLE GetRtv(const FResourceMeta& meta);

        static D3D12_RESOURCE_STATES ToDx(FResourceState state);
    };
}
