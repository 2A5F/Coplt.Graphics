#pragma once

#include <ranges>

#include "../../Api/FFI/Binding.h"
#include "../../Api/Include/Object.h"
#include "../../Api/Include/Ptr.h"
#include "../Include/View.h"
#include "Device.h"
#include "Layout.h"

namespace Coplt
{
    struct DescriptorHeap;
    struct ID3d12ShaderBinding;
    struct DescriptorAllocation;

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBindGroup, "5bb948b9-ad31-4eb8-b881-98017e048259", FShaderBindGroup)
    {
        virtual const Rc<FBindGroupLayout>& Layout() const noexcept = 0;
        virtual std::span<const View> Views() const noexcept = 0;
        virtual std::span<const u32> ItemIndexes() const noexcept = 0;
        virtual std::span<const u32> DefineIndexes() const noexcept = 0;
        virtual RwLock& SelfLock() noexcept = 0;
        virtual void EnsureAvailable(ID3d12ShaderBinding& binding, u32 group_index) = 0;
    };

    struct D3d12ShaderBindGroup final : GpuObject<D3d12ShaderBindGroup, ID3d12ShaderBindGroup>, FShaderBindGroupData
    {
        Rc<FString> m_name{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<FBindGroupLayout> m_layout{};
        std::vector<View> m_views{};
        std::vector<u32> m_define_indexes{};
        std::vector<u32> m_item_indexes{};
        // 绑定修改锁
        RwLock m_self_lock{};
        volatile bool m_changed{};

        explicit D3d12ShaderBindGroup(Rc<D3d12GpuDevice>&& device, const FShaderBindGroupCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderBindGroupData* ShaderBindGroupData() noexcept override;

        const Rc<FBindGroupLayout>& Layout() const noexcept override;
        std::span<const View> Views() const noexcept override;
        std::span<const u32> ItemIndexes() const noexcept override;
        std::span<const u32> DefineIndexes() const noexcept override;
        RwLock& SelfLock() noexcept override;

        // 需要外部锁 m_self_lock 必须是 Write 锁定状态
        void Set(std::span<const FSetBindItem> items);

        // 需要外部锁 m_self_lock 和 binding 的 m_self_lock 必须是 Read 锁定状态
        void EnsureAvailable(ID3d12ShaderBinding& binding, u32 group_index) override;
    };

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBinding, "5073f785-cfb7-414d-9a84-1602d3bf378d", FShaderBinding)
    {
        virtual const Rc<ID3d12BindingLayout>& Layout() const noexcept = 0;
        virtual std::span<const Rc<ID3d12ShaderBindGroup>> Groups() const noexcept = 0;
        virtual RwLock& SelfLock() noexcept = 0;
        virtual std::mutex& DescLock() noexcept = 0;
    };

    struct D3d12ShaderBinding final : GpuObject<D3d12ShaderBinding, ID3d12ShaderBinding>, FShaderBindingData
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<ID3d12BindingLayout> m_layout{};
        std::vector<Rc<ID3d12ShaderBindGroup>> m_groups{};
        ComPtr<ID3D12DescriptorHeap> m_resource_heap{};
        ComPtr<ID3D12DescriptorHeap> m_sampler_heap{};
        // 绑定修改锁
        RwLock m_self_lock{};
        // 标识符创建锁，必须在 m_self_lock Read 锁定状态使用
        std::mutex m_desc_lock{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderBindingData* ShaderBindingData() noexcept override;

        const Rc<ID3d12BindingLayout>& Layout() const noexcept override;
        std::span<const Rc<ID3d12ShaderBindGroup>> Groups() const noexcept override;
        RwLock& SelfLock() noexcept override;
        std::mutex& DescLock() noexcept override;

        // 需要外部锁 m_self_lock 必须是 Write 锁定状态
        void Set(std::span<FSetBindGroupItem> items);
    };
}
