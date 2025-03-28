#include "Record.h"

#include <pix3.h>

#include "Output.h"
#include "../FFI/Pipeline.h"
#include "../Include/GraphicsFormat.h"
#include "../Include/PipelineState.h"

using namespace Coplt;

void D3d12GpuRecord::PipelineContext::Reset()
{
    Pipeline = nullptr;
    Layout = nullptr;
    GPipeline = nullptr;
    // Binding = nullptr;
    PipelineChanged = false;
    BindingChanged = false;
}

void D3d12GpuRecord::PipelineContext::SetPipeline(NonNull<FShaderPipeline> pipeline, u32 i)
{
    if (pipeline == Pipeline) return;
    Pipeline = pipeline->QueryInterface<FD3d12PipelineState>();
    if (!Pipeline)
    {
        COPLT_THROW_FMT(
            "Pipeline({:#x}) comes from different backends at cmd {}",
            static_cast<size_t>(pipeline), i
        );
    }
    Layout = pipeline->GetLayout()->QueryInterface<FD3d12ShaderLayout>();
    if (!Layout)
        COPLT_THROW("Shader layout from different backends");
    const auto stages = pipeline->GetStages();
    GPipeline = nullptr;
    if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        GPipeline = pipeline->QueryInterface<FD3d12GraphicsShaderPipeline>();
        if (!GPipeline)
            COPLT_THROW("Pipeline from different backends or pipeline not a graphics pipeline");
    }
    PipelineChanged = true;
}

D3d12GpuRecord::D3d12GpuRecord(const NonNull<D3d12GpuIsolate> isolate)
    : FGpuRecordData(isolate->m_device->m_instance->m_allocator.get()), m_device(isolate->m_device)
{
    Id = m_isolate_id = isolate->m_object_id;
    m_record_id = isolate->m_record_inc++;
    m_context = new D3d12RecordContext(isolate);
    m_storage = new D3d12RecordStorage(isolate, m_context);
    m_barrier_recorder = isolate->m_barrier_analyzer->CreateRecorder(m_storage);
    Context = m_context.get();
}

FResult D3d12GpuRecord::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FGpuRecordData* D3d12GpuRecord::GpuFGpuRecordData() noexcept
{
    return this;
}

FGpuRecordData* D3d12GpuRecord::Data() noexcept
{
    return this;
}

const FGpuRecordData* D3d12GpuRecord::Data() const noexcept
{
    return this;
}

const Rc<D3d12RecordStorage>& D3d12GpuRecord::Storage()
{
    return m_storage;
}

const Rc<ID3d12BarrierRecorder>& D3d12GpuRecord::BarrierRecorder()
{
    return m_barrier_recorder;
}

void D3d12GpuRecord::RegisterWaitPoint(QueueWaitPoint&& wait_point)
{
    m_queue_wait_points.push_back(std::move(wait_point));
}

void D3d12GpuRecord::WaitAndRecycle(const HANDLE event)
{
    for (auto& wait_point : m_queue_wait_points)
    {
        wait_point.Wait(event);
    }
    Recycle();
}

void D3d12GpuRecord::Recycle()
{
    m_queue_wait_points.clear();
    m_barrier_recorder->Clear();
    m_storage->Clear();
    m_context->Recycle();
    m_resources_owner.clear();
    m_pipeline_context.Reset();
    ClearData();
    Ended = false;
}

FResult D3d12GpuRecord::End() noexcept
{
    return feb([&]
    {
        DoEnd();
    });
}

void D3d12GpuRecord::EnsureEnd()
{
    if (Ended) return;
    DoEnd();
}

void D3d12GpuRecord::DoEnd()
{
    if (Ended)
        COPLT_THROW("End cannot be called multiple times");
    if (Resources.size() >= std::numeric_limits<u32>::max())
        COPLT_THROW("Too many resources");
    ReadyResource();
    Interpret();
    Ended = true;
    Version++;
}

FCmdRes& D3d12GpuRecord::GetRes(const FCmdResRef& ref)
{
    return ref.Get(Resources);
}

D3d12RentedCommandList& D3d12GpuRecord::CurList() const
{
    return m_storage->CurList();
}

void D3d12GpuRecord::ReadyResource()
{
    m_resources_owner.reserve(Resources.size());
    for (u32 i = 0; i < Resources.size(); ++i)
    {
        const auto& res = Resources[i];
        m_resources_owner.push_back(Rc<FGpuObject>::UnsafeClone(res.GetObjectPtr()));
    }
}

