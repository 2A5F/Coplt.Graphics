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

#include "pix3.h"

using namespace Coplt;

void D3d12CommandInterpreter::BarrierContext::Reset()
{
    m_groups_count = 0;
    m_global_barriers.clear();
    m_texture_barriers.clear();
    m_buffer_barriers.clear();
}

void D3d12CommandInterpreter::BarrierContext::Add(const D3D12_GLOBAL_BARRIER& barrier)
{
    m_global_barriers.push_back(barrier);
}

void D3d12CommandInterpreter::BarrierContext::Add(const D3D12_TEXTURE_BARRIER& barrier)
{
    m_texture_barriers.push_back(barrier);
}

void D3d12CommandInterpreter::BarrierContext::Add(const D3D12_BUFFER_BARRIER& barrier)
{
    m_buffer_barriers.push_back(barrier);
}

namespace
{
    template <D3D12_BARRIER_TYPE Type, class T>
    constexpr void AddBarrierGroup(
        D3D12_BARRIER_GROUP (&barrier_groups)[3],
        i32& groups_count, std::vector<T>& barriers
    )
    {
        const auto index = groups_count;
        groups_count++;
        D3D12_BARRIER_GROUP group{};
        group.Type = Type;
        group.NumBarriers = barriers.size();
        if constexpr (Type == D3D12_BARRIER_TYPE_GLOBAL)
        {
            group.pGlobalBarriers = barriers.data();
        }
        else if constexpr (Type == D3D12_BARRIER_TYPE_TEXTURE)
        {
            group.pTextureBarriers = barriers.data();
        }
        else if constexpr (Type == D3D12_BARRIER_TYPE_BUFFER)
        {
            group.pBufferBarriers = barriers.data();
        }
        else static_assert(false);
        // ReSharper disable once CppSomeObjectMembersMightNotBeInitialized
        barrier_groups[index] = group;
    }
}

void D3d12CommandInterpreter::BarrierContext::BuildBarriers()
{
    if (!m_global_barriers.empty())
        AddBarrierGroup<D3D12_BARRIER_TYPE_GLOBAL>(m_barrier_groups, m_groups_count, m_global_barriers);
    if (!m_texture_barriers.empty())
        AddBarrierGroup<D3D12_BARRIER_TYPE_TEXTURE>(m_barrier_groups, m_groups_count, m_texture_barriers);
    if (!m_buffer_barriers.empty())
        AddBarrierGroup<D3D12_BARRIER_TYPE_BUFFER>(m_barrier_groups, m_groups_count, m_buffer_barriers);
}

void D3d12CommandInterpreter::Context::Reset()
{
    Pipeline = nullptr;
    D3dPipeline = nullptr;
}

D3d12CommandInterpreter::D3d12CommandInterpreter(D3d12GpuQueue* queue) : m_queue(queue)
{
}

void D3d12CommandInterpreter::Interpret(const FCommandSubmit& submit)
{
    Translate(submit);
    Reset();
}

void D3d12CommandInterpreter::Reset()
{
    m_barrier_context.Reset();
    m_context.Reset();
}

