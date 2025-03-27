#include "RecordStorage.h"

using namespace Coplt;

D3d12RecordStorage::D3d12RecordStorage(NonNull<D3d12GpuIsolate> isolate, const Rc<D3d12RecordContext>& context)
    : m_device(isolate->m_device), m_context(context)
{
}

void D3d12RecordStorage::Clear()
{
    m_result_lists.clear();
}

void D3d12RecordStorage::StartRecord(const FGpuRecordMode Mode)
{
    m_list_type = ToListType(Mode);
    Split();
    Split();
}

void D3d12RecordStorage::EndRecord()
{
}

void D3d12RecordStorage::BeforeSubmit()
{
    for (auto& list : m_result_lists)
    {
        list.Close();
    }
}

void D3d12RecordStorage::AfterSubmit()
{
}

u32 D3d12RecordStorage::Split()
{
    const auto i = m_result_lists.size();
    auto cmd_allocator = m_context->m_cmd_alloc_pool->RentCommandAllocator(m_list_type);
    m_result_lists.push_back(cmd_allocator.RentCommandList());
    m_context->m_recycled_command_allocators.push_back(std::move(cmd_allocator));
    return i;
}

D3d12RentedCommandList& D3d12RecordStorage::CurList()
{
    return m_result_lists.back();
}

u32 D3d12RecordStorage::CurListIndex()
{
    return m_result_lists.size() - 1;
}

std::span<D3d12RentedCommandList> D3d12RecordStorage::Lists()
{
    return m_result_lists;
}

void D3d12RecordStorage::DropLast()
{
    CurList().Close();
    m_result_lists.pop_back();
}

D3D12_COMMAND_LIST_TYPE Coplt::ToListType(const FGpuRecordMode value)
{
    switch (value)
    {
    case FGpuRecordMode::Direct:
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    case FGpuRecordMode::Compute:
        return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    case FGpuRecordMode::Copy:
        return D3D12_COMMAND_LIST_TYPE_COPY;
    }
    return D3D12_COMMAND_LIST_TYPE_DIRECT;
}
