#pragma once

#include "Logger.h"
#include "Result.h"

namespace Coplt
{
    struct FGpuDeviceCreateOptions;
    struct FGpuDevice;

    COPLT_INTERFACE_DEFINE(FInstance, "cc2894ba-57c7-474a-b777-1a3e3a7c922c", FUnknown)
    {
        virtual FResult SetLogger(const FLogger& logger) noexcept = 0;

        virtual FResult CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDevice** out) noexcept = 0;
    };
}
