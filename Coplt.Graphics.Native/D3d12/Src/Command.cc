#include "Command.h"
#include "../../Api/FFI/Command.h"

#include "fmt/format.h"
#include "fmt/xchar.h"

#include "Queue.h"
#include "../FFI/Output.h"

using namespace Coplt;

D3d12CommandInterpreter::D3d12CommandInterpreter(D3d12GpuQueue* queue) : m_queue(queue)
{
}

D3d12CommandInterpreter::StatePair::StatePair(
    FResourceState StartState, FResourceState FinalState, FResourceRefType Type
) : StartState(StartState), FinalState(FinalState), Type(Type)
{
}

D3d12CommandInterpreter::InterpreterContext::InterpreterContext(const FCommandSubmit& submit) : m_submit(&submit)
{
    m_items.reserve(submit.Count + (submit.Count >> 1));
    m_current_barrier = m_items.size();
    m_items.push_back(std::make_unique<CommandBarrierPart>());
    m_current_command = m_items.size();
    m_items.push_back(CommandItemPart{.index = 0, .count = 0});
}

D3d12CommandInterpreter::CommandBarrierPart& D3d12CommandInterpreter::InterpreterContext::GetCurrentBarrier() const
{
    return *std::get<std::unique_ptr<CommandBarrierPart>>(m_items[m_current_barrier]);
}

D3d12CommandInterpreter::CommandBarrierPart& D3d12CommandInterpreter::InterpreterContext::AddBarrier()
{
    m_current_barrier = m_items.size();
    m_items.push_back(std::make_unique<CommandBarrierPart>());

    const auto [index, count] = GetCurrentCommand();
    m_current_command = m_items.size();
    m_items.push_back(CommandItemPart{.index = index + count, .count = 0});

    return GetCurrentBarrier();
}

D3d12CommandInterpreter::CommandItemPart& D3d12CommandInterpreter::InterpreterContext::GetCurrentCommand()
{
    return std::get<CommandItemPart>(m_items[m_current_command]);
}

void D3d12CommandInterpreter::InterpreterContext::ReqState(FResourceSrc res_src, const FResourceState req_state)
{
    auto& res = m_submit->Resources[res_src.ResourceIndex];
    const auto res_obj = res.GetObjectPtr();
    if (res_obj == nullptr) throw WRuntimeException(fmt::format(L"Resource is null at {}", res_src.ResourceIndex));
    const auto new_state = ChangeState(res.CurrentState, req_state);
    if (new_state == res.CurrentState) return;
    auto barrier = &GetCurrentBarrier();
    if (const auto state_pair = barrier->m_states.try_get(res_obj))
    {
        if (IsCompatible(state_pair->FinalState, new_state))
        {
            state_pair->FinalState |= new_state;
            res.CurrentState = state_pair->FinalState;
        }
        else
        {
            barrier = &AddBarrier();
            barrier->m_states.do_insert(
                res_obj, StatePair(res.CurrentState, new_state, res.Type)
            );
            res.CurrentState = new_state;
        }
    }
    else
    {
        barrier->m_states.do_insert(
            res_obj, StatePair(res.CurrentState, new_state, res.Type)
        );
        res.CurrentState = new_state;
    }
}

void D3d12CommandInterpreter::InterpreterContext::CmdNext()
{
    GetCurrentCommand().count++;
}

void D3d12CommandInterpreter::Interpret(const FCommandSubmit& submit) const
{
    InterpreterContext context(submit);

    CollectBarrier(context, submit);
    Interpret(context, submit);
}

void D3d12CommandInterpreter::CollectBarrier(InterpreterContext& context, const FCommandSubmit& submit)
{
    for (u32 i = 0; i < submit.Count; ++i)
    {
        const auto& item = submit.Items[i];

        switch (item.Type)
        {
        case FCommandType::None:
            continue;
        case FCommandType::Transition:
            goto Transition;
        case FCommandType::Present:
            goto Present;
        case FCommandType::ClearColor:
            goto ClearColor;
        }
        throw WRuntimeException(fmt::format(L"Unknown command type {}", static_cast<u32>(item.Type)));

    Transition:
        {
            context.CmdNext();
            continue;
        }
    Present:
        {
            const auto& cmd = item.Present;
            context.ReqState(cmd.Image, FResourceState::Present);
            context.CmdNext();
            continue;
        }
    ClearColor:
        {
            const auto& cmd = item.ClearColor;
            context.ReqState(cmd.Image, FResourceState::RenderTarget);
            context.CmdNext();
            continue;
        }
    }
}

