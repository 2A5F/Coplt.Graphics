#include "Record.h"

#include <pix3.h>

#include "../Include/GraphicsFormat.h"
#include "../Include/PipelineState.h"
#include "../Include/States.h"
#include "Output.h"
#include "Image.h"
#include "Buffer.h"
#include "Pipeline.h"
#include "GraphicsPipeline.h"
#include "../../Api/Include/Finally.h"

using namespace Coplt;

ResourceInfo::ResourceInfo(Rc<FGpuObject>&& resource, const FCmdRes& res, const u32 index): Resource(std::move(resource)), Index(index)
{
    Type = res.Type;
    switch (res.Type)
    {
    case FCmdResType::Image:
        {
            const auto obj = res.Image->QueryInterface<ID3d12GpuImage>();
            if (obj == nullptr)
                COPLT_THROW("Image from different backends");
            Image = obj;
            return;
        }
    case FCmdResType::Buffer:
        {
            const auto obj = res.Image->QueryInterface<ID3d12GpuBuffer>();
            if (obj == nullptr)
                COPLT_THROW("Buffer from different backends");
            Buffer = obj;
            return;
        }
    case FCmdResType::Output:
        {
            const auto obj = res.Image->QueryInterface<ID3d12GpuOutput2>();
            if (obj == nullptr)
                COPLT_THROW("Output from different backends");
            Output = obj;
            return;
        }
    }
    COPLT_THROW("Unreachable");
}

ResourceInfo::ResourceInfo(Rc<FGpuObject>&& resource, const View& view, u32 index): Resource(std::move(resource)), Index(index)
{
    switch (view.m_type)
    {
    case FViewType::None:
        COPLT_THROW("Null view");
    case FViewType::Buffer:
        Type = FCmdResType::Buffer;
        Buffer = NonNull(view.TryGetBuffer());
        return;
    case FViewType::Image:
        Type = FCmdResType::Image;
        Image = NonNull(view.TryGetImage());
        return;
    case FViewType::Sampler:
        COPLT_THROW("Samplers cannot be used as resources");
    }
    COPLT_THROW("Unreachable");
}

bool ResourceInfo::IsImage() const
{
    switch (Type)
    {
    case FCmdResType::Image:
        return true;
    case FCmdResType::Buffer:
        return false;
    case FCmdResType::Output:
        return true;
    }
    return false;
}

NonNull<ID3D12Resource> ResourceInfo::GetResource() const
{
    switch (Type)
    {
    case FCmdResType::Image:
        {
            return Image->GetResourcePtr();
        }
    case FCmdResType::Buffer:
        {
            return Buffer->GetResourcePtr();
        }
    case FCmdResType::Output:
        {
            return Output->GetResourcePtr();
        }
    }
    COPLT_THROW("Unreachable");
}

NonNull<FGpuBufferData> ResourceInfo::GetBufferData() const
{
    switch (Type)
    {
    case FCmdResType::Buffer:
        {
            return Buffer->Data();
        }
    case FCmdResType::Image:
        COPLT_THROW("Image is not buffer");
    case FCmdResType::Output:
        COPLT_THROW("Output is not buffer");
    }
    COPLT_THROW("Unreachable");
}

NonNull<FGpuImageData> ResourceInfo::GetImageData() const
{
    switch (Type)
    {
    case FCmdResType::Image:
        {
            return Image->Data();
        }
    case FCmdResType::Output:
        {
            return Output->ImageData();
        }
    case FCmdResType::Buffer:
        COPLT_THROW("Buffer is not image");
    }
    COPLT_THROW("Unreachable");
}

CD3DX12_CPU_DESCRIPTOR_HANDLE ResourceInfo::GetRtv() const
{
    switch (Type)
    {
    case FCmdResType::Image:
        {
            COPLT_THROW("TODO");
        }
    case FCmdResType::Buffer:
        {
            COPLT_THROW("TODO");
        }
    case FCmdResType::Output:
        {
            return Output->GetRtv();
        }
    }
    COPLT_THROW("Unreachable");
}

CD3DX12_CPU_DESCRIPTOR_HANDLE ResourceInfo::GetDsv() const
{
    switch (Type)
    {
    case FCmdResType::Image:
        {
            COPLT_THROW("TODO");
        }
    case FCmdResType::Buffer:
        {
            COPLT_THROW("TODO");
        }
    case FCmdResType::Output:
        {
            COPLT_THROW("Output dose not support Dsv");
        }
    }
    COPLT_THROW("Unreachable");
}

NonNull<LayoutState> ResourceInfo::GetState() const
{
    switch (Type)
    {
    case FCmdResType::Image:
        {
            return Image->State();
        }
    case FCmdResType::Buffer:
        {
            return Buffer->State();
        }
    case FCmdResType::Output:
        {
            return Output->State();
        }
    }
    COPLT_THROW("Unreachable");
}

void D3d12GpuRecord::PipelineContext::Reset()
{
    Pipeline = nullptr;
    Layout = nullptr;
    GPipeline = nullptr;
    Binding = nullptr;
    PipelineChanged = false;
    BindingChanged = false;
}

