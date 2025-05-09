#include "Layout.h"

#include <ranges>

#include "../../Api/Include/AllocObjectId.h"
#include "../Include/ShaderVisibility.h"
#include "../Include/Sampler.h"

using namespace Coplt;
using namespace Coplt::Layout;

D3D12_DESCRIPTOR_RANGE_TYPE Layout::ToTableType(const FShaderLayoutItemView view)
{
    switch (view)
    {
    case FShaderLayoutItemView::Cbv:
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    case FShaderLayoutItemView::Srv:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    case FShaderLayoutItemView::Uav:
        return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
    case FShaderLayoutItemView::Sampler:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
    case FShaderLayoutItemView::Constants:
        return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    case FShaderLayoutItemView::StaticSampler:
        return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
    }
    return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
}

D3d12ShaderLayout::D3d12ShaderLayout(const FShaderLayoutCreateOptions& options)
{
    Flags = options.Flags;
    m_items = std::vector(options.Items, options.Items + options.NumItems);
    Items = m_items.data();
    NumItems = m_items.size();
    for (u32 i = 0; i < m_items.size(); ++i)
    {
        const auto& item = m_items[i];
        if (item.Count < 1)
        {
            COPLT_THROW_FMT(
                "Invalid binding define {{ Id = {}, Scope = {}, Stage = {} }} at [{}]; Count must >= 1",
                item.Id, item.Scope, static_cast<u32>(item.Stage), i
            );
        }
        if (item.View == FShaderLayoutItemView::StaticSampler && item.Count > 1)
        {
            COPLT_THROW_FMT(
                "Invalid binding define {{ Id = {}, Scope = {}, Stage = {} }} at [{}]; Count when StaticSampler must == 1",
                item.Id, item.Scope, static_cast<u32>(item.Stage), i
            );
        }
    }
}

FResult D3d12ShaderLayout::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FShaderLayoutData* D3d12ShaderLayout::ShaderLayoutData() noexcept
{
    return this;
}

D3d12BindGroupLayout::D3d12BindGroupLayout(const FBindGroupLayoutCreateOptions& options)
{
    m_items = std::vector(options.Items, options.Items + options.NumItems);
    m_static_samplers = std::vector(options.StaticSamplers, options.StaticSamplers + options.NumStaticSamplers);
    FBindGroupLayoutData::Items = m_items.data();
    StaticSamplers = m_static_samplers.data();
    NumItems = m_items.size();
    NumStaticSamplers = m_static_samplers.size();
    Usage = options.Usage;

    m_slots.reserve(m_items.size());
    for (u32 i = 0; i < m_items.size(); ++i)
    {
        BindSlotInfo info{};
        const auto& item = m_items[i];
        const auto count = std::max(1u, item.Count);
        if (item.View == FShaderLayoutItemView::Constants)
        {
            if (Usage != FBindGroupUsage::Dynamic)
            {
                COPLT_THROW_FMT(
                    "Invalid binding define {{ Id = {}, Scope = {}, Stages = {} }} at [{}]; Only dynamic usage groups support constants",
                    item.Id, item.Scope, static_cast<u32>(item.Stages), i
                );
            }
        }
        if (count == COPLT_U32_MAX)
        {
            if (Usage != FBindGroupUsage::Dynamic)
            {
                COPLT_THROW_FMT(
                    "Invalid binding define {{ Id = {}, Scope = {}, Stages = {} }} at [{}]; Only dynamic usage groups support unlimited length arrays",
                    item.Id, item.Scope, static_cast<u32>(item.Stages), i
                );
            }
            switch (item.View)
            {
            case FShaderLayoutItemView::Cbv:
            case FShaderLayoutItemView::Srv:
            case FShaderLayoutItemView::Uav:
                if (i + 1 != m_items.size())
                {
                    COPLT_THROW_FMT(
                        "Invalid binding define {{ Id = {}, Scope = {}, Stages = {} }} at [{}]; Dynamic length array must be the last item in the group",
                        item.Id, item.Scope, static_cast<u32>(item.Stages), i
                    );
                }
                DynamicArrayIndex = i;
                break;
            case FShaderLayoutItemView::Sampler:
            case FShaderLayoutItemView::Constants:
            case FShaderLayoutItemView::StaticSampler:
                COPLT_THROW_FMT(
                    "Invalid binding define {{ Id = {}, Scope = {}, Stages = {} }} at [{}]; Only Cbv|Srv|Uav View support unlimited length arrays",
                    item.Id, item.Scope, static_cast<u32>(item.Stages), i
                );
            }
        }
        switch (item.View)
        {
        case FShaderLayoutItemView::Cbv:
        case FShaderLayoutItemView::Srv:
        case FShaderLayoutItemView::Uav:
            // if (Usage == FBindGroupUsage::Dynamic && item.Count <= 1 && IsBuffer(item.Type))
            // {
            //     info.Place = BindSlotPlace::NonTable;
            //     break;
            // }
            // todo 暂时不支持直接资源
            info.OffsetInTable = ResourceTableSize;
            info.Place = BindSlotPlace::ResourceTable;
            if (count != COPLT_U32_MAX)
            {
                ResourceTableSize += count;
            }
            break;
        case FShaderLayoutItemView::Sampler:
            info.OffsetInTable = SamplerTableSize;
            info.Place = BindSlotPlace::SamplerTable;
            SamplerTableSize += count;
            break;
        case FShaderLayoutItemView::Constants:
        case FShaderLayoutItemView::StaticSampler:
            info.Place = BindSlotPlace::NonTable;
            break;
        }
        m_slots.push_back(info);
    }
}

