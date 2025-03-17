#include "Sampler.h"

#include "../Include/PipelineState.h"
#include "../Include/Sampler.h"

using namespace Coplt;

D3d12GpuSampler::D3d12GpuSampler(const FGpuSamplerCreateOptions& options) : m_info(options.Info)
{
    m_desc.MaxLOD = m_info.MaxLod;
    m_desc.MinLOD = m_info.MinLod;
    m_desc.BorderColor[0] = m_info.BorderColor[0];
    m_desc.BorderColor[1] = m_info.BorderColor[1];
    m_desc.BorderColor[2] = m_info.BorderColor[2];
    m_desc.BorderColor[3] = m_info.BorderColor[3];
    m_desc.ComparisonFunc = ToDx(m_info.Cmp);
    m_desc.MaxAnisotropy = m_info.MaxAnisotropy;
    m_desc.MipLODBias = m_info.MipLodBias;
    m_desc.AddressW = ToDx(m_info.W);
    m_desc.AddressV = ToDx(m_info.V);
    m_desc.AddressU = ToDx(m_info.U);
    m_desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    if (m_desc.MaxAnisotropy > 0)
    {
        if (m_info.Cmp != FCmpFunc::Off) m_desc.Filter = D3D12_FILTER_COMPARISON_ANISOTROPIC;
        else if (m_info.Mipmap == FFilter::Linear) m_desc.Filter = D3D12_FILTER_ANISOTROPIC;
        else m_desc.Filter = D3D12_FILTER_MIN_MAG_ANISOTROPIC_MIP_POINT;
    }
    else
    {
        if (m_info.Mipmap == FFilter::Linear) m_desc.Filter |= D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
        if (m_info.Mag == FFilter::Linear) m_desc.Filter |= D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        if (m_info.Min == FFilter::Linear) m_desc.Filter |= D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        if (m_info.Cmp != FCmpFunc::Off) m_desc.Filter |= D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
    }
}

FResult D3d12GpuSampler::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

const FSamplerInfo* D3d12GpuSampler::Info() const noexcept
{
    return &m_info;
}

NonNull<const D3D12_SAMPLER_DESC> D3d12GpuSampler::Desc() const noexcept
{
    return &m_desc;
}
