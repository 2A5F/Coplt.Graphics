#pragma once

#include "GpuObject.h"
#include "Queue.h"

namespace Coplt
{
    struct FGpuRecord;
    struct FGpuRecordCreateOptions;
    struct FGpuRecordCreateResult;

    struct FGpuIsolateCreateOptions
    {
        // 可选
        FStr8or16 Name{};
    };

    struct FGpuIsolateData
    {
    };

    COPLT_INTERFACE_DEFINE(FGpuIsolate, "777c5774-8eb8-4550-a977-62cccd7bdda6", FGpuObject)
    {
        // 返回的 FGpuQueueCreateResult 不会增加引用计数
        virtual FGpuQueueCreateResult* GetQueues(u32* OutNumQueues) noexcept = 0;
        virtual FGpuIsolateData* GpuIsolateData() noexcept = 0;

        virtual FResult CreateRecord(const FGpuRecordCreateOptions& options, FGpuRecordCreateResult& out) noexcept = 0;

        virtual FResult CreateSwapChainFromExists(const FGpuSwapChainFromExistsCreateOptions& options, FGpuSwapChainCreateResult& out) noexcept = 0;
        // 在非 windows 系统上会报错
        virtual FResult CreateSwapChainForHwnd(const FGpuSwapChainCreateOptions& options, void* hwnd, FGpuSwapChainCreateResult& out) noexcept = 0;
    };

    struct FMainQueueCreateResult
    {
        FGpuIsolate* Isolate{};
        FGpuIsolateData* Data{};
        FGpuQueueCreateResult* Queues{};
        u32 NumQueues{};
    };
}
