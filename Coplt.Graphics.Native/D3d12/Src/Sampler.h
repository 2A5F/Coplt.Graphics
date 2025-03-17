#pragma once

#include <directx/d3d12.h>

#include "../../Api/Include/GpuObject.h"
#include "../FFI/Sampler.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuSampler, "0d342b3b-5960-4e5b-97c0-4028feab682e", FD3d12GpuSampler)
    {
        virtual NonNull<const D3D12_SAMPLER_DESC> Desc() const noexcept = 0;
    };

    struct D3d12GpuSampler final : GpuObject<D3d12GpuSampler, ID3d12GpuSampler>
    {
        FSamplerInfo m_info;
        D3D12_SAMPLER_DESC m_desc{};

        explicit D3d12GpuSampler(const FGpuSamplerCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;
        const FSamplerInfo* Info() const noexcept override;
        NonNull<const D3D12_SAMPLER_DESC> Desc() const noexcept override;
    };
}
