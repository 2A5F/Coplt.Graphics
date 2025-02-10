#pragma once

#include "../FFI/Shader.h"
#include "../FFI/Layout.h"
#include "../Include/Object.h"

namespace Coplt
{
    struct ShaderModule final : Object<ShaderModule, FShaderModule>
    {
    private:
        using Self = ShaderModule;

        explicit ShaderModule(u8* data, size_t size, FShaderStage stage);

    public:
        static ShaderModule* Create(const FShaderModuleCreateOptions& options);

        void Free(void* self) override;

        FResult SetName(const Str8or16& name) noexcept override;
    };

    struct Shader final : Object<Shader, FShader>
    {
        constexpr static u32 MaxShaderModuleCount = 3;

        Rc<FShaderLayout> m_layout{};
        // 可能的组合：[Compute] | [Pixel, Vertex] | [Pixel, Mesh, Task?]
        Rc<FShaderModule> m_modules[3]{};

        explicit Shader(const FShaderCreateOptions& options);

        FResult SetName(const Str8or16& name) noexcept override;

        FShaderLayout* Layout() noexcept override;

        FShaderModule* Compute() noexcept override;
        FShaderModule* Pixel() noexcept override;
        FShaderModule* Vertex() noexcept override;
        FShaderModule* Mesh() noexcept override;
        FShaderModule* Task() noexcept override;
    };
}
