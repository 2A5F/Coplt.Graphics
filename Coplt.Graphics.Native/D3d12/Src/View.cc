#include "../Include/View.h"
#include "../Include/GraphicsFormat.h"
#include "Buffer.h"
#include "Image.h"
#include "Sampler.h"

using namespace Coplt;

View& View::swap(View& other) noexcept
{
    std::swap(m_viewable, other.m_viewable);
    std::swap(m_data, other.m_data);
    return *this;
}

View::View(const FView& view)
{
    if (view.Data.Type != FViewType::None)
    {
        m_data = view.Data;
        if (view.Viewable)
        {
            const auto viewable = view.Viewable->QueryInterface<ID3d12GpuViewable>();
            if (!viewable)
                COPLT_THROW("Viewable from different backends");
            m_viewable = Rc<ID3d12GpuViewable>::UnsafeClone(viewable);
        }
    }
}

View& View::operator=(const FView& view)
{
    View(view).swap(*this);
    return *this;
}

View::operator bool() const
{
    switch (m_data.Type)
    {
    case FViewType::None:
    case FViewType::UploadBuffer:
        return true;
    case FViewType::Sampler:
    case FViewType::Buffer:
    case FViewType::Image1D:
    case FViewType::Image1DArray:
    case FViewType::Image2D:
    case FViewType::Image2DArray:
    case FViewType::Image2DMs:
    case FViewType::Image2DMsArray:
    case FViewType::Image3D:
    case FViewType::ImageCube:
    case FViewType::ImageCubeArray:
        return static_cast<bool>(m_viewable);
    }
    return false;
}

bool View::IsCompatible(const FBindGroupItem& def) const
{
    if (!*this) return true;
    switch (m_data.Type)
    {
    case FViewType::None:
        return true;
    case FViewType::Sampler:
        return def.Type == FShaderLayoutItemType::Sampler;
    case FViewType::Buffer:
        switch (def.Type)
        {
        case FShaderLayoutItemType::ConstantBuffer:
        case FShaderLayoutItemType::Buffer:
        case FShaderLayoutItemType::RawBuffer:
        case FShaderLayoutItemType::StructureBuffer:
        case FShaderLayoutItemType::StructureBufferWithCounter:
        case FShaderLayoutItemType::RayTracingAccelerationStructure:
            return m_viewable->IsCompatible(def);
        default:
            return false;
        }
    case FViewType::UploadBuffer:
        switch (def.Type)
        {
        case FShaderLayoutItemType::ConstantBuffer:
        case FShaderLayoutItemType::Buffer:
        case FShaderLayoutItemType::RawBuffer:
        case FShaderLayoutItemType::StructureBuffer:
            return true;
        default:
            return false;
        }
    case FViewType::Image2DMs:
    case FViewType::Image2DMsArray:
        if (def.Type == FShaderLayoutItemType::Texture2DMultisample) break;
        if (def.Type == FShaderLayoutItemType::Texture2DArrayMultisample) break;
        return false;
    case FViewType::Image1D:
    case FViewType::Image1DArray:
    case FViewType::Image2D:
    case FViewType::Image2DArray:
    case FViewType::Image3D:
    case FViewType::ImageCube:
    case FViewType::ImageCubeArray:
        switch (def.Type)
        {
        case FShaderLayoutItemType::Texture1D:
        case FShaderLayoutItemType::Texture1DArray:
        case FShaderLayoutItemType::Texture2D:
        case FShaderLayoutItemType::Texture2DArray:
        case FShaderLayoutItemType::Texture3D:
        case FShaderLayoutItemType::TextureCube:
        case FShaderLayoutItemType::TextureCubeArray:
            return m_viewable->IsCompatible(def);
        default:
            return false;
        }
    }
    return m_viewable->IsCompatible(def);
}

bool View::IsBuffer() const
{
    switch (m_data.Type)
    {
    case FViewType::None:
    case FViewType::Buffer:
    case FViewType::UploadBuffer:
        return true;
    case FViewType::Sampler:
    case FViewType::Image1D:
    case FViewType::Image1DArray:
    case FViewType::Image2D:
    case FViewType::Image2DArray:
    case FViewType::Image2DMs:
    case FViewType::Image2DMsArray:
    case FViewType::Image3D:
    case FViewType::ImageCube:
    case FViewType::ImageCubeArray:
        break;
    }
    return false;
}

