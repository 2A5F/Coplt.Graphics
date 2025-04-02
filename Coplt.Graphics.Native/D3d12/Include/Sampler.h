#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/Sampler.h"
#include "../Include/PipelineState.h"

namespace Coplt
{
    inline D3D12_TEXTURE_ADDRESS_MODE ToDx(const FAddressMode value)
    {
        switch (value)
        {
        case FAddressMode::Repeat:
            return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        case FAddressMode::Mirror:
            return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
        case FAddressMode::Clamp:
            return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        case FAddressMode::Border:
            return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
        case FAddressMode::MirrorOnce:
            return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
        }
        return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }

    inline D3D12_FILTER operator |(const D3D12_FILTER a, const D3D12_FILTER b)
    {
        return static_cast<D3D12_FILTER>(static_cast<u32>(a) | static_cast<u32>(b));
    }

    inline D3D12_FILTER& operator |=(D3D12_FILTER& a, const D3D12_FILTER b)
    {
        a = a | b;
        return a;
    }

    inline void SetBorderColor(const FSamplerInfo& info, D3D12_SAMPLER_DESC& desc)
    {
        desc.BorderColor[0] = info.BorderColor[0];
        desc.BorderColor[1] = info.BorderColor[1];
        desc.BorderColor[2] = info.BorderColor[2];
        desc.BorderColor[3] = info.BorderColor[3];
    }

    inline void SetBorderColor(const FStaticSamplerInfo& info, D3D12_STATIC_SAMPLER_DESC& desc)
    {
        switch (info.BorderColor)
        {
        case FStaticSamplerBorderColor::Transparent:
            desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
            return;
        case FStaticSamplerBorderColor::Black:
            desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
            return;
        case FStaticSamplerBorderColor::White:
            desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
            return;
        case FStaticSamplerBorderColor::BlackUInt:
            desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK_UINT;
            return;
        case FStaticSamplerBorderColor::WhiteUInt:
            desc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE_UINT;
            return;
        }
        desc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    }

    void SetDesc(const auto& info, auto& desc)
    {
        desc.MaxLOD = info.MaxLod;
        desc.MinLOD = info.MinLod;
        SetBorderColor(info, desc);
        desc.ComparisonFunc = ToDx(info.Cmp);
        desc.MaxAnisotropy = info.MaxAnisotropy;
        desc.MipLODBias = info.MipLodBias;
        desc.AddressW = ToDx(info.W);
        desc.AddressV = ToDx(info.V);
        desc.AddressU = ToDx(info.U);
        desc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
        if (desc.MaxAnisotropy > 0)
        {
            if (info.Cmp != FCmpFunc::Off) desc.Filter = D3D12_FILTER_COMPARISON_ANISOTROPIC;
            else if (info.Mipmap == FFilter::Linear) desc.Filter = D3D12_FILTER_ANISOTROPIC;
            else desc.Filter = D3D12_FILTER_MIN_MAG_ANISOTROPIC_MIP_POINT;
        }
        else
        {
            if (info.Mipmap == FFilter::Linear) desc.Filter |= D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
            if (info.Mag == FFilter::Linear) desc.Filter |= D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
            if (info.Min == FFilter::Linear) desc.Filter |= D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
            if (info.Cmp != FCmpFunc::Off) desc.Filter |= D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
        }
    }
}
