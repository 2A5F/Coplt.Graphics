#include "Command.h"
#include "../../Api/FFI/Command.h"

using namespace Coplt;

void D3d12CommandInterpreter::Interpret(ID3D12GraphicsCommandList* command_list, const FCommandSubmit& submit)
{
    std::vector<CommandItem> commands{};

    for (u32 i = 0; i < submit.Count; ++i)
    {
        const auto& item = submit.Items[i];
    }
    // todo
}
