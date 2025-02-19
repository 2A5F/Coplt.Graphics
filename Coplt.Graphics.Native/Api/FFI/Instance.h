#pragma once

#include "Blob.h"
#include "Logger.h"
#include "Result.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(FAllocator, "534b3c0c-098d-4157-8add-9b79eb5d690f", FUnknown)
    {
        virtual void* MemoryAlloc(size_t size) noexcept = 0;
        virtual void* MemoryAlloc(size_t size, size_t align) noexcept = 0;
        virtual void* MemoryReAlloc(void* p, size_t size) noexcept = 0;
        virtual void* MemoryReAlloc(void* p, size_t size, size_t align) noexcept = 0;
        virtual void MemoryFree(void* p) noexcept = 0;
        virtual void MemoryFree(void* p, size_t align) noexcept = 0;
    };

    struct FGpuDeviceCreateOptions;
    struct FGpuDevice;

    COPLT_INTERFACE_DEFINE(FInstance, "cc2894ba-57c7-474a-b777-1a3e3a7c922c", FUnknown)
    {
        // 外部只能读取
        FAllocator* m_allocator{};

        virtual FResult SetLogger(const FLogger& logger) noexcept = 0;

        virtual FResult CreateString8(const Char8* data, usize size, FString8** out) noexcept = 0;
        virtual FResult CreateString16(const Char16* data, usize size, FString16** out) noexcept = 0;

        virtual FResult CreateBlob(usize size, FBlob** out) noexcept = 0;

        virtual FResult CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDevice** out) noexcept = 0;
    };
}
