#pragma once

#include <variant>

#include "../../Api/FFI/Resource.h"
#include "../../Api/FFI/Binding.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuViewable, "597cbbb5-add9-45e3-bbba-802f63465c8e", FGpuViewable)
    {
        virtual bool IsCompatible(const FBindGroupItem& def) const = 0;
    };

    struct View final
    {
        Rc<ID3d12GpuViewable> m_viewable{};
        FViewType m_type{};

        View() = default;

        View& swap(View& other) noexcept;

        View(const View& other) noexcept = default;
        View& operator=(const View& view) = default;

        View(View&& other) noexcept = default;
        View& operator=(View&& view) = default;

        View(const FView& view);
        View& operator=(const FView& view);

        operator bool() const;

        bool IsCompatible(const FBindGroupItem& def) const;

        // void CreateDescriptor(
        //     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        // ) const;
        // static void CreateNullDescriptor(
        //     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView view
        // );
        // void CreateBufferDescriptor(
        //     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        //     ) const;
        // void CreateImageDescriptor(
        //     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        //     ) const;
        // void CreateSamplerDescriptor(
        //     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        // ) const;
    };
}