void D3d12GpuRecord::PipelineContext::SetPipeline(NonNull<FShaderPipeline> pipeline, u32 i)
{
    if (Pipeline && pipeline->ObjectId() == Pipeline->ObjectId()) return;
    const Ptr dx_pipeline = pipeline->QueryInterface<ID3d12ShaderPipeline>();
    if (!dx_pipeline)
    {
        COPLT_THROW_FMT(
            "[{}] Pipeline({:#x}) from different backends",
            i, static_cast<size_t>(pipeline)
        );
    }
    Pipeline = dx_pipeline;
    Layout = Pipeline->Layout();
    const auto stages = pipeline->GetStages();
    GPipeline = nullptr;
    if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        const Ptr g_pipeline = pipeline->QueryInterface<ID3d12GraphicsShaderPipeline>();
        if (!g_pipeline)
        {
            COPLT_THROW_FMT("[{}] Invalid pipeline: pipeline is not a graphics pipeline, but there is a pixel shader in the stages.", i);
        }
        GPipeline = g_pipeline;
    }
    PipelineChanged = true;
    if (Binding && Binding->Layout()->ObjectId() != Layout->ObjectId())
    {
        Binding = nullptr;
    }
}

void D3d12GpuRecord::PipelineContext::SetBinding(NonNull<FShaderBinding> binding, u32 i, const FCmdSetBinding& cmd)
{
    if (Binding && binding->ObjectId() == Binding->ObjectId()) return;
    const Ptr dx_binding = binding->QueryInterface<ID3d12ShaderBinding>();
    if (!dx_binding)
    {
        COPLT_THROW_FMT(
            "[{}] Binding({:#x}) from different backends",
            i, static_cast<size_t>(binding)
        );
    }
    if (Layout && dx_binding->Layout()->ObjectId() != Layout->ObjectId())
    {
        COPLT_THROW_FMT("[{}] The binding layout is not compatible with the currently set pipeline", i);
    }
    Binding = dx_binding;
    SetBindingIndex = cmd.Index;
}

