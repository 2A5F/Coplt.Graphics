#pragma once

#include <queue>
#include <dxgi1_4.h>

#include "Queue.h"
#include "../FFI/Output.h"
#include "../../Api/Include/Object.h"

namespace Coplt
{
    struct D3d12GpuDevice;

    struct D3d12GpuSwapChainOutput final : Object<D3d12GpuSwapChainOutput, FD3d12GpuSwapChainOutput>
    {
        constexpr static UINT MaxBufferCount = 3;

        Rc<D3d12GpuQueue> m_queue{};
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12CommandQueue> m_dx_queue{};
        ComPtr<IDXGISwapChain3> m_swap_chain{};
        ComPtr<ID3D12CommandAllocator> m_command_allocators[MaxBufferCount]{};
        ComPtr<ID3D12DescriptorHeap> m_rtv_heap{};
        ComPtr<ID3D12DescriptorHeap> m_srv_heap{};
        ComPtr<ID3D12Resource> m_buffers[MaxBufferCount];
        ComPtr<ID3D12Fence> m_fence{};
        HANDLE m_fence_event{};
        UINT64 m_fence_value{};
        std::queue<UINT> m_fence_value_queue{};
        UINT m_frame_count{};
        UINT m_frame_index{};
        UINT m_frame_m_fence_values[MaxBufferCount]{};
        UINT m_rtv_descriptor_size{};
        UINT m_srv_descriptor_size{};
        u32 m_cur_width{};
        u32 m_cur_height{};
        mutable u32 m_new_width{};
        mutable u32 m_new_height{};
        DXGI_FORMAT m_format{};
        std::atomic_bool m_v_sync{};
        std::atomic_bool m_waiting{};
        std::atomic_bool m_need_resize{};

        explicit D3d12GpuSwapChainOutput(Rc<D3d12GpuQueue>&& queue, const FGpuOutputCreateOptions& options, HWND hwnd);

        ~D3d12GpuSwapChainOutput() override;

        FResult SetName(const Str8or16& name) noexcept override;

        FResult SetVSync(b8 Enable) noexcept override;

        FResult Resize(u32 Width, u32 Height) noexcept override;

        FResult Present() noexcept override;
        FResult PresentNoWait() noexcept override;
        FResult WaitNextFrame() noexcept override;

        void PresentNoWait_NoLock();
        void WaitNextFrame_NoLock();
        void Resize_NoLock(u32 width, u32 height);

        void Signal_NoLock();

        void WaitAll();
        void WaitAll_NoLock();

        void WaitFenceValue_NoLock(u32 fence_value);

        void CreateBuffers();
    };
}
