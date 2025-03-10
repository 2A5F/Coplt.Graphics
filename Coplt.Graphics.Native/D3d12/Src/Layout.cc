#include "Layout.h"

#include <ranges>

#include "../Include/ShaderVisibility.h"

using namespace Coplt;

namespace
{
    struct TableKey
    {
        FShaderStage Stage{};
        FShaderLayoutGroupView View{};

        TableKey() = default;

        explicit TableKey(const FShaderStage Stage, const FShaderLayoutGroupView View) : Stage(Stage), View(View)
        {
        }

        struct Hasher
        {
            size_t operator()(const TableKey& value) const
            {
                return std::hash<u16>{}(static_cast<u16>(value.Stage) | (static_cast<u16>(value.View) << 8));
            }
        };

        struct Eq
        {
            bool operator()(const TableKey& lhs, const TableKey& rhs) const
            {
                return lhs.Stage == rhs.Stage && lhs.View == rhs.View;
            }
        };
    };

    using TableMeta = ID3d12ShaderLayout::TableMeta;
    using TableScope = ID3d12ShaderLayout::TableScope;

    struct TableGroupKey
    {
        TableScope Scope{};
        bool Sampler{};

        TableGroupKey() = default;

        explicit TableGroupKey(const TableScope Scope, const bool Sampler) : Scope(Scope), Sampler(Sampler)
        {
        }

        struct Hasher
        {
            size_t operator()(const TableGroupKey& value) const
            {
                return std::hash<u16>{}(static_cast<u16>(value.Scope) | (static_cast<u16>(value.Sampler) << 8));
            }
        };

        struct Eq
        {
            bool operator()(const TableGroupKey& lhs, const TableGroupKey& rhs) const
            {
                return lhs.Scope == rhs.Scope && lhs.Sampler == rhs.Sampler;
            }
        };
    };

    struct TableDefine
    {
        u8 Index{};
        u32 IndexInc{};
        HashMap<TableKey, TableMeta, TableKey::Hasher, TableKey::Eq> Map{};
    };

    using TmpTableGroupMap = HashMap<TableGroupKey, TableDefine, TableGroupKey::Hasher, TableGroupKey::Eq>;
}

