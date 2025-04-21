#pragma once

#include <variant>

#include <directx/d3d12.h>
#include <directx/d3dx12.h>

#include "../../Api/FFI/Resource.h"
#include "../../Api/FFI/Binding.h"
#include "../Src/Context.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuViewable, "597cbbb5-add9-45e3-bbba-802f63465c8e", FGpuViewable)
    {
        virtual bool IsCompatible(const FBindGroupItem& def) const = 0;
    };

    struct ID3d12GpuBuffer;
    struct ID3d12GpuImage;
    struct ID3d12GpuSampler;

    struct View final
    {
        Rc<ID3d12GpuViewable> m_viewable{};
        FViewData m_data{};

        View() = default;

        View& swap(View& other) noexcept;

        View(const View& other) noexcept = default;
        View& operator=(const View& view) = default;

        View(View&& other) noexcept = default;
        View& operator=(View&& view) = default;

        View(const FView& view);
        View& operator=(const FView& view);

        explicit operator bool() const;

        bool IsCompatible(const FBindGroupItem& def) const;
        bool IsBuffer() const;
        bool IsImage() const;
        bool IsSampler() const;
        bool IsUploadBuffer() const;
        bool NeedBarrier() const;

        NonNull<ID3d12GpuViewable> GetViewable() const;
        Ptr<ID3d12GpuBuffer> TryGetBuffer() const;
        Ptr<ID3d12GpuImage> TryGetImage() const;
        Ptr<ID3d12GpuSampler> TryGetSampler() const;

        void CreateDescriptor(
            NonNull<ID3D12Device2> device, Ptr<D3d12RecordContext> context, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle
        ) const;
        static void CreateNullDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle);
        void CreateBufferDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) const;
        void CreateUploadBufferDescriptor(
            NonNull<ID3D12Device2> device, NonNull<D3d12RecordContext> context, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle
        ) const;
        void CreateImageDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) const;
        void CreateSamplerDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) const;
    };
}