bool View::IsImage() const
{
    switch (m_data.Type)
    {
    case FViewType::None:
    case FViewType::Sampler:
    case FViewType::Buffer:
    case FViewType::UploadBuffer:
        break;
    case FViewType::Image1D:
    case FViewType::Image1DArray:
    case FViewType::Image2D:
    case FViewType::Image2DArray:
    case FViewType::Image2DMs:
    case FViewType::Image2DMsArray:
    case FViewType::Image3D:
    case FViewType::ImageCube:
    case FViewType::ImageCubeArray:
        return true;
    }
    return false;
}

bool View::IsSampler() const
{
    return m_data.Type == FViewType::Sampler;
}

bool View::IsUploadBuffer() const
{
    return m_data.Type == FViewType::UploadBuffer;
}

bool View::NeedBarrier() const
{
    switch (m_data.Type)
    {
    case FViewType::None:
    case FViewType::Sampler:
    case FViewType::UploadBuffer:
        return false;
    case FViewType::Buffer:
    case FViewType::Image1D:
    case FViewType::Image1DArray:
    case FViewType::Image2D:
    case FViewType::Image2DArray:
    case FViewType::Image2DMs:
    case FViewType::Image2DMsArray:
    case FViewType::Image3D:
    case FViewType::ImageCube:
    case FViewType::ImageCubeArray:
        return true;
    }
    return false;
}

NonNull<ID3d12GpuViewable> View::GetViewable() const
{
    return m_viewable;
}

Ptr<ID3d12GpuBuffer> View::TryGetBuffer() const
{
    return m_viewable->QueryInterface<ID3d12GpuBuffer>();
}

Ptr<ID3d12GpuImage> View::TryGetImage() const
{
    return m_viewable->QueryInterface<ID3d12GpuImage>();
}

Ptr<ID3d12GpuSampler> View::TryGetSampler() const
{
    return m_viewable->QueryInterface<ID3d12GpuSampler>();
}

void View::CreateDescriptor(
    NonNull<ID3D12Device2> device, Ptr<D3d12RecordContext> context, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle
) const
{
    switch (m_data.Type)
    {
    case FViewType::None:
        CreateNullDescriptor(device, def, handle);
        break;
    case FViewType::Buffer:
        CreateBufferDescriptor(device, def, handle);
        break;
    case FViewType::Image1D:
    case FViewType::Image1DArray:
    case FViewType::Image2D:
    case FViewType::Image2DArray:
    case FViewType::Image2DMs:
    case FViewType::Image2DMsArray:
    case FViewType::Image3D:
    case FViewType::ImageCube:
    case FViewType::ImageCubeArray:
        CreateImageDescriptor(device, def, handle);
        break;
    case FViewType::Sampler:
        CreateSamplerDescriptor(device, def, handle);
        break;
    case FViewType::UploadBuffer:
        CreateUploadBufferDescriptor(device, context, def, handle);
        break;
    }
}

