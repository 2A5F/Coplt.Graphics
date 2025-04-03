#include "Binding.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "Queue.h"
#include "../../Api/Include/AllocObjectId.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

D3d12ShaderBindGroup::D3d12ShaderBindGroup(Rc<D3d12GpuDevice>&& device, const FShaderBindGroupCreateOptions& options) : m_device(std::move(device))
{
    m_layout = Rc<FBindGroupLayout>::UnsafeClone(NonNull(options.Layout));
    const auto items = m_layout->GetItems();
    usize sum_count = 0;
    for (const auto& item : items)
    {
        sum_count += std::max(1u, item.Count);
    }
    m_views = std::vector(sum_count, View{});
    m_item_indexes.reserve(items.size());
    for (u32 i = 0, off = 0; i < items.size(); ++i)
    {
        m_item_indexes.push_back(off);
        off += std::max(1u, items[i].Count);
    }
    Set(std::span(options.InitBindings, options.InitBindings + options.NumInitBindings));
    if (!options.Name.is_null())
    {
        m_name = options.Name.ToString();
    }
}

FResult D3d12ShaderBindGroup::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        m_name = name.ToString();
    });
}

void D3d12ShaderBindGroup::Set(const std::span<const FBindItem> items)
{
    const auto defs = m_layout->GetItems();
    for (const auto& item : items)
    {
        if (item.Slot >= defs.size())
            COPLT_THROW("Index out of range");
        const auto& def = defs[item.Slot];
        if (item.Index >= def.Count)
            COPLT_THROW("Index out of range");
        const auto index = m_item_indexes[item.Slot] + item.Index;
        View view(item.View);
        if (!view.IsCompatible(def))
        {
            const auto id = ObjectId();
            if (m_device->Debug() && m_device->Logger().IsEnable(FLogLevel::Error))
            {
                const auto name = m_name->GetStr();
                if (name.is_null())
                    m_device->Logger().Log(FLogLevel::Error, fmt::format("Incompatible bindings in BindGroup({})", id));
                else if (name.is8())
                    m_device->Logger().Log(FLogLevel::Error, fmt::format("Incompatible bindings in BindGroup({}, \"{}\")", id, name.str8));
                else
                    m_device->Logger().LogW(
                        FLogLevel::Error,
                        fmt::format(
                            L"Incompatible bindings in BindGroup({}, \"{}\")", id,
                            reinterpret_cast<const wchar_t*>(name.str16)
                        )
                    );
            }
            COPLT_THROW_FMT("Incompatible bindings [{},{}] in BindGroup({})", item.Slot, item.Index, id);
        }
        m_views[index] = std::move(view);
    }
}

D3d12ShaderBinding::D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options) : m_device(std::move(device))
{
    // todo
}

FResult D3d12ShaderBinding::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}
