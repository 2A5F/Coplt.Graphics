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
    CountSlots = sum_count;
    m_views = std::vector(sum_count, View{});
    m_item_indexes.reserve(items.size());
    m_define_indexes.reserve(sum_count);
    for (u32 i = 0, off = 0; i < items.size(); ++i)
    {
        m_item_indexes.push_back(off);
        const auto count = std::max(1u, items[i].Count);
        off += count;
        for (int j = 0; j < count; ++j)
        {
            m_define_indexes.push_back(i);
        }
    }
    Set(std::span(options.Bindings, options.Bindings + options.NumBindings));
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

FShaderBindGroupData* D3d12ShaderBindGroup::ShaderBindGroupData() noexcept
{
    return this;
}

const Rc<FBindGroupLayout>& D3d12ShaderBindGroup::Layout() const noexcept
{
    return m_layout;
}

std::span<const View> D3d12ShaderBindGroup::Views() const noexcept
{
    return m_views;
}

std::span<const u32> D3d12ShaderBindGroup::ItemIndexes() const noexcept
{
    return m_item_indexes;
}

std::span<const u32> D3d12ShaderBindGroup::DefineIndexes() const noexcept
{
    return m_define_indexes;
}

RwLock& D3d12ShaderBindGroup::Lock() noexcept
{
    return m_lock;
}

void D3d12ShaderBindGroup::Set(const std::span<const FSetBindItem> items)
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
    const auto layout = NonNull(options.Layout)->QueryInterface<ID3d12BindingLayout>();
    if (layout == nullptr)
        COPLT_THROW("Layout from different backends");
    m_layout = Rc<ID3d12BindingLayout>::UnsafeClone(layout);
    const auto groups = m_layout->Groups();
    m_groups = std::vector(groups.size(), Rc<ID3d12ShaderBindGroup>{});
    Set(std::span(options.BindGroups, options.BindGroups + options.NumBindGroups));
}

FResult D3d12ShaderBinding::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FShaderBindingData* D3d12ShaderBinding::ShaderBindingData() noexcept
{
    return this;
}

const Rc<ID3d12BindingLayout>& D3d12ShaderBinding::Layout() const noexcept
{
    return m_layout;
}

std::span<const Rc<ID3d12ShaderBindGroup>> D3d12ShaderBinding::Groups() const noexcept
{
    return m_groups;
}

RwLock& D3d12ShaderBinding::Lock() noexcept
{
    return m_lock;
}

void D3d12ShaderBinding::Set(const std::span<FSetBindGroupItem> items)
{
    for (const auto& item : items)
    {
        if (item.Index >= m_groups.size())
            COPLT_THROW("Index out of range");
        if (item.BindGroup)
        {
            const auto bind_group = item.BindGroup->QueryInterface<ID3d12ShaderBindGroup>();
            if (bind_group == nullptr)
                COPLT_THROW("BindGroup from different backends");
            m_groups[item.Index] = Rc<ID3d12ShaderBindGroup>::UnsafeClone(bind_group);
        }
        else
        {
            m_groups[item.Index] = nullptr;
        }
    }
}
