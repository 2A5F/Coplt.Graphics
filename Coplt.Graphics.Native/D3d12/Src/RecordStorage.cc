#include "RecordStorage.h"

using namespace Coplt;

D3d12RecordStorage::D3d12RecordStorage(NonNull<D3d12GpuIsolate> isolate, const Rc<D3d12RecordContext>& context)
    : m_device(isolate->m_device), m_context(context)
{
}

void D3d12RecordStorage::Clear()
{
    m_cur_list = {};
    m_result_lists.clear();
}

void D3d12RecordStorage::StartRecord(const FGpuRecordMode Mode)
{
    m_cmd_allocator = m_context->m_cmd_alloc_pool->RentCommandAllocator(ToListType(Mode));
    m_cur_list = m_cmd_allocator.RentCommandList();
}

void D3d12RecordStorage::EndRecord()
{
    m_result_lists.push_back(std::move(m_cur_list));
    for (auto& list : m_result_lists)
    {
        list.Close();
    }
}

void D3d12RecordStorage::AfterSubmit()
{
    m_context->m_recycled_command_allocators.push_back(std::move(m_cmd_allocator));
}

void D3d12RecordStorage::Split()
{
    m_result_lists.push_back(std::exchange(m_cur_list, m_cmd_allocator.RentCommandList()));
}

D3d12RentedCommandList& D3d12RecordStorage::CurList()
{
    return m_cur_list;
}

std::span<D3d12RentedCommandList> D3d12RecordStorage::Lists()
{
    return m_result_lists;
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
