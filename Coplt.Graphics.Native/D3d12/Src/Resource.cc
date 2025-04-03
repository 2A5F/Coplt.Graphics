#include "Resource.h"

#include "Adapter.h"
#include "../ThirdParty/DirectXTK12/Src/d3dx12.h"

using namespace Coplt;

ResourcePack::ResourcePack(
    D3d12GpuDevice* device, D3D12MA::Allocator* allocator, const FCpuAccess cpu_access, ResLayout& layout,
    const D3D12_RESOURCE_DESC1* desc, const D3D12_CLEAR_VALUE* clear_value, bool image
)
{
    D3D12MA::ALLOCATION_DESC alloc_desc{};
    alloc_desc.Flags = D3D12MA::ALLOCATION_FLAG_NONE;
    switch (cpu_access)
    {
    case FCpuAccess::Write:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
        break;
    case FCpuAccess::Read:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
        break;
    case FCpuAccess::ReadWrite:
        if (device->m_uma_pool) alloc_desc.CustomPool = device->m_uma_pool.Get();
        else
            COPLT_THROW("Non-UMA devices cannot create read write resources");
    default:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
        break;
    }

    D3D12_BARRIER_LAYOUT init_layout = D3D12_BARRIER_LAYOUT_UNDEFINED;
    D3D12_RESOURCE_STATES init_state{};
    switch (cpu_access)
    {
    case FCpuAccess::Write:
        init_state = D3D12_RESOURCE_STATE_GENERIC_READ;
        if (image)
        {
            layout = ResLayout::GenericRead;
            init_layout = D3D12_BARRIER_LAYOUT_GENERIC_READ;
        }
        break;
    case FCpuAccess::Read:
        init_state = D3D12_RESOURCE_STATE_COPY_DEST;
        if (image)
        {
            layout = ResLayout::CopyDest;
            init_layout = D3D12_BARRIER_LAYOUT_COPY_DEST;
        }
        break;
    case FCpuAccess::ReadWrite:
    default:
        init_state = D3D12_RESOURCE_STATE_COMMON;
        break;
    }

    if (device->m_adapter->m_features.EnhancedBarriers)
    {
        chr | allocator->CreateResource3(
            &alloc_desc, desc, init_layout, clear_value,
            0, nullptr, &m_allocation, IID_PPV_ARGS(&m_resource)
        );
    }
    else
    {
        D3D12_RESOURCE_DESC desc0{};
        desc0.Dimension = desc->Dimension;
        desc0.Alignment = desc->Alignment;
        desc0.Width = desc->Width;
        desc0.Height = desc->Height;
        desc0.DepthOrArraySize = desc->DepthOrArraySize;
        desc0.MipLevels = desc->MipLevels;
        desc0.Format = desc->Format;
        desc0.SampleDesc = desc->SampleDesc;
        desc0.Layout = desc->Layout;
        desc0.Flags = desc->Flags;


        chr | allocator->CreateResource(
            &alloc_desc, &desc0, init_state, clear_value, &m_allocation, IID_PPV_ARGS(&m_resource)
        );
    }
}

BufferPack::BufferPack(
    D3d12GpuDevice* device, D3D12MA::Allocator* allocator, FCpuAccess cpu_access,
    const u64 size, const D3D12_RESOURCE_FLAGS Flags
) : m_size(size)
{
    ResLayout layout{};
    const auto desc = CD3DX12_RESOURCE_DESC1::Buffer(size, Flags);
    m_resource = ResourcePack(device, allocator, cpu_access, layout, &desc, nullptr, false);
}
