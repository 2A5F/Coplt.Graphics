#pragma once

#include <mutex>
#include <directx/d3d12.h>

#include "Device.h"
#include "../Include/Utils.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Queue.h"

namespace Coplt
{
    struct D3d12GpuQueue final : Object<D3d12GpuQueue, FD3d12GpuQueue>
    {
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12CommandQueue> m_queue{};
        ComPtr<ID3D12CommandAllocator> m_command_allocator{};
        ComPtr<ID3D12GraphicsCommandList> m_command_list{};
        std::mutex m_mutex{};

        explicit D3d12GpuQueue(Rc<D3d12GpuDevice>&& device, const FMainQueueCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        void* GetRawQueue() noexcept override;

        FResult CreateOutputForHwnd(
            const FGpuOutputCreateOptions& options,
            void* hwnd,
            FGpuOutput** out
        ) noexcept override;
    };
} // Coplt
