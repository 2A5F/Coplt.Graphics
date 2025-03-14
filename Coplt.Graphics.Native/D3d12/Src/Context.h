#pragma once

#include "../../Api/FFI/Context.h"
#include "../../Api/Include/Object.h"
#include "CmdListPack.h"
#include "Device.h"
#include "Queue.h"
#include "Resource.h"

namespace Coplt
{
    struct D3d12FrameContext final : GpuObject<D3d12FrameContext, FFrameContext>
    {
        constexpr static u64 InitUploadBufferSize = 1024 * 1024;

        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        Rc<D3d12GpuQueue> m_queue{};
        ComPtr<ID3D12CommandAllocator> m_command_allocator{};
        Rc<DescriptorContext> m_descriptors{};
        std::vector<BufferPack> m_upload_buffers{};
        usize m_cur_upload_buffer_index{};
        usize m_cur_upload_buffer_size{};

        explicit D3d12FrameContext(Rc<D3d12GpuQueue>&& queue);

        FResult SetName(const FStr8or16& name) noexcept override;

        FResult GrowUploadBuffer(u64 min_required_size) noexcept override;

        // 回收帧使用的资源，重复使用，需要在队列中等待帧完成后调用
        void Recycle();
    };
}
