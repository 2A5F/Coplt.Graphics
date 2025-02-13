#include "Layout.h"

#include "../Include/ShaderVisibility.h"

using namespace Coplt;

D3d12ShaderLayout::D3d12ShaderLayout(Rc<D3d12GpuDevice>&& device, const FShaderLayoutCreateOptions& options)
    : m_device(std::move(device))
{
    m_dx_device = m_device->m_device;

    Flags = options.Flags;

    m_layout_item_defines = std::vector(options.Items, options.Items + options.Count);
    m_item_metas = std::vector<ItemMeta>(options.Count, {});

    std::vector<D3D12_ROOT_PARAMETER1> root_parameters{};

    HashMap<TableKey, TableMeta, TableKey::Hasher, TableKey::Eq> tables[2]{};
    u32 table_index[2]{};

    for (u32 i = 0; i < m_layout_item_defines.size(); i++)
    {
        const auto& item = m_layout_item_defines[i];

        if (item.Type == FShaderLayoutItemType::Constants)
        {
            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            param.Constants.ShaderRegister = item.Slot;
            param.Constants.RegisterSpace = item.Space;
            param.Constants.Num32BitValues = item.CountOrIndex;
            param.ShaderVisibility = ToDx(item.Stage);
            m_item_metas[i] = ItemMeta{.Index = static_cast<u32>(root_parameters.size()), .Type = ItemType::Const};
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
            if (item.CountOrIndex > 1)
            {
                table_scope = TableScope::Common;
                goto DefineDescriptorTable;
            }
            switch (item.Type)
            {
            case FShaderLayoutItemType::Cbv:
                type = D3D12_ROOT_PARAMETER_TYPE_CBV;
                goto DefineDescriptor;
            case FShaderLayoutItemType::SrvTexture:
            case FShaderLayoutItemType::SrvBuffer:
                type = D3D12_ROOT_PARAMETER_TYPE_SRV;
                goto DefineDescriptor;
            case FShaderLayoutItemType::UavTexture:
            case FShaderLayoutItemType::UavBuffer:
                type = D3D12_ROOT_PARAMETER_TYPE_UAV;
                goto DefineDescriptor;
            case FShaderLayoutItemType::Sampler:
                throw WRuntimeException(L"TODO"); // 静态采样器
            default:
                throw WRuntimeException(
                    fmt::format(L"Unknown shader layout item type {}", static_cast<u32>(item.Type))
                );
            }
        default:
            throw WRuntimeException(fmt::format(L"Unknown shader layout item usage {}", static_cast<u32>(item.Usage)));
        }

        continue;

    DefineDescriptor:
        {
            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = type;
            param.Descriptor.ShaderRegister = item.Slot;
            param.Descriptor.RegisterSpace = item.Space;
            param.Descriptor.Flags = D3D12_ROOT_DESCRIPTOR_FLAG_NONE;
            param.ShaderVisibility = ToDx(item.Stage);
            m_item_metas[i] = ItemMeta{.Index = static_cast<u32>(root_parameters.size()), .Type = ItemType::Direct};
            root_parameters.push_back(param);
            continue;
        }
    DefineDescriptorTable:
        {
            auto& table = tables[static_cast<u8>(table_scope)];
            RangeType range_type{};
            D3D12_DESCRIPTOR_RANGE1 range{};
            switch (item.Type)
            {
            case FShaderLayoutItemType::Cbv:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                range_type = RangeType::Cbv;
                break;
            case FShaderLayoutItemType::SrvTexture:
            case FShaderLayoutItemType::SrvBuffer:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                range_type = RangeType::Srv;
                break;
            case FShaderLayoutItemType::UavTexture:
            case FShaderLayoutItemType::UavBuffer:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                range_type = RangeType::Uav;
                break;
            case FShaderLayoutItemType::Sampler:
                range.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                range_type = RangeType::Sampler;
                break;
            default:
                throw WRuntimeException(
                    fmt::format(L"Unknown shader layout item type {}", static_cast<u32>(item.Type))
                );
            }
            auto& meta = table.GetOrAdd(TableKey(item.Stage, range_type), [&](auto* p)
            {
                new(p) TableMeta(table_index[static_cast<u8>(table_scope)]++, item.Stage, range_type);
            });
            range.NumDescriptors = item.CountOrIndex;
            range.BaseShaderRegister = item.Slot;
            range.RegisterSpace = item.Space;
            range.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            m_item_metas[i] = ItemMeta{
                .Index = static_cast<u32>(meta.ranges.size()), .Table = table_scope,
                .Type = ItemType::InTable, .RangeType = range_type
            };
            meta.ranges.push_back(range);
            continue;
        }
    }

    for (u8 ts = 0; ts < 2; ++ts)
    {
        auto& table = tables[ts];
        auto& table_metas = m_table_metas[ts];
        table_metas = std::vector<TableMeta>(table.Count(), {});
        for (auto& pair : table)
        {
            D3D12_ROOT_PARAMETER1 param{};
            param.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;

            auto& meta = table_metas[pair.second.Index];
            meta = std::move(pair.second);
            meta.RootIndex = root_parameters.size();

            param.ShaderVisibility = ToDx(meta.Stage);
            param.DescriptorTable.NumDescriptorRanges = meta.ranges.size();
            param.DescriptorTable.pDescriptorRanges = meta.ranges.data();

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

FResult D3d12ShaderLayout::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        chr | m_root_signature >> SetNameEx(name);
    });
}

void* D3d12ShaderLayout::GetRootSignaturePtr() noexcept
{
    return m_root_signature.Get();
}

FShaderLayoutItemDefine* D3d12ShaderLayout::GetItemDefines(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_layout_item_defines.size());
    return m_layout_item_defines.data();
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

FShaderInputLayoutElement* D3d12ShaderInputLayout::GetElements(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_elements.size());
    return m_elements.data();
}

FResult D3d12ShaderInputLayout::SetName(const Str8or16& name) noexcept
{
    return FResult::None();
}
