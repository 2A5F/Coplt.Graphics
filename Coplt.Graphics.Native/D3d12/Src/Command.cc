#include "Command.h"
#include "../../Api/FFI/Command.h"

#include "fmt/format.h"
#include "fmt/xchar.h"

#include "Queue.h"
#include "../FFI/Layout.h"
#include "../FFI/Output.h"
#include "../FFI/Pipeline.h"
#include "../Include/PipelineState.h"

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
    bool exists{};
    auto state_pair = barrier->m_states.GetOrAdd(res_obj, exists, [&](auto& p)
    {
        p = StatePair(res.CurrentState, new_state, res.Type);
        res.CurrentState = new_state;
    });
    if (exists)
    {
        if (IsCompatible(state_pair.FinalState, new_state))
        {
            state_pair.FinalState |= new_state;
            res.CurrentState = state_pair.FinalState;
        }
        else
        {
            barrier = &AddBarrier();
            barrier->m_states.TryAdd(
                res_obj, StatePair(res.CurrentState, new_state, res.Type)
            );
            res.CurrentState = new_state;
        }
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
    for (u32 i = 0; i < submit.Count; ++i, context.CmdNext())
    {
        const auto& item = submit.Items[i];

        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Transition:
        case FCommandType::SetPipeline:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
            goto CmdNext;
        case FCommandType::Present:
            goto Present;
        case FCommandType::ClearColor:
            goto ClearColor;
        case FCommandType::ClearDepthStencil:
            goto ClearDepthStencil;
        case FCommandType::SetRenderTargets:
            goto SetRenderTargets;
        }
        throw WRuntimeException(fmt::format(L"Unknown command type {}", static_cast<u32>(item.Type)));

    CmdNext:
        {
            continue;
        }
    Present:
        {
            const auto& cmd = item.Present;
            context.ReqState(cmd.Image, FResourceState::Present);
            continue;
        }
    ClearColor:
        {
            const auto& cmd = item.ClearColor;
            context.ReqState(cmd.Image, FResourceState::RenderTarget);
            continue;
        }
    ClearDepthStencil:
        {
            const auto& cmd = item.ClearDepthStencil;
            context.ReqState(cmd.Image, FResourceState::DepthWrite);
            continue;
        }
    SetRenderTargets:
        {
            const auto& cmd = item.SetRenderTargets;
            if (!cmd.Dsv.IsEmpty()) context.ReqState(cmd.Dsv, FResourceState::DepthWrite);
            for (u32 n = 0; n < std::min(cmd.NumRtv, 8u); ++n)
            {
                context.ReqState(cmd.Rtv[n], FResourceState::RenderTarget);
            }
            continue;
        }
    }
}

void SetPipeline(
    D3d12CommandInterpreter::InterpreterContext& context, const CmdListPack& cmd_pack, FShaderPipeline* pipeline, u32 i
)
{
    if (pipeline == context.m_current_pipeline.get()) return;
    context.m_current_pipeline = Rc<FShaderPipeline>::UnsafeClone(pipeline);
    const auto ps = pipeline->QueryInterface<FD3d12PipelineState>();
    if (!ps)
        throw WRuntimeException(
            fmt::format(
                L"Pipeline({:#x}) comes from different backends at cmd {}",
                reinterpret_cast<size_t>(pipeline), i
            )
        );
    const auto stages = pipeline->GetStages();
    const auto layout = pipeline->GetLayout()->QueryInterface<FD3d12ShaderLayout>();
    if (!layout)
        throw WRuntimeException(L"Shader layout from different backends");
    if (HasFlags(stages, FShaderStageFlags::Compute))
    {
        cmd_pack->SetComputeRootSignature(static_cast<ID3D12RootSignature*>(layout->GetRootSignaturePtr()));
    }
    else if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        const auto gp = pipeline->QueryInterface<FD3d12GraphicsShaderPipeline>();
        if (!gp)
            throw WRuntimeException(L"Pipeline from different backends or pipeline not a graphics pipeline");
        const auto& states = gp->GetGraphicsState();
        cmd_pack->IASetPrimitiveTopology(ToDx(states->Topology));
        cmd_pack->SetGraphicsRootSignature(static_cast<ID3D12RootSignature*>(layout->GetRootSignaturePtr()));
    }
    cmd_pack->SetPipelineState(static_cast<ID3D12PipelineState*>(ps->GetPipelineStatePtr()));
}

