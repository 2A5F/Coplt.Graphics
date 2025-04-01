#pragma once

#include <queue>
#include <dxgi1_4.h>
#include <directx/d3dx12.h>

#include "Context.h"
#include "../Include/ResState.h"

namespace Coplt
{
    struct D3d12GpuDevice;

    COPLT_INTERFACE_DEFINE(ID3d12GpuOutput2, "59aa68d8-91bd-4032-ba51-6a05795945b6", FGpuOutput2)
    {
        virtual NonNull<LayoutState> State() = 0;
        virtual NonNull<FGpuOutputData> Data() = 0;
        virtual NonNull<FGpuImageData> ImageData() = 0;
        virtual NonNull<ID3D12Resource> GetResourcePtr() = 0;
        virtual CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv() = 0;
    };
}
