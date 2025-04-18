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

    struct DescriptorHeapPair
    {
        u64 ResourceVersion{};
        u64 SamplerVersion{};
        ComPtr<ID3D12DescriptorHeap> ResourceHeap{};
        ComPtr<ID3D12DescriptorHeap> SamplerHeap{};

        explicit operator bool() const;
    };

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBindGroup, "5bb948b9-ad31-4eb8-b881-98017e048259", FShaderBindGroup)
    {
        virtual u64 Version() const noexcept = 0;
        virtual const Rc<ID3d12BindGroupLayout>& Layout() const noexcept = 0;
        virtual std::span<const View> Views() const noexcept = 0;
        virtual std::span<const u32> ItemIndexes() const noexcept = 0;
        virtual std::span<const u32> DefineIndexes() const noexcept = 0;
        virtual RwLock& SelfLock() noexcept = 0;
        // 返回是否有改变
        virtual bool EnsureAvailable(DescriptorHeapPair& out) = 0;

        virtual const ComPtr<ID3D12DescriptorHeap>& ResourceHeap() noexcept = 0;
        virtual const ComPtr<ID3D12DescriptorHeap>& SamplerHeap() noexcept = 0;
    };

    struct D3d12ShaderBindGroup final : GpuObject<D3d12ShaderBindGroup, ID3d12ShaderBindGroup>, FShaderBindGroupData
    {
        volatile u64 m_version{};
        Rc<FString> m_name{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<ID3d12BindGroupLayout> m_layout{};
        // 动态组永远不会创建描述符暂存堆
        ComPtr<ID3D12DescriptorHeap> m_resource_heap{};
        ComPtr<ID3D12DescriptorHeap> m_sampler_heap{};
        u64 m_resource_version{};
        u64 m_sampler_version{};
        u32 m_resource_heap_inc{};
        u32 m_sampler_heap_inc{};
        std::vector<View> m_views{};
        // 这里就是 vector<bool> 当 bitset 用
        std::vector<bool> m_changed_marks{};
        // view index => 布局 item index
        std::vector<u32> m_define_indexes{};
        // 布局 item index => view index
        std::vector<u32> m_item_indexes{};
        // 绑定修改锁
        RwLock m_self_lock{};
        // 标识符创建锁，必须在 m_self_lock Read 锁定状态使用
        std::mutex m_desc_lock{};
        volatile bool m_changed{};

        explicit D3d12ShaderBindGroup(Rc<D3d12GpuDevice>&& device, const FShaderBindGroupCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;
        void DoSetName(const FStr8or16& name) const;
        static void DoSetName(const FStr8or16& name, const DescriptorHeapPair& pair);

        FShaderBindGroupData* ShaderBindGroupData() noexcept override;

        u64 Version() const noexcept override;
        const Rc<ID3d12BindGroupLayout>& Layout() const noexcept override;
        std::span<const View> Views() const noexcept override;
        std::span<const u32> ItemIndexes() const noexcept override;
        std::span<const u32> DefineIndexes() const noexcept override;
        RwLock& SelfLock() noexcept override;

        // 需要外部锁 m_self_lock 必须是 Write 锁定状态
        void Set(std::span<const FSetBindItem> items);

        DescriptorHeapPair Cow() const;

        // 需要外部锁 m_self_lock 必须是 Read 锁定状态
        bool EnsureAvailable(DescriptorHeapPair& out) override;

        const ComPtr<ID3D12DescriptorHeap>& ResourceHeap() noexcept override;
        const ComPtr<ID3D12DescriptorHeap>& SamplerHeap() noexcept override;
    };

    COPLT_INTERFACE_DEFINE(ID3d12ShaderBinding, "5073f785-cfb7-414d-9a84-1602d3bf378d", FShaderBinding)
    {
        virtual u64 Version() const noexcept = 0;
        virtual const Rc<ID3d12BindingLayout>& Layout() const noexcept = 0;
        virtual std::span<const Rc<ID3d12ShaderBindGroup>> Groups() const noexcept = 0;
        virtual RwLock& SelfLock() noexcept = 0;
    };

    struct D3d12ShaderBinding final : GpuObject<D3d12ShaderBinding, ID3d12ShaderBinding>, FShaderBindingData
    {
        volatile u64 m_version{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<ID3d12BindingLayout> m_layout{};
        std::vector<Rc<ID3d12ShaderBindGroup>> m_groups{};
        // 绑定修改锁
        RwLock m_self_lock{};

        explicit D3d12ShaderBinding(Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options);
        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderBindingData* ShaderBindingData() noexcept override;

        u64 Version() const noexcept override;
        const Rc<ID3d12BindingLayout>& Layout() const noexcept override;
        std::span<const Rc<ID3d12ShaderBindGroup>> Groups() const noexcept override;
        RwLock& SelfLock() noexcept override;

        // 需要外部锁 m_self_lock 必须是 Write 锁定状态
        void Set(std::span<FSetBindGroupItem> items);
    };
}
