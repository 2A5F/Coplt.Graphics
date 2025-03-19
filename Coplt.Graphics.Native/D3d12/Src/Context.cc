#include "Context.h"

using namespace Coplt;

D3d12FrameContext::D3d12FrameContext(Rc<D3d12GpuQueue>&& queue) : m_queue(std::move(queue))
{
    m_device = m_queue->m_device;
    m_dx_device = m_queue->m_dx_device;

    m_descriptors = new DescriptorContext(m_dx_device);

    m_upload_buffer = FList<FUploadBufferBlock>(m_device->m_instance->m_allocator.get());

    chr | m_dx_device->CreateCommandAllocator(
        ToDx(m_queue->m_queue_type), IID_PPV_ARGS(&m_command_allocator)
    );
    if (m_device->Debug())
    {
        chr | m_command_allocator >> SetNameEx(FStr8or16(fmt::format(L"Frame Context ({}) Command Allocator", m_object_id)));
    }
}

FResult D3d12FrameContext::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
    });
}

FResult D3d12FrameContext::GrowUploadBuffer(const u64 min_required_size) noexcept
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

void D3d12FrameContext::Recycle()
{
    chr | m_command_allocator->Reset();
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

D3d12RecordContext::D3d12RecordContext(NonNull<D3d12GpuIsolate> isolate)
{
    m_device = isolate->m_device;
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
    m_command_allocators.clear();

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