void D3d12GpuRecord::Interpret()
{
    m_storage->StartRecord(Mode);
    m_barrier_recorder->StartRecord(Resources.AsSpan());
    const auto commands = Commands.AsSpan();
    for (u32 i = 0; i < commands.size(); ++i)
    {
        const auto& command = commands[i];
        switch (command.Type)
        {
        case FCmdType::None:
            break;
        case FCmdType::End:
            if (m_state == InterpretState::Render)
                Interpret_RenderEnd(i, m_cur_render.Cmd);
            else
                COPLT_THROW("Cannot use End in main scope");
            break;
        case FCmdType::Label:
            Interpret_Label(i, command.Label);
            break;
        case FCmdType::BeginScope:
            Interpret_BeginScope(i, command.BeginScope);
            break;
        case FCmdType::EndScope:
            Interpret_EndScope(i, command.EndScope);
            break;
        case FCmdType::PreparePresent:
            Interpret_PreparePresent(i, command.PreparePresent);
            break;
        case FCmdType::ClearColor:
            Interpret_ClearColor(i, command.ClearColor);
            break;
        case FCmdType::ClearDepthStencil:
            Interpret_ClearDepthStencil(i, command.ClearDepthStencil);
            break;
        case FCmdType::BufferCopy:
            Interpret_BufferCopy(i, command.BufferCopy);
            break;
        case FCmdType::Render:
            Interpret_Render(i, command.Render);
            break;
        case FCmdType::Compute:
            COPLT_THROW("TODO");
        case FCmdType::SetPipeline:
            Interpret_SetPipeline(i, command.SetPipeline);
            break;
        case FCmdType::SetBinding:
            COPLT_THROW("TODO");
        case FCmdType::SetViewportScissor:
            Interpret_SetViewportScissor(i, command.SetViewportScissor);
            break;
        case FCmdType::SetMeshBuffers:
            COPLT_THROW("TODO");
        case FCmdType::Draw:
            Interpret_Draw(i, command.Draw);
            break;
        case FCmdType::Dispatch:
            COPLT_THROW("TODO");
            break;
        }
    }
    m_barrier_recorder->EndRecord();
    m_storage->EndRecord();
}

void D3d12GpuRecord::Interpret_Label(u32 i, const FCmdLabel& cmd) const
{
    if (!m_device->Debug()) return;
    const auto& list = CurList();
    if (!list->g0) return;
    auto color = PIX_COLOR_DEFAULT;
    if (cmd.HasColor) color = PIX_COLOR(cmd.Color[0], cmd.Color[1], cmd.Color[2]);
    if (cmd.StrType == FStrType::Str8)
    {
        PIXSetMarker(list->g0.Get(), color, "%s", reinterpret_cast<const char*>(Blob.data() + cmd.StringIndex));
    }
    else
    {
        PIXSetMarker(list->g0.Get(), color, L"%s", reinterpret_cast<const wchar_t*>(Blob.data() + cmd.StringIndex));
    }
}

void D3d12GpuRecord::Interpret_BeginScope(u32 i, const FCmdBeginScope& cmd) const
{
    if (!m_device->Debug()) return;
    const auto& list = CurList();
    if (!list->g0) return;
    u32 color = PIX_COLOR_DEFAULT;
    if (cmd.HasColor) color = PIX_COLOR(cmd.Color[0], cmd.Color[1], cmd.Color[2]);
    if (cmd.StrType == FStrType::Str8)
    {
        PIXBeginEvent(list->g0.Get(), color, "%s", reinterpret_cast<const char*>(Blob.data() + cmd.StringIndex));
    }
    else
    {
        PIXBeginEvent(list->g0.Get(), color, L"%s", reinterpret_cast<const wchar_t*>(Blob.data() + cmd.StringIndex));
    }
}

void D3d12GpuRecord::Interpret_EndScope(u32 i, const FCmdEndScope& cmd) const
{
    if (!m_device->Debug()) return;
    const auto& list = CurList();
    if (!list->g0) return;
    PIXEndEvent(list->g0.Get());
}

void D3d12GpuRecord::Interpret_PreparePresent(const u32 i, const FCmdPreparePresent& cmd) const
{
    if (m_state != InterpretState::Main)
        COPLT_THROW("Cannot use PreparePresent in sub scope");
    if (Mode != FGpuRecordMode::Direct)
        COPLT_THROW("Can only present on the direct mode");
    m_barrier_recorder->OnUse(cmd.Output, ResAccess::None, ResUsage::Common, ResLayout::Common);
}

