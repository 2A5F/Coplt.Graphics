#include "Binding.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "../FFI/Layout.h"
#include "Queue.h"

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
    m_items = std::vector<std::vector<std::vector<u32>>>(classes.size(), {});
    m_allocations = std::vector<std::vector<DescriptorAllocation>>(classes.size(), {});
    for (size_t i = 0; i < classes.size(); ++i)
    {
        const auto& group = classes[i];
        m_allocations[i] = std::vector<DescriptorAllocation>(group.Metas.size(), {});
        auto& items = m_items[i] = std::vector<std::vector<u32>>(group.Metas.size(), {});
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
        m_items[info.Class][info.Group].push_back(i);
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
        m_changed_items.Add(Index);
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
    return !m_changed_items.IsEmpty() || !m_changed_groups.IsEmpty();
}

const HashSet<u32>& D3d12ShaderBinding::ChangedItems() noexcept
{
    return m_changed_items;
}

const HashSet<u64>& D3d12ShaderBinding::ChangedGroups() noexcept
{
    return m_changed_groups;
}

void D3d12ShaderBinding::Update(D3d12GpuQueue* queue)
{
    if (!Changed()) return;
    auto& dm = queue->m_descriptor_manager;
    const auto defs = m_layout->GetItemDefines();
    const auto infos = m_layout->GetItemInfos();
    const auto classes = m_layout->GetTableGroups();
    const auto& changed_groups = m_changed_groups;
    const auto& changed_items = m_changed_items;
    for (auto cg : changed_groups)
    {
        auto c = static_cast<u32>(cg >> 32);
        auto g = static_cast<u32>(cg & 0xFFFFFFFF);
        const auto& groups = classes[c];
        const auto& meta = groups.Metas[g];
        auto& ai = m_allocations[c][g];
        const auto type = groups.Scope == TableScope::Material ? DescriptorAllocationType::Persist : DescriptorAllocationType::Transient;
        if (meta.Size == 0) continue;
        ai = (groups.Sampler ? dm.m_sampler : dm.m_cbv_srv_uav)->Allocate(meta.Size, type);
        // todo 写入描述符
    }
    for (auto i : changed_items)
    {
        const auto& def = defs[i];
        const auto& info = infos[i];
        // todo 改成只处理表外项
    }
    m_changed_items.Clear();
    m_changed_groups.Clear();
}
