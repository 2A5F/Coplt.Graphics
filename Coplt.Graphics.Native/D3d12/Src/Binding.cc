#include "Binding.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "../FFI/Layout.h"

using namespace Coplt;

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
        m_changed_groups.Add(info.Group);
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

const HashSet<u32>& D3d12ShaderBinding::ChangedGroups() noexcept
{
    return m_changed_groups;
}

void D3d12ShaderBinding::ApplyChange()
{
    m_changed_items.Clear();
    m_changed_groups.Clear();
}
