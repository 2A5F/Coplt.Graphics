#pragma once

#include "GpuObject.h"
#include "Queue.h"

namespace Coplt
{
    struct FGpuRecord;
    struct FGpuRecordCreateResult;

    struct FGpuIsolateCreateOptions
    {
        // 可选
        FStr8or16 Name{};
    };

    struct FGpuIsolateData
    {
        u64 FrameId{};
    };

    COPLT_INTERFACE_DEFINE(FGpuIsolate, "777c5774-8eb8-4550-a977-62cccd7bdda6", FGpuObject)
    {
        // 返回的 FGpuQueueCreateResult 不会增加引用计数
        virtual FGpuQueueCreateResult* GetQueues(u32* OutNumQueues) noexcept = 0;
        virtual FGpuIsolateData* GpuIsolateData() noexcept = 0;

        // 租用记录
        virtual FResult RentRecords(u32 NumRecords, FGpuRecordCreateResult* OutRecords) noexcept = 0;
        // 归还记录，不会进行提交，记录内容将被丢弃，不要调用归还的 FGpuRecord* 的 Release
        virtual FResult ReturnRecords(u32 NumRecords, FGpuRecord** Records) noexcept = 0;
        // 提交记录，并用返回新的记录用于替换，旧的记录将被拿走加入等待队列
        virtual FResult Submit(u32 NumRecords, FGpuRecord** Records, FGpuRecordCreateResult* OutRecords) noexcept = 0;
        // 提交记录，并归还记录，不要调用归还的 FGpuRecord* 的 Release
        virtual FResult SubmitReturn(u32 NumRecords, FGpuRecord** Records) noexcept = 0;

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