void D3d12GpuRecord::Interpret_ClearColor(const u32 i, const FCmdClearColor& cmd)
{
    if (m_state != InterpretState::Main)
        COPLT_THROW("Cannot use ClearColor in sub scope");
    m_barrier_recorder->OnUse(cmd.Image, ResAccess::RenderTargetWrite, ResUsage::Common, ResLayout::RenderTarget);
    m_barrier_recorder->OnCmd();
    const auto rtv = GetRtv(GetRes(cmd.Image));
    CurList()->g0->ClearRenderTargetView(
        rtv, cmd.Color, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&PayloadRect[cmd.RectIndex])
    );
}

void D3d12GpuRecord::Interpret_ClearDepthStencil(const u32 i, const FCmdClearDepthStencil& cmd)
{
    if (m_state != InterpretState::Main)
        COPLT_THROW("Cannot use ClearDepthStencil in sub scope");
    m_barrier_recorder->OnUse(cmd.Image, ResAccess::DepthStencilWrite, ResUsage::Common, ResLayout::DepthStencilWrite);
    m_barrier_recorder->OnCmd();
    const auto dsv = GetDsv(GetRes(cmd.Image));
    D3D12_CLEAR_FLAGS flags{};
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Depth)) flags |= D3D12_CLEAR_FLAG_DEPTH;
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Stencil)) flags |= D3D12_CLEAR_FLAG_STENCIL;
    CurList()->g0->ClearDepthStencilView(
        dsv, flags, cmd.Depth, cmd.Stencil, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&PayloadRect[cmd.RectIndex])
    );
}

void D3d12GpuRecord::Interpret_BufferCopy(u32 i, const FCmdBufferCopy& cmd)
{
    if (m_state != InterpretState::Main)
        COPLT_THROW("Cannot use BufferCopy in sub scope");
    const auto& range = PayloadBufferCopyRange[cmd.RangeIndex];
    if (cmd.SrcType == FBufferRefType2::Buffer && cmd.DstType == FBufferRefType2::Buffer)
    {
        m_barrier_recorder->OnUse(cmd.Dst.Buffer, ResAccess::CopyDestWrite, ResUsage::Common, ResLayout::Common);
        m_barrier_recorder->OnUse(cmd.Src.Buffer, ResAccess::CopySourceRead, ResUsage::Common, ResLayout::Common);
        m_barrier_recorder->OnCmd();
        const auto dst = GetResource(GetRes(cmd.Dst.Buffer));
        const auto src = GetResource(GetRes(cmd.Src.Buffer));
        if (range.Size == std::numeric_limits<u64>::max())
        {
            CurList()->g0->CopyResource(dst, src);
        }
        else
        {
            CurList()->g0->CopyBufferRegion(dst, range.DstOffset, src, range.SrcOffset, range.Size);
        }
    }
    else if (cmd.SrcType == FBufferRefType2::Upload && cmd.DstType == FBufferRefType2::Buffer)
    {
        m_barrier_recorder->OnUse(cmd.Dst.Buffer, ResAccess::CopyDestWrite, ResUsage::Common, ResLayout::Common);
        m_barrier_recorder->OnCmd();
        const auto dst = GetResource(GetRes(cmd.Dst.Buffer));
        if (cmd.Src.Upload.Index >= m_context->m_upload_buffers.size())
            COPLT_THROW_FMT("Index out of bounds at command {}", i);
        const auto& src_obj = m_context->m_upload_buffers[cmd.Src.Upload.Index];
        if (range.SrcOffset + range.Size >= src_obj.m_size)
            COPLT_THROW_FMT("Size out of range at command {}", i);
        const auto src = src_obj.m_resource.m_resource.Get();
        CurList()->g0->CopyBufferRegion(dst, range.DstOffset, src, range.SrcOffset, range.Size);
    }
    else
    {
        COPLT_THROW_FMT(
            "Unsupported copy combination {{ SrcType = {} DstType = {} }} at command {}",
            static_cast<u8>(cmd.SrcType), static_cast<u8>(cmd.DstType), i
        );
    }
}

