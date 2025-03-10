#pragma once

#include <variant>

#include "../../Api/FFI/Resource.h"
#include "../../Api/Include/Ptr.h"
#include "../Src/Buffer.h"

namespace Coplt
{
    struct View
    {
        enum class Type : u8
        {
            None,
            Buffer,
        };

    private:
        union
        {
            Rc<D3d12GpuBuffer> m_buffer;
        };

        Type m_type;

    public:
        Type Type() const;
        Rc<D3d12GpuBuffer>& Buffer() ;
        const Rc<D3d12GpuBuffer>& Buffer() const ;

        ~View();

        View();

        View(const View& other);
        View(View&& other) noexcept;
        View& operator=(const View& view) noexcept;
        View& operator=(View&& view) noexcept;

        View(const FView& view);
        View& operator=(const FView& view);

        View(const Rc<D3d12GpuBuffer>& buffer);
        View(Rc<D3d12GpuBuffer>&& buffer);
        View& operator=(const Rc<D3d12GpuBuffer>& buffer);
        View& operator=(Rc<D3d12GpuBuffer>&& buffer);

        void CreateDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        ) const;
        static void CreateNullDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView view
        );
        void CreateBufferDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        ) const;
    };
}
