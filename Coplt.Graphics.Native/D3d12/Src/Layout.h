#pragma once

#include <vector>
#include <array>

#include "../../Api/Include/HashMap.h"

#include "Device.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Layout.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12ShaderLayout, "d8cea40e-7b0c-4a5f-98a9-88fd3abf9ddc", FD3d12ShaderLayout)
    {
        enum class TableScope : u8
        {
            Common,
            Material,
        };

        struct TableMeta
        {
            std::vector<D3D12_DESCRIPTOR_RANGE1> Ranges{};
            u32 Index{};
            u32 RootIndex{};
            FShaderStage Stage{};
            FShaderLayoutGroupView View{};

            TableMeta() = default;

            explicit TableMeta(const u32 index, const FShaderStage stage, const FShaderLayoutGroupView view)
            {
                Index = index;
                Stage = stage;
                View = view;
            }
        };

        struct TableGroup
        {
            std::vector<TableMeta> Metas{};
            std::vector<FShaderLayoutGroupInfo> Infos{};
            TableScope Scope{};
            bool Sampler{};
        };

        virtual std::span<TableGroup> GetTableGroups() noexcept = 0;
    };

    struct D3d12ShaderLayout final : Object<D3d12ShaderLayout, ID3d12ShaderLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12RootSignature> m_root_signature{};
        std::vector<FShaderLayoutItemDefine> m_layout_item_defines{};
        // 长度和 m_layout_item_defines 相同
        std::vector<FShaderLayoutItemInfo> m_item_infos{};
        std::vector<FShaderLayoutGroupClass> m_group_classes{};
        std::vector<TableGroup> m_table_groups{};

        explicit D3d12ShaderLayout(Rc<D3d12GpuDevice>&& device, const FShaderLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        void* GetRootSignaturePtr() noexcept override;

        const FShaderLayoutItemDefine* GetItemDefines(u32* out_count) noexcept override;
        const FShaderLayoutItemInfo* GetItemInfos(u32* out_count) noexcept override;
        const FShaderLayoutGroupClass* GetGroupClasses(u32* out_count) noexcept override;

        std::span<TableGroup> GetTableGroups() noexcept override;
    };

    struct D3d12ShaderInputLayout final : Object<D3d12ShaderInputLayout, FD3d12ShaderInputLayout>
    {
        Rc<D3d12GpuDevice> m_device{};
        std::vector<Rc<FString8>> m_slot_names{};
        std::vector<FShaderInputLayoutElement> m_elements{};

        explicit D3d12ShaderInputLayout(Rc<D3d12GpuDevice>&& device, const FShaderInputLayoutCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        const FShaderInputLayoutElement* GetElements(u32* out_count) noexcept override;
    };

    struct D3d12MeshLayout final : Object<D3d12MeshLayout, FD3d12MeshLayout>
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