D3d12GpuRecord::D3d12GpuRecord(const NonNull<D3d12GpuIsolate> isolate)
    : FGpuRecordData(isolate->m_device->m_instance->m_allocator.get()),
      m_isolate_config(isolate->m_config), m_device(isolate->m_device)
{
    Id = m_isolate_id = isolate->m_object_id;
    m_record_id = isolate->m_record_inc++;
    m_context = new D3d12RecordContext(isolate);
    m_barrier_analyzer = isolate->m_barrier_marshal->CreateAnalyzer();
    FGpuRecordData::Context = m_context.get();
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

std::span<ResourceInfo> D3d12GpuRecord::ResourceInfos() noexcept
{
    return m_resource_infos;
}

const Rc<D3d12RecordContext>& D3d12GpuRecord::Context() const noexcept
{
    return m_context;
}

const D3d12RentedCommandList& D3d12GpuRecord::ResultList() const noexcept
{
    return m_result_list;
}

const Rc<ID3d12BarrierAnalyzer>& D3d12GpuRecord::BarrierAnalyzer()
{
    return m_barrier_analyzer;
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
    m_barrier_analyzer->Clear();
    m_context->Recycle();
    m_resource_map.Clear();
    m_resource_infos.clear();
    m_binding_infos.clear();
    m_set_binding_infos.clear();
    m_allocations.clear();
    m_bind_items.clear();
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
    m_barrier_analyzer->StartAnalyze(this);
    ReadyResource();
    u32 MinResHeapSize{}, MinSmpHeapSize{};
    ReadyBindings(MinResHeapSize, MinSmpHeapSize);
    m_context->m_descriptor_manager.ReadyFrame(MinResHeapSize, MinSmpHeapSize);
    Analyze();
    m_barrier_analyzer->EndAnalyze();
    if (m_isolate_config->MultiThreadRecord)
    {
        auto allocator = m_context->m_cmd_alloc_pool->RentCommandAllocator(GetType(Mode));
        auto list = allocator.RentCommandList();
        m_barrier_analyzer->Interpret(list, *this);
        list.Close();
        m_result_list = std::move(list);
        m_context->m_recycled_command_allocators.push_back(std::move(allocator));
    }
    Ended = true;
    Version++;
}

void D3d12GpuRecord::AfterSubmit()
{
    m_result_list = {};
}

ResourceInfo& D3d12GpuRecord::GetRes(const FCmdResRef& ref)
{
    return m_resource_infos[ref.ResIndex()];
}

const ResourceInfo& D3d12GpuRecord::GetRes(const FCmdResRef& ref) const
{
    return m_resource_infos[ref.ResIndex()];
}

void D3d12GpuRecord::ResetState()
{
    m_state = RecordState::Main;
    m_cur_render = {};
    m_pipeline_context.Reset();
}

FResIndex D3d12GpuRecord::AddResource(const FCmdRes& res)
{
    const NonNull obj = res.GetObjectPtr();
    const auto index = m_resource_map.GetOrAdd(obj->ObjectId(), [this, obj, &res](auto& p)
    {
        const auto index = m_resource_infos.size();
        p.put(static_cast<u32>(index));
        m_resource_infos.push_back(ResourceInfo(Rc<FGpuObject>::UnsafeClone(obj), res, index));
        m_barrier_analyzer->OnAddRes();
    });
    return FResIndex(index);
}

FResIndex D3d12GpuRecord::AddResource(const View& view)
{
    const auto obj = view.GetViewable();
    const auto index = m_resource_map.GetOrAdd(obj->ObjectId(), [this, obj, &view](auto& p)
    {
        const auto index = m_resource_infos.size();
        p.put(static_cast<u32>(index));
        m_resource_infos.push_back(ResourceInfo(Rc<FGpuObject>::UnsafeClone(obj), view, index));
        m_barrier_analyzer->OnAddRes();
    });
    return FResIndex(index);
}

void D3d12GpuRecord::ReadyResource()
{
    m_resource_infos.clear();
    m_resource_infos.reserve(Resources.size());
    for (u32 i = 0; i < Resources.size(); ++i)
    {
        const auto& res = Resources[i];
        const auto index = AddResource(res);
        COPLT_DEBUG_ASSERT(index == i);
    }
}

void D3d12GpuRecord::ReadyBindings(u32& MinResHeapSize, u32& MinSmpHeapSize)
{
    MinResHeapSize = 0;
    MinSmpHeapSize = 0;
    m_binding_infos.clear();
    m_binding_infos.reserve(Bindings.size());
    for (u32 i = 0; i < Bindings.size(); ++i)
    {
        m_binding_infos.push_back({});
    }
    m_set_binding_infos.clear();
    m_set_binding_infos.reserve(NumSetBindings);
    for (u32 i = 0; i < NumSetBindings; ++i)
    {
        m_set_binding_infos.push_back(SetBindingInfo());
    }
    for (u32 i = 0; i < BindingChange.size(); ++i)
    {
        const auto& item = BindingChange[i];
        const auto binding = NonNull(Bindings[item.Binding].Binding)->QueryInterface<ID3d12ShaderBinding>();
        if (binding == nullptr)
            COPLT_THROW_FMT("Binding [{}] comes from different backends", item.Binding);
        const auto& data = binding->Layout()->Data();
        MinResHeapSize += data.SumTableResourceSlots;
        MinSmpHeapSize += data.SumTableSamplerSlots;
    }
}

void D3d12GpuRecord::Analyze()
{
    const auto commands = Commands.AsSpan();
    for (u32 i = 0; i < commands.size(); ++i, m_barrier_analyzer->CmdNext())
    {
        const auto& command = commands[i];
        switch (command.Type)
        {
        case FCmdType::None:
        case FCmdType::Label:
        case FCmdType::BeginScope:
        case FCmdType::EndScope:
            break;
        case FCmdType::End:
            if (m_state == RecordState::Render)
                Analyze_RenderEnd(i, m_cur_render.Cmd);
            else
                COPLT_THROW("Cannot use End in main scope");
            break;
        case FCmdType::PreparePresent:
            Analyze_PreparePresent(i, command.PreparePresent);
            break;
        case FCmdType::ClearColor:
            Analyze_ClearColor(i, command.ClearColor);
            break;
        case FCmdType::ClearDepthStencil:
            Analyze_ClearDepthStencil(i, command.ClearDepthStencil);
            break;
        case FCmdType::BufferCopy:
            Analyze_BufferCopy(i, command.BufferCopy);
            break;
        case FCmdType::Render:
            Analyze_Render(i, command.Render);
            break;
        case FCmdType::Compute:
            COPLT_THROW("TODO");
        case FCmdType::SetPipeline:
            Analyze_SetPipeline(i, command.SetPipeline);
            break;
        case FCmdType::SetBinding:
            Analyze_SetBinding(i, command.SetBinding);
            break;
        case FCmdType::SetMeshBuffers:
            Analyze_SetMeshBuffers(i, command.SetMeshBuffers);
            break;
        case FCmdType::SetViewportScissor:
            if (Mode != FGpuRecordMode::Direct)
                COPLT_THROW_FMT("[{}] Can only SetViewportScissor on the direct mode", i);
            break;
        case FCmdType::Draw:
            if (Mode != FGpuRecordMode::Direct)
                COPLT_THROW_FMT("[{}] Can only Draw on the direct mode", i);
            break;
        case FCmdType::Dispatch:
            Analyze_Dispatch(i, command.Dispatch);
            break;
        }
    }
}

void D3d12GpuRecord::Analyze_PreparePresent(u32 i, const FCmdPreparePresent& cmd) const
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use PreparePresent in sub scope", i);
    if (Mode != FGpuRecordMode::Direct)
        COPLT_THROW_FMT("[{}] Can only present on the direct mode", i);
    m_barrier_analyzer->OnUse(cmd.Output.ResIndex(), ResAccess::None, ResUsage::Common, ResLayout::Common);
}

void D3d12GpuRecord::Analyze_ClearColor(u32 i, const FCmdClearColor& cmd) const
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use ClearColor in sub scope", i);
    m_barrier_analyzer->OnUse(cmd.Image.ResIndex(), ResAccess::RenderTargetWrite, ResUsage::Common, ResLayout::RenderTarget);
    m_barrier_analyzer->OnCmd();
}

void D3d12GpuRecord::Analyze_ClearDepthStencil(u32 i, const FCmdClearDepthStencil& cmd) const
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use ClearDepthStencil in sub scope", i);
    m_barrier_analyzer->OnUse(cmd.Image.ResIndex(), ResAccess::DepthStencilWrite, ResUsage::Common, ResLayout::DepthStencilWrite);
    m_barrier_analyzer->OnCmd();
}

