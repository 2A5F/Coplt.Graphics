#pragma once

#include "Context.h"
#include "Isolate.h"
#include "../../Api/FFI/Record.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuRecord, "57a9c7f9-1ec0-4d78-89b9-e547667c50b3", FGpuRecord)
    {
        std::vector<Rc<FUnknown>> m_object_owner{};
        bool m_ended{};

        virtual FGpuRecordData* Data() noexcept = 0;
        virtual const FGpuRecordData* Data() const noexcept = 0;

        virtual void Recycle() = 0;

        virtual void EnsureEnd() = 0;
    };

    struct D3d12GpuRecord final : GpuObject<D3d12GpuRecord, ID3d12GpuRecord>, FGpuRecordData
    {
        u64 m_isolate_id{};
        u64 m_record_id{};
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12RecordContext> m_context{};

        explicit D3d12GpuRecord(NonNull<D3d12GpuIsolate> isolate);

        FResult SetName(const FStr8or16& name) noexcept override;
        FGpuRecordData* GpuFGpuRecordData() noexcept override;
        FGpuRecordData* Data() noexcept override;
        const FGpuRecordData* Data() const noexcept override;

        void Recycle() override;

        FResult End() noexcept override;
        void EnsureEnd() override;
        void DoEnd();
    };
}
