#include "Layout.h"

using namespace Coplt;

struct TmpBySpace
{
};

D3d12ShaderLayout::D3d12ShaderLayout(Rc<D3d12GpuDevice>&& device, const FShaderLayoutCreateOptions& options)
    : m_device(std::move(device))
{
    m_dx_device = m_device->m_device;

    m_layout_item_defines = std::vector(options.Items, options.Items + options.Count);

    HashMap<u32, TmpBySpace> tmp_by_space{};
    tmp_by_space.TryAdd(1, {});

    for (UINT i = 0; i < m_layout_item_defines.size(); i++)
    {
    }

    std::vector<D3D12_ROOT_PARAMETER1> root_parameters{};

    D3D12_VERSIONED_ROOT_SIGNATURE_DESC desc{};
    desc.Version = D3D_ROOT_SIGNATURE_VERSION_1_2;
    // todo

    ComPtr<ID3DBlob> blob{};
    ComPtr<ID3DBlob> error_blob{};
    if (const auto r = D3D12SerializeVersionedRootSignature(&desc, &blob, &error_blob); !SUCCEEDED(r))
    {
        m_device->Logger().Log(
            FLogLevel::Error, FLogType::DirectX,
            static_cast<const char*>(error_blob->GetBufferPointer()), error_blob->GetBufferSize()
        );
        chr | r;
    }

    chr | m_dx_device->CreateRootSignature(
        0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)
    );

    if (m_device->Debug())
    {
        chr | m_root_signature >> SetNameEx(options.Name);
    }
}

FResult D3d12ShaderLayout::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        chr | m_root_signature >> SetNameEx(name);
    });
}

void* D3d12ShaderLayout::GetRootSignaturePtr() noexcept
{
    return m_root_signature.Get();
}

FShaderLayoutItemDefine* D3d12ShaderLayout::GetItemDefines(u32* out_count) noexcept
{
    *out_count = static_cast<u32>(m_layout_item_defines.size());
    return m_layout_item_defines.data();
}
