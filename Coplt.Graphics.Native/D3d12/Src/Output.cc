#include "Output.h"

#include "directx/d3dx12.h"

#include "TextureFormat.h"
#include "../../Api/Include/Error.h"

using namespace Coplt;

D3d12GpuSwapChainOutput::D3d12GpuSwapChainOutput(
    Rc<D3d12GpuQueue>&& queue,
    const FGpuOutputCreateOptions& options, const HWND hwnd
) : m_queue(std::move(queue))
{
    if (m_queue->m_queue_type != FGpuQueueType::Direct)
        throw WRuntimeException(L"Cannot create output on a non direct queue.");

    m_device = m_queue->m_device;
    m_dx_device = m_queue->m_dx_device;
    m_dx_queue = m_queue->m_queue;

    m_v_sync = options.VSync;

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.Width = options.Width;
    desc.Height = options.Height;
    desc.Format = ToDXGIFormat(options.Format);
    desc.SampleDesc.Count = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    switch (options.PresentMode)
    {
    case FPresentMode::NoBuffer:
        m_frame_count = desc.BufferCount = 2;
        desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
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

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_handle(m_rtv_heap->GetCPUDescriptorHandleForHeapStart());
    CD3DX12_CPU_DESCRIPTOR_HANDLE srv_handle(m_srv_heap->GetCPUDescriptorHandleForHeapStart());

    for (UINT i = 0; i < m_frame_count; ++i)
    {
        chr | m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_buffers[i]));
        m_dx_device->CreateRenderTargetView(m_buffers[i].Get(), nullptr, rtv_handle);
        rtv_handle.Offset(1, m_rtv_descriptor_size);
        m_dx_device->CreateShaderResourceView(m_buffers[i].Get(), nullptr, srv_handle);
        srv_handle.Offset(1, m_rtv_descriptor_size);
    }

    chr | m_dx_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator));

    chr | m_dx_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
    m_fence_value = 1;

    m_fence_event = CreateEvent(nullptr, false, false, nullptr);
    if (m_fence_event == nullptr) chr | HRESULT_FROM_WIN32(GetLastError());
}

D3d12GpuSwapChainOutput::~D3d12GpuSwapChainOutput()
{
    // todo
    CloseHandle(m_fence_event);
}

FResult D3d12GpuSwapChainOutput::SetName(const Str8or16& name) noexcept
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