void View::CreateNullDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle)
{
    switch (def.View)
    {
    case FShaderLayoutItemView::Cbv:
        {
            device->CreateConstantBufferView(nullptr, handle);
            break;
        }
    case FShaderLayoutItemView::Srv:
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
            desc.Format = ToDx(def.Format);
            if (desc.Format == DXGI_FORMAT_UNKNOWN) desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Srv does not support ConstantBuffer");
            case FShaderLayoutItemType::Buffer:
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                break;
            case FShaderLayoutItemType::RawBuffer:
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
                break;
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                desc.Format = DXGI_FORMAT_UNKNOWN;
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                desc.Buffer.FirstElement = 0;
                desc.Buffer.NumElements = 1;
                desc.Buffer.StructureByteStride = 1;
                break;
            case FShaderLayoutItemType::Texture1D:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
                desc.Texture1D.MostDetailedMip = 0;
                desc.Texture1D.MipLevels = 1;
                desc.Texture1D.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::Texture1DArray:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                desc.Texture1DArray.MostDetailedMip = 0;
                desc.Texture1DArray.MipLevels = 1;
                desc.Texture1DArray.FirstArraySlice = 0;
                desc.Texture1DArray.ArraySize = 1;
                desc.Texture1DArray.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::Texture2D:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                desc.Texture2D.MostDetailedMip = 0;
                desc.Texture2D.MipLevels = 1;
                desc.Texture2D.PlaneSlice = 0;
                desc.Texture2D.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::Texture2DArray:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                desc.Texture2DArray.MostDetailedMip = 0;
                desc.Texture2DArray.MipLevels = 1;
                desc.Texture2DArray.FirstArraySlice = 0;
                desc.Texture2DArray.ArraySize = 1;
                desc.Texture2DArray.PlaneSlice = 0;
                desc.Texture2DArray.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::Texture2DMultisample:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                break;
            case FShaderLayoutItemType::Texture2DArrayMultisample:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
                desc.Texture2DMSArray.FirstArraySlice = 0;
                desc.Texture2DMSArray.ArraySize = 1;
                break;
            case FShaderLayoutItemType::Texture3D:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
                desc.Texture3D.MostDetailedMip = 0;
                desc.Texture3D.MipLevels = 1;
                desc.Texture3D.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::TextureCube:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
                desc.TextureCube.MostDetailedMip = 0;
                desc.TextureCube.MipLevels = 1;
                desc.TextureCube.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::TextureCubeArray:
                desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
                desc.TextureCubeArray.MostDetailedMip = 0;
                desc.TextureCubeArray.MipLevels = 1;
                desc.TextureCubeArray.First2DArrayFace = 0;
                desc.TextureCubeArray.NumCubes = 1;
                desc.TextureCubeArray.ResourceMinLODClamp = 0;
                break;
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Srv does does not support sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
                break;
            }
            device->CreateShaderResourceView(nullptr, &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Uav:
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
            desc.Format = ToDx(def.Format);
            if (desc.Format == DXGI_FORMAT_UNKNOWN) desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Uav does not support ConstantBuffer");
            case FShaderLayoutItemType::Buffer:
                desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                break;
            case FShaderLayoutItemType::RawBuffer:
                desc.Format = DXGI_FORMAT_R32_TYPELESS;
                desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
                break;
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                desc.Format = DXGI_FORMAT_UNKNOWN;
                desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                desc.Buffer.FirstElement = 0;
                desc.Buffer.NumElements = 1;
                desc.Buffer.StructureByteStride = 1;
                desc.Buffer.CounterOffsetInBytes = 0;
                break;
            case FShaderLayoutItemType::Texture1D:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
                desc.Texture1D.MipSlice = 0;
                break;
            case FShaderLayoutItemType::Texture1DArray:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
                desc.Texture1DArray.MipSlice = 0;
                desc.Texture1DArray.FirstArraySlice = 0;
                desc.Texture1DArray.ArraySize = 1;
                break;
            case FShaderLayoutItemType::Texture2D:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                desc.Texture2D.MipSlice = 0;
                desc.Texture2D.PlaneSlice = 0;
                break;
            case FShaderLayoutItemType::Texture2DArray:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
                desc.Texture2DArray.MipSlice = 0;
                desc.Texture2DArray.FirstArraySlice = 0;
                desc.Texture2DArray.ArraySize = 1;
                desc.Texture2DArray.PlaneSlice = 0;
                break;
            case FShaderLayoutItemType::Texture2DMultisample:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMS;
                break;
            case FShaderLayoutItemType::Texture2DArrayMultisample:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMSARRAY;
                desc.Texture2DMSArray.FirstArraySlice = 0;
                desc.Texture2DMSArray.ArraySize = 1;
                break;
            case FShaderLayoutItemType::Texture3D:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
                desc.Texture3D.MipSlice = 0;
                desc.Texture3D.FirstWSlice = 0;
                desc.Texture3D.WSize = 1;
                break;
            case FShaderLayoutItemType::TextureCube:
                COPLT_THROW("Uav does not support TextureCube");
            case FShaderLayoutItemType::TextureCubeArray:
                COPLT_THROW("Uav does not support TextureCubeArray");
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Uav does not support Sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                COPLT_THROW("Uav does not support RayTracingAccelerationStructure");
            }
            device->CreateUnorderedAccessView(nullptr, nullptr, &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Sampler:
        {
            D3D12_SAMPLER_DESC desc{};
            desc.Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            desc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            desc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            desc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            desc.MaxLOD = FLT_MAX;
            device->CreateSampler(&desc, handle);
            break;
        }
    default:
        std::unreachable();
    }
}

