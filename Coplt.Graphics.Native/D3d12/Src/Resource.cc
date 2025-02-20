#include "Resource.h"

using namespace Coplt;

ResourcePack::ResourcePack(
    D3D12MA::Allocator* allocator, const FCpuAccess cpu_access, FResourceState& state,
    const D3D12_RESOURCE_DESC* desc, const D3D12_CLEAR_VALUE* clear_value
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
        state = FResourceState::GenericRead;
        break;
    case FCpuAccess::Read:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
        init_state = D3D12_RESOURCE_STATE_COPY_DEST;
        state = FResourceState::CopyDst;
        break;
    default:
        alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;
        init_state = D3D12_RESOURCE_STATE_COMMON;
        state = FResourceState::Common;
        break;
    }
    chr | allocator->CreateResource(
        &alloc_desc, desc, init_state, clear_value, &m_allocation, IID_PPV_ARGS(&m_resource)
    );
}

BufferPack::BufferPack(
    D3D12MA::Allocator* allocator, const FCpuAccess cpu_access, const u64 size,
    const D3D12_RESOURCE_FLAGS Flags
) : m_size(size)
{
    D3D12_RESOURCE_DESC desc{};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = Flags;
    m_resource = ResourcePack(allocator, cpu_access, m_state, &desc, nullptr);
}
