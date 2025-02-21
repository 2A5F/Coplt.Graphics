#include "Command.h"
#include "../../Api/FFI/Command.h"

#include "Context.h"
#include "fmt/format.h"
#include "fmt/xchar.h"

#include "Queue.h"
#include "../FFI/Layout.h"
#include "../FFI/Output.h"
#include "../FFI/Pipeline.h"
#include "../FFI/Resource.h"
#include "../Include/GraphicsFormat.h"
#include "../Include/PipelineState.h"
#include "../ThirdParty/DirectXTK12/Src/d3dx12.h"

using namespace Coplt;

D3d12CommandInterpreter::D3d12CommandInterpreter(D3d12GpuQueue* queue) : m_queue(queue)
{
}

void D3d12CommandInterpreter::Interpret(const FCommandSubmit& submit)
{
    Init();
    InitStates(submit);
    CollectBarrier(submit);
    Translate(submit);
    Reset();
}

void D3d12CommandInterpreter::CmdNext()
{
    m_items.back().CommandCount++;
}

void D3d12CommandInterpreter::ReqState(const u32 CmdIndex, const FResourceRef ResSrc, const FResourceState ReqState)
{
    auto& state = m_states[ResSrc.ResourceIndex];
    const auto old_state = state.CurrentState;
    const auto new_state = ChangeState(old_state, ReqState);
    if (new_state == old_state) return;
    const auto has_barrier = state.CurrentBarrierIndex != COPLT_U32_MAX;
    if (!has_barrier) AddBarrier(state, ResSrc);
    else if (!IsCompatible(old_state, new_state))
    {
        BarNext();
        AddBarrier(state, ResSrc);
    }
    state.CurrentState = new_state;
}

void D3d12CommandInterpreter::AddBarrier(ResState& state, const FResourceRef ResSrc)
{
    state.CurrentBarrierIndex = m_barriers.size();
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = state.Resource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = ToDx(state.LastState);
    m_barriers.push_back(barrier);
    m_barrier_resources.push_back(ResSrc.ResourceIndex);
}

void D3d12CommandInterpreter::BarNext()
{
    for (u32 i = m_last_barrier_index; i < m_barriers.size(); ++i)
    {
        auto& state = m_states[m_barrier_resources[i]];
        auto& barrier = m_barriers[i];
        barrier.Transition.StateAfter = ToDx(state.CurrentState);
        state.LastState = state.CurrentState;
    }
    auto& item = m_items.back();
    const auto size = m_barriers.size();
    const auto count = size - m_last_barrier_index;
    item.BarrierCount = count;
    m_last_barrier_index = size;
    m_items.push_back({.CommandIndex = item.CommandIndex + item.CommandCount, .BarrierIndex = m_last_barrier_index});
}

void D3d12CommandInterpreter::Init()
{
    m_items.push_back({});
}

void D3d12CommandInterpreter::Reset()
{
    m_current_pipeline = nullptr;
    m_states.clear();
    m_barriers.clear();
    m_barrier_resources.clear();
    m_items.clear();
    m_last_barrier_index = 0;
}

void D3d12CommandInterpreter::InitStates(const FCommandSubmit& submit)
{
    m_states.reserve(submit.ResourceCount);
    for (u32 i = 0; i < submit.ResourceCount; ++i)
    {
        const auto& res = submit.Resources[i];
        if (res.GetObjectPtr() == nullptr) throw WRuntimeException(fmt::format(L"Resource is null at {}", i));
        ResState state{};
        state.Resource = GetResource(res);
        state.Type = res.Type;
        state.CurrentState = state.LastState = res.CurrentState;
        m_states.push_back(std::move(state));
    }
}

