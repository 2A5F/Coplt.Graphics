#pragma once
#include "../FFI/Instance.h"
#include "../Include/Object.h"

namespace Coplt
{
    struct Allocator final : Object<Allocator, FAllocator>
    {
        void* MemoryAlloc(size_t size) noexcept override;
        void* MemoryAlloc(size_t size, size_t align) noexcept override;
        void* MemoryReAlloc(void* p, size_t size) noexcept override;
        void* MemoryReAlloc(void* p, size_t size, size_t align) noexcept override;
        void MemoryFree(void* p) noexcept override;
        void MemoryFree(void* p, size_t align) noexcept override;
    };
}
