#pragma once

#include "GpuObject.h"
#include "Output.h"
#include "Queue.h"

namespace Coplt
{
    struct FGpuRecord;
    struct FGpuRecordCreateResult;

    enum class FGpuIsolateType
    {
        // 包含 Direct Compute Copy 队列，最多同时具有这3个队列
        // 可以提交异步任务自动调度到队列上，同时如果不支持多队列将回退到主 Direct 队列
        Main,
        // 仅包含 Copy 队列，不一定有
        Copy,
        // todo video
    };

    struct FGpuIsolateCreateOptions
    {
        // 可选
        FStr8or16 Name{};
        FGpuIsolateType Type{};
    };

    struct FGpuIsolateConfig
    {
        // 提示可能会使用多线程命令录制，将在命令录制的 End 中完成转义操作，以分摊提交开销，
        // 但是这会导致 gpu 屏障处于非最优状态，可能导致提交批次开始时的 gpu 管线停转，建议只在具有大量命令时使用多线程录制
        b8 MultiThreadRecord{};
        // 仅 d3d12，提示尽可能使用拆分屏障
        b8 UseSplitBarrier{};
    };

    struct FGpuIsolateData
    {
        u64 FrameId{};
        FGpuIsolateConfig* Config{};
    };

    COPLT_INTERFACE_DEFINE(FGpuIsolate, "777c5774-8eb8-4550-a977-62cccd7bdda6", FGpuObject)
    {
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
        virtual FResult CreateSwapChainForComposition(const FGpuSwapChainCreateOptions& options, FGpuSwapChainCreateResult& out) noexcept = 0;
        // 在非 windows 系统上会报错
        virtual FResult CreateSwapChainForHwnd(const FGpuSwapChainCreateOptions& options, void* hwnd, FGpuSwapChainCreateResult& out) noexcept = 0;
        // 在非 windows 系统上会报错
        virtual FResult CreateSwapChainForCoreWindow(const FGpuSwapChainCreateOptions& options, void* w, FGpuSwapChainCreateResult& out) noexcept = 0;
    };

    struct FGpuIsolateCreateResult
    {
        FGpuIsolate* Isolate{};
        FGpuIsolateData* Data{};
    };
}