void D3d12CommandInterpreter::Translate(const FCommandSubmit& submit)
{
    const auto& cmd_pack = m_queue->m_cmd;
    const std::span command_items(submit.Commands, submit.CommandCount);
    for (u32 i = 0; i < command_items.size(); i++)
    {
        const auto& item = command_items[i];
        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Present:
            continue;
        case FCommandType::Label:
            Label(submit, item.Label);
            continue;
        case FCommandType::BeginScope:
            BeginScope(submit, item.BeginScope);
            continue;
        case FCommandType::EndScope:
            EndScope(submit, item.EndScope);
            continue;
        case FCommandType::Barrier:
            Barrier(submit, i, item.Barrier);
            continue;
        case FCommandType::ClearColor:
            ClearColor(submit, i, item.ClearColor);
            continue;
        case FCommandType::ClearDepthStencil:
            ClearDepthStencil(submit, i, item.ClearDepthStencil);
            continue;
        case FCommandType::Bind:
            continue; // todo
        case FCommandType::BufferCopy:
            BufferCopy(submit, i, item.BufferCopy);
            continue;
        case FCommandType::Render:
            Render(submit, i, item.Render);
            continue;
        case FCommandType::Compute:
            COPLT_THROW("TODO");

        // subcommands
        case FCommandType::SetPipeline:
        case FCommandType::SetBinding:
        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
            COPLT_THROW("Sub commands cannot be placed in the main command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));
    }
}

void D3d12CommandInterpreter::Label(const FCommandSubmit& submit, const FCommandLabel& cmd) const
{
    if (!m_queue->m_device->Debug()) return;
    auto color = PIX_COLOR_DEFAULT;
    if (cmd.HasColor) color = PIX_COLOR(cmd.Color[0], cmd.Color[1], cmd.Color[2]);
    if (cmd.StrType == FStrType::Str8)
    {
        PIXSetMarker(m_queue->m_cmd.m_list0.Get(), color, "%s", reinterpret_cast<const char*>(submit.Str8 + cmd.StringIndex));
    }
    else
    {
        PIXSetMarker(m_queue->m_cmd.m_list0.Get(), color, L"%s", reinterpret_cast<const wchar_t*>(submit.Str16 + cmd.StringIndex));
    }
}

void D3d12CommandInterpreter::BeginScope(const FCommandSubmit& submit, const FCommandBeginScope& cmd) const
{
    if (!m_queue->m_device->Debug()) return;
    auto color = PIX_COLOR_DEFAULT;
    if (cmd.HasColor) color = PIX_COLOR(cmd.Color[0], cmd.Color[1], cmd.Color[2]);
    if (cmd.StrType == FStrType::Str8)
    {
        PIXBeginEvent(m_queue->m_cmd.m_list0.Get(), color, "%s", reinterpret_cast<const char*>(submit.Str8 + cmd.StringIndex));
    }
    else
    {
        PIXBeginEvent(m_queue->m_cmd.m_list0.Get(), color, L"%s", reinterpret_cast<const wchar_t*>(submit.Str16 + cmd.StringIndex));
    }
}

void D3d12CommandInterpreter::EndScope(const FCommandSubmit& submit, const FCommandEndScope& cmd) const
{
    if (!m_queue->m_device->Debug()) return;
    PIXEndEvent(m_queue->m_cmd.m_list0.Get());
}

void D3d12CommandInterpreter::Barrier(const FCommandSubmit& submit, u32 i, const FCommandBarrier& cmd)
{
    m_barrier_context.Reset();
    const std::span barriers = std::span(&submit.Barriers[cmd.BarrierIndex], cmd.BarrierCount);
    for (const auto& barrier : barriers)
    {
        switch (barrier.Type)
        {
        case FBarrierType::None:
            continue;
        case FBarrierType::Global:
            Barrier(submit, barrier.Global);
            continue;
        case FBarrierType::Buffer:
            Barrier(submit, barrier.Buffer);
            continue;
        case FBarrierType::Image:
            Barrier(submit, barrier.Image);
            continue;
        default:
            COPLT_THROW_FMT("Unknown barrier type {} at command {}", static_cast<u32>(barrier.Type), i);
        }
    }
    m_barrier_context.BuildBarriers();
    if (m_barrier_context.m_groups_count == 0) return;
    m_queue->m_cmd.m_list7->Barrier(m_barrier_context.m_groups_count, m_barrier_context.m_barrier_groups);
}

void D3d12CommandInterpreter::Barrier(const FCommandSubmit& submit, const FGlobalBarrier& item)
{
    D3D12_GLOBAL_BARRIER barrier{};
    barrier.AccessBefore = ToDx(item.AccessBefore);
    barrier.AccessAfter = ToDx(item.AccessAfter);
    barrier.SyncBefore = CalcSync(item.StagesBefore, item.AccessBefore, item.AccessAfter);
    barrier.SyncAfter = CalcSync(item.StagesAfter, item.AccessAfter, item.AccessBefore);
    m_barrier_context.m_global_barriers.push_back(barrier);
}

void D3d12CommandInterpreter::Barrier(const FCommandSubmit& submit, const FBufferBarrier& item)
{
    D3D12_BUFFER_BARRIER barrier{};
    barrier.pResource = GetResource(item.Buffer.Get(submit));
    barrier.Offset = item.Offset;
    barrier.Size = item.Size;
    barrier.AccessBefore = ToDx(item.AccessBefore);
    barrier.AccessAfter = ToDx(item.AccessAfter);
    barrier.SyncBefore = CalcSync(item.StagesBefore, item.AccessBefore, item.AccessAfter);
    barrier.SyncAfter = CalcSync(item.StagesAfter, item.AccessAfter, item.AccessBefore);
    m_barrier_context.m_buffer_barriers.push_back(barrier);
}

void D3d12CommandInterpreter::Barrier(const FCommandSubmit& submit, const FImageBarrier& item)
{
    const auto cross_queue = HasFlags(item.Flags, FImageBarrierFlags::CrossQueue);
    D3D12_TEXTURE_BARRIER barrier{};
    barrier.pResource = GetResource(item.Image.Get(submit));
    barrier.Subresources.IndexOrFirstMipLevel = item.SubResourceRange.IndexOrFirstMipLevel;
    barrier.Subresources.NumMipLevels = item.SubResourceRange.NumMipLevels;
    barrier.Subresources.FirstArraySlice = item.SubResourceRange.FirstArraySlice;
    barrier.Subresources.NumArraySlices = item.SubResourceRange.NumArraySlices;
    barrier.Subresources.FirstPlane = item.SubResourceRange.FirstPlane;
    barrier.Subresources.NumPlanes = item.SubResourceRange.NumPlanes;
    barrier.LayoutBefore = ToDx(item.LayoutBefore, cross_queue ? std::nullopt : std::optional(m_queue->m_queue_type));
    barrier.LayoutAfter = ToDx(item.LayoutAfter, cross_queue ? std::nullopt : std::optional(m_queue->m_queue_type));
    barrier.AccessBefore = ToDx(item.AccessBefore);
    barrier.AccessAfter = ToDx(item.AccessAfter);
    barrier.SyncBefore = CalcSync(item.StagesBefore, item.AccessBefore, item.AccessAfter);
    barrier.SyncAfter = CalcSync(item.StagesAfter, item.AccessAfter, item.AccessBefore);
    if (HasFlags(item.Flags, FImageBarrierFlags::Discard)) barrier.Flags |= D3D12_TEXTURE_BARRIER_FLAG_DISCARD;
    m_barrier_context.m_texture_barriers.push_back(barrier);
}

void D3d12CommandInterpreter::ClearColor(const FCommandSubmit& submit, u32 i, const FCommandClearColor& cmd) const
{
    const auto rtv = GetRtv(cmd.Image.Get(submit));
    m_queue->m_cmd->ClearRenderTargetView(
        rtv, cmd.Color, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Rects[cmd.RectIndex])
    );
}

