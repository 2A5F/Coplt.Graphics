#include "../Include/View.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

enum View::Type View::Type() const
{
    return m_type;
}

Rc<D3d12GpuBuffer>& View::Buffer()
{
    if (m_type != Type::Buffer)
        COPLT_THROW("null");
    return m_buffer;
}

const Rc<D3d12GpuBuffer>& View::Buffer() const
{
    if (m_type != Type::Buffer)
        COPLT_THROW("null");
    return m_buffer;
}

View::~View()
{
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch (m_type)
    {
    case Type::None:
        return;
    case Type::Buffer:
        m_buffer.~Rc();
        return;
    }
    std::unreachable();
}

// ReSharper disable once CppPossiblyUninitializedMember
View::View() : m_type(Type::None)
{
}

View::View(const View& other) : m_type(other.m_type)
{
    switch (m_type)
    {
    case Type::None:
        break;
    case Type::Buffer:
        new(std::addressof(m_buffer)) Rc(other.m_buffer);
        break;
    }
}

View::View(View&& other) noexcept : m_type(std::exchange(other.m_type, Type::None))
{
    switch (m_type)
    {
    case Type::None:
        break;
    case Type::Buffer:
        new(std::addressof(m_buffer)) Rc(std::move(other.m_buffer));
        break;
    }
}

View& View::operator=(const View& view) noexcept
{
    this->~View();
    new(this) View(view);
    return *this;
}

View& View::operator=(View&& view) noexcept
{
    this->~View();
    new(this) View(std::forward<View>(view));
    return *this;
}

View::View(const FView& view)
{
    switch (view.Type)
    {
    case FViewType::None:
        m_type = Type::None;
        break;
    case FViewType::Buffer:
        m_type = Type::Buffer;
        if (view.Buffer == nullptr) new(std::addressof(m_buffer)) Rc<D3d12GpuBuffer>();
        const auto db = view.Buffer->QueryInterface<D3d12GpuBuffer>();
        if (db == nullptr)
            COPLT_THROW("GpuBuffer from different backends.");
        new(std::addressof(m_buffer)) Rc(Rc<D3d12GpuBuffer>::UnsafeClone(db));
        break;
    }
}

View& View::operator=(const FView& view)
{
    this->~View();
    new(this) View(view);
    return *this;
}

View::View(const Rc<D3d12GpuBuffer>& buffer) : m_type(Type::Buffer)
{
    new(std::addressof(m_buffer)) Rc(buffer);
}

View::View(Rc<D3d12GpuBuffer>&& buffer) : m_type(Type::Buffer)
{
    new(std::addressof(m_buffer)) Rc(std::move(buffer));
}

View& View::operator=(const Rc<D3d12GpuBuffer>& buffer)
{
    this->~View();
    new(this) View(buffer);
    return *this;
}

View& View::operator=(Rc<D3d12GpuBuffer>&& buffer)
{
    this->~View();
    new(this) View(std::forward<Rc<D3d12GpuBuffer>>(buffer));
    return *this;
}

void View::CreateDescriptor(
    NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, const CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const FShaderLayoutGroupView type
) const
{
    switch (Type())
    {
    case Type::None:
        CreateNullDescriptor(device, def, handle, type);
        break;
    case Type::Buffer:
        CreateBufferDescriptor(device, def, handle, type);
        break;
    }
}

void View::CreateNullDescriptor(
    NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const FShaderLayoutGroupView view
)
{
    switch (view)
    {
    case FShaderLayoutGroupView::Cbv:
        {
            device->CreateConstantBufferView(nullptr, handle);
            break;
        }
    case FShaderLayoutGroupView::Srv:
        {
            D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
            desc.Format = ToDx(def.Format);
            desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Srv not support ConstantBuffer");
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
                COPLT_THROW("Srv not support sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                desc.ViewDimension = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
                break;
            }
            device->CreateShaderResourceView(nullptr, &desc, handle);
            break;
        }
    case FShaderLayoutGroupView::Uav:
        {
            D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
            desc.Format = ToDx(def.Format);
            switch (def.Type)
            {
            case FShaderLayoutItemType::ConstantBuffer:
                COPLT_THROW("Uav not support ConstantBuffer");
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
                break;
            case FShaderLayoutItemType::Texture3D:
                desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
                desc.Texture3D.MipSlice = 0;
                desc.Texture3D.FirstWSlice = 0;
                desc.Texture3D.WSize = 1;
                break;
            case FShaderLayoutItemType::TextureCube:
                COPLT_THROW("Uav not support TextureCube");
            case FShaderLayoutItemType::TextureCubeArray:
                COPLT_THROW("Uav not support TextureCubeArray");
            case FShaderLayoutItemType::Sampler:
                COPLT_THROW("Uav not support Sampler");
            case FShaderLayoutItemType::RayTracingAccelerationStructure:
                COPLT_THROW("Uav not support RayTracingAccelerationStructure");
            }
            device->CreateUnorderedAccessView(nullptr, nullptr, &desc, handle);
            break;
        }
    case FShaderLayoutGroupView::Sampler:
        {
            D3D12_SAMPLER_DESC desc{};
            desc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
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

void View::CreateBufferDescriptor(
    NonNull<ID3D12Device2> device, const FShaderLayoutItemDefine& def, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, FShaderLayoutGroupView type
) const
{
    auto buffer = Buffer().get();
    switch (type)
    {
    case FShaderLayoutGroupView::Cbv:
        {
            D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
            desc.BufferLocation = buffer->m_resource->m_resource->GetGPUVirtualAddress();
            desc.SizeInBytes = buffer->m_size;
            device->CreateConstantBufferView(&desc, handle);
            break;
        }
    case FShaderLayoutGroupView::Srv:
        // todo
        break;
    case FShaderLayoutGroupView::Uav:
        // todo
        break;
    case FShaderLayoutGroupView::Sampler:
        COPLT_THROW("Buffer cannot be used as a sampler");
    }
}