void D3d12CommandInterpreter::Interpret(InterpreterContext& context, const FCommandSubmit& submit) const
{
    const auto cmd_list = m_queue->m_command_list.Get();
    for (const auto& cmd_item : context.m_items)
    {
        if (std::holds_alternative<std::unique_ptr<CommandBarrierPart>>(cmd_item))
        {
            const auto& barrier_map = std::get<std::unique_ptr<CommandBarrierPart>>(cmd_item)->m_states;

            std::vector<D3D12_RESOURCE_BARRIER> barriers{};
            barriers.reserve(barrier_map.size());

            for (const auto& [obj, pair] : barrier_map)
            {
                if (pair.StartState == pair.FinalState) continue;
                D3D12_RESOURCE_BARRIER barrier{};
                barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                barrier.Transition.pResource = GetResource(obj, pair.Type);
                barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                barrier.Transition.StateBefore = ToDx(pair.StartState);
                barrier.Transition.StateAfter = ToDx(pair.FinalState);
                barriers.push_back(barrier);
            }

            if (!barriers.empty())
            {
                cmd_list->ResourceBarrier(barriers.size(), barriers.data());
            }
            continue;
        }

        const auto [cmd_index, cmd_count] = std::get<CommandItemPart>(cmd_item);
        for (u32 i = 0; i < cmd_count; ++i)
        {
            const auto& item = submit.Items[cmd_index + i];

            switch (item.Type)
            {
            case FCommandType::None:
            case FCommandType::Present:
                continue;
            case FCommandType::Transition:
                goto Transition;
            case FCommandType::ClearColor:
                goto ClearColor;
            }
            throw WRuntimeException(fmt::format(L"Unknown command type {}", static_cast<u32>(item.Type)));

        Transition:
            throw WRuntimeException(L"TODO");
            continue;
        ClearColor:
            const auto& cmd = item.ClearColor;
            const auto rtv = GetRtv(cmd.Image.Get(submit));
            cmd_list->ClearRenderTargetView(
                rtv, cmd.Color, cmd.RectCount,
                cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.RectIndex])
            );
            continue;
        }
    }
}

ID3D12Resource* D3d12CommandInterpreter::GetResource(const FResourceMeta& meta)
{
    switch (meta.Type)
    {
    case FResourceRefType::Texture:
        throw WRuntimeException(L"TODO");
    case FResourceRefType::Output:
        {
            const auto output = meta.Output->QueryInterface<FD3d12GpuOutput>();
            if (!output) throw WRuntimeException(L"The memory may be corrupted");
            ID3D12Resource* ptr{};
            output->GetCurrentResourcePtr(&ptr).TryThrow();
            return ptr;
        }
    default:
        throw WRuntimeException(fmt::format(L"Unknown resource ref type {}", static_cast<u8>(meta.Type)));
    }
}

ID3D12Resource* D3d12CommandInterpreter::GetResource(FUnknown* object, FResourceRefType type)
{
    switch (type)
    {
    case FResourceRefType::Texture:
        throw WRuntimeException(L"TODO");
    case FResourceRefType::Output:
        {
            const auto output = object->QueryInterface<FD3d12GpuOutput>();
            if (!output) throw WRuntimeException(L"The memory may be corrupted");
            ID3D12Resource* ptr{};
            output->GetCurrentResourcePtr(&ptr).TryThrow();
            return ptr;
        }
    default:
        throw WRuntimeException(fmt::format(L"Unknown resource ref type {}", static_cast<u8>(type)));
    }
}

D3D12_CPU_DESCRIPTOR_HANDLE D3d12CommandInterpreter::GetRtv(const FResourceMeta& meta)
{
    switch (meta.Type)
    {
    case FResourceRefType::Texture:
        throw WRuntimeException(L"TODO");
    case FResourceRefType::Output:
        {
            const auto output = meta.Output->QueryInterface<FD3d12GpuOutput>();
            if (!output) throw WRuntimeException(L"The memory may be corrupted");
            D3D12_CPU_DESCRIPTOR_HANDLE rtv{};
            output->GetCurrentRtv(&rtv).TryThrow();
            return rtv;
        }
    default:
        throw WRuntimeException(fmt::format(L"Unknown resource ref type {}", static_cast<u8>(meta.Type)));
    }
}

D3D12_RESOURCE_STATES D3d12CommandInterpreter::ToDx(const FResourceState state)
{
    switch (state)
    {
    case FResourceState::Common:
    case FResourceState::Present:
        return D3D12_RESOURCE_STATE_COMMON;
    case FResourceState::RenderTarget:
        return D3D12_RESOURCE_STATE_RENDER_TARGET;
    case FResourceState::DepthWrite:
        return D3D12_RESOURCE_STATE_DEPTH_WRITE;
    case FResourceState::UnorderedAccess:
        return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    case FResourceState::CopyDst:
        return D3D12_RESOURCE_STATE_COPY_DEST;
    case FResourceState::ResolveDst:
        return D3D12_RESOURCE_STATE_RESOLVE_DEST;
    case FResourceState::RayTracing:
        return D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
    case FResourceState::ShadingRate:
        return D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;
    case FResourceState::StreamOutput:
        return D3D12_RESOURCE_STATE_STREAM_OUT;
    default:
        break;
    }
    D3D12_RESOURCE_STATES r = D3D12_RESOURCE_STATE_COMMON;
    if (HasAnyFlags(state, FResourceState::ConstantBuffer | FResourceState::VertexBuffer))
        r |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    if (HasFlags(state, FResourceState::IndexBuffer)) r |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
    if (HasFlags(state, FResourceState::IndirectBuffer)) r |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
    if (HasFlags(state, FResourceState::DepthRead)) r |= D3D12_RESOURCE_STATE_DEPTH_READ;
    if (HasFlags(state, FResourceState::ShaderResource)) r |= D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE;
    if (HasFlags(state, FResourceState::CopySrc)) r |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if (HasFlags(state, FResourceState::ResolveSrc)) r |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
    return r;
}
