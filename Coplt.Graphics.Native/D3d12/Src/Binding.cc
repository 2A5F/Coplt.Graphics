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

    const auto item_metas = m_layout->GetItemMetas();
    m_f_views = std::vector<FView>(item_metas.size(), {});
    m_views = std::vector<View>(item_metas.size(), {});
}

FResult D3d12ShaderBinding::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FView* D3d12ShaderBinding::GetViews(u32* out_size) noexcept
{
    *out_size = m_f_views.size();
    return m_f_views.data();
}

FResult D3d12ShaderBinding::Set(const u32 count, const FShaderBindingBatchSet* bindings) noexcept
{
    return feb([&]
    {
        Set(std::span(bindings, count));
    });
}

void D3d12ShaderBinding::Set(const std::span<const FShaderBindingBatchSet> bindings)
{
    auto item_metas = m_layout->GetItemMetas();
    auto item_defines = m_layout->GetItemDefines();
    for (const auto& [View, Index] : bindings)
    {
        if (Index >= m_views.size())
            COPLT_THROW("Index out of bounds");
        const auto& define = item_defines[Index];
        switch (View.Type)
        {
        case FViewType::None:
            break;
        case FViewType::Buffer:
            if (!define.IsAllowBuffer())
                COPLT_THROW_FMT("Binding index {} is not allowed to bind to buffer.", Index);
            break;
        }
        m_f_views[Index] = View;
        m_views[Index] = View;
    }
}
