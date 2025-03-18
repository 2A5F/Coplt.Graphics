#include "Record.h"

using namespace Coplt;

D3d12GpuRecord::D3d12GpuRecord(NonNull<D3d12GpuIsolate> isolate, const FGpuRecordCreateOptions& options)
    : FGpuRecordData(isolate->m_device->m_instance->m_allocator.get()), m_device(isolate->m_device)
{
}

FResult D3d12GpuRecord::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FGpuRecordData* D3d12GpuRecord::GpuFGpuRecordData() noexcept
{
    return this;
}

FGpuRecordData* D3d12GpuRecord::Data() noexcept
{
    return this;
}

const FGpuRecordData* D3d12GpuRecord::Data() const noexcept
{
    return this;
}
