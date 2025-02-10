#include "GraphicsPipeline.h"

#include "../../Api/Include/Error.h"
#include "../ThirdParty/DirectXTK12/Src/d3dx12.h"

using namespace Coplt;

namespace
{
    void SetGraphicsState(auto& dst, const FGraphicsShaderPipelineCreateOptions& options)
    {
        // todo
    }
}

D3d12GraphicsShaderPipeline::D3d12GraphicsShaderPipeline(
    Rc<D3d12GpuDevice>&& device, const FGraphicsShaderPipelineCreateOptions& options
) : m_device(std::move(device))
{
    m_dx_device = m_device->m_device;
    if (options.Shader == nullptr) throw WRuntimeException(L"Shader is null");
    m_shader = Rc<FShader>::UnsafeClone(options.Shader);
    if (!HasFlags(m_shader->Stages, FShaderStageFlags::Pixel)) throw WRuntimeException(L"The shader is not graphics");
    auto layout = m_shader->Layout()->QueryInterface<FD3d12ShaderLayout>();
    if (layout == nullptr) throw WRuntimeException(L"Shader layout from different backends");
    m_layout = Rc<FD3d12ShaderLayout>::UnsafeClone(layout);

    auto root_signature = static_cast<ID3D12RootSignature*>(m_layout->GetRootSignaturePtr());

    CD3DX12_PIPELINE_STATE_STREAM2 stream{};

    if (HasFlags(m_shader->Stages, FShaderStageFlags::Vertex))
    {
        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
        desc.pRootSignature = root_signature;
        auto ps = m_shader->Pixel();
        auto vs = m_shader->Vertex();
        desc.PS = D3D12_SHADER_BYTECODE{ps->Data, ps->Size};
        desc.VS = D3D12_SHADER_BYTECODE{vs->Data, vs->Size};
        SetGraphicsState(desc, options);
        // todo input layout
        stream = CD3DX12_PIPELINE_STATE_STREAM2(desc);
    }
    else if (HasFlags(m_shader->Stages, FShaderStageFlags::Mesh))
    {
        D3DX12_MESH_SHADER_PIPELINE_STATE_DESC desc{};
        desc.pRootSignature = root_signature;
        auto ps = m_shader->Pixel();
        auto ms = m_shader->Mesh();
        auto ts = m_shader->Task();
        desc.PS = D3D12_SHADER_BYTECODE{ps->Data, ps->Size};
        desc.MS = D3D12_SHADER_BYTECODE{ms->Data, ms->Size};
        if (ts != nullptr)
            desc.AS = D3D12_SHADER_BYTECODE{ts->Data, ts->Size};
        SetGraphicsState(desc, options);
        stream = CD3DX12_PIPELINE_STATE_STREAM2(desc);
    }
    else throw WRuntimeException(L"Unknown shader stage combination");

    D3D12_PIPELINE_STATE_STREAM_DESC stream_desc{};
    stream_desc.SizeInBytes = sizeof(stream);
    stream_desc.pPipelineStateSubobjectStream = &stream;
    chr | m_dx_device->CreatePipelineState(&stream_desc, IID_PPV_ARGS(&m_pipeline));

    if (m_device->Debug())
    {
        chr | m_pipeline >> SetNameEx(options.Name);
    }
}

FResult D3d12GraphicsShaderPipeline::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        chr | m_pipeline >> SetNameEx(name);
    });
}
