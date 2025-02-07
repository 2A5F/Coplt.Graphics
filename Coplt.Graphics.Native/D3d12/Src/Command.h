#pragma once

#include "../Include/Utils.h"

#include <variant>

namespace Coplt
{
    struct FCommandSubmit;

    struct D3d12CommandInterpreter
    {
        void Interpret(
            ID3D12GraphicsCommandList* command_list,
            const FCommandSubmit& submit
        );
    };

    struct CommandBarrierPart
    {
        // todo
    };

    struct CommandItemPart
    {
        u32 index;
    };

    using CommandItem = std::variant<CommandItemPart, CommandBarrierPart>;
}