void D3d12CommandInterpreter::Interpret(InterpreterContext& context, const FCommandSubmit& submit) const
{
    const auto& cmd_pack = m_queue->m_cmd;
    for (const auto& cmd_item : context.m_items)
    {
        if (std::holds_alternative<std::unique_ptr<CommandBarrierPart>>(cmd_item))
        {
            const auto& barrier_map = std::get<std::unique_ptr<CommandBarrierPart>>(cmd_item)->m_states;

            std::vector<D3D12_RESOURCE_BARRIER> barriers{};
            barriers.reserve(barrier_map.Count());

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
                cmd_pack->ResourceBarrier(barriers.size(), barriers.data());
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
            case FCommandType::ClearDepthStencil:
                goto ClearDepthStencil;
            case FCommandType::SetRenderTargets:
                goto SetRenderTargets;
            case FCommandType::SetPipeline:
                goto SetPipeline;
            case FCommandType::Draw:
                goto Draw;
            case FCommandType::Dispatch:
                goto Dispatch;
            }
            throw WRuntimeException(fmt::format(L"Unknown command type {}", static_cast<u32>(item.Type)));

        Transition:
            throw WRuntimeException(L"TODO");
            continue;
        ClearColor:
            {
                const auto& cmd = item.ClearColor;
                const auto rtv = GetRtv(cmd.Image.Get(submit));
                cmd_pack->ClearRenderTargetView(
                    rtv, cmd.Color, cmd.RectCount,
                    cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.RectIndex])
                );
                continue;
            }
        ClearDepthStencil:
            {
                const auto& cmd = item.ClearDepthStencil;
                D3D12_CPU_DESCRIPTOR_HANDLE dsv{}; // todo
                D3D12_CLEAR_FLAGS flags{};
                if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Depth)) flags |= D3D12_CLEAR_FLAG_DEPTH;
                if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Stencil)) flags |= D3D12_CLEAR_FLAG_STENCIL;
                cmd_pack->ClearDepthStencilView(
                    dsv, flags, cmd.Depth, cmd.Stencil, cmd.RectCount,
                    cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.RectIndex])
                );
                continue;
            }
        SetRenderTargets:
            {
                const auto& cmd = item.SetRenderTargets;
                auto dsv = nullptr; // todo
                auto num_rtv = std::min(cmd.NumRtv, 8u);
                D3D12_CPU_DESCRIPTOR_HANDLE rtvs[num_rtv];
                for (u32 n = 0; n < num_rtv; ++n)
                {
                    rtvs[n] = GetRtv(cmd.Rtv[n].Get(submit));
                }
                cmd_pack->OMSetRenderTargets(num_rtv, rtvs, false, dsv);
                continue;
            }
        SetPipeline:
            {
                const auto& cmd = item.SetPipeline;
                SetPipeline(context, cmd_pack, cmd.Pipeline, i);
                continue;
            }
        Draw:
            {
                const auto& cmd = item.Draw;
                if (cmd.Pipeline != nullptr) SetPipeline(context, cmd_pack, cmd.Pipeline, i);
                if (context.m_current_pipeline == nullptr) throw WRuntimeException(L"Pipeline not set");
                const auto stages = context.m_current_pipeline->GetStages();
                if (!HasFlags(stages, FShaderStageFlags::Vertex))
                    throw WRuntimeException(L"Non Vertex pipelines cannot use Draw");
                if (cmd.Indexed)
                {
                    cmd_pack->DrawIndexedInstanced(
                        cmd.VertexOrIndexCount, cmd.InstanceCount,
                        cmd.FirstVertexOrIndex, cmd.VertexOffset, cmd.FirstInstance
                    );
                }
                else
                {
                    cmd_pack->DrawInstanced(
                        cmd.VertexOrIndexCount, cmd.InstanceCount,
                        cmd.FirstVertexOrIndex, cmd.FirstInstance
                    );
                }
                continue;
            }
        Dispatch:
            {
                const auto& cmd = item.Dispatch;
                if (cmd.Pipeline != nullptr) SetPipeline(context, cmd_pack, cmd.Pipeline, i);
                FShaderStageFlags stages{};
                switch (cmd.Type)
                {
                case FDispatchType::Auto:
                    if (context.m_current_pipeline == nullptr) throw WRuntimeException(L"Pipeline not set");
                    stages = context.m_current_pipeline->GetStages();
                    if (HasFlags(stages, FShaderStageFlags::Compute))
                        goto Dispatch_Compute;
                    if (HasFlags(stages, FShaderStageFlags::Mesh))
                        goto Dispatch_Mesh;
                    throw WRuntimeException(L"Non Mesh and Compute pipelines cannot use Dispatch");
                case FDispatchType::Compute:
                Dispatch_Compute:
                    cmd_pack->Dispatch(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
                    break;
                case FDispatchType::Mesh:
                Dispatch_Mesh:
                    if (!cmd_pack.m_list7)
                        throw WRuntimeException(L"Mesh Shader is not supported on this device");
                    cmd_pack.m_list7->DispatchMesh(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
                    break;
                }
                continue;
            }
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
