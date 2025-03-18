#pragma once

#include "Isolate.h"
#include "../../Api/FFI/Record.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{

    COPLT_INTERFACE_DEFINE(ID3d12GpuRecord, "57a9c7f9-1ec0-4d78-89b9-e547667c50b3", FGpuRecord)
    {
        virtual FGpuRecordData* Data() noexcept = 0;
        virtual const FGpuRecordData* Data() const noexcept = 0;
    };

    struct D3d12GpuRecord final : GpuObject<D3d12GpuRecord, ID3d12GpuRecord>, FGpuRecordData
    {
        Rc<D3d12GpuDevice> m_device{};

        explicit D3d12GpuRecord(NonNull<D3d12GpuIsolate> isolate, const FGpuRecordCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuRecordData* GpuFGpuRecordData() noexcept override;
        FGpuRecordData* Data() noexcept override;
        const FGpuRecordData* Data() const noexcept override;
    };
}
