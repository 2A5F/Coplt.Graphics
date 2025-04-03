#include "Image.h"

#include "../Include/GraphicsFormat.h"
#include "../Include/Resource.h"

using namespace Coplt;

D3d12GpuImage::D3d12GpuImage(Rc<D3d12GpuDevice>&& device, const FGpuImageCreateOptions& options)
    : m_device(std::move(device))
{
    m_allocator = m_device->m_gpu_allocator;

    m_resource_type = ResourceType::Image;
    m_purpose = options.Purpose;
    m_cpu_access = options.CpuAccess;
    m_format = options.Format;
    m_width = options.Width;
    m_height = options.Height;
    m_depth_or_length = options.DepthOrLength;
    m_mip_levels = options.MipLevels;
    m_multisample_count = options.MultisampleCount;
    m_dimension = options.Dimension;
    m_layout = options.Layout;

    if (m_format == FGraphicsFormat::Unknown)
        COPLT_THROW("Image must have the format");
    const auto format = ToDx(m_format);
    m_planes = D3D12GetFormatPlaneCount(m_device->m_device.Get(), ToDx(m_format));

    auto flags = D3D12_RESOURCE_FLAG_NONE;
    if (HasAnyFlags(m_purpose, FResourcePurpose::RenderTarget))
        flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    if (HasAnyFlags(m_purpose, FResourcePurpose::DepthStencil))
        flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    if (HasAnyFlags(m_purpose, FResourcePurpose::UnorderedAccess))
        flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    if (!HasAnyFlags(m_purpose, FResourcePurpose::ShaderResource))
        flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

    switch (m_dimension)
    {
    case FImageDimension::One:
        m_desc = CD3DX12_RESOURCE_DESC1::Tex1D(
            format, m_width, m_depth_or_length, m_mip_levels, flags, ToDx(m_layout)
        );
        break;
    case FImageDimension::Two:
        m_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
            format, m_width, m_height, m_depth_or_length, m_mip_levels,
            m_multisample_count, m_multisample_count > 1 ? 1 : 0, flags, ToDx(m_layout)
        );
        break;
    case FImageDimension::Three:
        m_desc = CD3DX12_RESOURCE_DESC1::Tex3D(
            format, m_width, m_height, m_depth_or_length, m_mip_levels, flags, ToDx(m_layout)
        );
        break;
    case FImageDimension::Cube:
        m_desc = CD3DX12_RESOURCE_DESC1::Tex2D(
            format, m_width, m_height, m_depth_or_length * 6, m_mip_levels,
            m_multisample_count, m_multisample_count > 1 ? 1 : 0, flags, ToDx(m_layout)
        );
        break;
    default:
        COPLT_THROW_FMT("Unsupported dimension {}", static_cast<u32>(m_dimension));
    }

    D3D12_CLEAR_VALUE clear_value{};
    if (options.HasOptimizedClearValue)
    {
        clear_value.Format = ToDx(options.OptimizedClearValue.Format);
        if (IsDepthStencil(options.OptimizedClearValue.Format))
        {
            clear_value.DepthStencil.Depth = options.OptimizedClearValue.Depth;
            clear_value.DepthStencil.Stencil = options.OptimizedClearValue.Stencil;
        }
        else
        {
            clear_value.Color[0] = options.OptimizedClearValue.Color[0];
            clear_value.Color[1] = options.OptimizedClearValue.Color[1];
            clear_value.Color[2] = options.OptimizedClearValue.Color[2];
            clear_value.Color[3] = options.OptimizedClearValue.Color[3];
        }
    }

    m_resource = box<ResourcePack>(
        m_device.get(), m_allocator.Get(), m_cpu_access, m_layout_state.Layout, &m_desc,
        options.HasOptimizedClearValue ? &clear_value : nullptr, true
    );
    m_raw_resource_ptr = GetRawResourcePtr();

    if (m_device->Debug())
    {
        chr | m_resource->m_resource >> SetNameEx(options.Name);
    }
}

FResult D3d12GpuImage::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        chr | m_resource->m_resource >> SetNameEx(name);
    });
}

FGpuResourceData* D3d12GpuImage::GpuResourceData() noexcept
{
    return this;
}

FGpuImageData* D3d12GpuImage::GpuImageData() noexcept
{
    return this;
}

NonNull<FGpuImageData> D3d12GpuImage::Data()
{
    return this;
}

NonNull<LayoutState> D3d12GpuImage::State()
{
    return &m_layout_state;
}

void* D3d12GpuImage::GetRawResourcePtr() noexcept
{
    return GetResourcePtr();
}

NonNull<ID3D12Resource> D3d12GpuImage::GetResourcePtr()
{
    return m_resource->m_resource.Get();
}

bool D3d12GpuImage::IsCompatible(const FBindGroupItem& def) const
{
    // todo
    return true;
}
