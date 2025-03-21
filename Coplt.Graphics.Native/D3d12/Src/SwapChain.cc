#include "SwapChain.h"

#include "Isolate.h"
#include "../../Api/Include/Finally.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

D3d12GpuSwapChain::D3d12GpuSwapChain(
    const NonNull<D3d12GpuIsolate> isolate, const FGpuOutput2CreateOptions& options,
    const FOutputAlphaMode AlphaMode, const b8 VSync
)
{
    m_isolate = isolate->CloneThis();
    m_device = m_isolate->m_device->m_device;
    m_queue = m_isolate->m_main_queue->m_queue;
    m_debug_enabled = m_isolate->m_device->Debug();
    this->VSync = VSync;
    this->PresentMode = options.PresentMode;
    this->AlphaMode = AlphaMode;

    if (m_debug_enabled && !options.Name.is_null())
    {
        m_name = options.Name.ToString();
    }

    m_event = CreateEventW(nullptr, false, false, nullptr);
    if (m_event == nullptr) chr | HRESULT_FROM_WIN32(GetLastError());
}

void D3d12GpuSwapChain::Initialize()
{
    m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();

    D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc{};
    rtv_heap_desc.NumDescriptors = m_frame_count;
    rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    chr | m_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_rtv_heap));
    m_rtv_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

D3d12GpuSwapChain::D3d12GpuSwapChain(const NonNull<D3d12GpuIsolate> isolate, const FGpuSwapChainFromExistsCreateOptions& options)
    : D3d12GpuSwapChain(isolate, options, options.AlphaMode, options.VSync)
{
    m_swap_chain = static_cast<IDXGISwapChain3*>(options.ExistsSwapChain);

    DXGI_SWAP_CHAIN_DESC1 desc{};
    chr | m_swap_chain->GetDesc1(&desc);

    this->Format = FromDx(desc.Format);
    this->Width = desc.Width;
    this->Height = desc.Height;
    m_frame_count = desc.BufferCount;

    m_image_data.m_format = this->Format;
    m_image_data.m_width = this->Width;
    m_image_data.m_height = this->Height;
    m_image_data.m_depth_or_length = 1;
    m_image_data.m_mip_levels = 1;
    m_image_data.m_multisample_count = 1;
    m_image_data.m_planes = 1;
    m_image_data.m_dimension = FImageDimension::Two;
    m_image_data.m_layout = FImageLayout::Undefined;

    Initialize();
}

D3d12GpuSwapChain::D3d12GpuSwapChain(const NonNull<D3d12GpuIsolate> isolate, const FGpuSwapChainCreateOptions& options, HWND hwnd)
    : D3d12GpuSwapChain(isolate, options, options.AlphaMode, options.VSync)
{
    bool is_hdr = false;
    this->Format = SelectFormat(options, is_hdr);

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = this->Width = options.Width;
    desc.Height = this->Height = options.Height;
    desc.Format = ToDx(this->Format);
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
    chr | m_isolate->m_device->m_instance->m_factory->CreateSwapChainForHwnd(
        m_queue.Get(),
        hwnd,
        &desc,
        nullptr,
        nullptr,
        &swap_chain
    );

    chr | swap_chain.As(&m_swap_chain);

    if (this->Format == FGraphicsFormat::R10G10B10A2_UNorm)
    {
        chr | m_swap_chain->SetColorSpace1(DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
    }

    Initialize();
}

FGraphicsFormat D3d12GpuSwapChain::SelectFormat(const FGpuSwapChainCreateOptions& options, bool& is_hdr)
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

D3d12GpuSwapChain::~D3d12GpuSwapChain()
{
    WaitAll();
    CloseHandle(m_event);
}

void D3d12GpuSwapChain::CreateRts()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
    for (u32 i = 0; i < m_frame_count; ++i)
    {
        chr | m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i]));
        m_device->CreateRenderTargetView(m_buffers[i].Get(), nullptr, rtv_handle);
        rtv_handle.Offset(1, m_rtv_descriptor_size);
    }
}

void D3d12GpuSwapChain::SetRtNames()
{
    if (!m_debug_enabled || m_name == nullptr) return;
    for (u32 i = 0; i < m_frame_count; ++i)
    {
        const auto str = m_name->GetStr();
        if (str.is8())
        {
            const auto name = fmt::format("[{}]::Buffer {} ({}, {})", str.str8, i, this->Width, this->Height);
            chr | m_buffers[i] >> SetNameEx(FStr8or16(name));
        }
        else
        {
            const auto name = fmt::format(L"[{}]::Buffer {} ({}, {})", str.wcstr(), i, this->Width, this->Height);
            chr | m_buffers[i] >> SetNameEx(FStr8or16(name));
        }
    }
}