FResult D3d12BindGroupLayout::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FBindGroupLayoutData* D3d12BindGroupLayout::BindGroupLayoutData() noexcept
{
    return this;
}

std::span<const FBindGroupItem> D3d12BindGroupLayout::Items() const noexcept
{
    return m_items;
}

std::span<const FStaticSamplerInfo> D3d12BindGroupLayout::StaticSamplerInfos() const noexcept
{
    return m_static_samplers;
}

const D3d12BindGroupLayoutData& D3d12BindGroupLayout::Data() const noexcept
{
    return *this;
}

std::span<const BindSlotInfo> D3d12BindGroupLayout::Slots() const noexcept
{
    return m_slots;
}

D3d12BindingLayout::D3d12BindingLayout(Rc<D3d12GpuDevice>&& device, const FBindingLayoutCreateOptions& options)
    : m_device(device)
{
    m_shader_layout = Rc<FShaderLayout>::UnsafeClone(options.ShaderLayout);
    m_groups.reserve(options.NumGroups);
    for (u32 i = 0; i < options.NumGroups; i++)
    {
        const auto group = NonNull(options.Groups[i])->QueryInterface<ID3d12BindGroupLayout>();
        if (group == nullptr)
            COPLT_THROW_FMT("Group [{}] from different backends", i);
        m_groups.push_back(Rc<ID3d12BindGroupLayout>::UnsafeClone(group));
    }

    const auto defines = m_shader_layout->GetItems();
    for (u32 i = 0; i < defines.size(); ++i)
    {
        const auto& item = defines[i];
        BindSlot slot(item);
        const auto is_new = m_slot_to_info.TryAdd(slot, [&](auto& p)
        {
            p.put(m_slot_infos.size());
            m_slot_infos.push_back(SlotInfo(i));
        });
        if (!is_new)
        {
            COPLT_THROW_FMT(
                "Duplicate binding slot {{ Id = {}, Scope = {}, Stage = {} }} at [{}]",
                slot.Id, slot.Scope, static_cast<u32>(slot.Stage), i
            );
        }
    }
    m_group_item_infos.reserve(options.NumGroups);
    for (u32 g = 0; g < options.NumGroups; g++)
    {
        std::vector<GroupItemInfo> group_item_infos{};
        const auto& group = m_groups[g];
        const auto& group_data = group->Data();
        SumTableSamplerSlots += group_data.ResourceTableSize;
        SumTableResourceSlots += group_data.SamplerTableSize;
        const auto items = group->GetItems();
        group_item_infos.reserve(items.size());
        for (u32 i = 0; i < items.size(); ++i)
        {
            const auto& item = items[i];
            GroupItemInfo group_item_info{};
            group_item_info.Group = g;
            group_item_info.IndexInGroup = i;
            group_item_info.Format = item.Format;
            group_item_info.Stages = item.Stages;
            group_item_info.View = item.View;
            group_item_info.Type = item.Type;
            group_item_info.UavAccess = item.UavAccess;
            for (const auto stage : IterStage(item.Stages))
            {
                BindSlot slot(item, stage);
                const auto info_index = m_slot_to_info.TryGet(slot);
                if (!info_index) continue;
                auto& info = m_slot_infos[*info_index];
                if (info.Group != COPLT_U32_MAX)
                {
                    COPLT_THROW_FMT(
                        "Duplicate binding slot {{ Id = {}, Scope = {}, Stage = {} }} at Group {} [{}]",
                        slot.Id, slot.Scope, static_cast<u32>(slot.Stage), g, i
                    );
                }
                const auto& def = defines[info.Index];
                if (
                    !IsCompatible(def.View, item.View) ||
                    (item.View == FShaderLayoutItemView::StaticSampler || def.Count == COPLT_U32_MAX ? false : def.Count != item.Count)
                    || def.Type != item.Type || def.Format != item.Format
                )
                {
                    if (item.View == FShaderLayoutItemView::Constants && def.View == FShaderLayoutItemView::Cbv)
                    {
                        if (def.Count == 1) goto Compatible;
                    }
                    COPLT_THROW_FMT(
                        "Incompatible binding slot {{ Id = {}, Scope = {}, Stage = {} }} at Group {} [{}]",
                        slot.Id, slot.Scope, static_cast<u32>(slot.Stage), g, i
                    );
                Compatible:;
                }
                if (def.Count != COPLT_U32_MAX && item.Count < def.Count)
                {
                    COPLT_THROW_FMT(
                        "Incompatible binding slot {{ Id = {}, Scope = {}, Stage = {} }} at Group {} [{}]; The group provides fewer bindings than the shader requires",
                        slot.Id, slot.Scope, static_cast<u32>(slot.Stage), g, i
                    );
                }
                info.Group = g;
                info.IndexInGroup = i;
                switch (def.UavAccess)
                {
                case FResourceAccess::ReadOnly:
                    if (group_item_info.UavAccess == FResourceAccess::Unknown)
                        group_item_info.UavAccess = FResourceAccess::ReadOnly;
                    else if (group_item_info.UavAccess != FResourceAccess::ReadOnly)
                        group_item_info.UavAccess = FResourceAccess::ReadWrite;
                    break;
                case FResourceAccess::WriteOnly:
                    if (group_item_info.UavAccess == FResourceAccess::Unknown)
                        group_item_info.UavAccess = FResourceAccess::WriteOnly;
                    else if (group_item_info.UavAccess != FResourceAccess::WriteOnly)
                        group_item_info.UavAccess = FResourceAccess::ReadWrite;
                    break;
                case FResourceAccess::ReadWrite:
                case FResourceAccess::Unknown:
                default:
                    group_item_info.UavAccess = FResourceAccess::ReadWrite;
                    break;
                }
            }
            group_item_infos.push_back(group_item_info);
        }
        m_group_item_infos.push_back(std::move(group_item_infos));
    }

    std::vector<D3D12_ROOT_PARAMETER1> root_parameters{};
    std::vector<D3D12_STATIC_SAMPLER_DESC1> static_samplers{};
    std::vector<HashMap<TableKey, TableDefine>> tables{};
    tables.reserve(m_groups.size());
    for (u32 i = 0; i < m_groups.size(); i++) tables.push_back({});
    m_group_bind_item_infos.reserve(m_groups.size());
    for (u32 i = 0; i < m_groups.size(); i++) m_group_bind_item_infos.push_back({});

    for (auto& info : m_slot_infos)
    {
        const auto& item = defines[info.Index];
        const auto& group = m_groups[info.Group];
        const auto& group_item = group->Items()[info.IndexInGroup];
        const auto view = group_item.View;

        if (view == FShaderLayoutItemView::Constants)
        {
            if (item.Type != FShaderLayoutItemType::ConstantBuffer)
                COPLT_THROW("Push Const / Root Const view must be ConstantBuffer");

            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            param.Constants.ShaderRegister = item.Slot;
            param.Constants.RegisterSpace = item.Space;
            param.Constants.Num32BitValues = std::max(1u, group_item.Count);
            param.ShaderVisibility = ToDxVisibility(item.Stage);
            info.SigIndex = static_cast<u32>(root_parameters.size());
            info.SigPlace = SigPlace::Const;
            root_parameters.push_back(param);

            RootItemInfo item_info{};
            item_info.Group = info.Group;
            item_info.IndexInGroup = info.Index;
            item_info.RootIndex = info.SigIndex;
            item_info.Place = RootItemPlace::Const;
            item_info.Type = RootItemType::Resource;
            m_bind_item_infos.push_back(item_info);
            m_group_bind_item_infos[info.Group].push_back(item_info);
            continue;
        }

        // 组未提供绑定，使用隐藏组创建描述符，并且永远保持默认值
        if (info.Group == COPLT_U32_MAX)
            COPLT_THROW("TODO");

        D3D12_ROOT_PARAMETER_TYPE type;

        if (view == FShaderLayoutItemView::StaticSampler)
            goto DefineStaticSampler;
        if (view == FShaderLayoutItemView::Sampler)
            goto DefineDescriptorTable;

        const auto usage = group->Data().Usage;
        if (usage == FBindGroupUsage::Dynamic)
        {
            goto DefineDescriptorTable; // 暂时不支持直接资源
            // if (item.Count > 1 || !IsBuffer(item.Type))
            // {
            //     goto DefineDescriptorTable;
            // }
            // switch (view)
            // {
            // case FShaderLayoutItemView::Cbv:
            //     type = D3D12_ROOT_PARAMETER_TYPE_CBV;
            //     goto DefineDescriptor;
            // case FShaderLayoutItemView::Srv:
            //     type = D3D12_ROOT_PARAMETER_TYPE_SRV;
            //     goto DefineDescriptor;
            // case FShaderLayoutItemView::Uav:
            //     type = D3D12_ROOT_PARAMETER_TYPE_UAV;
            //     goto DefineDescriptor;
            // case FShaderLayoutItemView::StaticSampler:
            // case FShaderLayoutItemView::Sampler:
            // case FShaderLayoutItemView::Constants:
            // default:
            //     COPLT_THROW_FMT("Unknown shader layout item type {}", static_cast<u32>(item.Type));
            // }
        }

    DefineDescriptorTable:
        {
            const auto slots = group->Slots();
            const auto& slot = slots[info.IndexInGroup];
            COPLT_DEBUG_ASSERT(slot.Place != BindSlotPlace::NonTable);
            D3D12_DESCRIPTOR_RANGE1 range{};
            const auto range_type = ToTableType(view);
            range.RangeType = range_type;
            auto& table = tables[info.Group].GetOrAdd(TableKey(item.Stage, range_type), [&](auto& p)
            {
                p.put(TableDefine{
                    .Info = TableInfo{
                        .Group = info.Group,
                        .Stage = item.Stage,
                        .Type = range_type,
                    }
                });
            });
            range.NumDescriptors = std::max(1u, item.Count);
            range.BaseShaderRegister = item.Slot;
            range.RegisterSpace = item.Space;
            range.OffsetInDescriptorsFromTableStart = slot.OffsetInTable;
            info.SigIndex = static_cast<u32>(table.Ranges.size());
            info.SigPlace = SigPlace::Grouped;
            table.Ranges.push_back(range);
            continue;
        }
    DefineDescriptor:
        {
            // D3D12_ROOT_PARAMETER1 param{};
            // param.ParameterType = type;
            // param.Descriptor.ShaderRegister = item.Slot;
            // param.Descriptor.RegisterSpace = item.Space;
            // param.Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE;
            // param.ShaderVisibility = ToDxVisibility(item.Stage);
            // info.SigIndex = static_cast<u32>(root_parameters.size());
            // info.SigPlace = SigPlace::Direct;
            // root_parameters.push_back(param);
            //
            // RootItemInfo item_info{};
            // item_info.Group = info.Group;
            // item_info.IndexInGroup = info.Index;
            // item_info.RootIndex = info.SigIndex;
            // item_info.Place = RootItemPlace::Direct;
            // item_info.Type = RootItemType::Resource;
            // m_bind_item_infos.push_back(item_info);
            // m_group_bind_item_infos[info.Group].push_back(item_info);
            // continue;
        }
    DefineStaticSampler:
        {
            const auto samplers = group->StaticSamplerInfos();
            const auto& sampler = samplers[group_item.StaticSamplerIndex];
            D3D12_STATIC_SAMPLER_DESC1 desc{};
            desc.ShaderRegister = item.Slot;
            desc.RegisterSpace = item.Space;
            desc.ShaderVisibility = ToDxVisibility(item.Stage);
            SetDesc(sampler, desc);
            info.SigIndex = static_cast<u32>(static_samplers.size());
            info.SigPlace = SigPlace::StaticSampler;
            static_samplers.push_back(desc);
            continue;
        }
    }

    m_tables.reserve(tables.size());
    for (usize i = 0; i < tables.size(); ++i)
    {
        auto& table = tables[i];
        std::vector<TableInfo> infos{};
        infos.reserve(table.Count());
        for (auto& [Ranges, Info] : table | std::views::values)
        {
            Info.RootIndex = static_cast<u32>(root_parameters.size());
            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            param.ShaderVisibility = ToDxVisibility(Info.Stage);
            param.DescriptorTable.NumDescriptorRanges = Ranges.size();
            param.DescriptorTable.pDescriptorRanges = Ranges.data();
            root_parameters.push_back(param);
            infos.push_back(Info);

            RootItemInfo item_info{};
            item_info.Group = Info.Group;
            item_info.IndexInGroup = 0;
            item_info.RootIndex = Info.RootIndex;
            item_info.Place = RootItemPlace::Table;
            item_info.Type = Info.Type == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER ? RootItemType::Sampler : RootItemType::Resource;
            m_bind_item_infos.push_back(item_info);
            m_group_bind_item_infos[Info.Group].push_back(item_info);
        }
        m_tables.push_back(std::move(infos));
    }

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
    desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_2;
    desc.Desc_1_2.NumParameters = static_cast<u32>(root_parameters.size());
    desc.Desc_1_2.pParameters = root_parameters.data();
    desc.Desc_1_2.NumStaticSamplers = static_cast<u32>(static_samplers.size());
    desc.Desc_1_2.pStaticSamplers = static_samplers.data();
    desc.Desc_1_2.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

    const auto layout = *NonNull(m_shader_layout->ShaderLayoutData());
    if (HasFlags(layout.Flags, FShaderLayoutFlags::InputAssembler))
        desc.Desc_1_2.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    if (HasFlags(layout.Flags, FShaderLayoutFlags::StreamOutput))
        desc.Desc_1_2.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
    if (HasFlags(layout.Flags, FShaderLayoutFlags::DynBindLess))
        desc.Desc_1_2.Flags |=
            D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED |
            D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;

    ComPtr<ID3DBlob> blob{};
    ComPtr<ID3DBlob> error_blob{};
    if (const auto r = D3D12SerializeVersionedRootSignature(&desc, &blob, &error_blob); !SUCCEEDED(r))
    {
        m_device->Logger().Log(
            FLogLevel::Error, FLogType::DirectX,
            static_cast<const char*>(error_blob->GetBufferPointer()), error_blob->GetBufferSize()
        );
        chr | r;
    }

    chr | m_device->m_device->CreateRootSignature(
        0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)
    );

    if (m_device->Debug())
    {
        chr | m_root_signature >> SetNameEx(options.Name);
    }
}

