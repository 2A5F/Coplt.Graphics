#pragma once

#include "Output.h"
#include "Queue.h"
#include "../../Api/FFI/Output.h"
#include "../../Api/Include/GpuObject.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12GpuSwapChain, "d726caf3-41c4-4d60-895e-61c68a4dff98", FGpuSwapChain)
    {
        constexpr static UINT MaxBufferCount = 3;
    };

    enum class SwapChainFor
    {
        Hwnd,
        CoreWindow,
        Composition,
    };

    struct D3d12GpuSwapChain final : GpuObject<FGpuSwapChain, ID3d12GpuSwapChain, ID3d12GpuOutput2>, FGpuSwapChainData
    {
        Rc<D3d12GpuIsolate> m_isolate{};
        // 可空
        Rc<FString> m_name{};
        ComPtr<ID3D12Device2> m_device{};
        ComPtr<ID3D12CommandQueue> m_queue{};
        ComPtr<IDXGISwapChain3> m_swap_chain{};
        ComPtr<ID3D12DescriptorHeap> m_rtv_heap{};
        ComPtr<ID3D12Resource> m_buffers[MaxBufferCount];
        FGpuImageData m_image_data{};
        LayoutState m_layout_state{};
        std::mutex m_resize_mutex{};
        std::mutex m_present_mutex{};
        HANDLE m_event{};
        u64 m_event_values[MaxBufferCount]{};
        u32 m_frame_count{};
        u32 m_frame_index{};
        u32 m_rtv_descriptor_size{};
        mutable u32 m_new_width{};
        mutable u32 m_new_height{};
        std::atomic_bool m_waiting{};
        std::atomic_bool m_need_resize{};
        b8 m_debug_enabled{};

    private:
        explicit D3d12GpuSwapChain(
            NonNull<D3d12GpuIsolate> isolate, const FGpuOutput2CreateOptions& options,
            FOutputAlphaMode AlphaMode, b8 VSync
        );
        void Initialize();

    public:
        explicit D3d12GpuSwapChain(NonNull<D3d12GpuIsolate> isolate, const FGpuSwapChainFromExistsCreateOptions& options);
        explicit D3d12GpuSwapChain(NonNull<D3d12GpuIsolate> isolate, const FGpuSwapChainCreateOptions& options, void* handle, SwapChainFor For);

        static FGraphicsFormat SelectFormat(const FGpuSwapChainCreateOptions& options, bool& is_hdr);

        ~D3d12GpuSwapChain() override;

        void CreateRts();
        void SetRtNames();

        FResult SetName(const FStr8or16& name) noexcept override;
        const FGpuOutputData* GpuOutputData() noexcept override;
        const FGpuSwapChainData* GpuSwapChainData() noexcept override;
        FGpuIsolate* GetIsolate() noexcept override;

        NonNull<LayoutState> State() override;
        NonNull<FGpuOutputData> Data() override;
        NonNull<FGpuImageData> ImageData() override;
        NonNull<ID3D12Resource> GetResourcePtr() override;
        CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtv() override;

        FResult Resize(u32 Width, u32 Height) noexcept override;
        FResult SetVSync(b8 Enable) noexcept override;

        FResult Present() noexcept override;
        FResult PresentNoWait() noexcept override;
        FResult Wait() noexcept override;

        bool IsAvailable() noexcept override;

        void PresentNoLock();
        void WaitNextFrameNoLock();

        void ResizeNoLock(u32 width, u32 height);

        void WaitAll();
        void WaitAllNoLock() const;

        void WaitFenceValueNoLock(u64 fence_value) const;

        void* GetRawPtr() const noexcept override;
    };
}
