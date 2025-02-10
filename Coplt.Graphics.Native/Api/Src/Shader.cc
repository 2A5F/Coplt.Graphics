#include "Shader.h"

#include "mimalloc.h"
#include "../Include/Error.h"

using namespace Coplt;

ShaderModule::ShaderModule(u8* const data, const size_t size, const FShaderStage stage)
{
    Data = data;
    Size = size;
    Stage = stage;
}

ShaderModule* ShaderModule::Create(const FShaderModuleCreateOptions& options)
{
    auto mem = static_cast<char*>(mi_malloc_aligned(sizeof(Self) + options.Size + 4, alignof(Self)));
    const auto start = reinterpret_cast<u8*>(mem + sizeof(Self));
    memcpy(start, options.Data, options.Size);
    *reinterpret_cast<u32*>(&start[options.Size]) = 0;
    new(mem) Self(start, options.Size, options.Stage);
    return reinterpret_cast<Self*>(mem);
}

void ShaderModule::Free(void* self)
{
    mi_free_aligned(self, alignof(Self));
}

FResult ShaderModule::SetName(const Str8or16& name) noexcept
{
    return FResult::None();
}

Shader::Shader(const FShaderCreateOptions& options)
{
    if (options.Layout == nullptr) throw RuntimeException("options.Layout is null");
    m_layout = Rc<FShaderLayout>::UnsafeClone(options.Layout);

    if (options.Count == 0) throw RuntimeException("options.Count cannot be 0");
    if (options.Count > MaxShaderModuleCount)
        throw RuntimeException(
            fmt::format("Too many shader modules, there can be a maximum of {}", MaxShaderModuleCount)
        );
    if (options.Modules == nullptr) throw RuntimeException("options.Modules is null");

    for (u32 i = 0; i < options.Count; i++)
    {
        const auto module = options.Modules[i];
        if (module == nullptr) throw RuntimeException(fmt::format("options.Modules[{}] is null", i));
        switch (module->Stage)
        {
        case FShaderStage::Compute:
            if (options.Count != 1) throw RuntimeException("The compute stage cannot be combined with other stage");
            Stages = FShaderStageFlags::Compute;
            m_modules[0] = Rc<FShaderModule>::UnsafeClone(module);
            return;
        case FShaderStage::Pixel:
            if (options.Count == 1) throw RuntimeException("The pixel stage cannot exist alone");
            Stages |= FShaderStageFlags::Pixel;
            m_modules[0] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        case FShaderStage::Vertex:
            if (HasAnyFlags(Stages, FShaderStageFlags::Mesh | FShaderStageFlags::Task))
                throw RuntimeException("The vertex stage and the mesh/task stage are mutually exclusive");
            Stages |= FShaderStageFlags::Vertex;
            m_modules[1] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        case FShaderStage::Mesh:
            if (HasFlags(Stages, FShaderStageFlags::Vertex))
                throw RuntimeException("The vertex stage and the mesh/task stage are mutually exclusive");
            Stages |= FShaderStageFlags::Mesh;
            m_modules[1] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        case FShaderStage::Task:
            if (HasFlags(Stages, FShaderStageFlags::Vertex))
                throw RuntimeException("The vertex stage and the mesh/task stage are mutually exclusive");
            Stages |= FShaderStageFlags::Task;
            m_modules[2] = Rc<FShaderModule>::UnsafeClone(module);
            break;
        default:
            throw RuntimeException(
                fmt::format("Unknown shader stage {}", static_cast<u32>(module->Stage))
            );
        }
    }

    if (HasFlags(Stages, FShaderStageFlags::Task) && !HasFlags(Stages, FShaderStageFlags::Mesh))
        throw RuntimeException("Having a task stage must also have a mesh stage");
}

FResult Shader::SetName(const Str8or16& name) noexcept
{
    return FResult::None();
}

FShaderLayout* Shader::Layout() noexcept
{
    return m_layout.get();
}

FShaderModule* Shader::Compute() noexcept
{
    return HasFlags(Stages, FShaderStageFlags::Compute) ? m_modules[0].get() : nullptr;
}

FShaderModule* Shader::Pixel() noexcept
{
    return HasFlags(Stages, FShaderStageFlags::Pixel) ? m_modules[0].get() : nullptr;
}

FShaderModule* Shader::Vertex() noexcept
{
    return HasFlags(Stages, FShaderStageFlags::Vertex) ? m_modules[1].get() : nullptr;
}

FShaderModule* Shader::Mesh() noexcept
{
    return HasFlags(Stages, FShaderStageFlags::Mesh) ? m_modules[1].get() : nullptr;
}

FShaderModule* Shader::Task() noexcept
{
    return HasFlags(Stages, FShaderStageFlags::Task) ? m_modules[2].get() : nullptr;
}
