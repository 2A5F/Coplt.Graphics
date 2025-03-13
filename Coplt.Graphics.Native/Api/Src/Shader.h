#pragma once

#include "../FFI/Device.h"
#include "../FFI/Shader.h"
#include "../FFI/Layout.h"
#include "../Include/Object.h"

namespace Coplt
{
    struct ShaderModule final : Object<ShaderModule, FShaderModule>
    {
    private:
        using Self = ShaderModule;

        explicit ShaderModule(u8* data, size_t size, FShaderStage stage, Rc<FString8>&& entry_point);

    public:
        // 仅用于计算 Hash，保证唯一, 自增 Id u64 基本不可能耗尽
        u64 m_object_id{};
        // 可空
        Rc<FString8> m_entry_point{};

        static ShaderModule* Create(const FShaderModuleCreateOptions& options);

        void Free(void* self) override;

        u64 ObjectId() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;

        // 可空
        FString8* GetEntryPoint() noexcept override;
    };

    struct Shader final : Object<Shader, FShader>
    {
        // 仅用于计算 Hash，保证唯一, 自增 Id u64 基本不可能耗尽
        u64 m_object_id{};
        Rc<FGpuDevice> m_device{};
        // 可空
        Rc<FShaderLayout> m_layout{};
        // 可空
        Rc<FShaderInputLayout> m_input_layout{};
        // 可能的组合：[Compute] | [Pixel, Vertex] | [Pixel, Mesh, Task?]
        Rc<FShaderModule> m_modules[MaxShaderModuleCount]{};

        explicit Shader(Rc<FGpuDevice>&& device, const FShaderCreateOptions& options);

        u64 ObjectId() noexcept override;
        FResult SetName(const FStr8or16& name) noexcept override;

        FShaderLayout* Layout() noexcept override;

        FShaderInputLayout* InputLayout() noexcept override;

        FShaderModule* Compute() noexcept override;
        FShaderModule* Pixel() noexcept override;
        FShaderModule* Vertex() noexcept override;
        FShaderModule* Mesh() noexcept override;
        FShaderModule* Task() noexcept override;
    };
}
