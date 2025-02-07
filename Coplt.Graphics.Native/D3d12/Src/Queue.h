#pragma once

#include <mutex>
#include <directx/d3d12.h>

#include "Device.h"
#include "../Include/Utils.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Queue.h"
#include "Command.h"

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

        D3d12CommandInterpreter m_command_interpreter{};

        explicit D3d12GpuQueue(Rc<D3d12GpuDevice>&& device, const FMainQueueCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        void* GetRawQueue() noexcept override;

        FResult CreateOutputForHwnd(
            const FGpuOutputCreateOptions& options,
            void* hwnd,
            FGpuOutput** out
        ) noexcept override;

        ~D3d12GpuQueue() noexcept override;

        // 提交命令，并互换命令分配器，参数提供的 command_allocator 必须可用
        void Submit(ComPtr<ID3D12CommandAllocator>& command_allocator, /* 可选 */ const FCommandSubmit* submit);
        // 提交命令，并互换命令分配器，但是无锁，需要外部手动锁，参数提供的 command_allocator 必须可用
        void SubmitNoLock(ComPtr<ID3D12CommandAllocator>& command_allocator, /* 可选 */ const FCommandSubmit* submit);
    };
} // Coplt
