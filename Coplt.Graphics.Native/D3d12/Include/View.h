#pragma once

#include <variant>

#include "../../Api/FFI/Resource.h"
#include "../../Api/Include/Ptr.h"
#include "../Src/Buffer.h"
#include "../Src/Image.h"
#include "../Src/Sampler.h"

namespace Coplt
{
    struct View final
    {
        enum class Type : u8
        {
            None,
            Buffer,
            Image,
            Sampler,
        };

    private:
        union
        {
            Rc<ID3d12GpuBuffer> m_buffer;
            Rc<ID3d12GpuImage> m_image;
            Rc<ID3d12GpuSampler> m_sampler;
        };

        Type m_type;

    public:
        Type Type() const;
        Rc<ID3d12GpuBuffer>& Buffer();
        const Rc<ID3d12GpuBuffer>& Buffer() const;
        Rc<ID3d12GpuImage>& Image();
        const Rc<ID3d12GpuImage>& Image() const;
        Rc<ID3d12GpuSampler>& Sampler();
        const Rc<ID3d12GpuSampler>& Sampler() const;

        ~View();

        View();

        View(const View& other);
        View(View&& other) noexcept;
        View& operator=(const View& view) noexcept;
        View& operator=(View&& view) noexcept;

        View(const FView& view);
        View& operator=(const FView& view);

        View(const Rc<ID3d12GpuBuffer>& buffer);
        View(Rc<ID3d12GpuBuffer>&& buffer);
        View& operator=(const Rc<ID3d12GpuBuffer>& buffer);
        View& operator=(Rc<ID3d12GpuBuffer>&& buffer);

        View(const Rc<ID3d12GpuImage>& image);
        View(Rc<ID3d12GpuImage>&& image);
        View& operator=(const Rc<ID3d12GpuImage>& image);
        View& operator=(Rc<ID3d12GpuImage>&& image);

        void CreateDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        ) const;
        static void CreateNullDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView view
        );
        void CreateBufferDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
            ) const;
        void CreateImageDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
            ) const;
        void CreateSamplerDescriptor(
            NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
        ) const;
    };
}
