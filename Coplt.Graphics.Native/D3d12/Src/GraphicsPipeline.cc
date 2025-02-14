#include "GraphicsPipeline.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "../ThirdParty/DirectXTK12/Src/d3dx12.h"
#include "../Include/PipelineState.h"
#include "../Include/TextureFormat.h"

using namespace Coplt;

namespace
{
    void StencilState(D3D12_DEPTH_STENCILOP_DESC& dst, const FStencilState& src)
    {
        dst.StencilFailOp = ToDx(src.Fail);
        dst.StencilDepthFailOp = ToDx(src.DepthFail);
        dst.StencilPassOp = ToDx(src.Pass);
        dst.StencilFunc = ToDx(src.Func);
    }

    void SetGraphicsState(auto& dst, const FGraphicsShaderPipelineCreateOptions& options)
    {
        const auto& src = options.GraphicsState;
        dst.BlendState.AlphaToCoverageEnable = src.MultiSample.AlphaToCoverageEnable;
        dst.BlendState.IndependentBlendEnable = src.BlendState.IndependentBlendEnable;
        for (u8 i = 0; i < 8; ++i)
        {
            const auto& rt_src = src.BlendState.Rts[i];
            auto& rt_dst = dst.BlendState.RenderTarget[i];
            rt_dst.BlendEnable = rt_src.Op != FBlendOp::None;
            rt_dst.LogicOpEnable = rt_src.LogicOp != FLogicOp::None;
            rt_dst.SrcBlend = ToDx(rt_src.Src);
            rt_dst.DestBlend = ToDx(rt_src.Dst);
            rt_dst.BlendOp = ToDx(rt_src.Op);
            rt_dst.SrcBlendAlpha = ToDx(rt_src.AlphaSrc);
            rt_dst.DestBlendAlpha = ToDx(rt_src.AlphaDst);
            rt_dst.BlendOpAlpha = ToDx(rt_src.AlphaOp);
            rt_dst.LogicOp = ToDx(rt_src.LogicOp);
            rt_dst.RenderTargetWriteMask = rt_src.WriteMask;
        }
        dst.SampleMask = src.MultiSample.SampleMask;
        dst.SampleDesc.Count = std::max(static_cast<u32>(src.MultiSample.SampleCount), 1u);
        dst.SampleDesc.Quality = src.MultiSample.SampleCount > 1 ? 1 : 0;
        dst.RasterizerState.FillMode = ToDx(src.RasterizerState.Fill);
        dst.RasterizerState.CullMode = ToDx(src.RasterizerState.Cull);
        dst.RasterizerState.FrontCounterClockwise = src.RasterizerState.FrontFace == FRotationDirection::ClockWise;
        dst.RasterizerState.DepthBias = src.RasterizerState.DepthBias;
        dst.RasterizerState.DepthBiasClamp = src.RasterizerState.DepthBiasClamp;
        dst.RasterizerState.SlopeScaledDepthBias = src.RasterizerState.SlopeScaledDepthBias;
        dst.RasterizerState.DepthClipEnable = src.RasterizerState.DepthClip;
        dst.RasterizerState.MultisampleEnable =
            src.RasterizerState.AlphaAALine ? false : src.MultiSample.SampleCount > 0;
        dst.RasterizerState.AntialiasedLineEnable = src.RasterizerState.AlphaAALine;
        dst.RasterizerState.ForcedSampleCount = 0;
        dst.RasterizerState.ConservativeRaster =
            src.RasterizerState.ConservativeRaster
                ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON
                : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        dst.DepthStencilState.DepthEnable = src.DepthStencilState.Enable;
        dst.DepthStencilState.DepthWriteMask =
            src.DepthStencilState.DepthWrite ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
        dst.DepthStencilState.DepthFunc = ToDx(src.DepthStencilState.DepthFunc);
        dst.DepthStencilState.StencilEnable = src.DepthStencilState.StencilEnable;
        dst.DepthStencilState.StencilReadMask = src.DepthStencilState.StencilReadMask;
        dst.DepthStencilState.StencilWriteMask = src.DepthStencilState.StencilWriteMask;
        StencilState(dst.DepthStencilState.FrontFace, src.DepthStencilState.Front);
        StencilState(dst.DepthStencilState.BackFace, src.DepthStencilState.Back);
        dst.PrimitiveTopologyType = ToDx(src.Topology);
        dst.NumRenderTargets = src.NumRts;
        for (u8 i = 0; i < 8; ++i)
        {
            dst.RTVFormats[i] = ToDx(src.RtvFormats[i]);
        }
        dst.DSVFormat = ToDx(src.DsvFormat);
    }

