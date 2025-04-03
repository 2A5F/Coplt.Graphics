#pragma once

#include <variant>

#include "../../Api/FFI/Resource.h"
#include "../../Api/Include/Ptr.h"
#include "../Src/Buffer.h"
#include "../Src/Image.h"
#include "../Src/Sampler.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuViewable, "78155a99-1110-42f8-a1ef-c70fc23dbc52", FGpuViewable)
    {
    };

    struct View final
    {
        Rc<ID3d12GpuViewable> m_viewable{};
        FViewType m_type{};

        View(const FView& view);

        operator bool() const;

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
