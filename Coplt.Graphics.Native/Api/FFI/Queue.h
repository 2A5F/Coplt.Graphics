#pragma once

#include "GpuObject.h"
#include "Output.h"
#include "Command.h"
#include "Context.h"

namespace Coplt
{
    struct FMainQueueCreateOptions
    {
        // 可选
        FStr8or16 Name{};
    };

    enum class FGpuQueueType : u8
    {
        Direct,
        Compute,
        Copy,
        VideoEncode,
        VideoDecode,
        VideoProcess,
    };

    COPLT_INTERFACE_DEFINE(FGpuQueue, "95e60e28-e387-4055-9b33-2d23af901f8a", FGpuObject)
    {
        // 不安全的内存暴露，外部不能修改，每帧必须保证当前指向可用的帧上下文
        FFrameContext* m_context{};
        // 不安全的内存暴露，外部不能修改，每次提交都会增加计数
        u64 m_submit_id{};
        // 不安全的内存暴露，外部不能修改
        FGpuQueueType m_queue_type{};

        // d3d12 返回 ID3D12CommandQueue*
        virtual void* GetRawQueue() noexcept = 0;

        // swapchain：d3d12 为 IDXGISwapChain3*
        virtual FResult CreateOutputFromRawSwapchain(
            const FGpuOutputFromSwapChainCreateOptions& options,
            void* swapchain,
            FGpuOutput** out
        ) noexcept = 0;

        // 在非 windows 平台调用会报错
        virtual FResult CreateOutputForHwnd(
            const FGpuOutputCreateOptions& options,
            void* hwnd,
            FGpuOutput** out
        ) noexcept = 0;

        // 提交命令并等待执行器可用
        virtual FResult Submit(FGpuExecutor* Executor, const FCommandSubmit* submit) noexcept = 0;
        // 提交命令但是不等待执行器可用，必须先等待才能重新使用此执行器提交
        virtual FResult SubmitNoWait(FGpuExecutor* Executor, const FCommandSubmit* submit) noexcept = 0;
    };

    struct FGpuQueueData
    {
        FGpuQueueType QueueType{};
    };

    COPLT_INTERFACE_DEFINE(FGpuQueue2, "fc94e4d0-5f01-4d4f-894c-6aefaad90499", FGpuObject)
    {
        virtual FGpuQueueData* GpuQueueData() noexcept = 0;
        // d3d12 返回 ID3D12CommandQueue*
        virtual void* GetRawQueue() noexcept = 0;
    };

    struct FGpuQueueCreateResult
    {
        FGpuQueue2* Queue{};
        FGpuQueueData* Data{};
    };
}