void View::CreateBufferDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) const
{
    const NonNull buffer = TryGetBuffer();
    const auto resource = buffer->GetResourcePtr();
    const auto data = buffer->GpuBufferData();
    switch (def.View)
    {
    case FShaderLayoutItemView::Cbv:
        {
            D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
            desc.BufferLocation = resource->GetGPUVirtualAddress() + m_data.Buffer.Offset;
            desc.SizeInBytes = m_data.Buffer.Size;
            device->CreateConstantBufferView(&desc, handle);
            break;
        }
    case FShaderLayoutItemView::Srv:
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
            desc.Format = ToDx(m_data.Buffer.Format == FGraphicsFormat::Unknown ? def.Format : m_data.Buffer.Format);
            desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Srv does not support ConstantBuffer");
            case FShaderLayoutItemType::Buffer:
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                desc.Buffer.StructureByteStride = 0;
                if (m_data.Buffer.Stride != 0)
                {
                    desc.Buffer.FirstElement = 0;
                    desc.Buffer.NumElements = data->m_size;
                }
                else
                {
                    desc.Buffer.FirstElement = m_data.Buffer.Offset;
                    desc.Buffer.NumElements = m_data.Buffer.Size;
                }
                break;
            case FShaderLayoutItemType::RawBuffer:
                desc.Format = DXGI_FORMAT_R32_TYPELESS;
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                desc.Buffer.StructureByteStride = 0;
                if (m_data.Buffer.Stride != 0)
                {
                    if (data->m_usage != FBufferUsage::Raw)
                        COPLT_THROW("Buffer not a Raw Buffer");
                    desc.Buffer.FirstElement = 0;
                    desc.Buffer.NumElements = data->m_size;
                }
                else
                {
                    desc.Buffer.FirstElement = m_data.Buffer.Offset;
                    desc.Buffer.NumElements = m_data.Buffer.Size;
                }
                desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
                break;
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                desc.Format = DXGI_FORMAT_UNKNOWN;
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                if (m_data.Buffer.Stride == 0)
                {
                    if (data->m_usage != FBufferUsage::Structured)
                        COPLT_THROW("Buffer not a Structured Buffer");
                    desc.Buffer.FirstElement = 0;
                    desc.Buffer.NumElements = data->m_count;
                    desc.Buffer.StructureByteStride = data->m_stride;
                }
                else
                {
                    desc.Buffer.FirstElement = m_data.Buffer.Offset;
                    desc.Buffer.NumElements = m_data.Buffer.Size;
                    desc.Buffer.StructureByteStride = m_data.Buffer.Stride;
                }
                break;
            case FShaderLayoutItemType::Texture1D:
            case FShaderLayoutItemType::Texture1DArray:
            case FShaderLayoutItemType::Texture2D:
            case FShaderLayoutItemType::Texture2DArray:
            case FShaderLayoutItemType::Texture2DMultisample:
            case FShaderLayoutItemType::Texture2DArrayMultisample:
            case FShaderLayoutItemType::Texture3D:
            case FShaderLayoutItemType::TextureCube:
            case FShaderLayoutItemType::TextureCubeArray:
                COPLT_THROW("Buffer Srv does not support Texture");
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Buffer Srv does not support Sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
                desc.RaytracingAccelerationStructure.Location = resource->GetGPUVirtualAddress() + m_data.Buffer.Offset;
                break;
            }
            device->CreateShaderResourceView(resource, &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Uav:
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
            desc.Format = ToDx(m_data.Buffer.Format == FGraphicsFormat::Unknown ? def.Format : m_data.Buffer.Format);
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Uav does not support ConstantBuffer");
            case FShaderLayoutItemType::Buffer:
                desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                desc.Buffer.StructureByteStride = 0;
                if (m_data.Buffer.Stride != 0)
                {
                    desc.Buffer.FirstElement = 0;
                    desc.Buffer.NumElements = data->m_size;
                }
                else
                {
                    desc.Buffer.FirstElement = m_data.Buffer.Offset;
                    desc.Buffer.NumElements = m_data.Buffer.Size;
                }
                break;
            case FShaderLayoutItemType::RawBuffer:
                desc.Format = DXGI_FORMAT_R32_TYPELESS;
                desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                desc.Buffer.StructureByteStride = 0;
                if (m_data.Buffer.Stride != 0)
                {
                    if (data->m_usage != FBufferUsage::Raw)
                        COPLT_THROW("Buffer not a Raw Buffer");
                    desc.Buffer.FirstElement = 0;
                    desc.Buffer.NumElements = data->m_size;
                }
                else
                {
                    desc.Buffer.FirstElement = m_data.Buffer.Offset;
                    desc.Buffer.NumElements = m_data.Buffer.Size;
                }
                desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_RAW;
                break;
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                desc.Format = DXGI_FORMAT_UNKNOWN;
                desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
                if (m_data.Buffer.Stride == 0)
                {
                    if (data->m_usage != FBufferUsage::Structured)
                        COPLT_THROW("Buffer not a Structured Buffer");
                    desc.Buffer.FirstElement = 0;
                    desc.Buffer.NumElements = data->m_count;
                    desc.Buffer.StructureByteStride = data->m_stride;
                }
                else
                {
                    desc.Buffer.FirstElement = m_data.Buffer.Offset;
                    desc.Buffer.NumElements = m_data.Buffer.Size;
                    desc.Buffer.StructureByteStride = m_data.Buffer.Stride;
                }
                desc.Buffer.CounterOffsetInBytes = 0;
                break;
            case FShaderLayoutItemType::Texture1D:
            case FShaderLayoutItemType::Texture1DArray:
            case FShaderLayoutItemType::Texture2D:
            case FShaderLayoutItemType::Texture2DArray:
            case FShaderLayoutItemType::Texture2DMultisample:
            case FShaderLayoutItemType::Texture2DArrayMultisample:
            case FShaderLayoutItemType::Texture3D:
            case FShaderLayoutItemType::TextureCube:
            case FShaderLayoutItemType::TextureCubeArray:
                COPLT_THROW("Buffer Uav does not support Texture");
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Buffer Uav does not support Sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                COPLT_THROW("Buffer Uav does not support RayTracingAccelerationStructure");
            }
            device->CreateUnorderedAccessView(resource, nullptr, &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Sampler:
        COPLT_THROW("Buffer cannot be used as a sampler");
    default:
        std::unreachable();
    }
}

