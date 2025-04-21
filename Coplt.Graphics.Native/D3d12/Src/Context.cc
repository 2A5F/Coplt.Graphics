#include "Context.h"

using namespace Coplt;

D3d12RecordContext::D3d12RecordContext(NonNull<D3d12GpuIsolate> isolate)
{
    m_device = isolate->m_device;
    m_descriptor_manager = DescriptorManager(m_device.get());
    m_cmd_alloc_pool = isolate->m_cmd_alloc_pool;
    m_upload_buffer = FList<FUploadBufferBlock>(m_device->m_instance->m_allocator.get());
}

FResult D3d12RecordContext::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FResult D3d12RecordContext::GrowUploadBuffer(u64 min_required_size) noexcept
{
    return feb([&]
    {
        const auto size =
            std::max(
                InitUploadBufferSize,
                std::max(m_cur_upload_buffer_size * 2, std::bit_ceil(min_required_size))
            );
        m_cur_upload_buffer_index = m_upload_buffers.size();
        m_cur_upload_buffer_size = size;
        m_upload_buffers.push_back(
            BufferPack(m_device.get(), m_device->m_gpu_allocator.Get(), FCpuAccess::Write, m_cur_upload_buffer_size)
        );
        const auto& upload_buffer = m_upload_buffers.back();
        void* mapped_ptr{};
        if (m_device->Debug())
        {
            const auto name = fmt::format(
                L"Frame Context ({}) Upload Buffer ({})", m_object_id, upload_buffer.m_size
            );
            chr | upload_buffer.m_resource.m_resource >> SetNameEx(FStr8or16(name));
        }
        constexpr D3D12_RANGE read_range{.Begin = 0, .End = 0};
        chr | upload_buffer.m_resource.m_resource->Map(0, &read_range, &mapped_ptr);
        m_upload_buffer.Add(FUploadBufferBlock{
            .mapped_ptr = static_cast<volatile u8*>(mapped_ptr),
            .cur_offset = 0,
            .size = upload_buffer.m_size,
        });
    });
}

void D3d12RecordContext::Recycle()
{
    m_recycled_command_allocators.clear();

    if (!m_upload_buffers.empty())
    {
        {
            BufferPack last = std::move(m_upload_buffers.back());
            m_upload_buffers.clear();
            m_upload_buffers.push_back(std::move(last));
            m_cur_upload_buffer_index = 0;
        }
        {
            auto last = m_upload_buffer[m_upload_buffer.size() - 1];
            last.cur_offset = 0;
            m_upload_buffer.Clear();
            m_upload_buffer.Add(last);
        }
    }
}

D3d12CommandPool::D3d12CommandPool(const Rc<D3d12GpuDevice>& device, const D3D12_COMMAND_LIST_TYPE type)
    : m_device(device), m_type(type)
{
    m_command_allocator_pool = box<CommandAllocatorConcurrentQueue>();
    m_command_list_pool = box<CommandListConcurrentQueue>();
}

D3d12RentedCommandAllocator D3d12CommandPool::RentCommandAllocator()
{
    ComPtr<ID3D12CommandAllocator> allocator{};
    if (!m_command_allocator_pool->try_dequeue(allocator))
    {
        chr | m_device->m_device->CreateCommandAllocator(m_type, IID_PPV_ARGS(&allocator));
    }
    return D3d12RentedCommandAllocator(this->CloneThis(), std::move(allocator));
}

void D3d12CommandPool::ReturnCommandAllocator(ComPtr<ID3D12CommandAllocator>&& allocator) const
{
    chr | allocator->Reset();
    m_command_allocator_pool->enqueue(std::move(allocator));
}

void D3d12CommandPool::ReturnCommandList(Rc<D3d12CommandList>&& list) const
{
    m_command_list_pool->enqueue(std::move(list));
}

