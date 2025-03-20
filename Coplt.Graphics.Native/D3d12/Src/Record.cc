#include "Record.h"

using namespace Coplt;
using namespace Recording;

bool ResUse::IsReadCompatible(const D3D12_BARRIER_LAYOUT Layout) const
{
    return Type == ResUseType::Read && this->Layout == Layout;
}

void ResUse::MergeRead(const D3D12_BARRIER_ACCESS Access, const FShaderStageFlags Stages)
{
    this->Access |= Access;
    this->Stages |= Stages;
}

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

ResInfo::ResInfo(Rc<FUnknown>&& resource, const u32 res_index) : m_owner(std::move(resource)), m_res_index(res_index)
{
}

u32 ResInfo::MarkUse(
    const NonNull<D3d12GpuRecord> self, const D3D12_BARRIER_ACCESS Access, const D3D12_BARRIER_LAYOUT Layout,
    const FShaderStageFlags Stages, const ResUseType UavUse
)
{
    const auto use_type = GetUseType(Access, UavUse);
    switch (use_type)
    {
    case ResUseType::None:
    case ResUseType::Write:
    case ResUseType::ReadWrite:
        goto Add;
    case ResUseType::Read:
        if (m_last_unique_use == COPLT_U32_MAX) goto Add;
        goto TryMerge;
    }
    COPLT_THROW_FMT("Unknown ResUseType {}", static_cast<u32>(use_type));
Add:
    ResUse use{};
    use.FromUse = m_last_unique_use;
    use.ResIndex = m_res_index;
    use.Layout = Layout;
    use.Access = Access;
    use.Stages = Stages;
    use.Type = use_type;
    const auto cur = self->m_res_use.size();
    m_last_unique_use = cur;
    self->m_res_use.push_back(use);
    return cur;
TryMerge:
    auto& last = self->m_res_use[m_last_unique_use];
    if (!last.IsReadCompatible(Layout)) goto Add;
    last.MergeRead(Access, Stages);
    return m_last_unique_use;
}

D3d12GpuRecord::D3d12GpuRecord(const NonNull<D3d12GpuIsolate> isolate)
    : FGpuRecordData(isolate->m_device->m_instance->m_allocator.get()), m_device(isolate->m_device)
{
    m_queue_config = isolate->m_queue_config;
    m_isolate_id = isolate->m_object_id;
    m_record_id = isolate->m_record_inc++;
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

std::pair<NonNull<FCmdRes>, NonNull<ResInfo>> D3d12GpuRecord::Get(const FCmdResRef& ref)
{
    return {&ref.Get(Resources), &m_resources[ref.IndexPlusOne - 1]};
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
    m_resources.clear();
    m_res_use.clear();
    m_outputs.clear();
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
    m_resources.reserve(Resources.size());
    for (u32 i = 0; i < Resources.size(); ++i)
    {
        const auto& res = Resources[i];
        m_resources.push_back(
            ResInfo(Rc<FUnknown>::UnsafeClone(res.GetObjectPtr()), i)
        );
        if (res.Type == FCmdResType::Output)
        {
            m_outputs.push_back(i);
        }
    }
    Analyze();
    Ended = true;
}

void D3d12GpuRecord::Analyze()
{
    const auto commands = Commands.AsSpan();
    if (commands.size() >= std::numeric_limits<u32>::max())
        COPLT_THROW("Too many commands");
    for (u32 i = 0; i < commands.size(); ++i)
    {
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
            Analyze_ClearColor(i, command.ClearColor);
            continue;
        case FCmdType::ClearDepthStencil:
            // todo
            continue;
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(command.Type));
    }
}

void D3d12GpuRecord::Analyze_ClearColor(u32 i, const FCmdClearColor& cmd)
{
    const auto [meta, info] = Get(cmd.Image);
    const auto use = info->MarkUse(this, D3D12_BARRIER_ACCESS_RENDER_TARGET, D3D12_BARRIER_LAYOUT_RENDER_TARGET, FShaderStageFlags::None);
}