void View::CreateUploadBufferDescriptor(
    NonNull<ID3D12Device2> device, NonNull<D3d12RecordContext> context, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle
) const
{
    const auto& buffers = context->m_upload_buffers;
    if (m_data.UploadBuffer.Index >= buffers.size())
        COPLT_THROW("Index out of range");
    const auto& buffer = buffers[m_data.UploadBuffer.Index];
    const auto& resource = buffer.m_resource.m_resource;
    switch (def.View)
    {
    case FShaderLayoutItemView::Cbv:
        {
            D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
            desc.BufferLocation = resource->GetGPUVirtualAddress() + m_data.UploadBuffer.Offset;
            desc.SizeInBytes = m_data.UploadBuffer.Size;
            device->CreateConstantBufferView(&desc, handle);
            break;
        }
    case FShaderLayoutItemView::Srv:
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Srv does not support ConstantBuffer");
            case FShaderLayoutItemType::Buffer:
                COPLT_THROW("Upload Buffer does not support Srv Buffer");
            // desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            // desc.Buffer.StructureByteStride = 0;
            // desc.Buffer.FirstElement = m_data.UploadBuffer.Offset;
            // desc.Buffer.NumElements = m_data.UploadBuffer.Size;
                break;
            case FShaderLayoutItemType::RawBuffer:
                desc.Format = DXGI_FORMAT_R32_TYPELESS;
                desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
                desc.Buffer.StructureByteStride = 0;
                desc.Buffer.FirstElement = m_data.UploadBuffer.Offset;
                desc.Buffer.NumElements = m_data.UploadBuffer.Size;
                desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_RAW;
                break;
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                COPLT_THROW("Upload Buffer does not support StructureBuffer");
            // desc.Format = DXGI_FORMAT_UNKNOWN;
            // desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            // desc.Buffer.FirstElement = m_data.UploadBuffer.Offset;
            // desc.Buffer.NumElements = m_data.UploadBuffer.Size;
            // desc.Buffer.StructureByteStride = m_data.UploadBuffer.Stride;
                break;
            case FShaderLayoutItemType::Texture1D:
            case FShaderLayoutItemType::Texture1DArray:
            case FShaderLayoutItemType::Texture2D:
            case FShaderLayoutItemType::Texture2DArray:
            case FShaderLayoutItemType::Texture2DMultisample:
            case FShaderLayoutItemType::Texture2DArrayMultisample:
            case FShaderLayoutItemType::Texture3D:
            case FShaderLayoutItemType::TextureCube:
            case FShaderLayoutItemType::TextureCubeArray:
                COPLT_THROW("Buffer Srv does not support Texture");
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Buffer Srv does not support Sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                COPLT_THROW("Upload Buffer does not support RayTracingAccelerationStructure");
                break;
            }
            device->CreateShaderResourceView(resource.Get(), &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Uav:
        COPLT_THROW("The upload buffer cannot be used for unordered access");
    case FShaderLayoutItemView::Sampler:
        COPLT_THROW("Buffer cannot be used as a sampler");
    default:
        std::unreachable();
    }
}