void D3d12GpuRecord::Analyze_BufferCopy(u32 i, const FCmdBufferCopy& cmd) const
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use BufferCopy in sub scope", i);
    if (cmd.SrcType == FBufferRefType2::Buffer && cmd.DstType == FBufferRefType2::Buffer)
    {
        m_barrier_analyzer->OnUse(cmd.Dst.Buffer.ResIndex(), ResAccess::CopyDestWrite, ResUsage::Common, ResLayout::Common);
        m_barrier_analyzer->OnUse(cmd.Src.Buffer.ResIndex(), ResAccess::CopySourceRead, ResUsage::Common, ResLayout::Common);
        m_barrier_analyzer->OnCmd();
    }
    else if (cmd.SrcType == FBufferRefType2::Upload && cmd.DstType == FBufferRefType2::Buffer)
    {
        m_barrier_analyzer->OnUse(cmd.Dst.Buffer.ResIndex(), ResAccess::CopyDestWrite, ResUsage::Common, ResLayout::Common);
        m_barrier_analyzer->OnCmd();
    }
    else
    {
        COPLT_THROW_FMT(
            "[{}] Unsupported copy combination {{ SrcType = {} DstType = {} }}",
            i, static_cast<u8>(cmd.SrcType), static_cast<u8>(cmd.DstType)
        );
    }
}

void D3d12GpuRecord::Analyze_Render(u32 i, const FCmdRender& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use Render in sub scope", i);
    if (Mode != FGpuRecordMode::Direct)
        COPLT_THROW_FMT("[{}] Render can only be used in Direct mode", i);
    m_state = RecordState::Render;
    m_cur_render = RenderState{.StartIndex = i, .Cmd = cmd};
    const auto& info = PayloadRenderInfo[cmd.InfoIndex];
    const auto num_rtv = std::min(info.NumRtv, 8u);
    if (info.Dsv)
    {
        m_barrier_analyzer->OnUse(info.Dsv.ResIndex(), ResAccess::DepthStencilWrite, ResUsage::Common, ResLayout::DepthStencilWrite);
    }
    for (u32 n = 0; n < num_rtv; ++n)
    {
        m_barrier_analyzer->OnUse(info.Rtv[n].ResIndex(), ResAccess::RenderTargetWrite, ResUsage::Common, ResLayout::RenderTarget);
    }
    m_barrier_analyzer->OnCmd();
}

void D3d12GpuRecord::Analyze_RenderEnd(u32 i, const FCmdRender& cmd)
{
    if (m_state != RecordState::Render)
        COPLT_THROW_FMT("[{}] Cannot use End in main scope or some end is missing", i);
    m_state = RecordState::Main;
    const auto& info = PayloadRenderInfo[cmd.InfoIndex];
    const auto num_rtv = std::min(info.NumRtv, 8u);
    if (info.Dsv) m_barrier_analyzer->UpdateUse(info.Dsv.ResIndex());
    for (u32 n = 0; n < num_rtv; ++n) m_barrier_analyzer->UpdateUse(info.Rtv[n].ResIndex());
    m_barrier_analyzer->OnCmd();
    m_pipeline_context.Reset();
}

void D3d12GpuRecord::Analyze_Compute(u32 i, const FCmdCompute& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use Compute in sub scope", i);
    if (Mode != FGpuRecordMode::Direct && Mode != FGpuRecordMode::Compute)
        COPLT_THROW_FMT("[{}] Compute can only be used in Direct or Compute mode", i);
    m_state = RecordState::Compute;
    m_cur_compute = ComputeState{.StartIndex = i, .Cmd = cmd};
}

void D3d12GpuRecord::Analyze_ComputeEnd(u32 i, const FCmdCompute& cmd)
{
    if (m_state != RecordState::Compute)
        COPLT_THROW_FMT("[{}] Cannot use End in main scope or some end is missing", i);
    m_state = RecordState::Main;
    m_pipeline_context.Reset();
}

void D3d12GpuRecord::Analyze_SetPipeline(u32 i, const FCmdSetPipeline& cmd)
{
    if (m_state != RecordState::Render && m_state != RecordState::Compute)
        COPLT_THROW_FMT("[{}] Cannot use SetPipeline in main scope", i);
    SetPipeline(cmd.Pipeline, i);
}

