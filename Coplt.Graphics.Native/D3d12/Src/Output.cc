#include "Output.h"

#include "directx/d3dx12.h"

#include "../Include/GraphicsFormat.h"
#include "../../Api/Include/Error.h"

using namespace Coplt;

D3d12GpuSwapChainOutput::D3d12GpuSwapChainOutput(Rc<D3d12GpuQueue>&& queue) : m_queue(std::move(queue))
{
    if (m_queue->m_queue_type != FGpuQueueType::Direct)
        COPLT_THROW("Cannot create output on a non direct queue.");

    m_device = m_queue->m_device;
    m_dx_device = m_queue->m_dx_device;
    m_dx_queue = m_queue->m_queue;

    m_state = FResourceState::Present;
}

D3d12GpuSwapChainOutput::D3d12GpuSwapChainOutput(Rc<D3d12GpuQueue>&& queue, const FGpuOutputCreateOptions& options)
    : D3d12GpuSwapChainOutput(std::move(queue))
{
    m_v_sync = options.VSync;
}

void D3d12GpuSwapChainOutput::Init()
{
    m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();

    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc{};
    rtv_heap_desc.NumDescriptors = m_frame_count;
    rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    chr | m_dx_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_rtv_heap));
    m_rtv_descriptor_size = m_dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    D3D12_DESCRIPTOR_HEAP_DESC srv_heap_desc{};
    srv_heap_desc.NumDescriptors = m_frame_count;
    srv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    chr | m_dx_device->CreateDescriptorHeap(&srv_heap_desc, IID_PPV_ARGS(&m_srv_heap));
    m_srv_descriptor_size = m_dx_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    CreateRts();

    for (u32 i = 0; i < m_frame_count; ++i)
    {
        m_frame_contexts[i] = new D3d12FrameContext(m_queue->CloneThis());
    }

    chr | m_dx_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    m_fence_value = 1;

    m_fence_event = CreateEvent(nullptr, false, false, nullptr);
    if (m_fence_event == nullptr) chr | HRESULT_FROM_WIN32(GetLastError());
}

D3d12GpuSwapChainOutput::D3d12GpuSwapChainOutput(
    Rc<D3d12GpuQueue>&& queue,
    const FGpuOutputFromSwapChainCreateOptions& options,
    IDXGISwapChain3* swap_chain
) : D3d12GpuSwapChainOutput(std::move(queue))
{
    m_swap_chain = swap_chain;
    m_v_sync = options.VSync;

    DXGI_SWAP_CHAIN_DESC1 desc{};
    chr | m_swap_chain->GetDesc1(&desc);

    m_format = FromDx(desc.Format);
    m_width = desc.Width;
    m_height = desc.Height;
    m_frame_count = desc.BufferCount;

    Init();
}

D3d12GpuSwapChainOutput::D3d12GpuSwapChainOutput(
    Rc<D3d12GpuQueue>&& queue,
    const FGpuOutputCreateOptions& options, const HWND hwnd
) : D3d12GpuSwapChainOutput(std::move(queue), options)
{
    bool is_hdr = false;
    m_format = SelectFormat(options, is_hdr);

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = m_width = options.Width;
    desc.Height = m_height = options.Height;
    desc.Format = ToDx(m_format);
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    switch (options.PresentMode)
    {
    case FPresentMode::NoBuffer:
        if (is_hdr)
        {
            m_frame_count = desc.BufferCount = 2;
            desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        }
        else
        {
            m_frame_count = desc.BufferCount = 2;
            desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        }
        break;
    case FPresentMode::DoubleBuffer:
        m_frame_count = desc.BufferCount = 2;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        break;
    case FPresentMode::TripleBuffer:
    default:
        m_frame_count = desc.BufferCount = 3;
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        break;
    }
    switch (options.AlphaMode)
    {
    case FOutputAlphaMode::PrePremultiplied:
        desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
        break;
    case FOutputAlphaMode::PostPremultiplied:
        desc.AlphaMode = DXGI_ALPHA_MODE_STRAIGHT;
        break;
    case FOutputAlphaMode::Opaque:
    default:
        desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        break;
    }

    ComPtr<IDXGISwapChain1> swap_chain;
    chr | m_device->m_factory->CreateSwapChainForHwnd(
        m_dx_queue.Get(),
        hwnd,
        &desc,
        nullptr,
        nullptr,
        &swap_chain
    );

    chr | swap_chain.As(&m_swap_chain);

    if (m_format == FGraphicsFormat::R10G10B10A2_UNorm)
    {
        chr | m_swap_chain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
    }

    Init();
}

D3d12GpuSwapChainOutput::~D3d12GpuSwapChainOutput()
{
    WaitAll();
    CloseHandle(m_fence_event);
}

FGraphicsFormat D3d12GpuSwapChainOutput::SelectFormat(
    const FGpuOutputCreateOptions& options, bool& is_hdr
)
{
    if (options.FormatSelector.Specify) return options.Format;
    if (options.FormatSelector.Hdr == FHdrType::UNorm10 && options.AlphaMode == FOutputAlphaMode::Opaque)
    {
        is_hdr = true;
        return FGraphicsFormat::R10G10B10A2_UNorm;
    }
    if (options.FormatSelector.Hdr == FHdrType::Float16)
    {
        is_hdr = true;
        return FGraphicsFormat::R16G16B16A16_Float;
    }
    if (options.FormatSelector.Srgb) return FGraphicsFormat::R8G8B8A8_UNorm_sRGB;
    return FGraphicsFormat::R8G8B8A8_UNorm;
}

FResult D3d12GpuSwapChainOutput::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        // 交换链不能设置名字
    });
}

FResult D3d12GpuSwapChainOutput::SetVSync(const b8 Enable) noexcept
{
    return feb([&]
    {
        m_v_sync = static_cast<bool>(Enable);
    });
}