void D3d12CommandInterpreter::ClearDepthStencil(const FCommandSubmit& submit, u32 i, const FCommandClearDepthStencil& cmd) const
{
    D3D12_CPU_DESCRIPTOR_HANDLE dsv{}; // todo
    D3D12_CLEAR_FLAGS flags{};
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Depth)) flags |= D3D12_CLEAR_FLAG_DEPTH;
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Stencil)) flags |= D3D12_CLEAR_FLAG_STENCIL;
    m_queue->m_cmd->ClearDepthStencilView(
        dsv, flags, cmd.Depth, cmd.Stencil, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&submit.Rects[cmd.RectIndex])
    );
}

void D3d12CommandInterpreter::BufferCopy(const FCommandSubmit& submit, u32 i, const FCommandBufferCopy& cmd) const
{
    const auto& cmd_pack = m_queue->m_cmd;
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
}

void D3d12CommandInterpreter::Render(const FCommandSubmit& submit, u32 i, const FCommandRender& cmd)
{
    m_context.Reset();
    const auto& info = submit.RenderInfos[cmd.InfoIndex];
    {
        auto num_rtv = std::min(info.NumRtv, 8u);
        D3D12_RENDER_PASS_RENDER_TARGET_DESC rts[num_rtv];
        D3D12_RENDER_PASS_DEPTH_STENCIL_DESC ds[info.Dsv.IsEmpty() ? 0 : 1];
        if (!info.Dsv.IsEmpty())
        {
            // todo dsv
        }
        for (u32 n = 0; n < num_rtv; ++n)
        {
            const auto& rtv = info.Rtv[n];
            const auto& load = info.RtvLoadOp[n];
            const auto& store = info.RtvStoreOp[n];
            D3D12_RENDER_PASS_RENDER_TARGET_DESC desc{};
            desc.cpuDescriptor = GetRtv(rtv.Get(submit));
            switch (load)
            {
            case FLoadOp::Load:
                desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
                break;
            case FLoadOp::Clear:
                desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
                desc.BeginningAccess.Clear.ClearValue.Color[0] = info.Color[0];
                desc.BeginningAccess.Clear.ClearValue.Color[1] = info.Color[1];
                desc.BeginningAccess.Clear.ClearValue.Color[2] = info.Color[2];
                desc.BeginningAccess.Clear.ClearValue.Color[3] = info.Color[3];
                break;
            case FLoadOp::Discard:
                desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
                break;
            case FLoadOp::NoAccess:
                desc.BeginningAccess.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
                break;
            }
            switch (store)
            {
            case FStoreOp::Store:
                desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
                break;
            case FStoreOp::Discard:
                desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
                break;
            case FStoreOp::Resolve:
                desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
                {
                    const auto& res_info = submit.ResolveInfos[info.ResolveInfoIndex[n]];
                    desc.EndingAccess.Resolve.Format = ToDx(res_info.Format);
                    desc.EndingAccess.Resolve.pSrcResource = GetResource(res_info.Src.Get(submit));
                    desc.EndingAccess.Resolve.pDstResource = GetResource(res_info.Dst.Get(submit));
                    desc.EndingAccess.Resolve.SubresourceCount = 0;
                    desc.EndingAccess.Resolve.ResolveMode = ToDx(res_info.Mode);
                }
                break;
            case FStoreOp::NoAccess:
                desc.EndingAccess.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
                break;
            }
            rts[n] = desc;
        }
        D3D12_RENDER_PASS_FLAGS flags = D3D12_RENDER_PASS_FLAG_NONE;
        if (info.HasUavWrites) flags |= D3D12_RENDER_PASS_FLAG_ALLOW_UAV_WRITES;
        m_queue->m_cmd.m_list4->BeginRenderPass(info.NumRtv, rts, info.Dsv.IsEmpty() ? nullptr : ds, flags);
    }
    const auto commands = std::span(submit.RenderCommands + cmd.CommandStartIndex, cmd.CommandCount);
    for (u32 j = 0; j < commands.size(); j++)
    {
        const auto& item = commands[j];
        switch (item.Type)
        {
        case FCommandType::None:
            continue;
        case FCommandType::Label:
            Label(submit, item.Label);
            continue;
        case FCommandType::BeginScope:
            BeginScope(submit, item.BeginScope);
            continue;
        case FCommandType::EndScope:
            EndScope(submit, item.EndScope);
            continue;
        case FCommandType::SetPipeline:
            SetPipeline(m_queue->m_cmd, item.SetPipeline.Pipeline, j);
            continue;
        case FCommandType::SetBinding:
            continue; // todo
        case FCommandType::SetViewportScissor:
            RenderSetViewportScissor(submit, j, item.SetViewportScissor);
            continue;
        case FCommandType::SetMeshBuffers:
            continue; // todo
        case FCommandType::Draw:
            RenderDraw(submit, j, item.Draw);
            continue;
        case FCommandType::Dispatch:
            continue; // todo
        case FCommandType::Present:
        case FCommandType::Barrier:
        case FCommandType::ClearColor:
        case FCommandType::ClearDepthStencil:
        case FCommandType::Bind:
        case FCommandType::BufferCopy:
        case FCommandType::Render:
        case FCommandType::Compute:
            COPLT_THROW("Main commands cannot be placed in the sub command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));
    }
    {
        m_queue->m_cmd.m_list4->EndRenderPass();
    }
}

void D3d12CommandInterpreter::RenderDraw(const FCommandSubmit& submit, u32 i, const FCommandDraw& cmd) const
{
    if (cmd.Indexed)
    {
        m_queue->m_cmd->DrawIndexedInstanced(cmd.VertexOrIndexCount, cmd.InstanceCount, cmd.FirstVertexOrIndex, cmd.VertexOffset, cmd.FirstInstance);
    }
    else
    {
        m_queue->m_cmd->DrawInstanced(cmd.VertexOrIndexCount, cmd.InstanceCount, cmd.FirstVertexOrIndex, cmd.FirstInstance);
    }
}

void D3d12CommandInterpreter::RenderSetViewportScissor(const FCommandSubmit& submit, u32 i, const FCommandSetViewportScissor& cmd) const
{
    m_queue->m_cmd->RSSetViewports(cmd.ViewportCount, reinterpret_cast<const D3D12_VIEWPORT*>(submit.Viewports + cmd.ViewportIndex));
    m_queue->m_cmd->RSSetScissorRects(cmd.ScissorRectCount, reinterpret_cast<const D3D12_RECT*>(submit.Rects + cmd.ScissorRectIndex));
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
