#pragma once

#include <directx/d3d12.h>
#include "D3D12MemAlloc.h"
#include "Instance.h"
#include "../../Api/Include/Object.h"
#include "../FFI/Device.h"

namespace Coplt
{
    struct D3d12GpuDevice final : Object<D3d12GpuDevice, FD3d12GpuDevice>
    {
        Rc<D3d12Instance> m_instance{};
        ComPtr<ID3D12Debug> m_debug_controller{};
        ComPtr<IDXGIFactory4> m_factory{};
        ComPtr<IDXGIAdapter1> m_adapter{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12Device> m_device0{};
        ComPtr<ID3D12InfoQueue1> m_info_queue{};
        ComPtr<D3D12MA::Allocator> m_gpu_allocator{};
        DWORD m_callback_cookie{};

        explicit D3d12GpuDevice(
            Rc<D3d12Instance> instance,
            const FGpuDeviceCreateOptions& options
        );

        ~D3d12GpuDevice() override;

        const Logger& Logger() const noexcept
        {
            return m_instance->Logger();
        }

        bool Debug() const noexcept
        {
            return m_debug_controller != nullptr;
        }

        FResult SetName(const Str8or16& name) noexcept override;

        void* GetRawDevice() noexcept override;

        FResult CreateMainQueue(const FMainQueueCreateOptions& options, FGpuQueue** out) noexcept override;

        FResult CreateShaderModule(
            const FShaderModuleCreateOptions& options, FShaderModule** out
        ) noexcept override;

        FResult CreateShader(const FShaderCreateOptions& options, FShader** out) noexcept override;
    };
}
