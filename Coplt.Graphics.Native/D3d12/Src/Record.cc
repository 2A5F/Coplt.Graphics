#include "Record.h"

#include <pix3.h>

#include "Output.h"

using namespace Coplt;

D3d12GpuRecord::D3d12GpuRecord(const NonNull<D3d12GpuIsolate> isolate)
    : FGpuRecordData(isolate->m_device->m_instance->m_allocator.get()), m_device(isolate->m_device)
{
    m_isolate_id = isolate->m_object_id;
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
        case FCmdType::Render:
        case FCmdType::Compute:
        case FCmdType::SetPipeline:
        case FCmdType::SetBinding:
        case FCmdType::SetViewportScissor:
        case FCmdType::SetMeshBuffers:
        case FCmdType::Draw:
        case FCmdType::Dispatch:
            COPLT_THROW("TODO");
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
    auto color = PIX_COLOR_DEFAULT;
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

void D3d12GpuRecord::Interpret_Render(const u32 i, const FCmdRender& cmd)
{
    if (m_state != InterpretState::Main)
        COPLT_THROW("Cannot use Render in sub scope");
    m_state = InterpretState::Render;
    m_cur_render = RenderState{.StartIndex = i, .Cmd = cmd};
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
            return obj->State();
        }
    }
    COPLT_THROW("Unreachable");
}
