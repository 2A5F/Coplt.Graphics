#include "Command.h"
#include "../../Api/FFI/Command.h"

#include <ranges>

#include "Binding.h"
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
#include "../FFI/Binding.h"

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
    Layout = nullptr;
    GPipeline = nullptr;
    Binding = nullptr;
    PipelineChanged = false;
    BindingChanged = false;
}

D3d12CommandInterpreter::D3d12CommandInterpreter(const NonNull<D3d12GpuQueue>& queue) : m_queue(queue)
{
}

void D3d12CommandInterpreter::Interpret(const FCommandSubmit& submit)
{
    m_descriptors = m_queue->m_frame_context->m_descriptors.get();
    m_descriptors->InitFrame(submit.GrowingResourceBindingCapacity, submit.GrowingSamplerBindingCapacity);
    const auto desc_heaps = m_descriptors->GetDescriptorHeaps();
    m_queue->m_cmd->SetDescriptorHeaps(2, desc_heaps.data());

    Analyze(submit);
    Translate(submit);

    Reset();
    m_descriptors = nullptr;
}

void D3d12CommandInterpreter::Reset()
{
    m_barrier_context.Reset();
    m_context.Reset();
    m_bindings_contexts.clear();
    m_bindings_items.clear();
}

void D3d12CommandInterpreter::Analyze(const FCommandSubmit& submit)
{
    m_bindings_contexts.reserve(submit.SyncBindingCount);
    const std::span command_items(submit.Commands, submit.CommandCount);
    for (u32 i = 0; i < command_items.size(); i++)
    {
        const auto& item = command_items[i];
        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Present:
        case FCommandType::Label:
        case FCommandType::BeginScope:
        case FCommandType::EndScope:
        case FCommandType::Barrier:
        case FCommandType::ClearColor:
        case FCommandType::ClearDepthStencil:
        case FCommandType::BufferCopy:
        case FCommandType::BufferImageCopy:
            continue;

        case FCommandType::Bind:
            Bind(submit, i, item.Bind);
            continue;
        case FCommandType::Render:
            AnalyzeRender(submit, i, item.Render);
            continue;
        case FCommandType::Compute:
            AnalyzeCompute(submit, i, item.Compute);
            continue;

        // subcommands
        case FCommandType::SetPipeline:
        case FCommandType::SetBinding:
        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
        case FCommandType::SyncBinding:
            COPLT_THROW("Sub commands cannot be placed in the main command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));
    }
    // m_descriptors->SyncTmp();
}

void D3d12CommandInterpreter::Bind(const FCommandSubmit& submit, u32 i, const FCommandBind& cmd) const
{
    const auto binding = cmd.Binding->QueryInterface<ID3d12ShaderBinding>();
    if (binding == nullptr)
        COPLT_THROW_FMT("Binding from different backends at command {}", i);
    const auto items = std::span(submit.BindItems + cmd.ItemsIndex, cmd.ItemCount);
    binding->Set(items);
}

void D3d12CommandInterpreter::AnalyzeRender(const FCommandSubmit& submit, u32 i, const FCommandRender& cmd)
{
    const auto commands = std::span(submit.RenderCommands + cmd.CommandStartIndex, cmd.CommandCount);
    for (u32 j = 0; j < commands.size(); j++)
    {
        const auto& item = commands[j];
        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Label:
        case FCommandType::BeginScope:
        case FCommandType::EndScope:
        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
            continue;

        case FCommandType::SetPipeline:
            SetPipelineContext(item.SetPipeline.Pipeline, i);
            continue;
        case FCommandType::SetBinding:
            SetBindingContext(item.SetBinding.Binding, i);
            continue;
        case FCommandType::SyncBinding:
            AnalyzeSyncBinding(submit, i, item.SyncBinding);
            continue;

        case FCommandType::Present:
        case FCommandType::Barrier:
        case FCommandType::ClearColor:
        case FCommandType::ClearDepthStencil:
        case FCommandType::Bind:
        case FCommandType::BufferCopy:
        case FCommandType::BufferImageCopy:
        case FCommandType::Render:
        case FCommandType::Compute:
            COPLT_THROW("Main commands cannot be placed in the sub command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));
    }
}

void D3d12CommandInterpreter::AnalyzeCompute(const FCommandSubmit& submit, u32 i, const FCommandCompute& cmd)
{
    const auto commands = std::span(submit.ComputeCommands + cmd.CommandStartIndex, cmd.CommandCount);
    for (u32 j = 0; j < commands.size(); j++)
    {
        const auto& item = commands[j];
        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Label:
        case FCommandType::BeginScope:
        case FCommandType::EndScope:
        case FCommandType::Dispatch:
            continue;

        case FCommandType::SetPipeline:
            SetPipelineContext(item.SetPipeline.Pipeline, i);
            continue;
        case FCommandType::SetBinding:
            SetBindingContext(item.SetBinding.Binding, i);
            continue;
        case FCommandType::SyncBinding:
            AnalyzeSyncBinding(submit, i, item.SyncBinding);
            continue;

        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
            COPLT_THROW("Compute commands cannot be placed in the render command list");
        case FCommandType::Present:
        case FCommandType::Barrier:
        case FCommandType::ClearColor:
        case FCommandType::ClearDepthStencil:
        case FCommandType::Bind:
        case FCommandType::BufferCopy:
        case FCommandType::BufferImageCopy:
        case FCommandType::Render:
        case FCommandType::Compute:
            COPLT_THROW("Main commands cannot be placed in the sub command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));
    }
}

void D3d12CommandInterpreter::AnalyzeSyncBinding(const FCommandSubmit& submit, u32 i, const FCommandSyncBinding& cmd)
{
    const auto index = cmd.Index;
    assert(index == m_bindings_contexts.size());
    m_bindings_contexts.push_back(BindingContext{});
    auto& ctx = m_bindings_contexts.back();
    if (!(m_context.PipelineChanged || m_context.BindingChanged)) return;
    if (const auto binding = m_context.Binding.get())
    {
        ctx.Index = m_bindings_items.size();
        binding->Update(m_queue);
        AllocBindings(binding);
        binding->ApplyChange();
        ctx.Count = m_bindings_items.size() - ctx.Index;
    }
    m_context.PipelineChanged = false;
    m_context.BindingChanged = false;
}

void D3d12CommandInterpreter::AllocBindings(NonNull<ID3d12ShaderBinding> binding)
{
    const NonNull descriptors = m_descriptors;
    const auto layout = binding->Layout();
    const auto classes = layout->GetTableGroups();
    const auto heaps = binding->DescHeaps();
    for (u32 c = 0; c < classes.size(); ++c)
    {
        const auto& groups = classes[c];
        const auto da = groups.Sampler ? descriptors->SamplerHeap() : descriptors->ResourceHeap();
        for (u32 g = 0; g < groups.Metas.size(); ++g)
        {
            const auto& heap = heaps[c][g];
            if (!heap) continue;
            const auto al = AllocBindingGroup(groups, da, heap);
            m_bindings_items.push_back(BindingItem{.c = c, .g = g, .al = al});
        }
    }
}

DescriptorAllocation D3d12CommandInterpreter::AllocBindingGroup(
    const ID3d12ShaderLayout::TableGroup& groups,
    NonNull<DescriptorAllocator> da,
    const Rc<DescriptorHeap>& heap
)
{
    if (groups.Scope == FShaderLayoutGroupScope::Persist)
    {
        PersistDescriptorAllocation al_place{};
        PersistDescriptorAllocation* al = &al_place;
        bool old = true;
        da->AllocatePersistent(heap.get(), al, old);
        if (!old || al->Version != heap->Version())
        {
            al->Version = heap->Version();
            da->Upload(*al, heap);
        }
        return static_cast<DescriptorAllocation>(*al);
    }
    else
    {
        DescriptorAllocation al_place{};
        DescriptorAllocation* al = &al_place;
        da->AllocateTransient(heap->Size(), al);
        da->Upload(*al, heap);
        // da->StoreTmp(*al, heap);
        return *al;
    }
}

void D3d12CommandInterpreter::Translate(const FCommandSubmit& submit)
{
    const std::span command_items(submit.Commands, submit.CommandCount);
    for (u32 i = 0; i < command_items.size(); i++)
    {
        const auto& item = command_items[i];
        switch (item.Type)
        {
        case FCommandType::None:
        case FCommandType::Present:
        case FCommandType::Bind:
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
        case FCommandType::BufferCopy:
            BufferCopy(submit, i, item.BufferCopy);
            continue;
        case FCommandType::BufferImageCopy:
            BufferImageCopy(submit, i, item.BufferImageCopy);
            continue;
        case FCommandType::Render:
            Render(submit, i, item.Render);
            continue;
        case FCommandType::Compute:
            Compute(submit, i, item.Compute);
            continue;

        // subcommands
        case FCommandType::SetPipeline:
        case FCommandType::SetBinding:
        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
        case FCommandType::Dispatch:
        case FCommandType::SyncBinding:
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

void D3d12CommandInterpreter::BufferImageCopy(const FCommandSubmit& submit, u32 i, const FCommandBufferImageCopy& cmd) const
{
    const auto& cmd_pack = m_queue->m_cmd;
    const auto& range = submit.BufferImageCopyRanges[cmd.RangeIndex];
    const auto image_obj_may = TryGetImage(cmd.Image.Get(submit));
    if (image_obj_may == nullptr)
        COPLT_THROW_FMT("Image is null or resource is not image at command {}", i);
    const NonNull image_obj = image_obj_may;
    const auto image_data = image_obj->Data();
    const auto IsDsOrYCbCr = IsDepthStencil(image_data->m_format) || IsYCbCr(image_data->m_format);
    auto bytes_per_row = range.BytesPerRow;
    auto rows_per_image = range.RowsPerImage;
    if (bytes_per_row == 0)
    {
        const auto [block_width, _] = GetBlockDimensions(image_data->m_format);
        const auto block_size = GetBlockCopySize(image_data->m_format, IsDsOrYCbCr ? Some(range.Plane) : nullptr);
        bytes_per_row = (range.ImageExtent[0] / block_width) * block_size;
        bytes_per_row = Aligned256(bytes_per_row);
    }
    else
    {
        if (!IsAligned256(bytes_per_row))
            COPLT_THROW_FMT("BytesPerRow must must be a multiple of 256 at command {}", i);
    }
    if (rows_per_image == 0)
    {
        if (image_data->m_dimension == FImageDimension::Three)
        {
            rows_per_image = range.ImageExtent[1] * range.ImageExtent[2];
        }
        else
        {
            rows_per_image = range.ImageExtent[1];
        }
    }
    if (image_data->m_dimension == FImageDimension::Three)
    {
        if (range.ImageIndex != 0 || range.ImageCount != 1)
            COPLT_THROW_FMT("Image index or count out of range at command {}", i);
    }
    else
    {
        if (range.ImageCount < 1 || range.ImageIndex + range.ImageCount > image_data->m_depth_or_length)
            COPLT_THROW_FMT("Image index or count out of range at command {}", i);
    }
    if (IsDepthStencil(image_data->m_format) || IsYCbCr(image_data->m_format))
    {
        if (static_cast<u8>(range.Plane) >= 2)
            COPLT_THROW_FMT("Plane index or count out of range at command {}", i);
    }
    else
    {
        if (range.Plane != FImagePlane::All)
            COPLT_THROW_FMT("Plane index or count out of range at command {}", i);
    }
    const auto image = image_obj->GetResourcePtr();
    auto buffer = NonNull<ID3D12Resource>::Unchecked(nullptr);
    if (cmd.BufferType == FBufferRefType::Buffer)
    {
        buffer = GetResource(cmd.Buffer.Buffer.Get(submit));
    }
    else if (cmd.BufferType == FBufferRefType::Upload)
    {
        const auto& obj = m_queue->m_frame_context->m_upload_buffers[cmd.Buffer.Upload.Index];
        buffer = obj.m_resource.m_resource.Get();
    }
    else
    {
        COPLT_THROW_FMT("Unknown Buffer Type {} at command {}", static_cast<u8>(cmd.BufferType), i);
    }
    D3D12_TEXTURE_COPY_LOCATION buffer_loc{};
    buffer_loc.pResource = buffer;
    buffer_loc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    buffer_loc.PlacedFootprint.Footprint.Format = GetBufferImageCopyFormat(image_data->m_format, range.Plane, IsDsOrYCbCr);
    buffer_loc.PlacedFootprint.Footprint.Width = range.ImageExtent[0];
    buffer_loc.PlacedFootprint.Footprint.Height = range.ImageExtent[1];
    buffer_loc.PlacedFootprint.Footprint.Depth = range.ImageExtent[2];
    buffer_loc.PlacedFootprint.Footprint.RowPitch = bytes_per_row;
    D3D12_TEXTURE_COPY_LOCATION image_loc{};
    image_loc.pResource = image;
    image_loc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    const auto image_stride = bytes_per_row * rows_per_image;
    for (auto ai = 0u; ai < range.ImageCount; ++ai)
    {
        buffer_loc.PlacedFootprint.Offset = range.BufferOffset + ai * image_stride;
        image_loc.SubresourceIndex = D3D12CalcSubresource(
            range.MipLevel, range.ImageIndex + ai, static_cast<u8>(range.Plane),
            image_data->m_mip_levels, image_data->m_depth_or_length
        );
        if (cmd.ImageToBuffer)
        {
            D3D12_BOX box{};
            box.left = range.ImageOffset[0];
            box.top = range.ImageOffset[1];
            box.front = range.ImageOffset[2];
            box.right = range.ImageOffset[0] + range.ImageExtent[0];
            box.bottom = range.ImageOffset[1] + range.ImageExtent[1];
            box.back = range.ImageOffset[2] + range.ImageExtent[2];
            cmd_pack->CopyTextureRegion(
                &buffer_loc,
                0,
                0,
                0,
                &image_loc,
                &box
            );
        }
        else
        {
            cmd_pack->CopyTextureRegion(
                &image_loc,
                range.ImageOffset[0],
                range.ImageOffset[1],
                range.ImageOffset[2],
                &buffer_loc,
                nullptr
            );
        }
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
            const auto& d_load = info.DsvLoadOp[0];
            const auto& d_store = info.DsvStoreOp[0];
            const auto& s_load = info.DsvLoadOp[1];
            const auto& s_store = info.DsvStoreOp[1];
            D3D12_RENDER_PASS_DEPTH_STENCIL_DESC desc{};
            desc.cpuDescriptor = GetDsv(info.Dsv.Get(submit));
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
                const auto& res_info = submit.ResolveInfos[info.DsvResolveInfoIndex];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetResource(res_info.Src.Get(submit));
                res.pDstResource = GetResource(res_info.Dst.Get(submit));
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            desc.StencilEndingAccess = ToDx(s_store, [&](D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS& res)
            {
                const auto& res_info = submit.ResolveInfos[info.DsvResolveInfoIndex];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetResource(res_info.Src.Get(submit));
                res.pDstResource = GetResource(res_info.Dst.Get(submit));
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
            ds[0] = desc;
        }
        for (u32 n = 0; n < num_rtv; ++n)
        {
            const auto& rtv = info.Rtv[n];
            const auto& load = info.RtvLoadOp[n];
            const auto& store = info.RtvStoreOp[n];
            D3D12_RENDER_PASS_RENDER_TARGET_DESC desc{};
            desc.cpuDescriptor = GetRtv(rtv.Get(submit));
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
                const auto& res_info = submit.ResolveInfos[info.ResolveInfoIndex[n]];
                res.Format = ToDx(res_info.Format);
                res.pSrcResource = GetResource(res_info.Src.Get(submit));
                res.pDstResource = GetResource(res_info.Dst.Get(submit));
                res.SubresourceCount = 0;
                res.ResolveMode = ToDx(res_info.Mode);
            });
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
            SetBindingContext(item.SetBinding.Binding, j);
            continue;
        case FCommandType::SetViewportScissor:
            RenderSetViewportScissor(submit, j, item.SetViewportScissor);
            continue;
        case FCommandType::SetMeshBuffers:
            RenderSetMeshBuffers(submit, j, item.SetMeshBuffers);
            continue;
        case FCommandType::Draw:
            RenderDraw(submit, j, item.Draw);
            continue;
        case FCommandType::Dispatch:
            RenderDispatch(submit, j, item.Dispatch);
            continue;
        case FCommandType::SyncBinding:
            SyncBinding(submit, j, item.SyncBinding);
            continue;

        case FCommandType::Present:
        case FCommandType::Barrier:
        case FCommandType::ClearColor:
        case FCommandType::ClearDepthStencil:
        case FCommandType::Bind:
        case FCommandType::BufferCopy:
        case FCommandType::BufferImageCopy:
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

void D3d12CommandInterpreter::RenderDispatch(const FCommandSubmit& submit, u32 i, const FCommandDispatch& cmd) const
{
    if (cmd.Type != FDispatchType::Mesh)
        COPLT_THROW_FMT("Render only supports DispatchMesh and does not support Dispatch for Compute at command {}", i);
    m_queue->m_cmd.m_list6->DispatchMesh(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
}

void D3d12CommandInterpreter::RenderSetViewportScissor(const FCommandSubmit& submit, u32 i, const FCommandSetViewportScissor& cmd) const
{
    m_queue->m_cmd->RSSetViewports(cmd.ViewportCount, reinterpret_cast<const D3D12_VIEWPORT*>(submit.Viewports + cmd.ViewportIndex));
    m_queue->m_cmd->RSSetScissorRects(cmd.ScissorRectCount, reinterpret_cast<const D3D12_RECT*>(submit.Rects + cmd.ScissorRectIndex));
}

void D3d12CommandInterpreter::RenderSetMeshBuffers(const FCommandSubmit& submit, u32 i, const FCommandSetMeshBuffers& cmd) const
{
    const auto& cmd_pack = m_queue->m_cmd;
    const auto& buffers = submit.MeshBuffers[cmd.PayloadIndex];
    const auto defs = buffers.MeshLayout->GetBuffers();
    const auto vbs = std::span(submit.VertexBufferRanges + buffers.VertexBuffersIndex, buffers.VertexBufferCount);
    if (!buffers.IndexBuffer.Buffer.IsEmpty())
    {
        const auto resource = GetResource(buffers.IndexBuffer.Buffer.Get(submit));
        D3D12_INDEX_BUFFER_VIEW view{};
        view.BufferLocation = resource->GetGPUVirtualAddress() + buffers.IndexBuffer.Offset;
        view.SizeInBytes = buffers.IndexBuffer.Size;
        view.Format = ToDx(cmd.IndexFormat);
        cmd_pack->IASetIndexBuffer(&view);
    }
    D3D12_VERTEX_BUFFER_VIEW views[vbs.size()];
    for (u32 j = 0; j < vbs.size(); j++)
    {
        const auto& range = vbs[j];
        const auto& def = defs[range.Index];
        const auto resource = GetResource(range.Buffer.Get(submit));
        D3D12_VERTEX_BUFFER_VIEW view{};
        view.BufferLocation = resource->GetGPUVirtualAddress() + range.Offset;
        view.SizeInBytes = range.Size;
        view.StrideInBytes = def.Stride;
        views[j] = view;
    }
    cmd_pack->IASetVertexBuffers(cmd.VertexStartSlot, buffers.VertexBufferCount, views);
}

void D3d12CommandInterpreter::Compute(const FCommandSubmit& submit, u32 i, const FCommandCompute& cmd)
{
    m_context.Reset();
    const auto& _info = submit.ComputeInfos[cmd.InfoIndex];
    const auto commands = std::span(submit.ComputeCommands + cmd.CommandStartIndex, cmd.CommandCount);
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
            SetBindingContext(item.SetBinding.Binding, j);
            continue;
        case FCommandType::Dispatch:
            ComputeDispatch(submit, j, item.Dispatch);
            continue;
        case FCommandType::SyncBinding:
            SyncBinding(submit, j, item.SyncBinding);
            continue;

        case FCommandType::SetViewportScissor:
        case FCommandType::SetMeshBuffers:
        case FCommandType::Draw:
            COPLT_THROW("Compute commands cannot be placed in the render command list");
        case FCommandType::Present:
        case FCommandType::Barrier:
        case FCommandType::ClearColor:
        case FCommandType::ClearDepthStencil:
        case FCommandType::Bind:
        case FCommandType::BufferCopy:
        case FCommandType::BufferImageCopy:
        case FCommandType::Render:
        case FCommandType::Compute:
            COPLT_THROW("Main commands cannot be placed in the sub command list");
        }
        COPLT_THROW_FMT("Unknown command type {}", static_cast<u32>(item.Type));
    }
}

void D3d12CommandInterpreter::ComputeDispatch(const FCommandSubmit& submit, u32 i, const FCommandDispatch& cmd) const
{
    if (cmd.Type == FDispatchType::Mesh)
        m_queue->m_cmd.m_list6->DispatchMesh(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
    else
        m_queue->m_cmd->Dispatch(cmd.GroupCountX, cmd.GroupCountY, cmd.GroupCountZ);
}

void D3d12CommandInterpreter::SetPipelineContext(NonNull<FShaderPipeline> pipeline, const u32 i)
{
    if (pipeline == m_context.Pipeline) return;
    m_context.Pipeline = pipeline->QueryInterface<FD3d12PipelineState>();
    if (!m_context.Pipeline)
    {
        COPLT_THROW_FMT(
            "Pipeline({:#x}) comes from different backends at cmd {}",
            static_cast<size_t>(pipeline), i
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
    m_context.PipelineChanged = true;
}

void D3d12CommandInterpreter::SetPipeline(
    const CmdListPack& cmd_pack, NonNull<FShaderPipeline> pipeline, const u32 i
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
    cmd_pack->SetPipelineState(static_cast<ID3D12PipelineState*>(m_context.Pipeline->GetPipelineStatePtr()));
}

void D3d12CommandInterpreter::SetBindingContext(NonNull<FShaderBinding> binding, u32 i)
{
    if (binding == m_context.Binding) return;
    m_context.Binding = binding->QueryInterface<ID3d12ShaderBinding>();
    if (!m_context.Binding)
        COPLT_THROW_FMT("Shader bindings from different backend at command {}", i);
    m_context.BindingChanged = true;
}

void D3d12CommandInterpreter::SyncBinding(const FCommandSubmit& submit, u32 i, const FCommandSyncBinding& cmd)
{
    if (!(m_context.PipelineChanged || m_context.BindingChanged)) return;
    if (const auto binding = m_context.Binding.get())
    {
        const auto& ctx = m_bindings_contexts[cmd.Index];
        std::span<BindingItem> items{};
        if (ctx) items = std::span(m_bindings_items.data() + ctx.Index, ctx.Count);
        UseBinding(binding, items);
    }
    m_context.PipelineChanged = false;
    m_context.BindingChanged = false;
}

void D3d12CommandInterpreter::UseBinding(NonNull<ID3d12ShaderBinding> binding, const std::span<BindingItem> items)
{
    const auto has_pixel = HasFlags(m_context.Pipeline->GetStages(), FShaderStageFlags::Pixel);
    const auto& cmd_pack = m_queue->m_cmd;
    const auto layout = binding->Layout();
    const auto classes = layout->GetTableGroups();

    for (const auto& item : items)
    {
        const auto& groups = classes[item.c];
        const auto& meta = groups.Metas[item.g];
        const auto& da = groups.Sampler ? m_descriptors->SamplerHeap() : m_descriptors->ResourceHeap();
        const auto handle = da->GetRemoteHandle(item.al.Offset);
        if (has_pixel) cmd_pack->SetGraphicsRootDescriptorTable(meta.RootIndex, handle);
        else cmd_pack->SetComputeRootDescriptorTable(meta.RootIndex, handle);
    }

    // todo 其他 Root 项
}

Ptr<ID3d12GpuImage> D3d12CommandInterpreter::TryGetImage(const FResourceMeta& meta)
{
    return TryGetImage(meta.Output, meta.Type);
}

Ptr<ID3d12GpuImage> D3d12CommandInterpreter::TryGetImage(NonNull<FUnknown> object, FResourceRefType type)
{
    switch (type)
    {
    case FResourceRefType::Image:
        {
            const auto image = object->QueryInterface<ID3d12GpuImage>();
            if (!image)
                COPLT_THROW("Resources from different backends");
            return image;
        }
    case FResourceRefType::Buffer:
        return nullptr;
    case FResourceRefType::Output:
        return nullptr;
    default:
        COPLT_THROW_FMT("Unknown resource ref type {}", static_cast<u8>(type));
    }
}

Ptr<ID3d12GpuBuffer> D3d12CommandInterpreter::TryGetBuffer(const FResourceMeta& meta)
{
    return TryGetBuffer(meta.Output, meta.Type);
}

Ptr<ID3d12GpuBuffer> D3d12CommandInterpreter::TryGetBuffer(NonNull<FUnknown> object, FResourceRefType type)
{
    switch (type)
    {
    case FResourceRefType::Image:
        return nullptr;
    case FResourceRefType::Buffer:
        {
            const auto buffer = object->QueryInterface<ID3d12GpuBuffer>();
            if (!buffer)
                COPLT_THROW("Resources from different backends");
            return buffer;
        }
    case FResourceRefType::Output:
        return nullptr;
    default:
        COPLT_THROW_FMT("Unknown resource ref type {}", static_cast<u8>(type));
    }
}

NonNull<ID3D12Resource> D3d12CommandInterpreter::GetResource(const FResourceMeta& meta)
{
    return GetResource(meta.Output, meta.Type);
}

NonNull<ID3D12Resource> D3d12CommandInterpreter::GetResource(NonNull<FUnknown> object, FResourceRefType type)
{
    switch (type)
    {
    case FResourceRefType::Image:
        {
            const auto image = object->QueryInterface<ID3d12GpuImage>();
            if (!image)
                COPLT_THROW("Resources from different backends");
            return image->GetResourcePtr();
        }
    case FResourceRefType::Buffer:
        {
            const auto buffer = object->QueryInterface<ID3d12GpuBuffer>();
            if (!buffer)
                COPLT_THROW("Resources from different backends");
            return buffer->GetResourcePtr();
        }
    case FResourceRefType::Output:
        {
            const auto output = object->QueryInterface<FD3d12GpuOutput>();
            if (!output)
                COPLT_THROW("Resources from different backends");
            ID3D12Resource* ptr{};
            output->GetCurrentResourcePtr(&ptr).TryThrow();
            return NonNull<ID3D12Resource>::Unchecked(ptr);
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

D3D12_CPU_DESCRIPTOR_HANDLE D3d12CommandInterpreter::GetDsv(const FResourceMeta& meta)
{
    switch (meta.Type)
    {
    case FResourceRefType::Image:
    case FResourceRefType::Buffer:
        COPLT_THROW("TODO");
    case FResourceRefType::Output:
        COPLT_THROW("Output does not support Dsv usage");
    default:
        COPLT_THROW_FMT("Unknown resource ref type {}", static_cast<u8>(meta.Type));
    }
}
