#pragma once

#include "Cmd.h"
#include "GpuObject.h"
#include "Isolate.h"

namespace Coplt
{
    enum class FGpuRecordMode : u8
    {
        Direct = 0,
        Compute = 1,
        Copy = 2,
    };

    struct FGpuRecordData;

    COPLT_INTERFACE_DEFINE(FGpuRecord, "0ef83584-ca65-44de-b38a-648bfb3a85a6", FGpuObject)
    {
        virtual FGpuRecordData* GpuFGpuRecordData() noexcept = 0;

        // 结束录制，并分析命令，可能是耗时任务，如果未调用将在 submit 时自动调用，未重置多次调用将报错
        virtual FResult End() noexcept = 0;
    };

    // 里面的 FList<T*> 的 T* 生命周期由 c# 部分保证
    struct FGpuRecordData
    {
        u64 Id{};
        u64 Version{};
        FRecordContext* Context{};
        FList<FCmdItem> Commands;
        // 需要保证不重复，c# 部分需要使用 HashMap 去重
        FList<FCmdRes> Resources;
        FList<FCmdBinding> Bindings;
        FList<FRect> PayloadRect;
        FList<FViewport> PayloadViewport;
        FList<FRenderInfo2> PayloadRenderInfo;
        FList<FResolveInfo2> PayloadResolveInfo;
        FList<FBufferCopyRange> PayloadBufferCopyRange;
        FList<FMeshBuffers2> PayloadMeshBuffers;
        FList<FVertexBufferRange2> PayloadVertexBufferRange;
        FList<u8> Blob;
        u32 NumSetBindings{};
        b8 Ended{};
        FGpuRecordMode Mode{};

        #ifdef FFI_SRC
        explicit FGpuRecordData(FAllocator* allocator)
            : Commands(allocator), Resources(allocator), Bindings(allocator), PayloadRect(allocator), PayloadViewport(allocator),
              PayloadRenderInfo(allocator), PayloadResolveInfo(allocator), PayloadBufferCopyRange(allocator),
              PayloadMeshBuffers(allocator), PayloadVertexBufferRange(allocator), Blob(allocator)
        {
        }

        void ClearData()
        {
            Commands.Clear();
            Resources.Clear();
            Bindings.Clear();
            PayloadRect.Clear();
            PayloadViewport.Clear();
            PayloadRenderInfo.Clear();
            PayloadResolveInfo.Clear();
            PayloadBufferCopyRange.Clear();
            PayloadMeshBuffers.Clear();
            PayloadVertexBufferRange.Clear();
            Blob.Clear();
            NumSetBindings = 0;
        }
        #endif
    };

    struct FGpuRecordCreateResult
    {
        FGpuRecord* Record{};
        FGpuRecordData* Data{};
    };
}
