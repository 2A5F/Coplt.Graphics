#include "Binding.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "../FFI/Layout.h"
#include "Queue.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

namespace
{
    using TableScope = ID3d12ShaderLayout::TableScope;
}

D3d12ShaderBinding::D3d12ShaderBinding(
    Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options
) : m_device(std::move(device))
{
    m_dx_device = m_device->m_device;

    m_layout = Rc<ID3d12ShaderLayout>::UnsafeClone(options.Layout->QueryInterface<ID3d12ShaderLayout>());
    if (m_layout == nullptr)
        COPLT_THROW("Layout from different backends");

    const auto defs = m_layout->GetItemDefines();
    m_views = std::vector<View>(defs.size(), {});

    const auto classes = m_layout->GetTableGroups();
    m_item_indexes = std::vector<std::vector<std::vector<u32>>>(classes.size(), {});
    m_desc_heaps = std::vector<std::vector<Rc<DescriptorHeap>>>(classes.size(), {});
    for (size_t i = 0; i < classes.size(); ++i)
    {
        const auto& group = classes[i];
        m_desc_heaps[i] = std::vector<Rc<DescriptorHeap>>(group.Metas.size(), {});
        auto& items = m_item_indexes[i] = std::vector<std::vector<u32>>(group.Metas.size(), {});
        for (size_t j = 0; j < group.Metas.size(); ++j)
        {
            const auto& meta = group.Metas[j];
            items[j].reserve(meta.Ranges.size());
        }
    }

    const auto infos = m_layout->GetItemInfos();
    for (size_t i = 0; i < infos.size(); ++i)
    {
        const auto& info = infos[i];
        m_item_indexes[info.Class][info.Group].push_back(i);
    }
}

FResult D3d12ShaderBinding::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

void D3d12ShaderBinding::Set(const std::span<const FBindItem> bindings)
{
    auto infos = m_layout->GetItemInfos();
    auto item_defines = m_layout->GetItemDefines();
    for (const auto& [View, Index] : bindings)
    {
        if (Index >= m_views.size())
            COPLT_THROW("Index out of bounds");
        const auto& define = item_defines[Index];
        const auto info = infos[Index];
        switch (View.Type)
        {
        case FViewType::None:
            break;
        case FViewType::Buffer:
            if (!define.IsAllowBuffer())
                COPLT_THROW_FMT("Binding index {} is not allowed to bind to buffer.", Index);
            break;
        }
        m_views[Index] = View;
        if (info.Place == FShaderLayoutItemPlace::Grouped)
        {
            m_changed_groups.Add((static_cast<u64>(info.Class) << 32) | static_cast<u64>(info.Group));
        }
    }
}

const Rc<ID3d12ShaderLayout>& D3d12ShaderBinding::Layout() noexcept
{
    return m_layout;
}

std::span<View> D3d12ShaderBinding::Views() noexcept
{
    return m_views;
}

bool D3d12ShaderBinding::Changed() noexcept
{
    return !m_changed_groups.IsEmpty();
}

const HashSet<u64>& D3d12ShaderBinding::ChangedGroups() noexcept
{
    return m_changed_groups;
}

std::span<const std::vector<Rc<DescriptorHeap>>> D3d12ShaderBinding::DescHeaps() noexcept
{
    return m_desc_heaps;
}

void D3d12ShaderBinding::Update(NonNull<D3d12GpuQueue> queue)
{
    if (!Changed()) return;
    const auto defs = m_layout->GetItemDefines();
    const auto infos = m_layout->GetItemInfos();
    const auto classes = m_layout->GetTableGroups();
    const auto& changed_groups = m_changed_groups;
    for (const auto cg : changed_groups)
    {
        const auto c = static_cast<u32>(cg >> 32);
        const auto g = static_cast<u32>(cg & 0xFFFFFFFF);
        const auto& groups = classes[c];
        const auto& meta = groups.Metas[g];
        if (meta.Size == 0) continue;
        auto& heap = m_desc_heaps[c][g];
        if (heap == nullptr)
            heap = new DescriptorHeap(
                m_dx_device,
                groups.Sampler ? D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER : D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
                meta.Size, false
            );
        const auto& indexes = m_item_indexes[c][g];
        const auto handle_start = heap->GetLocalHandle();
        const auto stride = heap->Stride();
        for (const auto& i : indexes)
        {
            const auto& def = defs[i];
            const auto& info = infos[i];
            const auto& range = meta.Ranges[info.Index];
            const CD3DX12_CPU_DESCRIPTOR_HANDLE handle(handle_start, static_cast<int>(range.OffsetInDescriptorsFromTableStart), stride);
            const auto& view = m_views[i];
            view.CreateDescriptor(m_dx_device.Get(), def, handle, meta.View);
        }
    }
}

void D3d12ShaderBinding::ApplyChange()
{
    m_changed_groups.Clear();
}
