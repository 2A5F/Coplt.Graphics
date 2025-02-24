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
    if (m_layout == nullptr) throw WRuntimeException(L"Layout from different backends");

    const auto item_metas = m_layout->GetItemMeta();
    m_views = std::vector<Rc<FGpuView>>(item_metas.size(), {});
}

FResult D3d12ShaderBinding::SetName(const Str8or16& name) noexcept
{
    return FResult::None();
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
    for (const auto& [View, Index] : bindings)
    {
        if (Index >= m_views.size()) throw WRuntimeException(L"Index out of bounds");
        if (View == nullptr)
        {
            m_views[Index] = nullptr;
            continue;
        }
        Rc<FGpuView> view = Rc<FGpuView>::UnsafeClone(View);
        // todo check
        m_views[Index] = std::move(view);
    }
}
