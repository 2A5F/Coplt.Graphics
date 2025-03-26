#pragma once

#include <queue>
#include <dxgi1_4.h>
#include <directx/d3dx12.h>

#include "Context.h"
#include "Executor.h"
#include "Queue.h"
#include "../FFI/Output.h"
#include "../../Api/Include/Object.h"
#include "../Include/ResState.h"

namespace Coplt
{
    struct D3d12GpuDevice;

    struct D3d12GpuSwapChainOutput final : GpuObject<D3d12GpuSwapChainOutput, FD3d12GpuSwapChainOutput, ID3d12GpuExecutor>
    {
        constexpr static UINT MaxBufferCount = 3;

        Rc<D3d12GpuQueue> m_queue{};
        Rc<D3d12GpuDevice> m_device{};
        ComPtr<ID3D12Device2> m_dx_device{};
        ComPtr<ID3D12CommandQueue> m_dx_queue{};
        ComPtr<IDXGISwapChain3> m_swap_chain{};
        Rc<D3d12FrameContext> m_frame_contexts[MaxBufferCount]{};
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
        mutable u32 m_new_width{};
        mutable u32 m_new_height{};
        std::atomic_bool m_v_sync{};
        std::atomic_bool m_waiting{};
        std::atomic_bool m_need_resize{};

    private:
        explicit D3d12GpuSwapChainOutput(Rc<D3d12GpuQueue>&& queue);
        explicit D3d12GpuSwapChainOutput(Rc<D3d12GpuQueue>&& queue, const FGpuOutputCreateOptions& options);
        void Init();

    public:
        explicit D3d12GpuSwapChainOutput(
            Rc<D3d12GpuQueue>&& queue, const FGpuOutputFromSwapChainCreateOptions& options, IDXGISwapChain3* swap_chain
        );
        explicit D3d12GpuSwapChainOutput(Rc<D3d12GpuQueue>&& queue, const FGpuOutputCreateOptions& options, HWND hwnd);

        ~D3d12GpuSwapChainOutput() override;

        static FGraphicsFormat SelectFormat(const FGpuOutputCreateOptions& options, bool& is_hdr);

        FResult SetName(const FStr8or16& name) noexcept override;

        FResult SetVSync(b8 Enable) noexcept override;

        FResult Resize(u32 Width, u32 Height) noexcept override;

        FResult Wait() noexcept override;

        void Submit(D3d12GpuQueue* Queue, const FCommandSubmit* submit) override;
        void SubmitNoWait(D3d12GpuQueue* Queue, const FCommandSubmit* submit) override;

        void Submit_NoLock(/* 可选 */ const FCommandSubmit* submit);
        void Present_NoLock();
        void WaitNextFrame_NoLock();
        void Resize_NoLock(u32 width, u32 height);

        void Signal_NoLock();

        void WaitAll();
        void WaitAll_NoLock();

        void WaitFenceValue_NoLock(u32 fence_value);

        void CreateRts();

        FResult GetCurrentResourcePtr(void* out) const noexcept override;
        FResult GetCurrentRtv(void* out) noexcept override;
    };

    COPLT_INTERFACE_DEFINE(ID3d12GpuOutput2, "59aa68d8-91bd-4032-ba51-6a05795945b6", FGpuOutput2)
    {
        virtual NonNull<LayoutState> State() = 0;
        virtual NonNull<FGpuOutputData> Data() = 0;
        virtual NonNull<FGpuImageData> ImageData() = 0;
        virtual NonNull<ID3D12Resource> GetResourcePtr() = 0;
        virtual CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv() = 0;
    };
}