void D3d12GpuRecord::Analyze_SetBinding(u32 i, const FCmdSetBinding& cmd)
{
    if (m_state != RecordState::Render && m_state != RecordState::Compute)
        COPLT_THROW_FMT("[{}] Cannot use SetBinding in main scope", i);
    if (!SetBinding(Bindings[cmd.Binding].Binding, i, cmd)) return;
    const NonNull binding = m_pipeline_context.Binding;
    ReadGuard binding_guard(binding->SelfLock());
    Finally clear_lock([&] { m_tmp_locks.clear(); }); // todo 锁移到整个 render pass 范围，不允许在 render pass 中修改绑定内容
    const auto groups = binding->Groups();
    for (u32 g = 0; g < groups.size(); ++g)
    {
        const auto& group = groups[g];
        if (!group) continue;
        m_tmp_locks.push_back(ReadGuard{group->SelfLock()});
    }
    const auto& binding_layout = binding->Layout();
    const auto group_item_infos = binding_layout->GroupItemInfos();
    const auto bind_item_infos = binding_layout->BindItemInfos();
    bool any_changed = false;
    for (u32 g = 0; g < groups.size(); ++g)
    {
        const auto& group = groups[g];
        if (!group) continue;
        const auto views = group->Views();
        for (u32 v = 0; v < views.size(); ++v)
        {
            const auto& view = views[v];
            if (!view || view.IsSampler()) continue;
            const auto& res_index = AddResource(view);
            const auto& info = group_item_infos[g][v];
            m_barrier_analyzer->OnUse(res_index, view, info); // todo 屏障移动到每次 draw
        }
        if (group->EnsureAvailable()) any_changed = true;
    }
    auto& binding_info = m_binding_infos[cmd.Binding];
    auto& set_binding_info = m_set_binding_infos[cmd.Index];
    const auto binding_version = binding->Version();
    if (binding_info.AllocationIndex == COPLT_U32_MAX || binding_info.BindingVersion != binding_version || any_changed)
    {
        binding_info.BindingVersion = binding_version;
        binding_info.AllocationIndex = m_allocations.size();
        binding_info.BindItemIndex = m_bind_items.size();
        for (u32 g = 0; g < groups.size(); ++g)
        {
            const auto& group = groups[g];
            if (!group)
            {
                m_allocations.push_back({});
                continue;
            }
            const auto object_id = group->ObjectId();
            const auto& data = group->Layout()->Data();
            // todo 确定是最后一次修改
            const auto f = [&](const u32 size, const Rc<DescriptorHeap>& heap, const ComPtr<ID3D12DescriptorHeap>& dh, const bool last)
            {
                if (size == 0) return;
                DescriptorAllocation allocation;
                if (data.Usage == FBindGroupUsage::Dynamic && !last)
                {
                    allocation = heap->AllocateTmp(size);
                    m_allocations.push_back(allocation);
                }
                else
                {
                    bool exists = false;
                    allocation = heap->Allocate(object_id, size, exists);
                    m_allocations.push_back(allocation);
                    if (exists) return;
                }
                m_device->m_device->CopyDescriptorsSimple(size, allocation.GetCpuHandle(), dh->GetCPUDescriptorHandleForHeapStart(), heap->m_type);
            };
            f(data.ResourceTableSize, m_context->m_descriptor_manager.m_res, group->ResourceHeap(), true);
            f(data.SamplerTableSize, m_context->m_descriptor_manager.m_smp, group->SamplerHeap(), true);
        }
        for (const auto& bind_item_info : bind_item_infos)
        {
            BindItem item{};
            // todo 直接资源和常量
            m_bind_items.push_back(item);
        }
    }
    set_binding_info.Layout = binding_layout.get();
    set_binding_info.AllocationIndex = binding_info.AllocationIndex;
    set_binding_info.BindItemIndex = binding_info.BindItemIndex;
    m_barrier_analyzer->OnCmd();
}

void D3d12GpuRecord::Analyze_SetMeshBuffers(u32 i, const FCmdSetMeshBuffers& cmd)
{
    if (m_state != RecordState::Render)
        COPLT_THROW_FMT("[{}] Can only use SetMeshBuffers in render scope", i);
    const auto& buf = PayloadMeshBuffers[cmd.PayloadIndex];
    if (buf.IndexBuffer.Buffer)
    {
        m_barrier_analyzer->OnUse(buf.IndexBuffer.Buffer.ResIndex(), ResAccess::IndexBufferRead, ResUsage::VertexOrMesh, ResLayout::Undefined);
    }
    for (u32 c = 0; c < buf.VertexBufferCount; ++c)
    {
        const auto& item = PayloadVertexBufferRange[c];
        m_barrier_analyzer->OnUse(item.Buffer.ResIndex(), ResAccess::VertexBufferRead, ResUsage::VertexOrMesh, ResLayout::Undefined);
    }
    m_barrier_analyzer->OnCmd();
}

void D3d12GpuRecord::Analyze_Dispatch(u32 i, const FCmdDispatch& cmd) const
{
    switch (cmd.Type)
    {
    case FDispatchType::Compute:
        if (m_state != RecordState::Compute)
            COPLT_THROW_FMT("[{}] Can only use Dispatch (Compute) in compute scope", i);
        break;
    case FDispatchType::Mesh:
        if (m_state != RecordState::Render)
            COPLT_THROW_FMT("[{}] Can only use Dispatch (Mesh) in render scope", i);
        break;
    }
}

void D3d12GpuRecord::BeforeInterpret(const D3d12RentedCommandList& list)
{
    if (const auto& g0 = list->g0)
    {
        ID3D12DescriptorHeap* heaps[2]{m_context->m_descriptor_manager.m_res->m_heap.Get(), m_context->m_descriptor_manager.m_smp->m_heap.Get()};
        g0->SetDescriptorHeaps(2, heaps);
    }
}

void D3d12GpuRecord::AfterInterpret(const D3d12RentedCommandList& list)
{
}

void D3d12GpuRecord::Interpret(const D3d12RentedCommandList& list, const u32 offset, const u32 count)
{
    const auto commands = Commands.AsSpan();
    for (u32 c = 0; c < count; ++c)
    {
        const auto i = offset + c;
        const auto& command = commands[i];
        switch (command.Type)
        {
        case FCmdType::None:
        case FCmdType::PreparePresent:
            break;
        case FCmdType::End:
            if (m_state == RecordState::Render)
                Interpret_RenderEnd(list, i, m_cur_render.Cmd);
            else
                COPLT_THROW("Cannot use End in main scope");
            break;
        case FCmdType::Label:
            Interpret_Label(list, i, command.Label);
            break;
        case FCmdType::BeginScope:
            Interpret_BeginScope(list, i, command.BeginScope);
            break;
        case FCmdType::EndScope:
            Interpret_EndScope(list, i, command.EndScope);
            break;
        case FCmdType::ClearColor:
            Interpret_ClearColor(list, i, command.ClearColor);
            break;
        case FCmdType::ClearDepthStencil:
            Interpret_ClearDepthStencil(list, i, command.ClearDepthStencil);
            break;
        case FCmdType::BufferCopy:
            Interpret_BufferCopy(list, i, command.BufferCopy);
            break;
        case FCmdType::Render:
            Interpret_Render(list, i, command.Render);
            break;
        case FCmdType::Compute:
            COPLT_THROW("TODO");
        case FCmdType::SetPipeline:
            Interpret_SetPipeline(list, i, command.SetPipeline);
            break;
        case FCmdType::SetBinding:
            Interpret_SetBinding(list, i, command.SetBinding);
            break;
        case FCmdType::SetViewportScissor:
            Interpret_SetViewportScissor(list, i, command.SetViewportScissor);
            break;
        case FCmdType::SetMeshBuffers:
            Interpret_SetMeshBuffers(list, i, command.SetMeshBuffers);
            break;
        case FCmdType::Draw:
            Interpret_Draw(list, i, command.Draw);
            break;
        case FCmdType::Dispatch:
            Interpret_Dispatch(list, i, command.Dispatch);
            break;
        }
    }
}