void D3d12CommandInterpreter::CollectBarrier(const FCommandSubmit& submit)
{
    for (u32 i = 0; i < submit.CommandCount; ++i, CmdNext())
    {
        const auto& item = submit.Commands[i];

        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Transition:
        case FCommandType::SetPipeline:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
        case FCommandType::SetViewportScissor:
            goto CmdNext;
        case FCommandType::Present:
            goto Present;
        case FCommandType::ClearColor:
            goto ClearColor;
        case FCommandType::ClearDepthStencil:
            goto ClearDepthStencil;
        case FCommandType::SetRenderTargets:
            goto SetRenderTargets;
        case FCommandType::SetMeshBuffers:
            goto SetMeshBuffers;
        case FCommandType::BufferCopy:
            goto BufferCopy;
        }
        throw WRuntimeException(fmt::format(L"Unknown command type {}", static_cast<u32>(item.Type)));

    CmdNext:
        {
            continue;
        }
    Present:
        {
            const auto& cmd = item.Present;
            ReqState(i, cmd.Image, FResourceState::Present);
            continue;
        }
    ClearColor:
        {
            const auto& cmd = item.ClearColor;
            ReqState(i, cmd.Image, FResourceState::RenderTarget);
            continue;
        }
    ClearDepthStencil:
        {
            const auto& cmd = item.ClearDepthStencil;
            ReqState(i, cmd.Image, FResourceState::DepthWrite);
            continue;
        }
    SetRenderTargets:
        {
            const auto& cmd = item.SetRenderTargets;
            if (!cmd.Dsv.IsEmpty()) ReqState(i, cmd.Dsv, FResourceState::DepthWrite);
            for (u32 n = 0; n < std::min(cmd.NumRtv, 8u); ++n)
            {
                ReqState(i, cmd.Rtv[n], FResourceState::RenderTarget);
            }
            continue;
        }
    SetMeshBuffers:
        {
            const auto& cmd = item.SetMeshBuffers;
            if (!cmd.IndexBuffer.Buffer.IsEmpty())
                ReqState(i, cmd.IndexBuffer.Buffer, FResourceState::IndexBuffer);
            const auto vbs = reinterpret_cast<const FVertexBufferRange*>(&submit.Payload[cmd.VertexBuffersIndex]);
            for (u32 n = 0; n < cmd.VertexBufferCount; ++n)
            {
                ReqState(i, vbs[n].Buffer, FResourceState::VertexBuffer);
            }
            continue;
        }
    BufferCopy:
        {
            const auto& cmd = item.BufferCopy;
            switch (cmd.SrcType)
            {
            case FBufferRefType::Buffer:
                ReqState(i, cmd.Src.Buffer, FResourceState::CopySrc);
                break;
            case FBufferRefType::Upload:
                // 上传缓冲区状态永远在 GenericRead 不需要屏障
                break;
            }
            switch (cmd.DstType)
            {
            case FBufferRefType::Buffer:
                ReqState(i, cmd.Dst.Buffer, FResourceState::CopyDst);
                break;
            case FBufferRefType::Upload:
                throw new WRuntimeException(
                    fmt::format(L"The upload buffer cannot be used as a copy target at command {}", i)
                );
            }
            continue;
        }
    }
    BarNext();
}

