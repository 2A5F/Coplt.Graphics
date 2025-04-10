#include "Binding.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "Queue.h"
#include "../../Api/Include/AllocObjectId.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

D3d12ShaderBindGroup::D3d12ShaderBindGroup(Rc<D3d12GpuDevice>&& device, const FShaderBindGroupCreateOptions& options) : m_device(std::move(device))
{
    const auto layout = NonNull(options.Layout)->QueryInterface<ID3d12BindGroupLayout>();
    if (layout == nullptr)
        COPLT_THROW("Layout from different backends");
    m_layout = Rc<ID3d12BindGroupLayout>::UnsafeClone(layout);
    const auto items = m_layout->GetItems();
    usize sum_count = 0;
    for (const auto& item : items)
    {
        sum_count += std::max(1u, item.Count);
    }
    CountSlots = sum_count;
    m_views = std::vector(sum_count, View{});
    m_changed_marks = std::vector(sum_count, false);
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
    const auto& layout_data = m_layout->Data();
    m_resource_heap_inc = m_device->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    m_sampler_heap_inc = m_device->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
    if (layout_data.ResourceTableSize > 0)
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = layout_data.ResourceTableSize;
        chr | m_device->m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_resource_heap));
    }
    if (layout_data.SamplerTableSize > 0)
    {
        D3D12_DESCRIPTOR_HEAP_DESC desc{};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
        desc.NumDescriptors = layout_data.SamplerTableSize;
        chr | m_device->m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_sampler_heap));
    }
    if (!options.Name.is_null())
    {
        m_name = options.Name.ToString();
        DoSetName(m_name->GetStr());
    }
    Set(std::span(options.Bindings, options.Bindings + options.NumBindings));
}

FResult D3d12ShaderBindGroup::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        m_name = name.ToString();
        DoSetName(name);
    });
}

void D3d12ShaderBindGroup::DoSetName(const FStr8or16& name) const
{
    if (name.is_null()) return;
    if (m_resource_heap)
    {
        if (name.is8())
        {
            const auto str = fmt::format("[{}]::ResourceDescriptorHeap", name.str8);
            chr | m_resource_heap >> SetNameEx(FStr8or16(str));
        }
        else
        {
            const auto str = fmt::format(
                L"[{}]::ResourceDescriptorHeap",
                reinterpret_cast<const wchar_t*>(name.str16)
            );
            chr | m_resource_heap >> SetNameEx(FStr8or16(str));
        }
    }
    if (m_sampler_heap)
    {
        if (name.is8())
        {
            const auto str = fmt::format("[{}]::SamplerDescriptorHeap", name.str8);
            chr | m_resource_heap >> SetNameEx(FStr8or16(str));
        }
        else
        {
            const auto str = fmt::format(
                L"[{}]::SamplerDescriptorHeap",
                reinterpret_cast<const wchar_t*>(name.str16)
            );
            chr | m_resource_heap >> SetNameEx(FStr8or16(str));
        }
    }
}

FShaderBindGroupData* D3d12ShaderBindGroup::ShaderBindGroupData() noexcept
{
    return this;
}

u64 D3d12ShaderBindGroup::Version() const noexcept
{
    return m_version;
}

const Rc<ID3d12BindGroupLayout>& D3d12ShaderBindGroup::Layout() const noexcept
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

RwLock& D3d12ShaderBindGroup::SelfLock() noexcept
{
    return m_self_lock;
}

void D3d12ShaderBindGroup::Set(const std::span<const FSetBindItem> items)
{
    if (items.empty()) return;
    const auto defs = m_layout->GetItems();
    const auto slots = m_layout->Slots();
    for (const auto& item : items)
    {
        if (item.Slot >= defs.size())
            COPLT_THROW("Index out of range");
        const auto& def = defs[item.Slot];
        if (item.Index >= def.Count)
            COPLT_THROW("Index out of range");
        const auto index = m_item_indexes[item.Slot] + item.Index;
        View view(item.View);
        if (view)
        {
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
        }
        m_views[index] = std::move(view);
        const auto& slot = slots[item.Slot];
        if (slot.Place == Layout::BindSlotPlace::NonTable) continue;
        m_changed_marks[index] = true;
    }
    m_changed = true;
    m_version++;
}

bool D3d12ShaderBindGroup::EnsureAvailable()
{
    if (!m_changed) return false;
    std::lock_guard guard(m_desc_lock);
    if (!m_changed) return false;
    const auto defs = m_layout->GetItems();
    const auto slots = m_layout->Slots();
    for (usize i = 0; i < m_views.size(); ++i)
    {
        const auto def_index = m_define_indexes[i];
        const auto& slot = slots[def_index];
        if (slot.Place == Layout::BindSlotPlace::NonTable) continue;
        if (const auto& mark = m_changed_marks[i]) mark = false;
        else continue;
        const auto& view = m_views[i];
        const auto& def = defs[def_index];
        const auto& heap = slot.Place == Layout::BindSlotPlace::ResourceTable ? m_resource_heap : m_sampler_heap;
        COPLT_DEBUG_ASSERT(heap != nullptr);
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle(heap->GetCPUDescriptorHandleForHeapStart(), slot.OffsetInTable, m_resource_heap_inc);
        view.CreateDescriptor(m_device->m_device.Get(), def, handle);
    }
    m_changed = false;
    return true;
}

const ComPtr<ID3D12DescriptorHeap>& D3d12ShaderBindGroup::ResourceHeap() noexcept
{
    return m_resource_heap;
}

const ComPtr<ID3D12DescriptorHeap>& D3d12ShaderBindGroup::SamplerHeap() noexcept
{
    return m_sampler_heap;
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

u64 D3d12ShaderBinding::Version() const noexcept
{
    return m_version;
}

const Rc<ID3d12BindingLayout>& D3d12ShaderBinding::Layout() const noexcept
{
    return m_layout;
}

std::span<const Rc<ID3d12ShaderBindGroup>> D3d12ShaderBinding::Groups() const noexcept
{
    return m_groups;
}

RwLock& D3d12ShaderBinding::SelfLock() noexcept
{
    return m_self_lock;
}

void D3d12ShaderBinding::Set(const std::span<FSetBindGroupItem> items)
{
    if (items.size() == 0) return;
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
    m_version++;
}
