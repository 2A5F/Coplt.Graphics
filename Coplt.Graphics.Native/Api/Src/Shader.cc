#include "Shader.h"

#include "mimalloc.h"
#include "../Include/AllocObjectId.h"
#include "../Include/Error.h"

using namespace Coplt;

ShaderModule::ShaderModule(u8* const data, const size_t size, const FShaderStage stage, Rc<FString8>&& entry_point)
    : m_entry_point(std::move(entry_point))
{
    FShaderModuleData::Data = data;
    Size = size;
    Stage = stage;
}

ShaderModule* ShaderModule::Create(const FShaderModuleCreateOptions& options)
{
    auto mem = static_cast<char*>(mi_malloc_aligned(sizeof(Self) + options.Size + 4, alignof(Self)));
    const auto start = reinterpret_cast<u8*>(mem + sizeof(Self));
    memcpy(start, options.Data, options.Size);
    *reinterpret_cast<u32*>(&start[options.Size]) = 0;
    new(mem) Self(start, options.Size, options.Stage, Rc<FString8>::UnsafeClone(options.EntryPoint));
    return reinterpret_cast<Self*>(mem);
}

void ShaderModule::Free(void* self)
{
    mi_free_aligned(self, alignof(Self));
}

FResult ShaderModule::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FShaderModuleData* ShaderModule::ShaderModuleData() noexcept
{
    return this;
}

FString8* ShaderModule::GetEntryPoint() noexcept
{
    return m_entry_point.get();
}

Shader::Shader(Rc<FGpuDevice>&& device, const FShaderCreateOptions& options) : m_device(std::move(device))
{
    m_layout = Rc<FShaderLayout>::UnsafeClone(options.Layout);
    m_input_layout = Rc<FShaderInputLayout>::UnsafeClone(options.InputLayout);

    if (options.Count == 0)
        COPLT_THROW("options.Count cannot be 0");
    if (options.Count > MaxShaderModuleCount)
    {
        COPLT_THROW_FMT(
            "Too many shader modules, there can be a maximum of {}", MaxShaderModuleCount
        );
    }
    if (options.Modules == nullptr)
        COPLT_THROW("options.Modules is null");

    for (u32 i = 0; i < options.Count; i++)
    {
        const auto module = options.Modules[i];
        if (module == nullptr)
            COPLT_THROW_FMT("options.Modules[{}] is null", i);
        switch (module->Data().Stage)
        {
        case FShaderStage::Compute:
            if (options.Count != 1)
                COPLT_THROW("The compute stage cannot be combined with other stage");
            FShaderData::Stages = FShaderStageFlags::Compute;
            m_modules[0] = Rc<FShaderModule>::UnsafeClone(module);
            return;
        case FShaderStage::Pixel:
            if (options.Count == 1)
                COPLT_THROW("The pixel stage cannot exist alone");
            FShaderData::Stages |= FShaderStageFlags::Pixel;
            m_modules[0] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        case FShaderStage::Vertex:
            if (HasAnyFlags(FShaderData::Stages, FShaderStageFlags::Mesh | FShaderStageFlags::Task))
                COPLT_THROW("The vertex stage and the mesh/task stage are mutually exclusive");
            FShaderData::Stages |= FShaderStageFlags::Vertex;
            m_modules[1] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        case FShaderStage::Mesh:
            if (HasFlags(FShaderData::Stages, FShaderStageFlags::Vertex))
                COPLT_THROW("The vertex stage and the mesh/task stage are mutually exclusive");
            FShaderData::Stages |= FShaderStageFlags::Mesh;
            m_modules[1] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        case FShaderStage::Task:
            if (HasFlags(FShaderData::Stages, FShaderStageFlags::Vertex))
                COPLT_THROW("The vertex stage and the mesh/task stage are mutually exclusive");
            FShaderData::Stages |= FShaderStageFlags::Task;
            m_modules[2] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        default:
            COPLT_THROW_FMT(
                "Unknown shader stage {}", static_cast<u32>(module->Data().Stage)
            );
        }
    }

    if (HasFlags(FShaderData::Stages, FShaderStageFlags::Task) && !HasFlags(FShaderData::Stages, FShaderStageFlags::Mesh))
        COPLT_THROW("Having a task stage must also have a mesh stage");
}

FResult Shader::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}

FShaderData* Shader::ShaderData() noexcept
{
    return this;
}

FShaderLayout* Shader::Layout() noexcept
{
    return m_layout.get();
}

FShaderInputLayout* Shader::InputLayout() noexcept
{
    return m_input_layout.get();
}

FShaderModule* Shader::Compute() noexcept
{
    return HasFlags(FShaderData::Stages, FShaderStageFlags::Compute) ? m_modules[0].get() : nullptr;
}

FShaderModule* Shader::Pixel() noexcept
{
    return HasFlags(FShaderData::Stages, FShaderStageFlags::Pixel) ? m_modules[0].get() : nullptr;
}

FShaderModule* Shader::Vertex() noexcept
{
    return HasFlags(FShaderData::Stages, FShaderStageFlags::Vertex) ? m_modules[1].get() : nullptr;
}

FShaderModule* Shader::Mesh() noexcept
{
    return HasFlags(FShaderData::Stages, FShaderStageFlags::Mesh) ? m_modules[1].get() : nullptr;
}

FShaderModule* Shader::Task() noexcept
{
    return HasFlags(FShaderData::Stages, FShaderStageFlags::Task) ? m_modules[2].get() : nullptr;
}
