#pragma once

#include <vector>
#include <array>

#include "../../Api/Include/HashMap.h"

#include "Device.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Layout.h"

namespace Coplt
{
    struct D3d12ShaderLayout final : Object<D3d12ShaderLayout, FD3d12ShaderLayout>
    {
        enum class ItemType : u8
        {
            None,
            Const,
            Direct,
            InTable,
            StaticSampler,
        };

        enum class TableScope : u8
        {
            Common,
            Material,
        };

        enum class RangeType : u8
        {
            Cbv,
            Srv,
            Uav,
            Sampler,
        };

        struct TableKey
        {
            FShaderStage Stage{};
            RangeType Type{};

            TableKey() = default;

            explicit TableKey(const FShaderStage Stage, const RangeType Type) : Stage(Stage), Type(Type)
            {
            }

            struct Hasher
            {
                size_t operator()(const TableKey& value) const
                {
                    return std::hash<u16>{}(static_cast<u16>(value.Stage) | (static_cast<u16>(value.Type) << 8));
                }
            };

            struct Eq
            {
                bool operator()(const TableKey& lhs, const TableKey& rhs) const
                {
                    return lhs.Stage == rhs.Stage && lhs.Type == rhs.Type;
                }
            };
        };

        struct TableMeta
        {
            std::vector<D3D12_DESCRIPTOR_RANGE1> ranges{};
            u32 Index{};
            u32 RootIndex{};
            FShaderStage Stage{};
            RangeType Type{};

            TableMeta() = default;

            explicit TableMeta(const u32 index, const FShaderStage stage, const RangeType type)
                : Index(index), Stage(stage), Type(type)
            {
            }
        };

        struct ItemMeta
        {
            // Const | Direct 时是 Root Index，InTable 时是 Table 内的 Index
            u32 Index{};
            // InTable 时表示在哪个 Table
            TableScope Table{};
            ItemType Type{};
            RangeType RangeType{};
        };

        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12RootSignature> m_root_signature{};
        std::vector<FShaderLayoutItemDefine> m_layout_item_defines{};
        std::vector<ItemMeta> m_item_metas{};
        // [Common, Material]
        std::vector<TableMeta> m_table_metas[2]{};

        explicit D3d12ShaderLayout(Rc<D3d12GpuDevice>&& device, const FShaderLayoutCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        void* GetRootSignaturePtr() noexcept override;

        const FShaderLayoutItemDefine* GetItemDefines(u32* out_count) noexcept override;
    };

    struct D3d12ShaderInputLayout final : Object<D3d12ShaderInputLayout, FD3d12ShaderInputLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        std::vector<Rc<FString8>> m_slot_names{};
        std::vector<FShaderInputLayoutElement> m_elements{};

        explicit D3d12ShaderInputLayout(Rc<D3d12GpuDevice>&& device, const FShaderInputLayoutCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        const FShaderInputLayoutElement* GetElements(u32* out_count) noexcept override;
    };

    struct D3d12MeshLayout final : Object<D3d12MeshLayout, FD3d12MeshLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        std::vector<FMeshBufferDefine> m_buffers{};
        std::vector<FMeshBufferElement> m_elements{};
        HashMap<u64, const FMeshBufferElement*> m_slot_id_index_to_element{};

        explicit D3d12MeshLayout(Rc<D3d12GpuDevice>&& device, const FMeshLayoutCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        const FMeshBufferDefine* GetBuffers(u32* out_count) const noexcept override;
        const FMeshBufferElement* GetElements(u32* out_count) const noexcept override;

        const FMeshBufferElement* TryGetElement(u32 SlotId, u32 SlotIndex) const noexcept override;
    };
}
