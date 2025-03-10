#pragma once

#include "Object.h"
#include "Result.h"
#include "GraphicsFormat.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FGpuObject, "9fc6890b-e9a2-4f4a-9217-a14a4223a715", FUnknown)
    {
        virtual FResult SetName(const FStr8or16& name) noexcept = 0;
    };
}
