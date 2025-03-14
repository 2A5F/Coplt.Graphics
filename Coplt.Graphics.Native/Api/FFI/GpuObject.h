#pragma once

#include "Object.h"
#include "Result.h"
#include "GraphicsFormat.h"

#ifdef FFI_SRC
#include <concepts>
#endif

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FGpuObject, "9fc6890b-e9a2-4f4a-9217-a14a4223a715", FUnknown)
    {
        // 仅用于计算 Hash，保证唯一, 自增 Id u64 基本不可能耗尽
        virtual u64 ObjectId() const noexcept = 0;
        virtual FResult SetName(const FStr8or16& name) noexcept = 0;
    };

    #ifdef FFI_SRC
    template <class T>
    concept CGpuObject = requires(T* a, const FStr8or16& name)
    {
        { a->ObjectId() } -> std::same_as<u64>;
        { a->SetName(name) } -> std::same_as<FResult>;
    };
    #endif
}
