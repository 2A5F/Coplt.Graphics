#include "Record.h"

#include "Output.h"

using namespace Coplt;
using namespace Recording;

ResUseType Recording::GetUseType(const D3D12_BARRIER_ACCESS access, const ResUseType UavUse)
{
    if (access & D3D12_BARRIER_ACCESS_NO_ACCESS) return ResUseType::None;
    if (access & D3D12_BARRIER_ACCESS_UNORDERED_ACCESS) return UavUse;
    if (
        access & D3D12_BARRIER_ACCESS_RENDER_TARGET || access & D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE
        || access & D3D12_BARRIER_ACCESS_STREAM_OUTPUT || access & D3D12_BARRIER_ACCESS_COPY_DEST
        || access & D3D12_BARRIER_ACCESS_RESOLVE_DEST || access & D3D12_BARRIER_ACCESS_RAYTRACING_ACCELERATION_STRUCTURE_WRITE
        || access & D3D12_BARRIER_ACCESS_VIDEO_DECODE_WRITE
        || access & D3D12_BARRIER_ACCESS_VIDEO_PROCESS_WRITE
        || access & D3D12_BARRIER_ACCESS_VIDEO_ENCODE_WRITE
    )
        return ResUseType::Write;
    return ResUseType::Read;
}

D3d12GpuRecord::D3d12GpuRecord(const NonNull<D3d12GpuIsolate> isolate)
    : FGpuRecordData(isolate->m_device->m_instance->m_allocator.get()), m_device(isolate->m_device)
{
    m_isolate_id = isolate->m_object_id;
    m_record_id = isolate->m_record_inc++;
    m_barrier_analyzer = new D3d12BarrierAnalyzer();
    m_context = new D3d12RecordContext(isolate);
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
    m_resources_owner.reserve(Resources.size());
    for (u32 i = 0; i < Resources.size(); ++i)
    {
        const auto& res = Resources[i];
        m_resources_owner.push_back(Rc<FUnknown>::UnsafeClone(res.GetObjectPtr()));
    }
    Analyze();
    Ended = true;
}

FCmdRes& D3d12GpuRecord::GetRes(const FCmdResRef& ref)
{
    return ref.Get(Resources);
}

void D3d12GpuRecord::Analyze()
{
    m_barrier_analyzer->Analyzer(NonNull<D3d12GpuRecord>::Unchecked(this));
}

void D3d12GpuRecord::Interpret(const CmdListPack& clp, const u32 cmd_index, const u32 cmd_count)
{
    const auto commands = Commands.AsSpan();
    for (u32 n = 0; n < cmd_count; ++n)
    {
        const auto i = cmd_index + n;
        const auto& command = commands[i];
        switch (command.Type)
        {
        case FCmdType::None:
            continue;
        case FCmdType::Label:
            // todo
            continue;
        case FCmdType::BeginScope:
            // todo
            continue;
        case FCmdType::EndScope:
            // todo
            continue;
        case FCmdType::ClearColor:
            Interpret_ClearColor(clp, i, command.ClearColor);
            continue;
        case FCmdType::ClearDepthStencil:
            Interpret_ClearDepthStencil(clp, i, command.ClearDepthStencil);
            continue;
        }
    }
}

void D3d12GpuRecord::Interpret_ClearColor(const CmdListPack& clp, u32 i, const FCmdClearColor& cmd)
{
    const auto rtv = GetRtv(GetRes(cmd.Image));
    clp->ClearRenderTargetView(
        rtv, cmd.Color, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&PayloadRect[cmd.RectIndex])
    );
}

void D3d12GpuRecord::Interpret_ClearDepthStencil(const CmdListPack& clp, u32 i, const FCmdClearDepthStencil& cmd)
{
    const auto dsv = GetDsv(GetRes(cmd.Image));
    D3D12_CLEAR_FLAGS flags{};
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Depth)) flags |= D3D12_CLEAR_FLAG_DEPTH;
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Stencil)) flags |= D3D12_CLEAR_FLAG_STENCIL;
    clp->ClearDepthStencilView(
        dsv, flags, cmd.Depth, cmd.Stencil, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&PayloadRect[cmd.RectIndex])
    );
}

D3d12BarrierAnalyzer::ResInfo::ResInfo(const FCmdResType ResType) : ResInfo()
{
    this->ResType = ResType;
}

bool D3d12BarrierAnalyzer::ResInfo::IsReadCompatible(const D3D12_BARRIER_LAYOUT Layout) const
{
    return Type == ResUseType::Read && this->Layout == Layout;
}

bool D3d12BarrierAnalyzer::ResInfo::MergeRead(const D3D12_BARRIER_ACCESS Access, const D3D12_BARRIER_SYNC Sync)
{
    const auto old_access = Access;
    const auto old_sync = Sync;
    this->Access |= Access;
    this->Sync |= Sync;
    return this->Access != old_access || this->Sync != old_sync;
}

D3d12BarrierAnalyzer::D3d12BarrierAnalyzer() : m_record(NonNull<D3d12GpuRecord>::Unchecked(nullptr))
{
}

void D3d12BarrierAnalyzer::Recycle()
{
    m_res_inputs.clear();
    m_res_outputs.clear();
    m_resources.clear();
    m_parts.clear();
    m_begin_global_barriers.clear();
    m_begin_texture_barriers.clear();
    m_begin_buffer_barriers.clear();
    m_end_global_barriers.clear();
    m_end_texture_barriers.clear();
    m_end_buffer_barriers.clear();
    m_current_barrier_begin_part = 0;
    m_current_cmd_part = 0;
}

