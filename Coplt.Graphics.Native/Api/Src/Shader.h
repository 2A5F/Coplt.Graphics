#pragma once

#include "../FFI/Device.h"
#include "../FFI/Shader.h"
#include "../FFI/Layout.h"
#include "../Include/GpuObject.h"
#include "../Include/Object.h"

namespace Coplt
{
    struct ShaderModule final : GpuObject<ShaderModule, FShaderModule>, FShaderModuleData
    {
    private:
        using Self = ShaderModule;

        explicit ShaderModule(u8* data, size_t size, FShaderStage stage, Rc<FString8>&& entry_point);

    public:
        // 可空
        Rc<FString8> m_entry_point{};

        static ShaderModule* Create(const FShaderModuleCreateOptions& options);

        void Free(void* self) override;

        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderModuleData* ShaderModuleData() noexcept override;

        // 可空
        FString8* GetEntryPoint() noexcept override;
    };

    struct Shader final : GpuObject<Shader, FShader>, FShaderData
    {
        Rc<FGpuDevice> m_device{};
        // 可空
        Rc<FShaderLayout> m_layout{};
        // 可空
        Rc<FShaderInputLayout> m_input_layout{};
        // 可能的组合：[Compute] | [Pixel, Vertex] | [Pixel, Mesh, Task?]
        Rc<FShaderModule> m_modules[MaxShaderModuleCount]{};

        explicit Shader(Rc<FGpuDevice>&& device, const FShaderCreateOptions& options);

        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderData* ShaderData() noexcept override;

        FShaderLayout* Layout() noexcept override;

        FShaderInputLayout* InputLayout() noexcept override;

        FShaderModule* Compute() noexcept override;
        FShaderModule* Pixel() noexcept override;
        FShaderModule* Vertex() noexcept override;
        FShaderModule* Mesh() noexcept override;
        FShaderModule* Task() noexcept override;
    };
}
