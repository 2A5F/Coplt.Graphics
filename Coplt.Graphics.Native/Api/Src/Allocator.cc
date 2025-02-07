#include "Allocator.h"

#include "mimalloc.h"

using namespace Coplt;

void* Allocator::MemoryAlloc(size_t size) noexcept
{
    return mi_malloc(size);
}

void* Allocator::MemoryAlloc(size_t size, size_t align) noexcept
{
    return mi_malloc_aligned(size, align);
}

void* Allocator::MemoryReAlloc(void* p, size_t size) noexcept
{
    return mi_realloc(p, size);
}

void* Allocator::MemoryReAlloc(void* p, size_t size, size_t align) noexcept
{
    return mi_realloc_aligned(p, size, align);
}

void Allocator::MemoryFree(void* p) noexcept
{
    mi_free(p);
}

void Allocator::MemoryFree(void* p, size_t align) noexcept
{
    mi_free_aligned(p, align);
}
