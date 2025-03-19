#pragma once

#include "GpuObject.h"
#include "Isolate.h"

namespace Coplt
{
    struct FGpuRecordData;

    COPLT_INTERFACE_DEFINE(FGpuRecord, "0ef83584-ca65-44de-b38a-648bfb3a85a6", FGpuObject)
    {
        virtual FGpuRecordData* GpuFGpuRecordData() noexcept = 0;
    };

    // 里面的 FList<T*> 的 T* 生命周期由 c# 部分保证
    struct FGpuRecordData
    {
        FRecordContext* Context{};
        FList<FGpuOutput*> Outputs;

        #ifdef FFI_SRC
        explicit FGpuRecordData(FAllocator* allocator)
            : Outputs(allocator)
        {
        }
        #endif
    };

    struct FGpuRecordCreateResult
    {
        FGpuRecord* Record{};
        FGpuRecordData* Data{};
    };
}