FResult D3d12GpuSwapChain::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_debug_enabled || name.is_null()) return;
        std::lock_guard lock(m_present_mutex);
        m_name = name.ToString();
        SetRtNames();
    });
}

const FGpuOutputData* D3d12GpuSwapChain::GpuOutputData() noexcept
{
    return this;
}

const FGpuSwapChainData* D3d12GpuSwapChain::GpuSwapChainData() noexcept
{
    return this;
}

FGpuIsolate* D3d12GpuSwapChain::GetIsolate() noexcept
{
    return m_isolate.get();
}

NonNull<FGpuOutputData> D3d12GpuSwapChain::Data()
{
    return this;
}

NonNull<FGpuImageData> D3d12GpuSwapChain::ImageData()
{
    return &m_image_data;
}

NonNull<ID3D12Resource> D3d12GpuSwapChain::GetResourcePtr()
{
    return m_buffers[m_frame_index].Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE D3d12GpuSwapChain::GetRtv()
{
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
    rtv_handle.Offset(static_cast<i32>(m_frame_index), m_rtv_descriptor_size);
    return rtv_handle;
}

FResult D3d12GpuSwapChain::Resize(const u32 Width, const u32 Height) noexcept
{
    return feb([&]
    {
        std::lock_guard lock(m_resize_mutex);

        m_new_width = Width;
        m_new_height = Height;
        if (this->Width != Width || this->Height != Height)
        {
            m_need_resize = true;
        }
    });
}

FResult D3d12GpuSwapChain::SetVSync(const b8 Enable) noexcept
{
    return feb([&]
    {
        this->VSync = Enable;
    });
}

FResult D3d12GpuSwapChain::Present() noexcept
{
    return feb([&]
    {
        std::lock_guard lock(m_present_mutex);
        PresentNoLock();
        WaitNextFrameNoLock();
    });
}

FResult D3d12GpuSwapChain::PresentNoWait() noexcept
{
    return feb([&]
    {
        std::lock_guard lock(m_present_mutex);
        PresentNoLock();
    });
}

FResult D3d12GpuSwapChain::Wait() noexcept
{
    return feb([&]
    {
        std::lock_guard lock(m_present_mutex);
        WaitNextFrameNoLock();
    });
}

bool D3d12GpuSwapChain::IsAvailable() noexcept
{
    return !m_waiting;
}

void D3d12GpuSwapChain::PresentNoLock()
{
    chr | m_swap_chain->Present(this->VSync ? 1 : 0, 0);
    const auto fence_value = m_isolate->m_main_queue->Signal();
    m_event_values[m_frame_index] = fence_value;
}

void D3d12GpuSwapChain::WaitNextFrameNoLock()
{
    m_waiting = true;
    Finally finally([&] { m_waiting = false; });

    if (m_need_resize)
    {
        ResizeNoLock(m_new_width, m_new_height);
        m_need_resize = false;
    }
    u64 fence_value = std::numeric_limits<u64>::max();
    for (u32 i = 0; i < m_frame_count; ++i)
    {
        const auto frame_fence_value = m_event_values[m_frame_index];
        if (frame_fence_value < fence_value)
            fence_value = frame_fence_value;
    }
    WaitFenceValueNoLock(fence_value);
    m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
}

void D3d12GpuSwapChain::ResizeNoLock(const u32 width, const u32 height)
{
    if (this->Width == width && this->Height == height) return;
    WaitAllNoLock();
    for (u32 i = 0; i < m_frame_count; i++)
    {
        m_buffers[i] = nullptr;
    }
    chr | m_swap_chain->ResizeBuffers(m_frame_count, width, height, ToDx(this->Format), 0);
    CreateRts();
    m_frame_index = m_swap_chain->GetCurrentBackBufferIndex();
    m_image_data.m_width = this->Width = width;
    m_image_data.m_height = this->Height = height;
}

void D3d12GpuSwapChain::WaitAll()
{
    std::lock_guard lock(m_present_mutex);
    WaitAllNoLock();
}

void D3d12GpuSwapChain::WaitAllNoLock() const
{
    u64 fence_value = 0;
    for (u32 i = 0; i < m_frame_count; ++i)
    {
        const auto frame_fence_value = m_event_values[m_frame_index];
        if (frame_fence_value > fence_value)
            fence_value = frame_fence_value;
    }
    WaitFenceValueNoLock(fence_value);
}

void D3d12GpuSwapChain::WaitFenceValueNoLock(const u64 fence_value) const
{
    m_isolate->m_main_queue->WaitFenceValue(fence_value, m_event);
}
