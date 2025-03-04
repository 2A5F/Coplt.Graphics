#pragma once

#include "GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FGpuExecutor, "edab0989-e0c2-4b1e-826f-c73380088fc6", FGpuObject)
    {
        // 等待可重用，在提交后必须先等待才能再次提交
        virtual FResult Wait() noexcept = 0;
    };
}
