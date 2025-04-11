#include "ComputePipeline.h"

using namespace Coplt;

D3d12ComputeShaderPipeline::D3d12ComputeShaderPipeline(Rc<D3d12GpuDevice>&& device, const FShaderPipelineCreateOptions& options)
    : m_device(std::move(device))
{
    if (options.Shader == nullptr)
        COPLT_THROW("Shader is null");
    m_shader = Rc<FShader>::UnsafeClone(options.Shader);
    if (!HasAnyFlags(m_shader->Stages(), FShaderStageFlags::Compute)) // | FShaderStageFlags::Mesh
        COPLT_THROW("The shader is not compute");

    if (m_shader->Layout())
    {
        auto layout = NonNull(options.Layout)->QueryInterface<ID3d12BindingLayout>();
        if (layout == nullptr)
            COPLT_THROW("Shader layout from different backends");
        m_layout = Rc<ID3d12BindingLayout>::UnsafeClone(layout);
        if (m_layout->ShaderLayout()->ObjectId() != m_shader->Layout()->ObjectId())
            COPLT_THROW("Differences between binding layout and shader layout");
    }
    else
    {
        m_layout = m_device->GetEmptyBindingLayout(FShaderLayoutFlags::None);
    }

    auto root_signature = m_layout->RootSignature().Get();

    CD3DX12_PIPELINE_STATE_STREAM2 stream{};
    if (HasFlags(m_shader->Stages(), FShaderStageFlags::Compute))
    {
        D3D12_COMPUTE_PIPELINE_STATE_DESC desc{};
        desc.pRootSignature = root_signature;
        NonNull cs = m_shader->Compute();
        desc.CS = D3D12_SHADER_BYTECODE{cs->Data().Data, cs->Data().Size};
        stream = CD3DX12_PIPELINE_STATE_STREAM2(desc);
    }
    // else if (HasFlags(m_shader->Stages(), FShaderStageFlags::Mesh)) // 当前不确定是否应该在计算中支持 Mesh Shader
    // {
    //     D3DX12_MESH_SHADER_PIPELINE_STATE_DESC desc{};
    //     desc.pRootSignature = root_signature;
    //     NonNull ms = m_shader->Mesh();
    //     auto ts = m_shader->Task();
    //     desc.MS = D3D12_SHADER_BYTECODE{ms->Data().Data, ms->Data().Size};
    //     if (ts != nullptr)
    //         desc.AS = D3D12_SHADER_BYTECODE{ts->Data().Data, ts->Data().Size};
    //     stream = CD3DX12_PIPELINE_STATE_STREAM2(desc);
    // }
    else
        COPLT_THROW("Unknown shader stage combination");

    D3D12_PIPELINE_STATE_STREAM_DESC stream_desc{};
    stream_desc.SizeInBytes = sizeof(stream);
    stream_desc.pPipelineStateSubobjectStream = &stream;
    chr | m_device->m_device->CreatePipelineState(&stream_desc, IID_PPV_ARGS(&m_pipeline));

    if (m_device->Debug())
    {
        chr | m_pipeline >> SetNameEx(options.Name);
    }
}

FResult D3d12ComputeShaderPipeline::SetName(const FStr8or16& name) noexcept
{
    return feb([&]
    {
        if (!m_device->Debug()) return;
        chr | m_pipeline >> SetNameEx(name);
    });
}

const Rc<ID3d12BindingLayout>& D3d12ComputeShaderPipeline::Layout() const noexcept
{
    return m_layout;
}

const ComPtr<ID3D12PipelineState>& D3d12ComputeShaderPipeline::GetPipelineState() const noexcept
{
    return m_pipeline;
}

FShader* D3d12ComputeShaderPipeline::GetShader() noexcept
{
    return m_shader.get();
}

FBindingLayout* D3d12ComputeShaderPipeline::GetLayout() noexcept
{
    return m_layout.get();
}

FShaderStageFlags D3d12ComputeShaderPipeline::GetStages() noexcept
{
    return m_shader->Stages();
}