void View::CreateImageDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) const
{
    const NonNull image = TryGetImage();
    const auto resource = image->GetResourcePtr();
    const auto data = image->GpuImageData();
    switch (def.View)
    {
    case FShaderLayoutItemView::Cbv:
        COPLT_THROW("Image cannot be used as a Cbv");
    case FShaderLayoutItemView::Srv:
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
            desc.Format = ToDx(m_data.Image.Format == FGraphicsFormat::Unknown ? data->m_format : m_data.Image.Format);
            desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Srv does not support ConstantBuffer");
            case FShaderLayoutItemType::Buffer:
            case FShaderLayoutItemType::RawBuffer:
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                COPLT_THROW("Image Srv does not support Buffer");
            case FShaderLayoutItemType::Texture1D:
                {
                    if (data->m_dimension != FImageDimension::One)
                        COPLT_THROW("Bind required Texture1D, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
                    desc.Texture1D.MostDetailedMip = m_data.Image.Mip;
                    desc.Texture1D.MipLevels = m_data.Image.NumMips;
                    desc.Texture1D.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::Texture1DArray:
                {
                    if (data->m_dimension != FImageDimension::One)
                        COPLT_THROW("Bind required Texture1DArray, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                    desc.Texture1DArray.MostDetailedMip = m_data.Image.Mip;
                    desc.Texture1DArray.MipLevels = m_data.Image.NumMips;
                    desc.Texture1DArray.FirstArraySlice = m_data.Image.Index;
                    desc.Texture1DArray.ArraySize = m_data.Image.Size;
                    desc.Texture1DArray.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::Texture2D:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2D, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                    desc.Texture2D.MostDetailedMip = m_data.Image.Mip;
                    desc.Texture2D.MipLevels = m_data.Image.NumMips;
                    desc.Texture2D.PlaneSlice = m_data.Image.Plane;
                    desc.Texture2D.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::Texture2DArray:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2DArray, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                    desc.Texture2DArray.MostDetailedMip = m_data.Image.Mip;
                    desc.Texture2DArray.MipLevels = m_data.Image.NumMips;
                    desc.Texture2DArray.FirstArraySlice = m_data.Image.Index;
                    desc.Texture2DArray.ArraySize = m_data.Image.Size;
                    desc.Texture2DArray.PlaneSlice = m_data.Image.Plane;
                    desc.Texture2DArray.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::Texture2DMultisample:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2DMultisample, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                    break;
                }
            case FShaderLayoutItemType::Texture2DArrayMultisample:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2DArrayMultisample, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
                    desc.Texture2DMSArray.FirstArraySlice = m_data.Image.Index;
                    desc.Texture2DMSArray.ArraySize = m_data.Image.Size;
                    break;
                }
            case FShaderLayoutItemType::Texture3D:
                {
                    if (data->m_dimension != FImageDimension::Three)
                        COPLT_THROW("Bind required Texture3D, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
                    desc.Texture3D.MostDetailedMip = m_data.Image.Mip;
                    desc.Texture3D.MipLevels = m_data.Image.NumMips;
                    desc.Texture3D.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::TextureCube:
                {
                    if (data->m_dimension != FImageDimension::Cube)
                        COPLT_THROW("Bind required TextureCube, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
                    desc.TextureCube.MostDetailedMip = m_data.Image.Mip;
                    desc.TextureCube.MipLevels = m_data.Image.NumMips;
                    desc.TextureCube.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::TextureCubeArray:
                {
                    if (data->m_dimension != FImageDimension::Cube)
                        COPLT_THROW("Bind required TextureCubeArray, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
                    desc.TextureCubeArray.MostDetailedMip = m_data.Image.Mip;
                    desc.TextureCubeArray.MipLevels = m_data.Image.NumMips;
                    desc.TextureCubeArray.First2DArrayFace = m_data.Image.Index;
                    desc.TextureCubeArray.NumCubes = m_data.Image.Size;
                    desc.TextureCubeArray.ResourceMinLODClamp = 0;
                    break;
                }
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Srv does not support sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                COPLT_THROW("Image Srv does not support RayTracingAccelerationStructure");
            }
            device->CreateShaderResourceView(resource, &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Uav:
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
            desc.Format = ToDx(m_data.Image.Format == FGraphicsFormat::Unknown ? data->m_format : m_data.Image.Format);
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Uav not support ConstantBuffer");

            case FShaderLayoutItemType::Buffer:
            case FShaderLayoutItemType::RawBuffer:
            case FShaderLayoutItemType::StructureBuffer:
            case FShaderLayoutItemType::StructureBufferWithCounter:
                COPLT_THROW("Image Srv does not support Buffer");
            case FShaderLayoutItemType::Texture1D:
                {
                    if (data->m_dimension != FImageDimension::One)
                        COPLT_THROW("Bind required Texture1D, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
                    desc.Texture1D.MipSlice = m_data.Image.Mip;
                    break;
                }
            case FShaderLayoutItemType::Texture1DArray:
                {
                    if (data->m_dimension != FImageDimension::One)
                        COPLT_THROW("Bind required Texture1DArray, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
                    desc.Texture1DArray.MipSlice = m_data.Image.Mip;
                    desc.Texture1DArray.FirstArraySlice = m_data.Image.Index;
                    desc.Texture1DArray.ArraySize = m_data.Image.Size;
                    break;
                }
            case FShaderLayoutItemType::Texture2D:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2D, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                    desc.Texture2D.MipSlice = m_data.Image.Mip;
                    desc.Texture2D.PlaneSlice = m_data.Image.Plane;
                    break;
                }
            case FShaderLayoutItemType::Texture2DArray:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2DArray, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
                    desc.Texture2DArray.MipSlice = m_data.Image.Mip;
                    desc.Texture2DArray.FirstArraySlice = m_data.Image.Index;
                    desc.Texture2DArray.ArraySize = m_data.Image.Size;
                    desc.Texture2DArray.PlaneSlice = m_data.Image.Plane;
                    break;
                }
            case FShaderLayoutItemType::Texture2DMultisample:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2DMultisample, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMS;
                    break;
                }
            case FShaderLayoutItemType::Texture2DArrayMultisample:
                {
                    if (data->m_dimension != FImageDimension::Two)
                        COPLT_THROW("Bind required Texture2DArrayMultisample, but the actual resources provided are not");
                    desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DMSARRAY;
                    desc.Texture2DMSArray.FirstArraySlice = m_data.Image.Index;
                    desc.Texture2DMSArray.ArraySize = m_data.Image.Size;
                    break;
                }
            case FShaderLayoutItemType::Texture3D:
                {
                    if (data->m_dimension != FImageDimension::Three)
                        COPLT_THROW("Bind required Texture3D, but the actual resources provided are not");
                    desc.Texture3D.MipSlice = m_data.Image.Mip;
                    desc.Texture3D.FirstWSlice = m_data.Image.Z;
                    desc.Texture3D.WSize = m_data.Image.Depth;
                    break;
                }
            case FShaderLayoutItemType::TextureCube:
                COPLT_THROW("Uav not support TextureCube");
            case FShaderLayoutItemType::TextureCubeArray:
                COPLT_THROW("Uav not support TextureCubeArray");
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Uav not support Sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                COPLT_THROW("Uav not support RayTracingAccelerationStructure");
            }
            device->CreateUnorderedAccessView(resource, nullptr, &desc, handle);
            break;
        }
    case FShaderLayoutItemView::Sampler:
        COPLT_THROW("Image cannot be used as a sampler");
    default:
        std::unreachable();
    }
}

void View::CreateSamplerDescriptor(NonNull<ID3D12Device2> device, const FBindGroupItem& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle) const
{
    const NonNull sampler = TryGetSampler();
    switch (def.View)
    {
    case FShaderLayoutItemView::Cbv:
        COPLT_THROW("Sampler cannot be used as a Cbv");
    case FShaderLayoutItemView::Srv:
        COPLT_THROW("Sampler cannot be used as a Srv");
    case FShaderLayoutItemView::Uav:
        COPLT_THROW("Sampler cannot be used as a Uav");
    case FShaderLayoutItemView::Sampler:
        device->CreateSampler(sampler->Desc(), handle);
        break;
    default:
        std::unreachable();
    }
}