D3d12ShaderLayout::D3d12ShaderLayout(Rc<D3d12GpuDevice>&& device, const FShaderLayoutCreateOptions& options)
    : m_device(std::move(device))
{
    m_dx_device = m_device->m_device;

    Flags = options.Flags;

    m_layout_item_defines = std::vector(options.Items, options.Items + options.Count);
    m_item_infos = std::vector<FShaderLayoutItemInfo>(options.Count, {});

    std::vector<D3D12_ROOT_PARAMETER1> root_parameters{};

    TmpTableGroupMap tables{};
    u8 TableGroupIndexInc{};

    for (u32 i = 0; i < m_layout_item_defines.size(); i++)
    {
        const auto& item = m_layout_item_defines[i];

        if (item.View == FShaderLayoutItemView::Constants)
        {
            if (item.Type != FShaderLayoutItemType::ConstantBuffer)
                COPLT_THROW("Push Const / Root Const view must be ConstantBuffer");

            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            param.Constants.ShaderRegister = item.Slot;
            param.Constants.RegisterSpace = item.Space;
            param.Constants.Num32BitValues = std::max(1u, item.CountOrIndex);
            param.ShaderVisibility = ToDxVisibility(item.Stage);
            m_item_infos[i] = FShaderLayoutItemInfo{
                .Index = static_cast<u32>(root_parameters.size()),
                .Place = FShaderLayoutItemPlace::Const,
            };
            root_parameters.push_back(param);
            continue;
        }

        D3D12_ROOT_PARAMETER_TYPE type;
        TableScope table_scope;

        switch (item.Usage)
        {
        case FShaderLayoutItemUsage::Common:
            table_scope = TableScope::Common;
            goto DefineDescriptorTable;
        case FShaderLayoutItemUsage::Material:
            table_scope = TableScope::Material;
            goto DefineDescriptorTable;
        case FShaderLayoutItemUsage::Instant:
            if (item.CountOrIndex > 1 || !IsBuffer(item.Type))
            {
                table_scope = TableScope::Common;
                goto DefineDescriptorTable;
            }
            switch (item.View)
            {
            case FShaderLayoutItemView::Cbv:
                type = D3D12_ROOT_PARAMETER_TYPE_CBV;
                goto DefineDescriptor;
            case FShaderLayoutItemView::Srv:
                type = D3D12_ROOT_PARAMETER_TYPE_SRV;
                goto DefineDescriptor;
            case FShaderLayoutItemView::Uav:
                type = D3D12_ROOT_PARAMETER_TYPE_UAV;
                goto DefineDescriptor;
            case FShaderLayoutItemView::Sampler:
                COPLT_THROW("TODO"); // 静态采样器
            default:
                COPLT_THROW_FMT("Unknown shader layout item type {}", static_cast<u32>(item.Type));
            }
        default:
            COPLT_THROW_FMT("Unknown shader layout item usage {}", static_cast<u32>(item.Usage));
        }

        continue;

    DefineDescriptor:
        {
            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = type;
            param.Descriptor.ShaderRegister = item.Slot;
            param.Descriptor.RegisterSpace = item.Space;
            param.Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE;
            param.ShaderVisibility = ToDxVisibility(item.Stage);
            m_item_infos[i] = FShaderLayoutItemInfo{
                .Index = static_cast<u32>(root_parameters.size()),
                .Place = FShaderLayoutItemPlace::Direct,
            };
            root_parameters.push_back(param);
            continue;
        }
    DefineDescriptorTable:
        {
            auto& table = tables.GetOrAdd(TableGroupKey(table_scope, item.View == FShaderLayoutItemView::Sampler), [&](auto& p)
            {
                p = TableDefine{.Index = TableGroupIndexInc++};
            });
            FShaderLayoutGroupView group_view{};
            D3D12_DESCRIPTOR_RANGE1 range{};
            switch (item.View)
            {
            case FShaderLayoutItemView::Cbv:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                group_view = FShaderLayoutGroupView::Cbv;
                break;
            case FShaderLayoutItemView::Srv:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                group_view = FShaderLayoutGroupView::Srv;
                break;
            case FShaderLayoutItemView::Uav:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                group_view = FShaderLayoutGroupView::Uav;
                break;
            case FShaderLayoutItemView::Sampler:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                group_view = FShaderLayoutGroupView::Sampler;
                break;
            default:
                COPLT_THROW_FMT("Unknown shader layout item type {}", static_cast<u32>(item.Type));
            }
            auto& meta = table.Map.GetOrAdd(TableKey(item.Stage, group_view), [&](auto& p)
            {
                p = TableMeta(table.IndexInc++, item.Stage, group_view);
            });
            range.NumDescriptors = std::max(1u, item.CountOrIndex);
            range.BaseShaderRegister = item.Slot;
            range.RegisterSpace = item.Space;
            range.OffsetInDescriptorsFromTableStart = meta.Size;
            // range.Flags = D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;
            m_item_infos[i] = FShaderLayoutItemInfo{
                .Index = static_cast<u32>(meta.Ranges.size()),
                .Class = table.Index,
                .Group = meta.Index,
                .Place = FShaderLayoutItemPlace::Grouped,
            };
            meta.Ranges.push_back(range);
            meta.Size += range.NumDescriptors;
            continue;
        }
    }

    m_table_groups = std::vector<TableGroup>(tables.Count(), {});
    m_group_classes = std::vector<FShaderLayoutGroupClass>(tables.Count(), {});
    for (auto& [key, table] : tables)
    {
        auto& table_groups = m_table_groups[table.Index];
        auto& group_class = m_group_classes[table.Index];
        table_groups = TableGroup{
            .Metas = std::vector<TableMeta>(table.Map.Count(), {}),
            .Infos = std::vector<FShaderLayoutGroupInfo>(table.Map.Count(), {}),
            .Scope = key.Scope, .Sampler = key.Sampler
        };
        group_class = FShaderLayoutGroupClass{
            .Infos = table_groups.Infos.data(),
            .Size = static_cast<u32>(table_groups.Metas.size()),
            .Sampler = key.Sampler
        };
        for (auto& item : table.Map | std::views::values)
        {
            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

            auto& meta = table_groups.Metas[item.Index];
            auto& info = table_groups.Infos[item.Index];
            meta = std::move(item);
            meta.RootIndex = root_parameters.size();
            info.Index = meta.RootIndex;
            info.Size = meta.Ranges.size();
            info.Stage = meta.Stage;
            info.View = meta.View;

            param.ShaderVisibility = ToDxVisibility(meta.Stage);
            param.DescriptorTable.NumDescriptorRanges = meta.Ranges.size();
            param.DescriptorTable.pDescriptorRanges = meta.Ranges.data();

            root_parameters.push_back(param);
        }
    }

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
    desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_2;
    desc.Desc_1_1.NumParameters = static_cast<u32>(root_parameters.size());
    desc.Desc_1_1.pParameters = root_parameters.data();
    desc.Desc_1_1.NumStaticSamplers = 0;
    desc.Desc_1_1.pStaticSamplers = nullptr;
    desc.Desc_1_1.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

    if (HasFlags(options.Flags, FShaderLayoutFlags::InputAssembler))
        desc.Desc_1_1.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    if (HasFlags(options.Flags, FShaderLayoutFlags::StreamOutput))
        desc.Desc_1_1.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT;
    if (HasFlags(options.Flags, FShaderLayoutFlags::BindLess))
        desc.Desc_1_1.Flags |=
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

    chr | m_dx_device->CreateRootSignature(
        0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)
    );

    if (m_device->Debug())
    {
        chr | m_root_signature >> SetNameEx(options.Name);
    }
}

FResult D3d12ShaderLayout::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        chr | m_root_signature >> SetNameEx(name);
    });
}

void* D3d12ShaderLayout::GetRootSignaturePtr() noexcept
{
    return m_root_signature.Get();
}

const FShaderLayoutItemDefine* D3d12ShaderLayout::GetItemDefines(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_layout_item_defines.size());
    return m_layout_item_defines.data();
}

const FShaderLayoutItemInfo* D3d12ShaderLayout::GetItemInfos(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_item_infos.size());
    return m_item_infos.data();
}

const FShaderLayoutGroupClass* D3d12ShaderLayout::GetGroupClasses(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_group_classes.size());
    return m_group_classes.data();
}

std::span<ID3d12ShaderLayout::TableGroup> D3d12ShaderLayout::GetTableGroups() noexcept
{
    return m_table_groups;
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