void D3d12GpuRecord::Interpret_Label(const CmdList& list, u32 i, const FCmdLabel& cmd) const
{
    if (!m_device->Debug()) return;
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

void D3d12GpuRecord::Interpret_BeginScope(const CmdList& list, u32 i, const FCmdBeginScope& cmd) const
{
    if (!m_device->Debug()) return;
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

void D3d12GpuRecord::Interpret_EndScope(const CmdList& list, u32 i, const FCmdEndScope& cmd) const
{
    if (!m_device->Debug()) return;
    if (!list->g0) return;
    PIXEndEvent(list->g0.Get());
}

void D3d12GpuRecord::Interpret_ClearColor(const CmdList& list, const u32 i, const FCmdClearColor& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use ClearColor in sub scope", i);
    const auto rtv = GetRes(cmd.Image).GetRtv();
    list->g0->ClearRenderTargetView(
        rtv, cmd.Color, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&PayloadRect[cmd.RectIndex])
    );
}

void D3d12GpuRecord::Interpret_ClearDepthStencil(const CmdList& list, const u32 i, const FCmdClearDepthStencil& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use ClearDepthStencil in sub scope", i);
    const auto dsv = GetRes(cmd.Image).GetDsv();
    D3D12_CLEAR_FLAGS flags{};
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Depth)) flags |= D3D12_CLEAR_FLAG_DEPTH;
    if (HasFlags(cmd.Clear, FDepthStencilClearFlags::Stencil)) flags |= D3D12_CLEAR_FLAG_STENCIL;
    list->g0->ClearDepthStencilView(
        dsv, flags, cmd.Depth, cmd.Stencil, cmd.RectCount,
        cmd.RectCount == 0 ? nullptr : reinterpret_cast<const D3D12_RECT*>(&PayloadRect[cmd.RectIndex])
    );
}

void D3d12GpuRecord::Interpret_BufferCopy(const CmdList& list, u32 i, const FCmdBufferCopy& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use BufferCopy in sub scope", i);
    const auto& range = PayloadBufferCopyRange[cmd.RangeIndex];
    if (cmd.SrcType == FBufferRefType2::Buffer && cmd.DstType == FBufferRefType2::Buffer)
    {
        const auto dst = GetRes(cmd.Dst.Buffer).GetResource();
        const auto src = GetRes(cmd.Src.Buffer).GetResource();
        if (range.Size == std::numeric_limits<u64>::max())
        {
            list->g0->CopyResource(dst, src);
        }
        else
        {
            list->g0->CopyBufferRegion(dst, range.DstOffset, src, range.SrcOffset, range.Size);
        }
    }
    else if (cmd.SrcType == FBufferRefType2::Upload && cmd.DstType == FBufferRefType2::Buffer)
    {
        const auto dst = GetRes(cmd.Dst.Buffer).GetResource();
        if (cmd.Src.Upload.Index >= m_context->m_upload_buffers.size())
            COPLT_THROW_FMT("[{}] Index out of bounds", i);
        const auto& src_obj = m_context->m_upload_buffers[cmd.Src.Upload.Index];
        if (range.SrcOffset + range.Size >= src_obj.m_size)
            COPLT_THROW_FMT("[{}] Size out of range", i);
        const auto src = src_obj.m_resource.m_resource.Get();
        list->g0->CopyBufferRegion(dst, range.DstOffset, src, range.SrcOffset, range.Size);
    }
    else
    {
        COPLT_THROW_FMT(
            "[{}] Unsupported copy combination {{ SrcType = {} DstType = {} }}",
            i, static_cast<u8>(cmd.SrcType), static_cast<u8>(cmd.DstType)
        );
    }
}

