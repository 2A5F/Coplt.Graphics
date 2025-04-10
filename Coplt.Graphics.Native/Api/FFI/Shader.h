#pragma once

#ifdef FFI_SRC
#include <bit>
#endif

#include "GpuObject.h"
#include "PipelineState.h"

namespace Coplt
{
    enum class FShaderStage : u8
    {
        Compute = 0,
        Pixel = 1,
        Vertex = 2,
        Mesh = 3,
        Task = 4,
    };

    COPLT_ENUM_FLAGS(FShaderStageFlags, u32)
    {
        None = 0,
        Compute = 1 << 0,
        Pixel = 1 << 1,
        Vertex = 1 << 2,
        Mesh = 1 << 3,
        Task = 1 << 4,
    };

    struct FShaderModuleCreateOptions
    {
        FStr8or16 Name{};
        // 可选
        FString8* EntryPoint{};
        // 需要自己保证数据内容符合当前使用的后端，比如 dx 使用的 dxil，vk 使用的 spv
        void* Data{};
        usize Size{};
        FShaderStage Stage{};
    };

    struct FShaderModuleData
    {
        // 外部只读
        u8* Data{};
        // 外部只读
        usize Size{};
        // 外部只读
        FShaderStage Stage{};
    };

    COPLT_INTERFACE_DEFINE(FShaderModule, "5c0e1fdb-2acd-4fce-b985-09e12a7a1aad", FGpuObject)
    {
        virtual FShaderModuleData* ShaderModuleData() noexcept = 0;
        // 可空
        virtual FString8* GetEntryPoint() noexcept = 0;

        #ifdef FFI_SRC

        FShaderModuleData& Data() noexcept
        {
            return *ShaderModuleData();
        }

        #endif
    };

    struct FShaderModuleCreateResult
    {
        FShaderModule* ShaderModule{};
        FShaderModuleData* Data{};
    };

    struct FShaderLayout;
    struct FShaderInputLayout;

    struct FShaderCreateOptions
    {
        FStr8or16 Name{};
        // 此处是借用，不传递所有权
        FShaderLayout* Layout{};
        // 此处是借用，不传递所有权
        FShaderInputLayout* InputLayout{};
        // 此处是借用，不传递所有权
        FShaderModule** Modules{};
        u8 Count{};
    };

    struct FShaderData
    {
        FShaderStageFlags Stages{};
    };

    COPLT_INTERFACE_DEFINE(FShader, "de1352d5-023d-42b0-beac-122b3b296c9c", FGpuObject)
    {
        constexpr static u32 MaxShaderModuleCount = 3;

        virtual FShaderData* ShaderData() noexcept = 0;

        virtual FShaderLayout* Layout() noexcept = 0;

        virtual FShaderInputLayout* InputLayout() noexcept = 0;

        // 没有返回 null
        virtual FShaderModule* Compute() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Pixel() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Vertex() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Mesh() noexcept = 0;
        // 没有返回 null
        virtual FShaderModule* Task() noexcept = 0;

        #ifdef FFI_SRC

        FShaderData& Data() noexcept
        {
            return *ShaderData();
        }

        FShaderStageFlags& Stages() noexcept
        {
            return Data().Stages;
        }

        #endif
    };

    struct FShaderCreateResult
    {
        FShader* Shader{};
        FShaderData* Data{};
    };

    #ifdef FFI_SRC
    struct StageIterator final
    {
        FShaderStageFlags m_flags{};

        StageIterator() = default;

        explicit StageIterator(const FShaderStageFlags flags) noexcept : m_flags(flags)
        {
        }

        bool operator!=(const StageIterator& rhs) const noexcept
        {
            return m_flags != rhs.m_flags;
        }

        StageIterator& operator++() noexcept
        {
            m_flags &= static_cast<FShaderStageFlags>(static_cast<u32>(m_flags) - 1);
            return *this;
        }

        FShaderStage operator*() const noexcept
        {
            return static_cast<FShaderStage>(std::countr_zero(static_cast<u32>(m_flags)));
        }
    };

    struct IterStage final
    {
        FShaderStageFlags const m_flags{};

        IterStage() = default;

        explicit IterStage(const FShaderStageFlags flags) noexcept : m_flags(flags)
        {
        }

        StageIterator begin() const noexcept
        {
            return StageIterator(m_flags);
        }

        StageIterator end() const noexcept
        {
            return StageIterator(FShaderStageFlags::None);
        }
    };
    #endif
}