D3d12CommandListPoolCluster::D3d12CommandListPoolCluster(const Rc<D3d12GpuDevice>& device)
    : m_device(device)
{
    m_direct = new D3d12CommandPool(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
    m_compute = new D3d12CommandPool(m_device, D3D12_COMMAND_LIST_TYPE_COMPUTE);
    m_copy = new D3d12CommandPool(m_device, D3D12_COMMAND_LIST_TYPE_COPY);
    m_video_decode = new D3d12CommandPool(m_device, D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE);
    m_video_process = new D3d12CommandPool(m_device, D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS);
    m_video_encode = new D3d12CommandPool(m_device, D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE);
}

D3d12RentedCommandAllocator D3d12CommandListPoolCluster::RentCommandAllocator(const D3D12_COMMAND_LIST_TYPE type) const
{
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
        return m_direct->RentCommandAllocator();
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
        return m_compute->RentCommandAllocator();
    case D3D12_COMMAND_LIST_TYPE_COPY:
        return m_copy->RentCommandAllocator();
    case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
        return m_video_decode->RentCommandAllocator();
    case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
        return m_video_process->RentCommandAllocator();
    case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
        return m_video_encode->RentCommandAllocator();
    case D3D12_COMMAND_LIST_TYPE_BUNDLE:
    case D3D12_COMMAND_LIST_TYPE_NONE:
        break;
    }
    COPLT_THROW_FMT("Unknown or unsupported command list type {}", static_cast<u32>(type));
}

D3d12CommandList::D3d12CommandList(const D3d12RentedCommandAllocator& allocator)
{
    type = allocator.m_pool->m_type;
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
    case D3D12_COMMAND_LIST_TYPE_COPY:
        {
            chr | allocator.m_pool->m_device->m_device->CreateCommandList(
                0, type, allocator.m_allocator.Get(), nullptr, IID_PPV_ARGS(&g0)
            );
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList10>(&g10))) goto _9;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList9>(&g9))) goto _8;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList8>(&g8))) goto _7;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList7>(&g7))) goto _6;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList6>(&g6))) goto _5;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList5>(&g5))) goto _4;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList4>(&g4))) goto _3;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList3>(&g3))) goto _2;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList2>(&g2))) goto _1;
            if (SUCCEEDED(g0->QueryInterface<ID3D12GraphicsCommandList1>(&g1))) goto _0;
            return;
        _9:
            g9 = g10;
        _8:
            g8 = g9;
        _7:
            g7 = g8;
        _6:
            g6 = g7;
        _5:
            g5 = g6;
        _4:
            g4 = g5;
        _3:
            g3 = g4;
        _2:
            g2 = g3;
        _1:
            g1 = g2;
        _0:
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
        COPLT_THROW("TODO");
    case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
        COPLT_THROW("TODO");
    case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
        COPLT_THROW("TODO");
    case D3D12_COMMAND_LIST_TYPE_NONE:
    case D3D12_COMMAND_LIST_TYPE_BUNDLE:
        std::unreachable();
    }
    std::unreachable();
}