FResult D3d12GpuSwapChainOutput::Resize(const u32 Width, const u32 Height) noexcept
{
    return feb([&]
    {
        if (m_waiting || m_need_resize)
        {
            m_new_width = Width;
            m_new_height = Height;
            m_need_resize = true;

            std::lock_guard lock(m_queue->m_mutex);
            if (m_need_resize)
            {
                Resize_NoLock(m_new_width, m_new_height);
            }
        }
        else
        {
            std::lock_guard lock(m_queue->m_mutex);
            Resize_NoLock(Width, Height);
        }
    });
}

FResult D3d12GpuSwapChainOutput::Wait() noexcept
{
    return feb([&]
    {
        std::lock_guard lock(m_queue->m_mutex);
        WaitNextFrame_NoLock();
    });
}

void D3d12GpuSwapChainOutput::Submit(D3d12GpuQueue* Queue, const FCommandSubmit* submit)
{
    if (Queue != m_queue)
        COPLT_THROW("The executor does not belong to this queue");
    std::lock_guard lock(m_queue->m_mutex);
    Submit_NoLock(submit);
    Present_NoLock();
    WaitNextFrame_NoLock();
}

void D3d12GpuSwapChainOutput::SubmitNoWait(D3d12GpuQueue* Queue, const FCommandSubmit* submit)
{
    if (Queue != m_queue)
        COPLT_THROW("The executor does not belong to this queue");
    std::lock_guard lock(m_queue->m_mutex);
    Submit_NoLock(submit);
    Present_NoLock();
}

void D3d12GpuSwapChainOutput::Submit_NoLock(const FCommandSubmit* submit)
{
    m_queue->SubmitNoLock(m_frame_contexts[m_frame_index], submit);
}

void D3d12GpuSwapChainOutput::Present_NoLock()
{
    chr | m_swap_chain->Present(m_v_sync ? 1 : 0, 0);
    Signal_NoLock();
}

void D3d12GpuSwapChainOutput::WaitNextFrame_NoLock()
{
    if (m_need_resize)
    {
        Resize_NoLock(m_new_width, m_new_height);
        m_need_resize = false;
    }
    if (m_fence_value_queue.size() >= m_frame_count)
    {
        const auto fence_value = m_fence_value_queue.front();
        m_fence_value_queue.pop();
        WaitFenceValue_NoLock(fence_value);
    }
    m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
    {
        const auto fence_value = m_frame_m_fence_values[m_frame_index];
        WaitFenceValue_NoLock(fence_value);
    }
    m_frame_contexts[m_frame_index]->Recycle();
}

void D3d12GpuSwapChainOutput::Resize_NoLock(const u32 width, const u32 height)
{
    if (width == m_width && height == m_height) return;
    WaitAll_NoLock();
    for (u32 i = 0; i < m_frame_count; i++)
    {
        m_buffers[i] = nullptr;
    }
    chr | m_swap_chain->ResizeBuffers(m_frame_count, width, height, ToDx(m_format), 0);
    CreateRts();
    m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
    m_width = width;
    m_height = height;
}

void D3d12GpuSwapChainOutput::Signal_NoLock()
{
    const auto fence_value = m_fence_value;
    chr | m_dx_queue->Signal(m_fence.Get(), fence_value);
    m_fence_value_queue.push(fence_value);
    m_frame_m_fence_values[m_frame_index] = fence_value;
    m_fence_value++;
}

void D3d12GpuSwapChainOutput::WaitAll()
{
    std::lock_guard lock(m_queue->m_mutex);
    WaitAll_NoLock();
}

void D3d12GpuSwapChainOutput::WaitAll_NoLock()
{
    Signal_NoLock();
    const auto fence_value = m_fence_value_queue.back();
    m_fence_value_queue = {};
    WaitFenceValue_NoLock(fence_value);
}

void D3d12GpuSwapChainOutput::WaitFenceValue_NoLock(const u32 fence_value)
{
    if (m_fence->GetCompletedValue() < fence_value)
    {
        m_waiting = true;
        chr | m_fence->SetEventOnCompletion(fence_value, m_fence_event);
        WaitForSingleObject(m_fence_event, INFINITE);
        m_waiting = false;
    }
}

void D3d12GpuSwapChainOutput::CreateRts()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
    CD3DX12_CPU_DESCRIPTOR_HANDLE srv_handle(m_srv_heap->GetCPUDescriptorHandleForHeapStart());
    for (u32 i = 0; i < m_frame_count; ++i)
    {
        chr | m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i]));
        m_dx_device->CreateRenderTargetView(m_buffers[i].Get(), nullptr, rtv_handle);
        rtv_handle.Offset(1, m_rtv_descriptor_size);
        m_dx_device->CreateShaderResourceView(m_buffers[i].Get(), nullptr, srv_handle);
        srv_handle.Offset(1, m_rtv_descriptor_size);

        if (m_device->Debug())
        {
            chr | m_buffers[i]->SetName(
                fmt::format(L"Swap Chain Buffer {} ({}, {})", i, m_width, m_height).c_str());
        }
    }
}

FResult D3d12GpuSwapChainOutput::GetCurrentResourcePtr(void* out) const noexcept
{
    return feb([&]
    {
        *static_cast<ID3D12Resource**>(out) = m_buffers[m_frame_index].Get();
    });
}

FResult D3d12GpuSwapChainOutput::GetCurrentRtv(void* out) noexcept
{
    return feb([&]
    {
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
        rtv_handle.Offset(m_frame_index, m_rtv_descriptor_size);
        *static_cast<D3D12_CPU_DESCRIPTOR_HANDLE*>(out) = rtv_handle;
    });
}
