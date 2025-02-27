#include "Command.h"
#include "../../Api/FFI/Command.h"

#include "Context.h"
#include "DescriptorManager.h"
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

void D3d12CommandInterpreter::Context::ResetPipeline()
{
    Pipeline = nullptr;
    D3dPipeline = nullptr;
}

void D3d12CommandInterpreter::Context::Reset()
{
    ResetPipeline();
    NumRtv = 0;
    Dsv = {};
    Ibv = {};
    NumVbv = 0;
}

D3d12CommandInterpreter::D3d12CommandInterpreter(D3d12GpuQueue* queue) : m_queue(queue)
{
}

void D3d12CommandInterpreter::Interpret(const FCommandSubmit& submit)
{
    Init();
    InitStates(submit);
    Analyze(submit);
    Translate(submit);
    Reset();
}

void D3d12CommandInterpreter::CmdNext()
{
    m_items.back().CommandCount++;
}

void D3d12CommandInterpreter::ReqState(const FResourceRef ResSrc, const FLegacyState ReqState)
{
    auto& state = m_states[ResSrc.ResourceIndex - 1];
    const auto old_state = state.CurrentState;
    const auto new_state = ChangeState(old_state, ToDx(ReqState));
    const auto last_item_size = state.CurrentItemsSize;
    if (new_state == old_state)
    {
        state.CurrentItemsSize = m_items.size();
        return;
    }
    state.CurrentState = new_state;
    if (!state.HasBarrier()) AddBarrier(state, ResSrc);
    else if (!IsCompatible(old_state, new_state))
    {
        if (last_item_size >= m_items.size()) BarNext();
        state.LastState = old_state;
        AddBarrier(state, ResSrc);
    }
    else
    {
        state.CurrentItemsSize = m_items.size();
        m_barriers[state.CurrentBarrierIndex].Transition.StateAfter = state.CurrentState;
    }
}

void D3d12CommandInterpreter::MarkResUse(const FResourceRef ResSrc)
{
    auto& state = m_states[ResSrc.ResourceIndex - 1];
    state.CurrentItemsSize = m_items.size();
}

void D3d12CommandInterpreter::AddBarrier(ResState& state, const FResourceRef ResSrc)
{
    state.CurrentItemsSize = m_items.size();
    state.CurrentBarrierIndex = m_barriers.size();
    D3D12_RESOURCE_BARRIER barrier{};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = state.Resource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = state.LastState;
    barrier.Transition.StateAfter = state.CurrentState;
    m_barriers.push_back(barrier);
    m_barrier_resources.push_back(ResSrc.ResourceIndex - 1);
}

void D3d12CommandInterpreter::BarNext()
{
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
    m_states.clear();
    m_barriers.clear();
    m_barrier_resources.clear();
    m_items.clear();
    m_last_barrier_index = 0;
    m_context.Reset();
}

void D3d12CommandInterpreter::InitStates(const FCommandSubmit& submit)
{
    m_states.reserve(submit.ResourceCount);
    for (u32 i = 0; i < submit.ResourceCount; ++i)
    {
        const auto& res = submit.Resources[i];
        if (res.GetObjectPtr() == nullptr)
            COPLT_THROW_FMT("Resource is null at {}", i);
        ResState state{};
        state.Resource = GetResource(res);
        state.Type = res.Type;
        state.CurrentState = state.LastState = ToDx(res.CurrentState);
        m_states.push_back(state);
    }
}