    void SetInputLayout(
        const D3d12GpuDevice* device,
        std::vector<std::string>& tmp_strings,
        std::vector<D3D12_INPUT_ELEMENT_DESC>& dst,
        std::vector<u32>& input_slots,
        FD3d12ShaderInputLayout* input_layout,
        FD3d12MeshLayout* mesh_layout
    )
    {
        const auto buffers = mesh_layout->GetBuffers();
        const auto elements = input_layout->GetElements();
        input_slots = std::vector<u32>(elements.size(), COPLT_U32_MAX);
        for (usize i = 0; i < elements.size(); ++i)
        {
            const auto& element = elements[i];
            D3D12_INPUT_ELEMENT_DESC desc{};
            if (const auto buffer_element = mesh_layout->TryGetElement(element.SlotId, element.SlotIndex))
            {
                const auto& buffer = buffers[buffer_element->BufferIndex];
                desc.InputSlot = input_slots[i] = buffer_element->BufferIndex;
                desc.Format = ToDx(buffer_element->Format);
                desc.AlignedByteOffset = buffer_element->Offset;
                desc.InputSlotClass = ToDx(buffer.Rate);
                desc.InstanceDataStepRate = buffer_element->InstanceRepeat;
            }
            else
            {
                if (device->Debug() && device->Logger().IsEnable(FLogLevel::Warn))
                {
                    device->Logger().LogW(
                        FLogLevel::Warn,
                        fmt::format(
                            L"The input element {} ignored, because mesh layout does not provide a suitable element", i
                        )
                    );
                }
                continue;
            }
            desc.SemanticIndex = element.SlotIndex;
            if (element.SlotName != nullptr)
            {
                desc.SemanticName = reinterpret_cast<const char*>(element.SlotName->m_data);
            }
            else
            {
                tmp_strings.push_back(fmt::format("SLOT{}_", element.SlotId));
                desc.SemanticName = tmp_strings.back().c_str();
                if (device->Debug() && device->Logger().IsEnable(FLogLevel::Warn))
                {
                    device->Logger().LogW(
                        FLogLevel::Warn,
                        fmt::format(L"The SlotName of input element {} is null, which is required in the dx backend", i)
                    );
                }
            }
            dst.push_back(desc);
        }
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

    if (auto layout = m_shader->Layout())
    {
        auto dx_layout = layout->QueryInterface<FD3d12ShaderLayout>();
        if (dx_layout == nullptr) throw WRuntimeException(L"Shader layout from different backends");
        m_layout = Rc<FD3d12ShaderLayout>::UnsafeClone(dx_layout);
    }
    else
    {
        auto flags = FShaderLayoutFlags::BindLess;
        if (m_shader->InputLayout())
        {
            flags |= FShaderLayoutFlags::InputAssembler;
        }
        m_layout = m_device->GetEmptyLayout(flags);
    }

    m_graphics_state = options.GraphicsState;

    auto root_signature = static_cast<ID3D12RootSignature*>(m_layout->GetRootSignaturePtr());

    std::vector<std::string> tmp_strings{};
    std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_desc{};
    std::vector<u32> input_slots{};
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
        desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;
        desc.Flags = D3D12_PIPELINE_STATE_FLAG_DYNAMIC_DEPTH_BIAS |
            D3D12_PIPELINE_STATE_FLAG_DYNAMIC_INDEX_BUFFER_STRIP_CUT;
        if (auto input_layout = m_shader->InputLayout())
        {
            auto dx_input_layout = input_layout->QueryInterface<FD3d12ShaderInputLayout>();
            if (dx_input_layout == nullptr) throw WRuntimeException(L"Input layout from different backends");
            if (options.MeshLayout == nullptr)
                throw WRuntimeException(L"The shader requires a mesh input but no mesh layout was provided");
            auto dx_mesh_layout = options.MeshLayout->QueryInterface<FD3d12MeshLayout>();
            if (dx_mesh_layout == nullptr) throw WRuntimeException(L"Mesh layout from different backends");
            SetInputLayout(&m_device, tmp_strings, input_element_desc, input_slots, dx_input_layout, dx_mesh_layout);
            desc.InputLayout.NumElements = input_element_desc.size();
            desc.InputLayout.pInputElementDescs = input_element_desc.data();
            m_input_layout = Rc<FD3d12ShaderInputLayout>::UnsafeClone(dx_input_layout);
            m_mesh_layout = Rc<FD3d12MeshLayout>::UnsafeClone(dx_mesh_layout);
        }
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
        desc.Flags = D3D12_PIPELINE_STATE_FLAG_DYNAMIC_DEPTH_BIAS;
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

    m_input_slots = std::move(input_slots);
}

FResult D3d12GraphicsShaderPipeline::SetName(const Str8or16& name) noexcept
{
    return feb([&]
    {
        chr | m_pipeline >> SetNameEx(name);
    });
}