void D3d12GpuRecord::Interpret_Render(const u32 i, const FCmdRender& cmd)
{
    if (m_state != InterpretState::Main)
        COPLT_THROW("Cannot use Render in sub scope");
    if (Mode != FGpuRecordMode::Direct)
        COPLT_THROW("Render can only be used in Direct mode");
    m_state = InterpretState::Render;
    m_cur_render = RenderState{.StartIndex = i, .Cmd = cmd};
    const auto& info = PayloadRenderInfo[cmd.InfoIndex];
    auto num_rtv = std::min(info.NumRtv, 8u);
    if (CurList()->g4)
    {
        D3D12_RENDER_PASS_RENDER_TARGET_DESC rts[num_rtv];
        D3D12_RENDER_PASS_DEPTH_STENCIL_DESC ds[info.Dsv ? 1 : 0];
        if (info.Dsv)
        {
            m_barrier_recorder->OnUse(info.Dsv, ResAccess::DepthStencilWrite, ResUsage::Common, ResLayout::DepthStencilWrite);
            const auto& dsv = GetRes(info.Dsv);
            const auto& d_load = info.DsvLoadOp[0];
            const auto& d_store = info.DsvStoreOp[0];
            const auto& s_load = info.DsvLoadOp[1];
            const auto& s_store = info.DsvStoreOp[1];
            D3D12_RENDER_PASS_DEPTH_STENCIL_DESC desc{};
            desc.cpuDescriptor = GetDsv(dsv);
            desc.DepthBeginningAccess = ToDx(d_load, [&](D3D12_CLEAR_VALUE& cv)
            {
                cv.Format = ToDx(info.DsvFormat);
                cv.DepthStencil.Depth = info.Depth;
            });
            desc.DepthBeginningAccess = ToDx(s_load, [&](D3D12_CLEAR_VALUE& cv)
            {
                cv.Format = ToDx(info.DsvFormat);
                cv.DepthStencil.Stencil = info.Stencil;
            });
            desc.DepthEndingAccess = ToDx(d_store, [&](D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS& res)
            {
                const auto& res_info = PayloadResolveInfo[info.DsvResolveInfoIndex];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetResource(GetRes(res_info.Src));
                res.pDstResource = GetResource(GetRes(res_info.Dst));
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            desc.StencilEndingAccess = ToDx(s_store, [&](D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS& res)
            {
                const auto& res_info = PayloadResolveInfo[info.DsvResolveInfoIndex];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetResource(GetRes(res_info.Src));
                res.pDstResource = GetResource(GetRes(res_info.Dst));
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            ds[0] = desc;
        }
        for (u32 n = 0; n < num_rtv; ++n)
        {
            m_barrier_recorder->OnUse(info.Rtv[n], ResAccess::RenderTargetWrite, ResUsage::Common, ResLayout::RenderTarget);
            const auto& rtv = GetRes(info.Rtv[n]);
            const auto& load = info.RtvLoadOp[n];
            const auto& store = info.RtvStoreOp[n];
            D3D12_RENDER_PASS_RENDER_TARGET_DESC desc{};
            desc.cpuDescriptor = GetRtv(rtv);
            desc.BeginningAccess = ToDx(load, [&](D3D12_CLEAR_VALUE& cv)
            {
                cv.Format = ToDx(info.RtvFormat[n]);
                cv.Color[0] = info.Color[0];
                cv.Color[1] = info.Color[1];
                cv.Color[2] = info.Color[2];
                cv.Color[3] = info.Color[3];
            });
            desc.EndingAccess = ToDx(store, [&](D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS& res)
            {
                const auto& res_info = PayloadResolveInfo[info.ResolveInfoIndex[n]];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetResource(GetRes(res_info.Src));
                res.pDstResource = GetResource(GetRes(res_info.Dst));
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            rts[n] = desc;
        }
        D3D12_RENDER_PASS_FLAGS flags = D3D12_RENDER_PASS_FLAG_NONE;
        if (info.HasUavWrites) flags |= D3D12_RENDER_PASS_FLAG_ALLOW_UAV_WRITES;
        m_barrier_recorder->OnCmd();
        CurList()->g4->BeginRenderPass(info.NumRtv, rts, info.Dsv ? ds : nullptr, flags);
    }
    else
    {
        COPLT_THROW("TODO");
    }
}

void D3d12GpuRecord::Interpret_RenderEnd(u32 i, const FCmdRender& cmd)
{
    if (m_state != InterpretState::Render)
        COPLT_THROW("Cannot use End in main scope");
    m_state = InterpretState::Main;
    const auto& list = CurList();
    if (list->g4)
    {
        list->g4->EndRenderPass();
    }
}

void D3d12GpuRecord::Interpret_SetPipeline(const u32 i, const FCmdSetPipeline& cmd)
{
    if (m_state != InterpretState::Render && m_state != InterpretState::Compute)
        COPLT_THROW("Cannot use SetPipeline in main scope");
    SetPipeline(cmd.Pipeline, i);
}

void D3d12GpuRecord::Interpret_SetViewportScissor(u32 i, const FCmdSetViewportScissor& cmd) const
{
    if (m_state != InterpretState::Render)
        COPLT_THROW("Cannot use SetPipeline in main scope or compute scope");
    const auto& list = CurList();
    list->g0->RSSetViewports(cmd.ViewportCount, reinterpret_cast<const D3D12_VIEWPORT*>(PayloadViewport.data() + cmd.ViewportIndex));
    list->g0->RSSetScissorRects(cmd.ScissorRectCount, reinterpret_cast<const D3D12_RECT*>(PayloadRect.data() + cmd.ScissorRectIndex));
}

void D3d12GpuRecord::Interpret_Draw(u32 i, const FCmdDraw& cmd) const
{
    if (m_state != InterpretState::Render)
        COPLT_THROW("Cannot use SetPipeline in main scope or compute scope");
    const auto& list = CurList();
    if (cmd.Indexed)
    {
        list->g0->DrawIndexedInstanced(cmd.VertexOrIndexCount, cmd.InstanceCount, cmd.FirstVertexOrIndex, cmd.VertexOffset, cmd.FirstInstance);
    }
    else
    {
        list->g0->DrawInstanced(cmd.VertexOrIndexCount, cmd.InstanceCount, cmd.FirstVertexOrIndex, cmd.FirstInstance);
    }
}

void D3d12GpuRecord::SetPipeline(NonNull<FShaderPipeline> pipeline, u32 i)
{
    if (pipeline == m_pipeline_context.Pipeline) return;
    m_pipeline_context.SetPipeline(pipeline, i);
    const auto stages = pipeline->GetStages();
    const auto& list = CurList();
    if (HasFlags(stages, FShaderStageFlags::Compute))
    {
        list->g0->SetComputeRootSignature(static_cast<ID3D12RootSignature*>(m_pipeline_context.Layout->GetRootSignaturePtr()));
    }
    else if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        const auto& states = m_pipeline_context.GPipeline->GetGraphicsState();
        list->g0->IASetPrimitiveTopology(ToDx(states->Topology));
        list->g0->SetGraphicsRootSignature(static_cast<ID3D12RootSignature*>(m_pipeline_context.Layout->GetRootSignaturePtr()));
    }
    list->g0->SetPipelineState(static_cast<ID3D12PipelineState*>(m_pipeline_context.Pipeline->GetPipelineStatePtr()));
}

NonNull<ID3D12Resource> Coplt::GetResource(const FCmdRes& res)
{
    switch (res.Type)
    {
    case FCmdResType::Image:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuImage>();
            return obj->GetResourcePtr();
        }
    case FCmdResType::Buffer:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuBuffer>();
            return obj->GetResourcePtr();
        }
    case FCmdResType::Output:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuOutput2>();
            return obj->GetResourcePtr();
        }
    }
    return nullptr;
}

NonNull<FGpuBufferData> Coplt::GetBufferData(const FCmdRes& res)
{
    switch (res.Type)
    {
    case FCmdResType::Buffer:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuBuffer>();
            return obj->Data();
        }
    case FCmdResType::Image:
    case FCmdResType::Output:
        return nullptr;
    }
    return nullptr;
}

NonNull<FGpuImageData> Coplt::GetImageData(const FCmdRes& res)
{
    switch (res.Type)
    {
    case FCmdResType::Image:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuImage>();
            return obj->Data();
        }
    case FCmdResType::Output:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuOutput2>();
            return obj->ImageData();
        }
    case FCmdResType::Buffer:
        return nullptr;
    }
    return nullptr;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE Coplt::GetRtv(const FCmdRes& res)
{
    switch (res.Type)
    {
    case FCmdResType::Image:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuImage>();
            COPLT_THROW("TODO");
        }
    case FCmdResType::Buffer:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuBuffer>();
            COPLT_THROW("TODO");
        }
    case FCmdResType::Output:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuOutput2>();
            return obj->GetRtv();
        }
    }
    COPLT_THROW("Unreachable");
}

CD3DX12_CPU_DESCRIPTOR_HANDLE Coplt::GetDsv(const FCmdRes& res)
{
    switch (res.Type)
    {
    case FCmdResType::Image:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuImage>();
            COPLT_THROW("TODO");
        }
    case FCmdResType::Buffer:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuBuffer>();
            COPLT_THROW("TODO");
        }
    case FCmdResType::Output:
        {
            COPLT_THROW("Output dose not support Dsv");
        }
    }
    COPLT_THROW("Unreachable");
}

NonNull<LayoutState> Coplt::GetState(const FCmdRes& res)
{
    switch (res.Type)
    {
    case FCmdResType::Image:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuImage>();
            return obj->State();
        }
    case FCmdResType::Buffer:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuBuffer>();
            return obj->State();
        }
    case FCmdResType::Output:
        {
            const NonNull obj = res.Image->QueryInterface<ID3d12GpuOutput2>();
            return obj->State();
        }
    }
    COPLT_THROW("Unreachable");
}