void D3d12CommandInterpreter::Analyze(const FCommandSubmit& submit)
{
    for (u32 i = 0; i < submit.CommandCount; ++i, CmdNext())
    {
        const auto& item = submit.Commands[i];

        switch (item.Type)
        {
        case FCommandType::None:
            goto CmdNext;
        case FCommandType::Label:
            COPLT_THROW("TODO");
        case FCommandType::BeginScope:
            COPLT_THROW("TODO");
        case FCommandType::EndScope:
            COPLT_THROW("TODO");
        case FCommandType::Present:
            goto Present;
        case FCommandType::Barrier:
            COPLT_THROW("TODO");
        case FCommandType::ClearColor:
            goto ClearColor;
        case FCommandType::ClearDepthStencil:
            goto ClearDepthStencil;
        case FCommandType::Bind:
            goto Bind;
        case FCommandType::BufferCopy:
            goto BufferCopy;
        case FCommandType::Render:
            COPLT_THROW("TODO");
        case FCommandType::Compute:
            COPLT_THROW("TODO");
        case FCommandType::End:
        case FCommandType::SetPipeline:
        case FCommandType::SetBinding:
        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
            COPLT_THROW("Subcommands cannot be placed in the main command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));

    CmdNext:
        {
            continue;
        }
    Present:
        {
            const auto& cmd = item.Present;
            ReqState(cmd.Image, FLegacyState::Present);
            continue;
        }
    ClearColor:
        {
            const auto& cmd = item.ClearColor;
            ReqState(cmd.Image, FLegacyState::RenderTarget);
            continue;
        }
    ClearDepthStencil:
        {
            const auto& cmd = item.ClearDepthStencil;
            ReqState(cmd.Image, FLegacyState::DepthWrite);
            continue;
        }
    Bind:
        {
            const auto& cmd = item.Bind;
            // todo
            continue;
        }
    BufferCopy:
        {
            const auto& cmd = item.BufferCopy;
            switch (cmd.SrcType)
            {
            case FBufferRefType::Buffer:
                ReqState(cmd.Src.Buffer, FLegacyState::CopySrc);
                break;
            case FBufferRefType::Upload:
                // 上传缓冲区状态永远在 GenericRead 不需要屏障
                break;
            }
            switch (cmd.DstType)
            {
            case FBufferRefType::Buffer:
                ReqState(cmd.Dst.Buffer, FLegacyState::CopyDst);
                break;
            case FBufferRefType::Upload:
                COPLT_THROW_FMT(
                    "The upload buffer cannot be used as a copy target at command {}",
                    i
                );
            }
            continue;
        }
        // SetMeshBuffers:
        //     {
        //         const auto& cmd = item.SetMeshBuffers;
        //         if (!cmd.IndexBuffer.Buffer.IsEmpty())
        //         {
        //             ReqState(cmd.IndexBuffer.Buffer, FResourceState::IndexBuffer);
        //             m_context.Ibv = cmd.IndexBuffer.Buffer;
        //         }
        //         const auto vbs = reinterpret_cast<const FVertexBufferRange*>(&submit.Payload[cmd.VertexBuffersIndex]);
        //         if (cmd.VertexStartSlot + cmd.VertexBufferCount >= 31)
        //         {
        //             COPLT_THROW_FMT(
        //                 "The number of input buffers is out of range, the maximum allowed input slots is 31; at command {}",
        //                 i
        //             );
        //         }
        //         for (u32 n = 0; n < cmd.VertexBufferCount; ++n)
        //         {
        //             const auto& vb = vbs[n];
        //             ReqState(vb.Buffer, FResourceState::VertexBuffer);
        //             m_context.Vbv[cmd.VertexStartSlot + n] = vb.Buffer;
        //         }
        //         continue;
        //     }
        // Graphics_MarkResUse:
        //     {
        //         const auto g_state = m_context.GPipeline->GetGraphicsState();
        //         if (g_state->DsvFormat != FGraphicsFormat::Unknown && !m_context.Dsv.IsEmpty())
        //         {
        //             MarkResUse(m_context.Dsv);
        //         }
        //         const auto num_rts = std::min(m_context.NumRtv, static_cast<u32>(g_state->NumRts));
        //         for (u32 n = 0; n < num_rts; ++n)
        //         {
        //             const auto rt = m_context.Rtv[n];
        //             if (!rt.IsEmpty()) MarkResUse(rt);
        //         }
        //         if (const auto mesh_layout = m_context.GPipeline->GetMeshLayout())
        //         {
        //             auto buffers = mesh_layout->GetBuffers();
        //             for (u32 n = 0; n < buffers.size(); ++n)
        //             {
        //                 const auto vb = m_context.Vbv[n];
        //                 if (!vb.IsEmpty()) MarkResUse(vb);
        //             }
        //         }
        //         continue;
        //     }
        // Draw:
        //     {
        //         const auto& cmd = item.Draw;
        //         if (cmd.Pipeline != nullptr) SetPipelineContext(cmd.Pipeline, i);
        //         if (m_context.Pipeline == nullptr)
        //             COPLT_THROW_FMT("Pipeline not set at command {}", i);
        //         goto Graphics_MarkResUse;
        //     }
        // Dispatch:
        //     {
        //         const auto& cmd = item.Dispatch;
        //         if (cmd.Pipeline != nullptr) SetPipelineContext(cmd.Pipeline, i);
        //         if (m_context.Pipeline == nullptr)
        //             COPLT_THROW_FMT("Pipeline not set at command {}", i);
        //         if (cmd.Type == FDispatchType::Mesh && m_context.GPipeline) goto Graphics_MarkResUse;
        //         continue;
        //     }
    }
    BarNext();
}

void D3d12CommandInterpreter::Translate(const FCommandSubmit& submit)
{
    m_context.ResetPipeline();
    const auto& cmd_pack = m_queue->m_cmd;
    {
        const auto& descriptor_manager = *m_queue->m_device->m_descriptor_manager;
        ID3D12DescriptorHeap* heaps[] = {
            descriptor_manager.m_cbv_srv_uav_set->m_Gpu_heap.Get(),
            descriptor_manager.m_sampler_set->m_Gpu_heap.Get()
        };
        cmd_pack->SetDescriptorHeaps(2, heaps);
    }
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
            case FCommandType::Label:
                COPLT_THROW("TODO");
            case FCommandType::BeginScope:
                COPLT_THROW("TODO");
            case FCommandType::EndScope:
                COPLT_THROW("TODO");
            case FCommandType::Barrier:
                COPLT_THROW("TODO");
            case FCommandType::ClearColor:
                goto ClearColor;
            case FCommandType::ClearDepthStencil:
                goto ClearDepthStencil;
            case FCommandType::Bind:
                goto Bind;
            case FCommandType::BufferCopy:
                goto BufferCopy;
            case FCommandType::Render:
                COPLT_THROW("TODO");
            case FCommandType::Compute:
                COPLT_THROW("TODO");
            case FCommandType::End:
            case FCommandType::SetPipeline:
            case FCommandType::SetBinding:
            case FCommandType::SetViewportScissor:
            case FCommandType::SetMeshBuffers:
            case FCommandType::Draw:
            case FCommandType::Dispatch:
                COPLT_THROW("Subcommands cannot be placed in the main command list");
            }
            COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));

        ClearColor:
            {
                const auto& cmd = item.ClearColor;
                const auto rtv = GetRtv(cmd.Image.Get(submit));
                cmd_pack->ClearRenderTargetView(
                    rtv, cmd.Color, cmd.RectCount,
                    cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Rects[cmd.RectIndex])
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
                    cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Rects[cmd.RectIndex])
                );
                continue;
            }
        Bind:
            {
                const auto& cmd = item.Bind;
                // todo
                continue;
            }
        BufferCopy:
            {
                const auto& cmd = item.BufferCopy;
                const auto& range = submit.BufferCopyRanges[cmd.RangeIndex];
                if (cmd.SrcType == FBufferRefType::Buffer && cmd.DstType == FBufferRefType::Buffer)
                {
                    const auto dst = GetResource(cmd.Dst.Buffer.Get(submit));
                    const auto src = GetResource(cmd.Src.Buffer.Get(submit));
                    if (range.Size == std::numeric_limits<u64>::max())
                    {
                        cmd_pack->CopyResource(dst, src);
                    }
                    else
                    {
                        cmd_pack->CopyBufferRegion(dst, range.DstOffset, src, range.SrcOffset, range.Size);
                    }
                }
                else if (cmd.SrcType == FBufferRefType::Upload && cmd.DstType == FBufferRefType::Buffer)
                {
                    const auto dst = GetResource(cmd.Dst.Buffer.Get(submit));
                    if (cmd.Src.Upload.Index >= m_queue->m_frame_context->m_upload_buffers.size())
                        COPLT_THROW_FMT("Index out of bounds at command {}", i);
                    const auto& src_obj = m_queue->m_frame_context->m_upload_buffers[cmd.Src.Upload.Index];
                    if (range.SrcOffset + range.Size >= src_obj.m_size)
                        COPLT_THROW_FMT("Size out of range at command {}", i);
                    const auto src = src_obj.m_resource.m_resource.Get();
                    cmd_pack->CopyBufferRegion(dst, range.DstOffset, src, range.SrcOffset, range.Size);
                }
                else
                {
                    COPLT_THROW_FMT(
                        "Unsupported copy combination {{ SrcType = {} DstType = {} }} at command {}",
                        static_cast<u8>(cmd.SrcType), static_cast<u8>(cmd.DstType), i
                    );
                }
                continue;
            }
            // SetRenderTargets:
            //     {
            //         const auto& cmd = item.SetRenderTargets;
            //         auto dsv = nullptr; // todo
            //         auto num_rtv = std::min(cmd.NumRtv, 8u);
            //         D3D12_CPU_DESCRIPTOR_HANDLE rtvs[num_rtv];
            //         for (u32 n = 0; n < num_rtv; ++n)
            //         {
            //             rtvs[n] = GetRtv(cmd.Rtv[n].Get(submit));
            //         }
            //         cmd_pack->OMSetRenderTargets(num_rtv, rtvs, false, dsv);
            //
            //         auto viewport = reinterpret_cast<const D3D12_VIEWPORT*>(&submit.Payload[cmd.ViewportIndex]);
            //         auto scissor = reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.ScissorRectIndex]);
            //         if (cmd.ViewportCount > 0) cmd_pack->RSSetViewports(cmd.ViewportCount, viewport);
            //         if (cmd.ScissorRectCount > 0) cmd_pack->RSSetScissorRects(cmd.ScissorRectCount, scissor);
            //         continue;
            //     }
            // SetViewportScissor:
            //     {
            //         const auto& cmd = item.SetViewportScissor;
            //         auto viewport = reinterpret_cast<const D3D12_VIEWPORT*>(&submit.Payload[cmd.ViewportIndex]);
            //         auto scissor = reinterpret_cast<const D3D12_RECT*>(&submit.Payload[cmd.ScissorRectIndex]);
            //         if (cmd.ViewportCount > 0) cmd_pack->RSSetViewports(cmd.ViewportCount, viewport);
            //         if (cmd.ScissorRectCount > 0) cmd_pack->RSSetScissorRects(cmd.ScissorRectCount, scissor);
            //         continue;
            //     }
            // SetPipeline:
            //     {
            //         const auto& cmd = item.SetPipeline;
            //         SetPipeline(cmd_pack, cmd.Pipeline, i);
            //         continue;
            //     }
            // SetMeshBuffers:
            //     {
            //         const auto& cmd = item.SetMeshBuffers;
            //         const auto defs = cmd.MeshLayout->GetBuffers();
            //         const auto vbs = reinterpret_cast<const FVertexBufferRange*>(&submit.Payload[cmd.VertexBuffersIndex]);
            //         if (!cmd.IndexBuffer.Buffer.IsEmpty())
            //         {
            //             const auto resource = GetResource(cmd.IndexBuffer.Buffer.Get(submit));
            //             D3D12_INDEX_BUFFER_VIEW view{};
            //             view.BufferLocation = resource->GetGPUVirtualAddress() + cmd.IndexBuffer.Offset;
            //             view.SizeInBytes = cmd.IndexBuffer.Size;
            //             view.Format = Coplt::ToDx(cmd.IndexFormat);
            //             cmd_pack->IASetIndexBuffer(&view);
            //         }
            //         {
            //             D3D12_VERTEX_BUFFER_VIEW views[cmd.VertexBufferCount];
            //             for (u32 n = 0; n < cmd.VertexBufferCount; ++n)
            //             {
            //                 auto& range = vbs[n];
            //                 const auto& def = defs[range.Index];
            //                 const auto resource = GetResource(range.Buffer.Get(submit));
            //                 D3D12_VERTEX_BUFFER_VIEW view{};
            //                 view.BufferLocation = resource->GetGPUVirtualAddress() + range.Offset;
            //                 view.SizeInBytes = range.Size;
            //                 view.StrideInBytes = def.Stride;
            //                 views[n] = view;
            //             }
            //             cmd_pack->IASetVertexBuffers(cmd.VertexStartSlot, cmd.VertexBufferCount, views);
            //         }
            //         continue;
            //     }
            // Draw:
            //     {
            //         const auto& cmd = item.Draw;
            //         if (cmd.Pipeline != nullptr) SetPipeline(cmd_pack, cmd.Pipeline, i);
            //         if (m_context.Pipeline == nullptr)
            //             COPLT_THROW_FMT("Pipeline not set at command {}", i);
            //         const auto stages = m_context.Pipeline->GetStages();
            //         if (!HasFlags(stages, FShaderStageFlags::Vertex))
            //             COPLT_THROW("Non Vertex pipelines cannot use Draw");
            //         if (cmd.Indexed)
            //         {
            //             cmd_pack->DrawIndexedInstanced(
            //                 cmd.VertexOrIndexCount, cmd.InstanceCount,
            //                 cmd.FirstVertexOrIndex, cmd.VertexOffset, cmd.FirstInstance
            //             );
            //         }
            //         else
            //         {
            //             cmd_pack->DrawInstanced(
            //                 cmd.VertexOrIndexCount, cmd.InstanceCount,
            //                 cmd.FirstVertexOrIndex, cmd.FirstInstance
            //             );
            //         }
            //         continue;
            //     }
            // Dispatch:
            //     {
            //         const auto& cmd = item.Dispatch;
            //         if (cmd.Pipeline != nullptr) SetPipeline(cmd_pack, cmd.Pipeline, i);
            //         FShaderStageFlags stages{};
            //         switch (cmd.Type)
            //         {
            //         case FDispatchType::Compute:
            //             cmd_pack->Dispatch(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
            //             break;
            //         case FDispatchType::Mesh:
            //             if (!cmd_pack.m_list7)
            //                 COPLT_THROW("Mesh Shader is not supported on this device");
            //             cmd_pack.m_list7->DispatchMesh(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
            //             break;
            //         default:
            //             COPLT_THROW_FMT(
            //                 "Unknown dispatch type {} at command {}", static_cast<u32>(cmd.Type), i
            //             );
            //         }
            //         continue;
            //     }
        }
    }
}