void D3d12GpuRecord::Interpret_Render(const CmdList& list, const u32 i, const FCmdRender& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use Render in sub scope", i);
    if (Mode != FGpuRecordMode::Direct)
        COPLT_THROW_FMT("[{}] Render can only be used in Direct mode", i);
    m_state = RecordState::Render;
    m_cur_render = RenderState{.StartIndex = i, .Cmd = cmd};
    const auto& info = PayloadRenderInfo[cmd.InfoIndex];
    const auto num_rtv = std::min(info.NumRtv, 8u);
    if (list->g4)
    {
        D3D12_RENDER_PASS_RENDER_TARGET_DESC rts[num_rtv];
        D3D12_RENDER_PASS_DEPTH_STENCIL_DESC ds[info.Dsv ? 1 : 0];
        if (info.Dsv)
        {
            const auto& dsv = GetRes(info.Dsv);
            const auto& d_load = info.DsvLoadOp[0];
            const auto& d_store = info.DsvStoreOp[0];
            const auto& s_load = info.DsvLoadOp[1];
            const auto& s_store = info.DsvStoreOp[1];
            D3D12_RENDER_PASS_DEPTH_STENCIL_DESC desc{};
            desc.cpuDescriptor = dsv.GetDsv();
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
                res.pSrcResource = GetRes(res_info.Src).GetResource();
                res.pDstResource = GetRes(res_info.Dst).GetResource();
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            desc.StencilEndingAccess = ToDx(s_store, [&](D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS& res)
            {
                const auto& res_info = PayloadResolveInfo[info.DsvResolveInfoIndex];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetRes(res_info.Src).GetResource();
                res.pDstResource = GetRes(res_info.Dst).GetResource();
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            ds[0] = desc;
        }
        for (u32 n = 0; n < num_rtv; ++n)
        {
            const auto& rtv = GetRes(info.Rtv[n]);
            const auto& load = info.RtvLoadOp[n];
            const auto& store = info.RtvStoreOp[n];
            D3D12_RENDER_PASS_RENDER_TARGET_DESC desc{};
            desc.cpuDescriptor = rtv.GetRtv();
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
                res.pSrcResource = GetRes(res_info.Src).GetResource();
                res.pDstResource = GetRes(res_info.Dst).GetResource();
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            rts[n] = desc;
        }
        D3D12_RENDER_PASS_FLAGS flags = D3D12_RENDER_PASS_FLAG_NONE;
        if (info.HasUavWrites) flags |= D3D12_RENDER_PASS_FLAG_ALLOW_UAV_WRITES;
        list->g4->BeginRenderPass(info.NumRtv, rts, info.Dsv ? ds : nullptr, flags);
    }
    else
    {
        COPLT_THROW_FMT("[{}] Render pass is not supported, please check the agility sdk version", i);
    }
}

void D3d12GpuRecord::Interpret_RenderEnd(const CmdList& list, u32 i, const FCmdRender& cmd)
{
    if (m_state != RecordState::Render)
        COPLT_THROW_FMT("[{}] Cannot use End in main scope or some end is missing", i);
    m_state = RecordState::Main;
    if (list->g4)
    {
        list->g4->EndRenderPass();
    }
    else
    {
        COPLT_THROW_FMT("[{}] Render pass is not supported, please check the agility sdk version", i);
    }
    m_pipeline_context.Reset();
}

void D3d12GpuRecord::Interpret_Compute(const CmdList& list, u32 i, const FCmdCompute& cmd)
{
    if (m_state != RecordState::Main)
        COPLT_THROW_FMT("[{}] Cannot use Compute in sub scope", i);
    if (Mode != FGpuRecordMode::Direct && Mode != FGpuRecordMode::Compute)
        COPLT_THROW_FMT("[{}] Compute can only be used in Direct or Compute mode", i);
    m_state = RecordState::Compute;
    m_cur_compute = ComputeState{.StartIndex = i, .Cmd = cmd};
}

void D3d12GpuRecord::Interpret_ComputeEnd(const CmdList& list, u32 i, const FCmdCompute& cmd)
{
    if (m_state != RecordState::Compute)
        COPLT_THROW_FMT("[{}] Cannot use End in main scope or some end is missing", i);
    m_state = RecordState::Main;
    m_pipeline_context.Reset();
}

void D3d12GpuRecord::Interpret_SetPipeline(const CmdList& list, const u32 i, const FCmdSetPipeline& cmd)
{
    if (m_state != RecordState::Render && m_state != RecordState::Compute)
        COPLT_THROW_FMT("[{}] Cannot use SetPipeline in main scope", i);
    SetPipeline(list, cmd.Pipeline, i);
}

void D3d12GpuRecord::Interpret_SetBinding(const CmdList& list, u32 i, const FCmdSetBinding& cmd)
{
    if (m_state != RecordState::Render && m_state != RecordState::Compute)
        COPLT_THROW_FMT("[{}] Cannot use SetBinding in main scope", i);
    if (!SetBinding(Bindings[cmd.Binding].Binding, i, cmd)) return;
    const auto& set_binding_info = m_set_binding_infos[m_pipeline_context.SetBindingIndex];
    const auto bind_item_infos = set_binding_info.Layout->BindItemInfos();
    for (usize n = 0; n < bind_item_infos.size(); ++n)
    {
        const auto& bind_item_info = bind_item_infos[n];
        const auto& bind_item = m_bind_items[set_binding_info.BindItemIndex + n];
        // todo 直接资源和常量
        switch (bind_item_info.Place)
        {
        case Layout::BindItemPlace::Table:
            {
                const auto& allocation = m_allocations[set_binding_info.AllocationIndex + bind_item_info.Group];
                if (m_state == RecordState::Render)
                {
                    list->g0->SetGraphicsRootDescriptorTable(bind_item_info.RootIndex, allocation.GetGpuHandle());
                }
                else
                {
                    list->g0->SetComputeRootDescriptorTable(bind_item_info.RootIndex, allocation.GetGpuHandle());
                }
                break;
            }
        case Layout::BindItemPlace::Direct:
            // todo
            break;
        case Layout::BindItemPlace::Const:
            // todo
            break;
        }
    }
}

void D3d12GpuRecord::Interpret_SetViewportScissor(const CmdList& list, u32 i, const FCmdSetViewportScissor& cmd) const
{
    if (m_state != RecordState::Render)
        COPLT_THROW_FMT("[{}] Can only use SetViewportScissor in render scope", i);
    list->g0->RSSetViewports(cmd.ViewportCount, reinterpret_cast<const D3D12_VIEWPORT*>(PayloadViewport.data() + cmd.ViewportIndex));
    list->g0->RSSetScissorRects(cmd.ScissorRectCount, reinterpret_cast<const D3D12_RECT*>(PayloadRect.data() + cmd.ScissorRectIndex));
}

void D3d12GpuRecord::Interpret_SetMeshBuffers(const CmdList& list, u32 i, const FCmdSetMeshBuffers& cmd) const
{
    if (m_state != RecordState::Render)
        COPLT_THROW_FMT("[{}] Can only use SetViewportScissor in render scope", i);
    const auto& buffers = PayloadMeshBuffers[cmd.PayloadIndex];
    const auto defs = buffers.MeshLayout->GetBuffers();
    const auto vbs = std::span(PayloadVertexBufferRange.data() + buffers.VertexBuffersIndex, buffers.VertexBufferCount);
    if (buffers.IndexBuffer.Buffer)
    {
        const auto resource = GetRes(buffers.IndexBuffer.Buffer).GetResource();
        D3D12_INDEX_BUFFER_VIEW view{};
        view.BufferLocation = resource->GetGPUVirtualAddress() + buffers.IndexBuffer.Offset;
        view.SizeInBytes = buffers.IndexBuffer.Size;
        view.Format = ToDx(cmd.IndexFormat);
        list->g0->IASetIndexBuffer(&view);
    }
    D3D12_VERTEX_BUFFER_VIEW views[vbs.size()];
    for (u32 j = 0; j < vbs.size(); j++)
    {
        const auto& range = vbs[j];
        const auto& def = defs[range.Index];
        const auto resource = GetRes(range.Buffer).GetResource();
        D3D12_VERTEX_BUFFER_VIEW view{};
        view.BufferLocation = resource->GetGPUVirtualAddress() + range.Offset;
        view.SizeInBytes = range.Size;
        view.StrideInBytes = def.Stride;
        views[j] = view;
    }
    list->g0->IASetVertexBuffers(cmd.VertexStartSlot, buffers.VertexBufferCount, views);
}

void D3d12GpuRecord::Interpret_Draw(const CmdList& list, u32 i, const FCmdDraw& cmd) const
{
    if (m_state != RecordState::Render)
        COPLT_THROW_FMT("[{}] Can only use Draw in render scope", i);
    if (cmd.Indexed)
    {
        list->g0->DrawIndexedInstanced(cmd.VertexOrIndexCount, cmd.InstanceCount, cmd.FirstVertexOrIndex, cmd.VertexOffset, cmd.FirstInstance);
    }
    else
    {
        list->g0->DrawInstanced(cmd.VertexOrIndexCount, cmd.InstanceCount, cmd.FirstVertexOrIndex, cmd.FirstInstance);
    }
}

void D3d12GpuRecord::Interpret_Dispatch(const CmdList& list, u32 i, const FCmdDispatch& cmd) const
{
    switch (cmd.Type)
    {
    case FDispatchType::Compute:
        if (m_state != RecordState::Compute)
            COPLT_THROW_FMT("[{}] Can only use Dispatch (Compute) in compute scope", i);
        list->g0->Dispatch(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
        break;
    case FDispatchType::Mesh:
        if (m_state != RecordState::Render)
            COPLT_THROW_FMT("[{}] Can only use Dispatch (Mesh) in render scope", i);
        if (!list->g6)
            COPLT_THROW_FMT("[{}] The device does not support mesh shaders", i);
        list->g6->DispatchMesh(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
        break;
    }
}

void D3d12GpuRecord::SetPipeline(NonNull<FShaderPipeline> pipeline, u32 i)
{
    if (m_pipeline_context.Pipeline && pipeline->ObjectId() == m_pipeline_context.Pipeline->ObjectId()) return;
    m_pipeline_context.SetPipeline(pipeline, i);
}

bool D3d12GpuRecord::SetBinding(NonNull<FShaderBinding> binding, u32 i, const FCmdSetBinding& cmd)
{
    if (m_pipeline_context.Binding && binding->ObjectId() == m_pipeline_context.Binding->ObjectId()) return false;
    m_pipeline_context.SetBinding(binding, i, cmd);
    return true;
}

void D3d12GpuRecord::SetPipeline(const CmdList& list, NonNull<FShaderPipeline> pipeline, u32 i)
{
    if (pipeline == m_pipeline_context.Pipeline) return;
    m_pipeline_context.SetPipeline(pipeline, i);
    const auto stages = pipeline->GetStages();
    if (HasFlags(stages, FShaderStageFlags::Compute))
    {
        list->g0->SetComputeRootSignature(m_pipeline_context.Layout->RootSignature().Get());
    }
    else if (HasFlags(stages, FShaderStageFlags::Pixel))
    {
        const auto& states = m_pipeline_context.GPipeline->GetGraphicsState();
        list->g0->IASetPrimitiveTopology(ToDx(states->Topology));
        list->g0->SetGraphicsRootSignature(m_pipeline_context.Layout->RootSignature().Get());
    }
    list->g0->SetPipelineState(m_pipeline_context.Pipeline->GetPipelineState().Get());
}

D3D12_COMMAND_LIST_TYPE Coplt::GetType(const FGpuRecordMode Mode)
{
    switch (Mode)
    {
    case FGpuRecordMode::Direct:
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    case FGpuRecordMode::Compute:
        return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    case FGpuRecordMode::Copy:
        return D3D12_COMMAND_LIST_TYPE_COPY;
    }
    return D3D12_COMMAND_LIST_TYPE_DIRECT;
}