void D3d12CommandInterpreter::Translate(const FCommandSubmit& submit)
{
    const auto& cmd_pack = m_queue->m_cmd;
    for (const auto& cmd_item : m_items)
    {
        if (cmd_item.BarrierCount > 0)
        {
            cmd_pack->ResourceBarrier(cmd_item.BarrierCount, m_barriers.data() + cmd_item.BarrierIndex);
        }

        for (u32 i = 0; i < cmd_item.CommandCount; ++i)
        {
            const auto& item = submit.Commands[cmd_item.CommandIndex + i];

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
            case FCommandType::SetViewportScissor:
                goto SetViewportScissor;
            case FCommandType::SetPipeline:
                goto SetPipeline;
            case FCommandType::SetMeshBuffers:
                goto SetMeshBuffers;
            case FCommandType::Draw:
                goto Draw;
            case FCommandType::Dispatch:
                goto Dispatch;
            case FCommandType::BufferCopy:
                goto BufferCopy;
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

                auto viewport = reinterpret_cast<const D3D12_VIEWPORT*>(&submit.Payload[cmd.ViewportIndex]);
                auto scissor = reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.ScissorRectIndex]);
                if (cmd.ViewportCount > 0) cmd_pack->RSSetViewports(cmd.ViewportCount, viewport);
                if (cmd.ScissorRectCount > 0) cmd_pack->RSSetScissorRects(cmd.ScissorRectCount, scissor);
                continue;
            }
        SetViewportScissor:
            {
                const auto& cmd = item.SetViewportScissor;
                auto viewport = reinterpret_cast<const D3D12_VIEWPORT*>(&submit.Payload[cmd.ViewportIndex]);
                auto scissor = reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.ScissorRectIndex]);
                if (cmd.ViewportCount > 0) cmd_pack->RSSetViewports(cmd.ViewportCount, viewport);
                if (cmd.ScissorRectCount > 0) cmd_pack->RSSetScissorRects(cmd.ScissorRectCount, scissor);
                continue;
            }
        SetPipeline:
            {
                const auto& cmd = item.SetPipeline;
                SetPipeline(cmd_pack, cmd.Pipeline, i);
                continue;
            }
        SetMeshBuffers:
            {
                const auto& cmd = item.SetMeshBuffers;
                const auto defs = cmd.MeshLayout->GetBuffers();
                const auto vbs = reinterpret_cast<const FVertexBufferRange*>(&submit.Payload[cmd.VertexBuffersIndex]);
                if (!cmd.IndexBuffer.Buffer.IsEmpty())
                {
                    const auto resource = GetResource(cmd.IndexBuffer.Buffer.Get(submit));
                    D3D12_INDEX_BUFFER_VIEW view{};
                    view.BufferLocation = resource->GetGPUVirtualAddress() + cmd.IndexBuffer.Offset;
                    view.SizeInBytes = cmd.IndexBuffer.Size;
                    view.Format = Coplt::ToDx(cmd.IndexFormat);
                    cmd_pack->IASetIndexBuffer(&view);
                }
                {
                    D3D12_VERTEX_BUFFER_VIEW views[32]{};
                    for (u32 n = 0; n < cmd.VertexBufferCount; ++n)
                    {
                        auto& range = vbs[n];
                        auto& view = views[n];
                        const auto& def = defs[range.Index];
                        const auto resource = GetResource(range.Buffer.Get(submit));
                        view.BufferLocation = resource->GetGPUVirtualAddress() + range.Offset;
                        view.SizeInBytes = range.Size;
                        view.StrideInBytes = def.Stride;
                    }
                    cmd_pack->IASetVertexBuffers(cmd.VertexStartSlot, cmd.VertexBufferCount, views);
                }
                continue;
            }
        Draw:
            {
                const auto& cmd = item.Draw;
                if (cmd.Pipeline != nullptr) SetPipeline(cmd_pack, cmd.Pipeline, i);
                if (m_current_pipeline == nullptr) throw WRuntimeException(L"Pipeline not set");
                const auto stages = m_current_pipeline->GetStages();
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
                if (cmd.Pipeline != nullptr) SetPipeline(cmd_pack, cmd.Pipeline, i);
                FShaderStageFlags stages{};
                switch (cmd.Type)
                {
                case FDispatchType::Auto:
                    if (m_current_pipeline == nullptr) throw WRuntimeException(L"Pipeline not set");
                    stages = m_current_pipeline->GetStages();
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
                default:
                    throw WRuntimeException(fmt::format(L"Unknown dispatch type {} at command {}",
                                                        static_cast<u32>(cmd.Type), i));
                }
                continue;
            }
        BufferCopy:
            {
                const auto& cmd = item.BufferCopy;
                if (cmd.SrcType == FBufferRefType::Buffer && cmd.DstType == FBufferRefType::Buffer)
                {
                    const auto dst = GetResource(cmd.Dst.Buffer.Get(submit));
                    const auto src = GetResource(cmd.Src.Buffer.Get(submit));
                    if (cmd.Size == std::numeric_limits<u64>::max())
                    {
                        cmd_pack->CopyResource(dst, src);
                    }
                    else
                    {
                        cmd_pack->CopyBufferRegion(dst, cmd.DstOffset, src, cmd.SrcOffset, cmd.Size);
                    }
                }
                else if (cmd.SrcType == FBufferRefType::Upload && cmd.DstType == FBufferRefType::Buffer)
                {
                    const auto dst = GetResource(cmd.Dst.Buffer.Get(submit));
                    if (cmd.Src.Upload.Index >= m_queue->m_frame_context->m_upload_buffers.size())
                        throw WRuntimeException(fmt::format(L"Index out of bounds at command {}", i));
                    const auto& src_obj = m_queue->m_frame_context->m_upload_buffers[cmd.Src.Upload.Index];
                    if (cmd.SrcOffset + cmd.Size >= src_obj.m_size)
                        throw WRuntimeException(fmt::format(L"Size out of range at command {}", i));
                    const auto src = src_obj.m_resource.m_resource.Get();
                    cmd_pack->CopyBufferRegion(dst, cmd.DstOffset, src, cmd.SrcOffset, cmd.Size);
                }
                else
                {
                    throw WRuntimeException(
                        fmt::format(
                            L"Unsupported copy combination {{ SrcType = {} DstType = {} }} at command {}",
                            static_cast<u8>(cmd.SrcType), static_cast<u8>(cmd.DstType), i
                        )
                    );
                }
                continue;
            }
        }
    }
}

void D3d12CommandInterpreter::SetPipeline(
    const CmdListPack& cmd_pack, FShaderPipeline* pipeline, u32 i
)
{
    if (pipeline == m_current_pipeline) return;
    m_current_pipeline = pipeline;
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
        cmd_pack->IASetPrimitiveTopology(Coplt::ToDx(states->Topology));
        cmd_pack->SetGraphicsRootSignature(static_cast<ID3D12RootSignature*>(layout->GetRootSignaturePtr()));
    }
    cmd_pack->SetPipelineState(static_cast<ID3D12PipelineState*>(ps->GetPipelineStatePtr()));
}

ID3D12Resource* D3d12CommandInterpreter::GetResource(const FResourceMeta& meta)
{
    return GetResource(meta.Output, meta.Type);
}

ID3D12Resource* D3d12CommandInterpreter::GetResource(FUnknown* object, FResourceRefType type)
{
    switch (type)
    {
    case FResourceRefType::Image:
        throw WRuntimeException(L"TODO");
    case FResourceRefType::Buffer:
        {
            const auto buffer = object->QueryInterface<FD3d12GpuBuffer>();
            if (!buffer) throw WRuntimeException(L"The memory may be corrupted");
            ID3D12Resource* ptr{};
            buffer->GetCurrentResourcePtr(&ptr).TryThrow();
            return ptr;
        }
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
    case FResourceRefType::Image:
    case FResourceRefType::Buffer:
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
