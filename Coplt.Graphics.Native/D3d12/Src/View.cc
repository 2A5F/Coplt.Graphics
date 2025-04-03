#include "../Include/View.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

View& View::swap(View& other) noexcept
{
    std::swap(m_viewable, other.m_viewable);
    std::swap(m_type, other.m_type);
    return *this;
}

View::View(const FView& view)
{
    if (view.Type != FViewType::None && view.Viewable)
    {
        const auto viewable = view.Viewable->QueryInterface<ID3d12GpuViewable>();
        if (!viewable)
            COPLT_THROW("Viewable from different backends");
        m_viewable = Rc<ID3d12GpuViewable>::UnsafeClone(viewable);
        m_type = view.Type;
    }
}

View& View::operator=(const FView& view)
{
    return View(view).swap(*this);
}

View::operator bool() const
{
    return m_viewable && m_type != FViewType::None;
}

bool View::IsCompatible(const FBindGroupItem& def) const
{
    switch (m_type)
    {
    case FViewType::None:
        break;
    case FViewType::Buffer:
    case FViewType::Image:
    case FViewType::Sampler:
        return m_viewable->IsCompatible(def);
    }
    return true;
}

// void View::CreateDescriptor(
//     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, const CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const FShaderLayoutGroupView type
// ) const
// {
//     switch (Type())
//     {
//     case Type::None:
//         CreateNullDescriptor(device, def, handle, type);
//         break;
//     case Type::Buffer:
//         CreateBufferDescriptor(device, def, handle, type);
//         break;
//     case Type::Image:
//         CreateImageDescriptor(device, def, handle, type);
//         break;
//     case Type::Sampler:
//         CreateSamplerDescriptor(device, def, handle, type);
//         break;
//     }
// }
//
// void View::CreateNullDescriptor(
//     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const FShaderLayoutGroupView view
// )
// {
//     switch (view)
//     {
//     case FShaderLayoutGroupView::Cbv:
//         {
//             device->CreateConstantBufferView(nullptr, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Srv:
//         {
//             D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
//             desc.Format = ToDx(def.Format);
//             desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//             switch (def.Type)
//             {
//             case FShaderLayoutItemType::ConstantBuffer:
//                 COPLT_THROW("Srv does not support ConstantBuffer");
//             case FShaderLayoutItemType::Buffer:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//                 break;
//             case FShaderLayoutItemType::RawBuffer:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//                 desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
//                 break;
//             case FShaderLayoutItemType::StructureBuffer:
//             case FShaderLayoutItemType::StructureBufferWithCounter:
//                 desc.Format = DXGI_FORMAT_UNKNOWN;
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//                 desc.Buffer.FirstElement = 0;
//                 desc.Buffer.NumElements = 1;
//                 desc.Buffer.StructureByteStride = 1;
//                 break;
//             case FShaderLayoutItemType::Texture1D:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
//                 desc.Texture1D.MostDetailedMip = 0;
//                 desc.Texture1D.MipLevels = 1;
//                 desc.Texture1D.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::Texture1DArray:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
//                 desc.Texture1DArray.MostDetailedMip = 0;
//                 desc.Texture1DArray.MipLevels = 1;
//                 desc.Texture1DArray.FirstArraySlice = 0;
//                 desc.Texture1DArray.ArraySize = 1;
//                 desc.Texture1DArray.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::Texture2D:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//                 desc.Texture2D.MostDetailedMip = 0;
//                 desc.Texture2D.MipLevels = 1;
//                 desc.Texture2D.PlaneSlice = 0;
//                 desc.Texture2D.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::Texture2DArray:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
//                 desc.Texture2DArray.MostDetailedMip = 0;
//                 desc.Texture2DArray.MipLevels = 1;
//                 desc.Texture2DArray.FirstArraySlice = 0;
//                 desc.Texture2DArray.ArraySize = 1;
//                 desc.Texture2DArray.PlaneSlice = 0;
//                 desc.Texture2DArray.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::Texture2DMultisample:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
//                 break;
//             case FShaderLayoutItemType::Texture2DArrayMultisample:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
//                 desc.Texture2DMSArray.FirstArraySlice = 0;
//                 desc.Texture2DMSArray.ArraySize = 1;
//                 break;
//             case FShaderLayoutItemType::Texture3D:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
//                 desc.Texture3D.MostDetailedMip = 0;
//                 desc.Texture3D.MipLevels = 1;
//                 desc.Texture3D.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::TextureCube:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
//                 desc.TextureCube.MostDetailedMip = 0;
//                 desc.TextureCube.MipLevels = 1;
//                 desc.TextureCube.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::TextureCubeArray:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
//                 desc.TextureCubeArray.MostDetailedMip = 0;
//                 desc.TextureCubeArray.MipLevels = 1;
//                 desc.TextureCubeArray.First2DArrayFace = 0;
//                 desc.TextureCubeArray.NumCubes = 1;
//                 desc.TextureCubeArray.ResourceMinLODClamp = 0;
//                 break;
//             case FShaderLayoutItemType::Sampler:
//                 COPLT_THROW("Srv does does not support sampler");
//             case FShaderLayoutItemType::RayTracingAccelerationStructure:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
//                 break;
//             }
//             device->CreateShaderResourceView(nullptr, &desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Uav:
//         {
//             D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
//             desc.Format = ToDx(def.Format);
//             switch (def.Type)
//             {
//             case FShaderLayoutItemType::ConstantBuffer:
//                 COPLT_THROW("Uav does not support ConstantBuffer");
//             case FShaderLayoutItemType::Buffer:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
//                 break;
//             case FShaderLayoutItemType::RawBuffer:
//                 desc.Format = DXGI_FORMAT_R32_TYPELESS;
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
//                 desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
//                 break;
//             case FShaderLayoutItemType::StructureBuffer:
//             case FShaderLayoutItemType::StructureBufferWithCounter:
//                 desc.Format = DXGI_FORMAT_UNKNOWN;
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
//                 desc.Buffer.FirstElement = 0;
//                 desc.Buffer.NumElements = 1;
//                 desc.Buffer.StructureByteStride = 1;
//                 desc.Buffer.CounterOffsetInBytes = 0;
//                 break;
//             case FShaderLayoutItemType::Texture1D:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
//                 desc.Texture1D.MipSlice = 0;
//                 break;
//             case FShaderLayoutItemType::Texture1DArray:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
//                 desc.Texture1DArray.MipSlice = 0;
//                 desc.Texture1DArray.FirstArraySlice = 0;
//                 desc.Texture1DArray.ArraySize = 1;
//                 break;
//             case FShaderLayoutItemType::Texture2D:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
//                 desc.Texture2D.MipSlice = 0;
//                 desc.Texture2D.PlaneSlice = 0;
//                 break;
//             case FShaderLayoutItemType::Texture2DArray:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
//                 desc.Texture2DArray.MipSlice = 0;
//                 desc.Texture2DArray.FirstArraySlice = 0;
//                 desc.Texture2DArray.ArraySize = 1;
//                 desc.Texture2DArray.PlaneSlice = 0;
//                 break;
//             case FShaderLayoutItemType::Texture2DMultisample:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMS;
//                 break;
//             case FShaderLayoutItemType::Texture2DArrayMultisample:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMSARRAY;
//                 desc.Texture2DMSArray.FirstArraySlice = 0;
//                 desc.Texture2DMSArray.ArraySize = 1;
//                 break;
//             case FShaderLayoutItemType::Texture3D:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
//                 desc.Texture3D.MipSlice = 0;
//                 desc.Texture3D.FirstWSlice = 0;
//                 desc.Texture3D.WSize = 1;
//                 break;
//             case FShaderLayoutItemType::TextureCube:
//                 COPLT_THROW("Uav does not support TextureCube");
//             case FShaderLayoutItemType::TextureCubeArray:
//                 COPLT_THROW("Uav does not support TextureCubeArray");
//             case FShaderLayoutItemType::Sampler:
//                 COPLT_THROW("Uav does not support Sampler");
//             case FShaderLayoutItemType::RayTracingAccelerationStructure:
//                 COPLT_THROW("Uav does not support RayTracingAccelerationStructure");
//             }
//             device->CreateUnorderedAccessView(nullptr, nullptr, &desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Sampler:
//         {
//             D3D12_SAMPLER_DESC desc{};
//             desc.Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
//             desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//             desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//             desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//             desc.MaxLOD = FLT_MAX;
//             device->CreateSampler(&desc, handle);
//             break;
//         }
//     default:
//         std::unreachable();
//     }
// }
//
// void View::CreateBufferDescriptor(
//     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
// ) const
// {
//     const auto buffer = Buffer().get();
//     const auto resource = buffer->GetResourcePtr();
//     const auto data = buffer->GpuBufferData();
//     switch (type)
//     {
//     case FShaderLayoutGroupView::Cbv:
//         {
//             D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
//             desc.BufferLocation = resource->GetGPUVirtualAddress();
//             desc.SizeInBytes = data->m_size;
//             device->CreateConstantBufferView(&desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Srv:
//         {
//             D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
//             desc.Format = ToDx(def.Format);
//             desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//             switch (def.Type)
//             {
//             case FShaderLayoutItemType::ConstantBuffer:
//                 COPLT_THROW("Srv does not support ConstantBuffer");
//             case FShaderLayoutItemType::Buffer:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//                 break;
//             case FShaderLayoutItemType::RawBuffer:
//                 if (data->m_usage != FBufferUsage::Raw)
//                     COPLT_THROW("Buffer not a Raw Buffer");
//                 desc.Format = DXGI_FORMAT_R32_TYPELESS;
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//                 desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
//                 break;
//             case FShaderLayoutItemType::StructureBuffer:
//             case FShaderLayoutItemType::StructureBufferWithCounter:
//                 if (data->m_usage != FBufferUsage::Structured)
//                     COPLT_THROW("Buffer not a Structured Buffer");
//                 desc.Format = DXGI_FORMAT_UNKNOWN;
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//                 desc.Buffer.FirstElement = 0;
//                 desc.Buffer.NumElements = data->m_count;
//                 desc.Buffer.StructureByteStride = data->m_stride;
//                 break;
//             case FShaderLayoutItemType::Texture1D:
//             case FShaderLayoutItemType::Texture1DArray:
//             case FShaderLayoutItemType::Texture2D:
//             case FShaderLayoutItemType::Texture2DArray:
//             case FShaderLayoutItemType::Texture2DMultisample:
//             case FShaderLayoutItemType::Texture2DArrayMultisample:
//             case FShaderLayoutItemType::Texture3D:
//             case FShaderLayoutItemType::TextureCube:
//             case FShaderLayoutItemType::TextureCubeArray:
//                 COPLT_THROW("Buffer Srv does not support Texture");
//             case FShaderLayoutItemType::Sampler:
//                 COPLT_THROW("Buffer Srv does not support Sampler");
//             case FShaderLayoutItemType::RayTracingAccelerationStructure:
//                 desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
//                 desc.RaytracingAccelerationStructure.Location = resource->GetGPUVirtualAddress();
//                 break;
//             }
//             device->CreateShaderResourceView(resource, &desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Uav:
//         {
//             D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
//             desc.Format = ToDx(def.Format);
//             switch (def.Type)
//             {
//             case FShaderLayoutItemType::ConstantBuffer:
//                 COPLT_THROW("Uav does not support ConstantBuffer");
//             case FShaderLayoutItemType::Buffer:
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
//                 break;
//             case FShaderLayoutItemType::RawBuffer:
//                 if (data->m_usage != FBufferUsage::Raw)
//                     COPLT_THROW("Buffer not a Raw Buffer");
//                 desc.Format = DXGI_FORMAT_R32_TYPELESS;
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
//                 desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
//                 break;
//             case FShaderLayoutItemType::StructureBuffer:
//             case FShaderLayoutItemType::StructureBufferWithCounter:
//                 if (data->m_usage != FBufferUsage::Structured)
//                     COPLT_THROW("Buffer not a Structured Buffer");
//                 desc.Format = DXGI_FORMAT_UNKNOWN;
//                 desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
//                 desc.Buffer.FirstElement = 0;
//                 desc.Buffer.NumElements = data->m_count;
//                 desc.Buffer.StructureByteStride = data->m_stride;
//                 desc.Buffer.CounterOffsetInBytes = 0;
//                 break;
//             case FShaderLayoutItemType::Texture1D:
//             case FShaderLayoutItemType::Texture1DArray:
//             case FShaderLayoutItemType::Texture2D:
//             case FShaderLayoutItemType::Texture2DArray:
//             case FShaderLayoutItemType::Texture2DMultisample:
//             case FShaderLayoutItemType::Texture2DArrayMultisample:
//             case FShaderLayoutItemType::Texture3D:
//             case FShaderLayoutItemType::TextureCube:
//             case FShaderLayoutItemType::TextureCubeArray:
//                 COPLT_THROW("Buffer Uav does not support Texture");
//             case FShaderLayoutItemType::Sampler:
//                 COPLT_THROW("Buffer Uav does not support Sampler");
//             case FShaderLayoutItemType::RayTracingAccelerationStructure:
//                 COPLT_THROW("Buffer Uav does not support RayTracingAccelerationStructure");
//             }
//             device->CreateUnorderedAccessView(resource, nullptr, &desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Sampler:
//         COPLT_THROW("Buffer cannot be used as a sampler");
//     }
// }
//
// void View::CreateImageDescriptor(
//     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
// ) const
// {
//     const auto image = Image().get();
//     const auto resource = image->GetResourcePtr();
//     const auto data = image->GpuImageData();
//     switch (type)
//     {
//     case FShaderLayoutGroupView::Cbv:
//         COPLT_THROW("Image cannot be used as a Cbv");
//     case FShaderLayoutGroupView::Srv:
//         {
//             D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
//             desc.Format = ToDx(data->m_format);
//             desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//             switch (def.Type)
//             {
//             case FShaderLayoutItemType::ConstantBuffer:
//                 COPLT_THROW("Srv does not support ConstantBuffer");
//             case FShaderLayoutItemType::Buffer:
//             case FShaderLayoutItemType::RawBuffer:
//             case FShaderLayoutItemType::StructureBuffer:
//             case FShaderLayoutItemType::StructureBufferWithCounter:
//                 COPLT_THROW("Image Srv does not support Buffer");
//             case FShaderLayoutItemType::Texture1D:
//                 {
//                     if (data->m_dimension != FImageDimension::One)
//                         COPLT_THROW("Bind required Texture1D, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
//                     desc.Texture1D.MostDetailedMip = 0;
//                     desc.Texture1D.MipLevels = data->m_mip_levels;
//                     desc.Texture1D.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture1DArray:
//                 {
//                     if (data->m_dimension != FImageDimension::One)
//                         COPLT_THROW("Bind required Texture1DArray, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
//                     desc.Texture1DArray.MostDetailedMip = 0;
//                     desc.Texture1DArray.MipLevels = data->m_mip_levels;
//                     desc.Texture1DArray.FirstArraySlice = 0;
//                     desc.Texture1DArray.ArraySize = data->m_depth_or_length;
//                     desc.Texture1DArray.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2D:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2D, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//                     desc.Texture2D.MostDetailedMip = 0;
//                     desc.Texture2D.MipLevels = data->m_mip_levels;
//                     desc.Texture2D.PlaneSlice = 0;
//                     desc.Texture2D.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2DArray:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2DArray, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
//                     desc.Texture2DArray.MostDetailedMip = 0;
//                     desc.Texture2DArray.MipLevels = data->m_mip_levels;
//                     desc.Texture2DArray.FirstArraySlice = 0;
//                     desc.Texture2DArray.ArraySize = data->m_depth_or_length;
//                     desc.Texture2DArray.PlaneSlice = 0;
//                     desc.Texture2DArray.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2DMultisample:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2DMultisample, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2DArrayMultisample:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2DArrayMultisample, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
//                     desc.Texture2DMSArray.FirstArraySlice = 0;
//                     desc.Texture2DMSArray.ArraySize = data->m_depth_or_length;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture3D:
//                 {
//                     if (data->m_dimension != FImageDimension::Three)
//                         COPLT_THROW("Bind required Texture3D, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
//                     desc.Texture3D.MostDetailedMip = 0;
//                     desc.Texture3D.MipLevels = data->m_mip_levels;
//                     desc.Texture3D.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::TextureCube:
//                 {
//                     if (data->m_dimension != FImageDimension::Cube)
//                         COPLT_THROW("Bind required TextureCube, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
//                     desc.TextureCube.MostDetailedMip = 0;
//                     desc.TextureCube.MipLevels = data->m_mip_levels;
//                     desc.TextureCube.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::TextureCubeArray:
//                 {
//                     if (data->m_dimension != FImageDimension::Cube)
//                         COPLT_THROW("Bind required TextureCubeArray, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
//                     desc.TextureCubeArray.MostDetailedMip = 0;
//                     desc.TextureCubeArray.MipLevels = data->m_mip_levels;
//                     desc.TextureCubeArray.First2DArrayFace = 0;
//                     desc.TextureCubeArray.NumCubes = (data->m_depth_or_length + 5) / 6;
//                     desc.TextureCubeArray.ResourceMinLODClamp = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Sampler:
//                 COPLT_THROW("Srv does not support sampler");
//             case FShaderLayoutItemType::RayTracingAccelerationStructure:
//                 COPLT_THROW("Image Srv does not support RayTracingAccelerationStructure");
//             }
//             device->CreateShaderResourceView(resource, &desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Uav:
//         {
//             D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
//             desc.Format = ToDx(data->m_format);
//             switch (def.Type)
//             {
//             case FShaderLayoutItemType::ConstantBuffer:
//                 COPLT_THROW("Uav not support ConstantBuffer");
//
//             case FShaderLayoutItemType::Buffer:
//             case FShaderLayoutItemType::RawBuffer:
//             case FShaderLayoutItemType::StructureBuffer:
//             case FShaderLayoutItemType::StructureBufferWithCounter:
//                 COPLT_THROW("Image Srv does not support Buffer");
//             case FShaderLayoutItemType::Texture1D:
//                 {
//                     if (data->m_dimension != FImageDimension::One)
//                         COPLT_THROW("Bind required Texture1D, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
//                     desc.Texture1D.MipSlice = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture1DArray:
//                 {
//                     if (data->m_dimension != FImageDimension::One)
//                         COPLT_THROW("Bind required Texture1DArray, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
//                     desc.Texture1DArray.MipSlice = 0;
//                     desc.Texture1DArray.FirstArraySlice = 0;
//                     desc.Texture1DArray.ArraySize = data->m_depth_or_length;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2D:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2D, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
//                     desc.Texture2D.MipSlice = 0;
//                     desc.Texture2D.PlaneSlice = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2DArray:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2DArray, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
//                     desc.Texture2DArray.MipSlice = 0;
//                     desc.Texture2DArray.FirstArraySlice = 0;
//                     desc.Texture2DArray.ArraySize = data->m_depth_or_length;
//                     desc.Texture2DArray.PlaneSlice = 0;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2DMultisample:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2DMultisample, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMS;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture2DArrayMultisample:
//                 {
//                     if (data->m_dimension != FImageDimension::Two)
//                         COPLT_THROW("Bind required Texture2DArrayMultisample, but the actual resources provided are not");
//                     desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMSARRAY;
//                     desc.Texture2DMSArray.FirstArraySlice = 0;
//                     desc.Texture2DMSArray.ArraySize = data->m_depth_or_length;
//                     break;
//                 }
//             case FShaderLayoutItemType::Texture3D:
//                 {
//                     if (data->m_dimension != FImageDimension::Three)
//                         COPLT_THROW("Bind required Texture3D, but the actual resources provided are not");
//                     desc.Texture3D.MipSlice = 0;
//                     desc.Texture3D.FirstWSlice = 0;
//                     desc.Texture3D.WSize = data->m_depth_or_length;
//                     break;
//                 }
//             case FShaderLayoutItemType::TextureCube:
//                 COPLT_THROW("Uav not support TextureCube");
//             case FShaderLayoutItemType::TextureCubeArray:
//                 COPLT_THROW("Uav not support TextureCubeArray");
//             case FShaderLayoutItemType::Sampler:
//                 COPLT_THROW("Uav not support Sampler");
//             case FShaderLayoutItemType::RayTracingAccelerationStructure:
//                 COPLT_THROW("Uav not support RayTracingAccelerationStructure");
//             }
//             device->CreateUnorderedAccessView(resource, nullptr, &desc, handle);
//             break;
//         }
//     case FShaderLayoutGroupView::Sampler:
//         COPLT_THROW("Image cannot be used as a sampler");
//     }
// }
//
// void View::CreateSamplerDescriptor(
//     NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
// ) const
// {
//     const auto sampler = Sampler().get();
//     switch (type)
//     {
//     case FShaderLayoutGroupView::Cbv:
//         COPLT_THROW("Sampler cannot be used as a Cbv");
//     case FShaderLayoutGroupView::Srv:
//         COPLT_THROW("Sampler cannot be used as a Srv");
//     case FShaderLayoutGroupView::Uav:
//         COPLT_THROW("Sampler cannot be used as a Uav");
//     case FShaderLayoutGroupView::Sampler:
//         device->CreateSampler(sampler->Desc(), handle);
//         break;
//     }
// }