void D3d12CommandList::Reset(const D3d12RentedCommandAllocator& allocator) const
{
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
    case D3D12_COMMAND_LIST_TYPE_COPY:
        {
            chr | g0->Reset(allocator.m_allocator.Get(), nullptr);
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
        {
            chr | vd0->Reset(allocator.m_allocator.Get());
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
        {
            chr | vp0->Reset(allocator.m_allocator.Get());
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
        {
            chr | ve0->Reset(allocator.m_allocator.Get());
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_NONE:
    case D3D12_COMMAND_LIST_TYPE_BUNDLE:
        std::unreachable();
    }
    std::unreachable();
}

void D3d12CommandList::Close() const
{
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
    case D3D12_COMMAND_LIST_TYPE_COPY:
        {
            chr | g0->Close();
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
        {
            chr | vd0->Close();
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
        {
            chr | vp0->Close();
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
        {
            chr | ve0->Close();
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_NONE:
    case D3D12_COMMAND_LIST_TYPE_BUNDLE:
        std::unreachable();
    }
    std::unreachable();
}

ID3D12CommandList* D3d12CommandList::ToCommandList() const
{
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
    case D3D12_COMMAND_LIST_TYPE_COPY:
        {
            return g0.Get();
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
        {
            return vd0.Get();
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
        {
            return vp0.Get();
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
        {
            return ve0.Get();
        }
    case D3D12_COMMAND_LIST_TYPE_NONE:
    case D3D12_COMMAND_LIST_TYPE_BUNDLE:
        std::unreachable();
    }
    std::unreachable();
}

void D3d12CommandList::Barrier(const UINT32 NumBarrierGroups, const D3D12_BARRIER_GROUP* pBarrierGroups) const
{
    switch (type)
    {
    case D3D12_COMMAND_LIST_TYPE_DIRECT:
    case D3D12_COMMAND_LIST_TYPE_COMPUTE:
    case D3D12_COMMAND_LIST_TYPE_COPY:
        {
            if (g7 == nullptr)
                COPLT_THROW("Does not support enhanced barriers");
            g7->Barrier(NumBarrierGroups, pBarrierGroups);
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE:
        {
            if (vd3 == nullptr)
                COPLT_THROW("Does not support enhanced barriers");
            vd3->Barrier(NumBarrierGroups, pBarrierGroups);
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_PROCESS:
        {
            if (vp3 == nullptr)
                COPLT_THROW("Does not support enhanced barriers");
            vp3->Barrier(NumBarrierGroups, pBarrierGroups);
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE:
        {
            if (ve3 == nullptr)
                COPLT_THROW("Does not support enhanced barriers");
            ve3->Barrier(NumBarrierGroups, pBarrierGroups);
            return;
        }
    case D3D12_COMMAND_LIST_TYPE_NONE:
    case D3D12_COMMAND_LIST_TYPE_BUNDLE:
        std::unreachable();
    }
    std::unreachable();
}

D3d12RentedCommandList::D3d12RentedCommandList(const Rc<D3d12CommandPool>& pool, Rc<D3d12CommandList>&& list)
    : m_pool(pool), m_list(std::move(list))
{
}

D3d12RentedCommandList::~D3d12RentedCommandList() noexcept(false)
{
    if (!m_pool || !m_list) return;
    if (std::uncaught_exceptions())
        return;
    m_pool->ReturnCommandList(std::move(m_list));
}

D3d12CommandList* D3d12RentedCommandList::operator->() const noexcept
{
    return m_list.get();
}

D3d12CommandList& D3d12RentedCommandList::operator*() const noexcept
{
    return *m_list.get();
}

D3d12RentedCommandList::operator bool() const noexcept
{
    return static_cast<bool>(m_pool);
}

void D3d12RentedCommandList::Close()
{
    try
    {
        m_list->Close();
    }
    catch (...)
    {
        m_pool = {};
        m_list = {};
        std::rethrow_exception(std::current_exception());
    }
}

D3d12RentedCommandAllocator::D3d12RentedCommandAllocator(const Rc<D3d12CommandPool>& pool, ComPtr<ID3D12CommandAllocator>&& allocator)
    : m_pool(pool), m_allocator(std::move(allocator))
{
}

D3d12RentedCommandAllocator::~D3d12RentedCommandAllocator() noexcept(false)
{
    if (!m_pool || !m_allocator) return;
    if (std::uncaught_exceptions())
        return;
    m_pool->ReturnCommandAllocator(std::move(m_allocator));
}

D3d12RentedCommandList D3d12RentedCommandAllocator::RentCommandList() const
{
    Rc<D3d12CommandList> list{};
    if (m_pool->m_command_list_pool->try_dequeue(list))
    {
        list->Reset(*this);
    }
    else
    {
        list = new D3d12CommandList(*this);
    }
    return D3d12RentedCommandList(m_pool, std::move(list));
}

D3d12RentedCommandAllocator::operator bool() const noexcept
{
    return static_cast<bool>(m_pool);
}
