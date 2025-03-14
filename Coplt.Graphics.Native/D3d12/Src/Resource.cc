#include "Resource.h"

#include "Adapter.h"
#include "../ThirdParty/DirectXTK12/Src/d3dx12.h"

using namespace Coplt;

ResourcePack::ResourcePack(
    D3d12GpuDevice* device, D3D12MA::Allocator* allocator, const FCpuAccess cpu_access,
    FResState& state, const D3D12_RESOURCE_DESC1* desc, const D3D12_CLEAR_VALUE* clear_value,
    bool image
)
{
    D3D12MA::ALLOCATION_DESC alloc_desc{};
    alloc_desc.Flags = D3D12MA::ALLOCATION_FLAG_NONE;

    D3D12_RESOURCE_STATES init_state{};
    switch (cpu_access)
    {
    case FCpuAccess::Write:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
        init_state = D3D12_RESOURCE_STATE_GENERIC_READ;
        if (image) state = FResState::ImageGenericRead();
        else state = FResState::BufferGenericRead();
        break;
    case FCpuAccess::Read:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
        init_state = D3D12_RESOURCE_STATE_COPY_DEST;
        if (image) state = FResState::ImageCopyDst();
        else state = FResState::BufferCopyDst();
        break;
    default:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
        init_state = D3D12_RESOURCE_STATE_COMMON;
        if (image) state = FResState::ImageCommon();
        else state = FResState::BufferCommon();
        break;
    }

    if (device->m_adapter->m_features.EnhancedBarriers)
    {
        chr | allocator->CreateResource3(
            &alloc_desc, desc, D3D12_BARRIER_LAYOUT_UNDEFINED, clear_value,
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
    const auto desc = CD3DX12_RESOURCE_DESC1::Buffer(size, Flags);
    m_resource = ResourcePack(device, allocator, cpu_access, m_state, &desc, nullptr, false);
}
