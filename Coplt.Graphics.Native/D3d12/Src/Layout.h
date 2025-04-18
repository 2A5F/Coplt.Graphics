#pragma once

#include <vector>
#include <array>

#include "../../Api/Include/HashMap.h"

#include "Device.h"
#include "../../Api/Include/Object.h"

namespace Coplt
{
    namespace Layout
    {
        D3D12_DESCRIPTOR_RANGE_TYPE ToTableType(FShaderLayoutItemView view);

        enum class BindSlotPlace : u8
        {
            NonTable,
            ResourceTable,
            SamplerTable,
        };

        struct BindSlotInfo
        {
            u32 OffsetInTable{};
            BindSlotPlace Place{};
        };

        struct TableInfo final
        {
            u32 Group{};
            u32 RootIndex{};
            FShaderStage Stage{};
            D3D12_DESCRIPTOR_RANGE_TYPE Type{};
        };

        struct TableDefine final
        {
            std::vector<D3D12_DESCRIPTOR_RANGE1> Ranges{};
            TableInfo Info{};
        };

        struct TableKey final
        {
            FShaderStage Stage{};
            D3D12_DESCRIPTOR_RANGE_TYPE Type{};

            TableKey() = default;

            TableKey(const FShaderStage Stage, const D3D12_DESCRIPTOR_RANGE_TYPE Type) : Stage(Stage), Type(Type)
            {
            }

            usize GetHashCode() const
            {
                return hash_multi(static_cast<u8>(Stage), static_cast<u32>(Type));
            }

            bool operator==(const TableKey& other) const
            {
                return Stage == other.Stage && Type == other.Type;
            }
        };

        struct BindSlot final
        {
            u64 Id{};
            u64 Scope{};
            FShaderStage Stage{};

            BindSlot() = default;

            BindSlot(const u64 Id, const u64 Scope, const FShaderStage Stage) : Id(Id), Scope(Scope), Stage(Stage)
            {
            }

            explicit BindSlot(const FShaderLayoutItem& item) : BindSlot(item.Id, item.Scope, item.Stage)
            {
            }

            explicit BindSlot(const FBindGroupItem& item, const FShaderStage Stage) : BindSlot(item.Id, item.Scope, Stage)
            {
            }

            usize GetHashCode() const
            {
                return hash_multi(Id, Scope, static_cast<u8>(Stage));
            }

            bool operator==(const BindSlot& other) const
            {
                return Id == other.Id && Scope == other.Scope && Stage == other.Stage;
            }
        };

        enum class SigPlace : u8
        {
            None,
            Const,
            Direct,
            Grouped,
            StaticSampler,
        };

        struct SlotInfo final
        {
            // Shader Layout 中的 index
            u32 Index{};
            u32 Group{COPLT_U32_MAX};
            u32 IndexInGroup{COPLT_U32_MAX};
            // Place 为 Grouped 时是组内的 Index，StaticSampler 时是 StaticSampler 的 Index， 其他是 Root 的 Index
            u32 SigIndex{};
            SigPlace SigPlace{};

            SlotInfo() = default;

            explicit SlotInfo(const u32 index) : Index(index)
            {
            }
        };

        struct GroupItemInfo
        {
            u32 Group{};
            u32 IndexInGroup{};
            FGraphicsFormat Format{};
            FShaderStageFlags Stages{};
            FShaderLayoutItemView View{};
            FShaderLayoutItemType Type{};
            FResourceAccess UavAccess{};
        };

        enum class RootItemPlace : u8
        {
            Table,
            Direct,
            Const,
        };

        enum class RootItemType : u8
        {
            Resource,
            Sampler,
        };

        struct RootItemInfo
        {
            u32 Group{};
            // Place 为 Table 时永远为 0
            u32 IndexInGroup{};
            u32 RootIndex{};
            RootItemPlace Place{};
            RootItemType Type{};
        };
    }

    struct D3d12ShaderLayout final : GpuObject<D3d12ShaderLayout, FShaderLayout>, FShaderLayoutData
    {
        std::vector<FShaderLayoutItem> m_items{};

        explicit D3d12ShaderLayout(const FShaderLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderLayoutData* ShaderLayoutData() noexcept override;
    };

    struct D3d12BindGroupLayoutData : FBindGroupLayoutData
    {
        u32 ResourceTableSize{};
        u32 SamplerTableSize{};
    };

    COPLT_INTERFACE_DEFINE(ID3d12BindGroupLayout, "2e440ba8-f47a-4f98-a434-31125406e55c", FBindGroupLayout)
    {
        using BindSlotInfo = Layout::BindSlotInfo;
        virtual const D3d12BindGroupLayoutData& Data() const noexcept = 0;
        virtual std::span<const BindSlotInfo> Slots() const noexcept = 0;
    };

