#include "Sampler.h"

#include "../Include/PipelineState.h"
#include "../Include/Sampler.h"

using namespace Coplt;

D3d12GpuSampler::D3d12GpuSampler(const FGpuSamplerCreateOptions& options) : m_info(options.Info)
{
    SetDesc(m_info, m_desc);
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