FResult D3d12BindingLayout::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        chr | m_root_signature >> SetNameEx(name);
    });
}

const D3d12BindingLayoutData& D3d12BindingLayout::Data() const noexcept
{
    return *this;
}

const Rc<FShaderLayout>& D3d12BindingLayout::ShaderLayout() const noexcept
{
    return m_shader_layout;
}

std::span<const Rc<ID3d12BindGroupLayout>> D3d12BindingLayout::Groups() const noexcept
{
    return m_groups;
}

const ComPtr<ID3D12RootSignature>& D3d12BindingLayout::RootSignature() const noexcept
{
    return m_root_signature;
}

std::span<const D3d12BindingLayout::SlotInfo> D3d12BindingLayout::SlotInfos() const noexcept
{
    return m_slot_infos;
}

std::span<const RootItemInfo> D3d12BindingLayout::RootItemInfos() const noexcept
{
    return m_bind_item_infos;
}

std::span<const std::vector<RootItemInfo>> D3d12BindingLayout::GroupRootItemInfos() const noexcept
{
    return m_group_bind_item_infos;
}

std::span<const std::vector<TableInfo>> D3d12BindingLayout::TableInfos() const noexcept
{
    return m_tables;
}

std::span<const std::vector<GroupItemInfo>> D3d12BindingLayout::GroupItemInfos() const noexcept
{
    return m_group_item_infos;
}