void D3d12CommandInterpreter::SetPipelineContext(FShaderPipeline* pipeline, const u32 i)
{
    if (pipeline == m_context.Pipeline) return;
    m_context.Pipeline = pipeline;
    m_context.D3dPipeline = pipeline->QueryInterface<FD3d12PipelineState>();
    if (!m_context.D3dPipeline)
    {
        COPLT_THROW_FMT(
            "Pipeline({:#x}) comes from different backends at cmd {}",
            reinterpret_cast<size_t>(pipeline), i
        );
    }
    m_context.Layout = pipeline->GetLayout()->QueryInterface<FD3d12ShaderLayout>();
    if (!m_context.Layout)
        COPLT_THROW("Shader layout from different backends");
    const auto stages = pipeline->GetStages();
    m_context.GPipeline = nullptr;
    if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        m_context.GPipeline = pipeline->QueryInterface<FD3d12GraphicsShaderPipeline>();
        if (!m_context.GPipeline)
            COPLT_THROW("Pipeline from different backends or pipeline not a graphics pipeline");
    }
}

void D3d12CommandInterpreter::SetPipeline(
    const CmdListPack& cmd_pack, FShaderPipeline* pipeline, const u32 i
)
{
    if (pipeline == m_context.Pipeline) return;
    SetPipelineContext(pipeline, i);
    const auto stages = pipeline->GetStages();
    if (HasFlags(stages, FShaderStageFlags::Compute))
    {
        cmd_pack->SetComputeRootSignature(static_cast<ID3D12RootSignature*>(m_context.Layout->GetRootSignaturePtr()));
    }
    else if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        const auto& states = m_context.GPipeline->GetGraphicsState();
        cmd_pack->IASetPrimitiveTopology(ToDx(states->Topology));
        cmd_pack->SetGraphicsRootSignature(static_cast<ID3D12RootSignature*>(m_context.Layout->GetRootSignaturePtr()));
    }
    cmd_pack->SetPipelineState(static_cast<ID3D12PipelineState*>(m_context.D3dPipeline->GetPipelineStatePtr()));
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
        COPLT_THROW("TODO");
    case FResourceRefType::Buffer:
        {
            const auto buffer = object->QueryInterface<FD3d12GpuBuffer>();
            if (!buffer)
                COPLT_THROW("The memory may be corrupted");
            ID3D12Resource* ptr{};
            buffer->GetCurrentResourcePtr(&ptr).TryThrow();
            return ptr;
        }
    case FResourceRefType::Output:
        {
            const auto output = object->QueryInterface<FD3d12GpuOutput>();
            if (!output)
                COPLT_THROW("The memory may be corrupted");
            ID3D12Resource* ptr{};
            output->GetCurrentResourcePtr(&ptr).TryThrow();
            return ptr;
        }
    default:
        COPLT_THROW_FMT("Unknown resource ref type {}", static_cast<u8>(type));
    }
}

D3D12_CPU_DESCRIPTOR_HANDLE D3d12CommandInterpreter::GetRtv(const FResourceMeta& meta)
{
    switch (meta.Type)
    {
    case FResourceRefType::Image:
    case FResourceRefType::Buffer:
        COPLT_THROW("TODO");
    case FResourceRefType::Output:
        {
            const auto output = meta.Output->QueryInterface<FD3d12GpuOutput>();
            if (!output)
                COPLT_THROW("The memory may be corrupted");
            D3D12_CPU_DESCRIPTOR_HANDLE rtv{};
            output->GetCurrentRtv(&rtv).TryThrow();
            return rtv;
        }
    default:
        COPLT_THROW_FMT("Unknown resource ref type {}", static_cast<u8>(meta.Type));
    }
}
