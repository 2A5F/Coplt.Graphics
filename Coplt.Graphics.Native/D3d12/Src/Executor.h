#pragma once

#include "../../Api/FFI/Executor.h"

namespace Coplt
{
    struct D3d12GpuQueue;

    COPLT_INTERFACE_DEFINE(ID3d12GpuExecutor, "f9b5793d-b6e9-4b7e-8953-5fa07b815010", FGpuExecutor)
    {
        virtual void Submit(D3d12GpuQueue* Queue, const FCommandSubmit* submit) = 0;
        virtual void SubmitNoWait(D3d12GpuQueue* Queue, const FCommandSubmit* submit) = 0;
    };
}