D3d12ShaderInputLayout::D3d12ShaderInputLayout(
    Rc<D3d12GpuDevice>&& device, const FShaderInputLayoutCreateOptions& options
) : m_device(std::move(device))
{
    m_elements = std::vector(options.Element, options.Element + options.Count);
    m_slot_names.reserve(m_elements.size());
    for (auto& element : m_elements)
    {
        auto name = Rc<FString8>::UnsafeClone(element.SlotName);
        element.SlotName = name.get();
        m_slot_names.push_back(std::move(name));
    }
}

FResult D3d12ShaderInputLayout::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

const FShaderInputLayoutElement* D3d12ShaderInputLayout::GetElements(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_elements.size());
    return m_elements.data();
}

D3d12MeshLayout::D3d12MeshLayout(Rc<D3d12GpuDevice>&& device, const FMeshLayoutCreateOptions& options) : m_device(
    std::move(device))
{
    m_buffers = std::vector(options.Buffers, options.Buffers + options.BufferCount);
    m_elements = std::vector(options.Elements, options.Elements + options.ElementCount);
    // 保留 1 个 slot 永远为 0
    if (m_buffers.size() >= 31)
    {
        COPLT_THROW_FMT(
            "Too many buffers, a maximum of 31 buffers are supported, but actually provided {}",
            m_buffers.size()
        );
    }
    for (u32 i = 0; i < m_elements.size(); ++i)
    {
        const auto& element = m_elements[i];
        const auto key = static_cast<u64>(element.SlotId) | (static_cast<u64>(element.SlotIndex) << 32);
        m_slot_id_index_to_element.TryAdd(key, &element);
        if (element.BufferIndex > m_buffers.size())
            COPLT_THROW_FMT("BufferIndex of element {} is out of range", i);
    }
}

FResult D3d12MeshLayout::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

const FMeshBufferDefine* D3d12MeshLayout::GetBuffers(u32* out_count) const noexcept
{
    *out_count = static_cast<u32>(m_buffers.size());
    return m_buffers.data();
}

const FMeshBufferElement* D3d12MeshLayout::GetElements(u32* out_count) const noexcept
{
    *out_count = static_cast<u32>(m_elements.size());
    return m_elements.data();
}

const FMeshBufferElement* D3d12MeshLayout::TryGetElement(const u32 SlotId, const u32 SlotIndex) const noexcept
{
    const auto key = static_cast<u64>(SlotId) | (static_cast<u64>(SlotIndex) << 32);
    return m_slot_id_index_to_element.GetOr(key, nullptr);
}