    struct D3d12BindGroupLayout final : GpuObject<D3d12BindGroupLayout, ID3d12BindGroupLayout>, D3d12BindGroupLayoutData
    {
        std::vector<FBindGroupItem> m_items{};
        std::vector<FStaticSamplerInfo> m_static_samplers{};
        std::vector<BindSlotInfo> m_slots{};

        explicit D3d12BindGroupLayout(const FBindGroupLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        FBindGroupLayoutData* BindGroupLayoutData() noexcept override;
        const D3d12BindGroupLayoutData& Data() const noexcept override;
        std::span<const BindSlotInfo> Slots() const noexcept override;
    };

    struct D3d12BindingLayoutData
    {
        u32 SumTableResourceSlots{};
        u32 SumTableSamplerSlots{};
    };

    COPLT_INTERFACE_DEFINE(ID3d12BindingLayout, "7271c0de-ec05-44be-9d1b-4b54894e297e", FBindingLayout)
    {
        using SlotInfo = Layout::SlotInfo;
        using GroupItemInfo = Layout::GroupItemInfo;
        using TableInfo = Layout::TableInfo;
        using RootItemInfo = Layout::RootItemInfo;

        virtual const D3d12BindingLayoutData& Data() const noexcept = 0;
        virtual const Rc<FShaderLayout>& ShaderLayout() const noexcept = 0;
        virtual std::span<const Rc<ID3d12BindGroupLayout>> Groups() const noexcept = 0;
        virtual const ComPtr<ID3D12RootSignature>& RootSignature() const noexcept = 0;
        virtual std::span<const SlotInfo> SlotInfos() const noexcept = 0;
        virtual std::span<const RootItemInfo> RootItemInfos() const noexcept = 0;
        // 第一层索引是 Group index
        virtual std::span<const std::vector<RootItemInfo>> GroupRootItemInfos() const noexcept = 0;
        // 第一层索引是 Group index
        virtual std::span<const std::vector<TableInfo>> TableInfos() const noexcept = 0;
        // 第一层索引是 Group index
        virtual std::span<const std::vector<GroupItemInfo>> GroupItemInfos() const noexcept = 0;
    };

    struct D3d12BindingLayout final : GpuObject<D3d12BindingLayout, ID3d12BindingLayout>, D3d12BindingLayoutData
    {
        using BindSlot = Layout::BindSlot;

        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12RootSignature> m_root_signature{};
        Rc<FShaderLayout> m_shader_layout{};
        std::vector<Rc<ID3d12BindGroupLayout>> m_groups{};
        std::vector<SlotInfo> m_slot_infos{};
        std::vector<RootItemInfo> m_bind_item_infos{};
        HashMap<BindSlot, usize> m_slot_to_info{};
        // 第一层索引是 Group index
        std::vector<std::vector<RootItemInfo>> m_group_bind_item_infos{};
        // 第一层索引是 Group index
        std::vector<std::vector<TableInfo>> m_tables{};
        // 第一层索引是 Group index
        std::vector<std::vector<GroupItemInfo>> m_group_item_infos{};

        explicit D3d12BindingLayout(Rc<D3d12GpuDevice>&& device, const FBindingLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        const D3d12BindingLayoutData& Data() const noexcept override;
        const Rc<FShaderLayout>& ShaderLayout() const noexcept override;
        std::span<const Rc<ID3d12BindGroupLayout>> Groups() const noexcept override;
        const ComPtr<ID3D12RootSignature>& RootSignature() const noexcept override;
        std::span<const SlotInfo> SlotInfos() const noexcept override;
        std::span<const RootItemInfo> RootItemInfos() const noexcept override;
        std::span<const std::vector<RootItemInfo>> GroupRootItemInfos() const noexcept override;
        std::span<const std::vector<TableInfo>> TableInfos() const noexcept override;
        std::span<const std::vector<GroupItemInfo>> GroupItemInfos() const noexcept override;
    };

    struct D3d12ShaderInputLayout final : GpuObject<D3d12ShaderInputLayout, FShaderInputLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        std::vector<Rc<FString8>> m_slot_names{};
        std::vector<FShaderInputLayoutElement> m_elements{};

        explicit D3d12ShaderInputLayout(Rc<D3d12GpuDevice>&& device, const FShaderInputLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        const FShaderInputLayoutElement* GetElements(u32* out_count) noexcept override;
    };

    struct D3d12MeshLayout final : GpuObject<D3d12MeshLayout, FMeshLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        std::vector<FMeshBufferDefine> m_buffers{};
        std::vector<FMeshBufferElement> m_elements{};
        HashMap<u64, const FMeshBufferElement*> m_slot_id_index_to_element{};

        explicit D3d12MeshLayout(Rc<D3d12GpuDevice>&& device, const FMeshLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        const FMeshBufferDefine* GetBuffers(u32* out_count) const noexcept override;
        const FMeshBufferElement* GetElements(u32* out_count) const noexcept override;

        const FMeshBufferElement* TryGetElement(u32 SlotId, u32 SlotIndex) const noexcept override;
    };
}
