#pragma once

#include "Common.h"
#include "GpuObject.h"
#include "PipelineState.h"

namespace Coplt
{
    enum class FFilter : u8
    {
        Point,
        Linear,
    };

    enum class FAddressMode : u8
    {
        Repeat,
        Mirror,
        Clamp,
        Border,
        MirrorOnce,
    };

    struct FSamplerInfo
    {
        f32 BorderColor[4]{};
        u32 MaxAnisotropy{0};
        f32 MipLodBias{};
        f32 MinLod{};
        f32 MaxLod{3.402823466E38};
        FCmpFunc Cmp{};
        FFilter Mag{};
        FFilter Min{};
        FFilter Mipmap{};
        FAddressMode U{};
        FAddressMode V{};
        FAddressMode W{};
    };

    struct FGpuSamplerCreateOptions
    {
        FStr8or16 Name{};
        FSamplerInfo Info{};
    };

    COPLT_INTERFACE_DEFINE(FGpuSampler, "16a5b373-ad9e-4033-89fd-6a5b4aabaef2", FGpuObject)
    {
        virtual const FSamplerInfo* Info() const noexcept = 0;
    };
}
