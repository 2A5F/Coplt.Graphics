#pragma once

#include "Object.h"
#include "String.h"

namespace Coplt
{
    enum class FD3dFeatureLevel : i32
    {
        Unset = 0,
        _12_1 = 0xc100,
        _12_2 = 0xc200,
    };

    enum class FGpuPreference : u8
    {
        HighPerformance = 0,
        MinimumPower = 1,
        Any = 255,
    };

    struct FGpuDeviceCreateOptions
    {
        // 可选
        Str8or16 Name{};
        // 仅 dx 后端时可用
        FD3dFeatureLevel D3dFeatureLevel{};
        FGpuPreference Preference{};
        b8 Debug{};
    };

    COPLT_INTERFACE_DEFINE(FGpuDevice, "557f032d-ed50-403a-adc5-214fddbe6c6b", FUnknown)
    {

    };
}