void D3d12BarrierAnalyzer::Analyzer(const NonNull<D3d12GpuRecord> record)
{
    Recycle();
    m_record = record;
    m_resources.reserve(m_record->Resources.size());
    for (u32 i = 0; i < m_record->Resources.size(); ++i)
    {
        const auto& res = m_record->Resources[i];
        m_resources.push_back(ResInfo(res.Type));
    }
    m_parts.push_back(Part{.Barrier = {.Type = PartType::BarrierBegin}});
    m_parts.push_back(Part{.Cmd = {}});
    m_current_barrier_begin_part = 0;
    m_current_cmd_part = 1;
    const auto commands = m_record->Commands.AsSpan();
    if (commands.size() >= std::numeric_limits<u32>::max())
        COPLT_THROW("Too many commands");
    for (u32 i = 0; i < commands.size(); ++i)
    {
        const auto& command = commands[i];
        switch (command.Type)
        {
        case FCmdType::None:
        case FCmdType::Label:
        case FCmdType::BeginScope:
        case FCmdType::EndScope:
            goto CmdNext;
        case FCmdType::ClearColor:
            Analyze_ClearColor(i, command.ClearColor);
            goto CmdNext;
        case FCmdType::ClearDepthStencil:
            Analyze_ClearDepthStencil(i, command.ClearDepthStencil);
            goto CmdNext;
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(command.Type));
    CmdNext:
        CmdNext();
        continue;
    }
    BuildInputOutput();
}

D3d12BarrierAnalyzer::ResInfo& D3d12BarrierAnalyzer::GetInfo(const FCmdResRef& ref)
{
    return m_resources[ref.IndexPlusOne - 1];
}

std::pair<NonNull<FCmdRes>, NonNull<D3d12BarrierAnalyzer::ResInfo>> D3d12BarrierAnalyzer::Get(const FCmdResRef& ref)
{
    return {&ref.Get(m_record->Resources), &m_resources[ref.IndexPlusOne - 1]};
}

NonNull<ID3D12Resource> D3d12BarrierAnalyzer::GetResource(const u32 index) const
{
    const auto& res = m_record->Resources[index];
    return Coplt::GetResource(res);
}

NonNull<FGpuBufferData> D3d12BarrierAnalyzer::GetBufferData(const u32 index) const
{
    const auto& res = m_record->Resources[index];
    return Coplt::GetBufferData(res);
}

NonNull<FGpuImageData> D3d12BarrierAnalyzer::GetImageData(u32 index) const
{
    const auto& res = m_record->Resources[index];
    return Coplt::GetImageData(res);
}

void D3d12BarrierAnalyzer::MarkUse(
    ResInfo& Info, const D3D12_BARRIER_ACCESS Access, const D3D12_BARRIER_LAYOUT Layout, D3D12_BARRIER_SYNC Sync, const ResUseType UavUse
)
{
    const auto type = GetUseType(Access, UavUse);
    if (Info.First)
    {
        Info.First = false;
        Info.Type = type;
        Info.Access = Access;
        Info.Layout = Layout;
        Info.Sync = Sync;
        return;
    }
    COPLT_THROW("TODO");
    // if (type == ResUseType::Read && Info.IsReadCompatible(Layout))
    // {
    //     Info.MergeRead(Access, Stages);
    //     return;
    // }
    // const auto& cur_barrier = m_parts[m_current_barrier_begin_part].Barrier;
    // const auto& cur_cmd = m_parts[m_current_cmd_part].Cmd;
    // if (Info.WasDefInput)
    // {
    //
    // }
    // else if (Info.CurrentBarrierPart == COPLT_U32_MAX)
    // {
    //
    // }
    // return;
}

void D3d12BarrierAnalyzer::CmdNext()
{
    m_parts[m_current_cmd_part].Cmd.Count++;
}

void D3d12BarrierAnalyzer::BuildInputOutput()
{
    for (u32 i = 0; i < m_resources.size(); ++i)
    {
        const auto& res = m_resources[i];
        const auto use = ResUse{
            .ResIndex = i,
            .Layout = res.Layout,
            .Access = res.Access,
            .Sync = res.Sync,
            .Type = res.Type,
        };
        m_res_outputs.push_back(use);
        if (res.WasDefInput) continue;
        m_res_inputs.push_back(use);
    }
}

void D3d12BarrierAnalyzer::Analyze_ClearColor(const u32 i, const FCmdClearColor& cmd)
{
    auto& info = GetInfo(cmd.Image);
    MarkUse(info, D3D12_BARRIER_ACCESS_RENDER_TARGET, D3D12_BARRIER_LAYOUT_RENDER_TARGET, D3D12_BARRIER_SYNC_RENDER_TARGET);
}

void D3d12BarrierAnalyzer::Analyze_ClearDepthStencil(const u32 i, const FCmdClearDepthStencil& cmd)
{
    auto& info = GetInfo(cmd.Image);
    MarkUse(info, D3D12_BARRIER_ACCESS_DEPTH_STENCIL_WRITE, D3D12_BARRIER_LAYOUT_DEPTH_STENCIL_WRITE, D3D12_BARRIER_SYNC_DEPTH_STENCIL);
}

void D3d12BarrierAnalyzer::Interpret(NonNull<D3d12GpuRecord> record, CmdListPack& cmd)
{
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
