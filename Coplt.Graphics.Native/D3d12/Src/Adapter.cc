#include "Adapter.h"

#include "../../Api/Include/Error.h"

using namespace Coplt;

D3d12GpuAdapter::D3d12GpuAdapter(
    Rc<D3d12Instance>&& instance, ComPtr<IDXGIAdapter1>&& adapter
): m_instance(std::move(instance)), m_adapter(std::move(adapter))
{
    m_backend = FBackend::Dx12;

    ComPtr<ID3D12Device> device{};
    chr | D3D12CreateDevice(m_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

    chr | m_feature_support.Init(device.Get());

    m_d3d_feature_level = static_cast<FD3dFeatureLevel>(m_feature_support.MaxSupportedFeatureLevel());

    DXGI_ADAPTER_DESC1 desc1{};
    chr | m_adapter->GetDesc1(&desc1);
    m_name_string = std::wstring(desc1.Description);
    m_name = FStr8or16(m_name_string);
    m_vendor_id = desc1.VendorId;
    m_device_id = desc1.DeviceId;

    if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
    {
        m_device_type = FDeviceType::Cpu;
    }
    else if (m_feature_support.UMA())
    {
        m_device_type = FDeviceType::IntegratedGpu;
    }
    else
    {
        m_device_type = FDeviceType::DiscreteGpu;
    }

    LARGE_INTEGER li{};
    if (SUCCEEDED(m_adapter->CheckInterfaceSupport(__uuidof(IDXGIDevice), &li)))
    {
        const auto i = li.QuadPart;
        m_driver_string = fmt::format(L"{}.{}.{}.{}", i >> 48, (i >> 32) & 0xFFFF, (i >> 16) & 0xFFFF, i & 0xFFFF);
        m_driver = FStr8or16(m_driver_string);
    }

    m_features.DescriptorHeap = true;
    m_features.ArrBindless = true;
    m_features.ExtraIsolate = true;

    m_features.ShaderModelLevel = static_cast<FShaderModelLevel>(m_feature_support.HighestShaderModel());
    if (m_features.ShaderModelLevel >= FShaderModelLevel::SM_6_6 && m_feature_support.ResourceBindingTier() >= D3D12_RESOURCE_BINDING_TIER_3)
    {
        m_features.DynBindless = true;
    }

    if (m_feature_support.RaytracingTier() >= D3D12_RAYTRACING_TIER_1_0) m_features.RayTracing = true;
    if (m_feature_support.MeshShaderTier() >= D3D12_MESH_SHADER_TIER_1) m_features.MeshShader = true;
    if (m_feature_support.EnhancedBarriersSupported()) m_features.EnhancedBarriers = true;
}

FResult D3d12GpuAdapter::CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDeviceCreateResult* out) noexcept
{
    return feb([&]
    {
        out->Device = new D3d12GpuDevice(this->CloneThis(), options, *out);
    });
}

b8 D3d12GpuAdapter::IsSupport(const FDeviceRequires& Requires) noexcept
{
    switch (Requires.DeviceType)
    {
    case FDeviceTypeRequire::Any:
        break;
    case FDeviceTypeRequire::Gpu:
    case FDeviceTypeRequire::VirtualGpu:
        if (m_device_type != FDeviceType::DiscreteGpu && m_device_type != FDeviceType::IntegratedGpu) return false;
    case FDeviceTypeRequire::IntegratedGpu:
        if (m_device_type != FDeviceType::IntegratedGpu) return false;
        break;
    case FDeviceTypeRequire::DiscreteGpu:
        if (m_device_type != FDeviceType::DiscreteGpu) return false;
        break;
    case FDeviceTypeRequire::Cpu:
        if (m_device_type != FDeviceType::Cpu) return false;
        break;
    default:
        return false;
    }
    if (Requires.D3dFeatureLevel > m_d3d_feature_level)
        return false;
    if (Requires.FeatureRequires.ShaderModelLevel > m_features.ShaderModelLevel)
        return false;
    if (Requires.FeatureRequires.RayTracing && !m_features.RayTracing)
        return false;
    if (Requires.FeatureRequires.MeshShader && !m_features.MeshShader)
        return false;
    if (Requires.FeatureRequires.DescriptorHeap && !m_features.DescriptorHeap)
        return false;
    if (Requires.FeatureRequires.EnhancedBarriers && !m_features.EnhancedBarriers)
        return false;
    if (Requires.FeatureRequires.ArrBindless && !m_features.ArrBindless)
        return false;
    if (Requires.FeatureRequires.DynBindless && !m_features.DynBindless)
        return false;
    return true;
}
