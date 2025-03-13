#pragma once

#include "Binding.h"
#include "Blob.h"
#include "Output.h"
#include "Pipeline.h"
#include "States.h"

namespace Coplt
{
    enum class FCommandType : u32
    {
        None,

        Label,
        BeginScope,
        EndScope,

        // 仅内部使用
        Present,

        // 手动屏障
        Barrier,

        ClearColor,
        ClearDepthStencil,

        Bind,

        BufferCopy,

        Render,
        Compute,

        // Render / Compute
        SetPipeline,
        SetBinding,

        // Render
        SetViewportScissor,
        SetMeshBuffers,
        Draw,

        // Render / Compute
        Dispatch,

        // Render / Compute
        // 仅内部使用
        SyncBinding,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class FResourceRefType : u8
    {
        Image,
        Buffer,
        Output,
        // todo other
    };

    struct FResourceMeta
    {
        union
        {
            FGpuOutput* Output{};
            FGpuBuffer* Buffer;
            // todo other
        };

        FResourceRefType Type{};

        #ifdef FFI_SRC
        FUnknown* GetObjectPtr() const
        {
            switch (Type)
            {
            case FResourceRefType::Image:
                return nullptr; // todo
            case FResourceRefType::Buffer:
                return Buffer;
            case FResourceRefType::Output:
                return Output;
            }
            return nullptr;
        }
        #endif
    };

    struct FResourceRef
    {
        // u32::max 表示 empty
        u32 ResourceIndex{};

        #ifdef FFI_SRC
        FResourceMeta& Get(const FCommandSubmit& submit) const;

        bool IsEmpty() const
        {
            return ResourceIndex == 0;
        }
        #endif
    };

    struct FSubResourceRange
    {
        u32 IndexOrFirstMipLevel{};
        u32 NumMipLevels{};
        u32 FirstArraySlice{};
        u32 NumArraySlices{};
        u32 FirstPlane{};
        u32 NumPlanes{};
    };

    COPLT_ENUM_FLAGS(FImageBarrierFlags, u32)
    {
        None,
        Discard = 1 << 0,
        CrossQueue = 1 << 1,
    };

    struct FImageBarrier
    {
        FLegacyState LegacyBefore{};
        FLegacyState LegacyAfter{};
        FResAccess AccessBefore{};
        FResAccess AccessAfter{};
        FShaderStageFlags StagesBefore{};
        FShaderStageFlags StagesAfter{};
        FResLayout LayoutBefore{};
        FResLayout LayoutAfter{};
        FResourceRef Image{};
        FSubResourceRange SubResourceRange{};
        FImageBarrierFlags Flags{};
    };

    struct FBufferBarrier
    {
        FLegacyState LegacyBefore{};
        FLegacyState LegacyAfter{};
        FResAccess AccessBefore{};
        FResAccess AccessAfter{};
        FShaderStageFlags StagesBefore{};
        FShaderStageFlags StagesAfter{};
        FResourceRef Buffer{};
        u64 Offset{};
        u64 Size{};
    };

    struct FGlobalBarrier
    {
        FResAccess AccessBefore{};
        FResAccess AccessAfter{};
        FShaderStageFlags StagesBefore{};
        FShaderStageFlags StagesAfter{};
    };

    enum class FBarrierType : u8
    {
        None,
        Global,
        Buffer,
        Image,
    };

    struct FBarrier
    {
        FBarrierType Type{};

        union
        {
            FGlobalBarrier Global;
            FBufferBarrier Buffer;
            FImageBarrier Image;
        };
    };

    struct FRect
    {
        u32 Left;
        u32 Top;
        u32 Right;
        u32 Bottom;
    };

    struct FViewport
    {
        f32 X;
        f32 Y;
        f32 Width;
        f32 Height;
        f32 MinDepth;
        f32 MaxDepth;
    };

    COPLT_ENUM_FLAGS(FDepthStencilClearFlags, u8)
    {
        None = 0,
        Depth = 1,
        Stencil = 2,
    };

    struct FUploadLoc
    {
        u32 Index{};
    };

    union FBufferRef
    {
        FResourceRef Buffer;
        FUploadLoc Upload;
    };

    enum class FBufferRefType : u8
    {
        // GpuBuffer 对象资源引用
        Buffer = 0,
        // 当前帧上下文中第几个上传缓冲区
        Upload,
    };

    struct FBufferCopyRange
    {
        // Size 为 u64::max 时复制整个
        u64 Size{};
        u64 DstOffset{};
        u64 SrcOffset{};
    };

    enum class FResolveMode : u8
    {
        Decompress,
        Min,
        Max,
        Average,
    };

    struct FResolveInfo
    {
        FResourceRef Src{};
        FResourceRef Dst{};
        FGraphicsFormat Format{};
        FResolveMode Mode{};
    };

    enum class FLoadOp : u8
    {
        Load,
        Clear,
        Discard,
        NoAccess,
    };

    enum class FStoreOp : u8
    {
        Store,
        Discard,
        Resolve,
        NoAccess,
    };

    struct FRenderInfo
    {
        // 可选
        FResourceRef Dsv{};
        u32 NumRtv{};
        FResourceRef Rtv[8]{};
        // 类型为 FResolveInfo
        u32 ResolveInfoIndex[8]{};
        u32 DsvResolveInfoIndex{};
        FGraphicsFormat RtvFormat[8]{};
        FGraphicsFormat DsvFormat{};
        f32 Color[4 * 8]{};
        f32 Depth{};
        u8 Stencil{};
        FLoadOp DsvLoadOp[2]{};
        FStoreOp DsvStoreOp[2]{};
        FLoadOp RtvLoadOp[8]{};
        FStoreOp RtvStoreOp[8]{};
        b8 HasUavWrites{};
    };

    struct FComputeInfo
    {
    };

    struct FBufferRange
    {
        FResourceRef Buffer{};
        u32 Offset{};
        u32 Size{};
    };

    struct FVertexBufferRange : FBufferRange
    {
        u32 Index{};
    };

    struct FMeshBuffers
    {
        FMeshLayout* MeshLayout{};
        // 可选
        FBufferRange IndexBuffer{};
        // 0 .. 31
        u32 VertexBufferCount{};
        // Payload 内的索引，类型为 FVertexBufferRange
        u32 VertexBuffersIndex{};
    };

    enum class FDispatchType : u8
    {
        Compute,
        Mesh,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandBase
    {
        FCommandType Type{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandLabel : FCommandBase
    {
        u32 StringIndex{};
        u32 StringLength{};
        FStrType StrType{};
        u8 Color[3]{};
        b8 HasColor{};
    };

    struct FCommandBeginScope : FCommandBase
    {
        u32 StringIndex{};
        u32 StringLength{};
        FStrType StrType{};
        u8 Color[3]{};
        b8 HasColor{};
    };

    struct FCommandEndScope : FCommandBase
    {
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandPresent : FCommandBase
    {
        FResourceRef Image{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandBarrier : FCommandBase
    {
        // Payload 内的索引，类型为 FBarrier
        u32 BarrierIndex{};
        u32 BarrierCount{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandClearColor : FCommandBase
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 内的索引
        u32 RectIndex{};
        FResourceRef Image{};
        f32 Color[4]{};
    };

    struct FCommandClearDepthStencil : FCommandBase
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 内的索引, 类型为 FRect
        u32 RectIndex{};
        FResourceRef Image{};
        f32 Depth{};
        u8 Stencil{};
        FDepthStencilClearFlags Clear{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandBind : FCommandBase
    {
        // 有多少个绑定修改
        u32 ItemCount{};
        // Payload 内的索引, 类型为 FBindItem
        u32 ItemsIndex{};
        // 要修改的绑定集
        FShaderBinding* Binding{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandBufferCopy : FCommandBase
    {
        // 类型为 FBufferCopyRange
        u32 RangeIndex{};
        FBufferRef Dst{};
        FBufferRef Src{};
        FBufferRefType DstType{};
        FBufferRefType SrcType{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandRender : FCommandBase
    {
        // 类型为 FRenderInfo
        u32 InfoIndex{};
        u32 CommandStartIndex{};
        u32 CommandCount{};
    };

    struct FCommandCompute : FCommandBase
    {
        // 类型为 FComputeInfo
        u32 InfoIndex{};
        u32 CommandStartIndex{};
        u32 CommandCount{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandSetPipeline : FCommandBase
    {
        FShaderPipeline* Pipeline{};
    };

    struct FCommandSetBinding : FCommandBase
    {
        FShaderBinding* Binding{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandSetViewportScissor : FCommandBase
    {
        // 有多少个 Viewport
        u32 ViewportCount{};
        // Payload 内的索引
        u32 ViewportIndex{};
        // 有多少个 Rect
        u32 ScissorRectCount{};
        // Payload 内的索引, 类型为 FRect
        u32 ScissorRectIndex{};
    };

    struct FCommandSetMeshBuffers : FCommandBase
    {
        FGraphicsFormat IndexFormat{};
        // 0 .. 31
        u32 VertexStartSlot{};
        // 类型为 FMeshBuffers
        u32 PayloadIndex{};
    };

    struct FCommandDraw : FCommandBase
    {
        u32 VertexOrIndexCount{};
        u32 InstanceCount{};
        u32 FirstVertexOrIndex{};
        u32 FirstInstance{};
        // 仅 Indexed 使用
        u32 VertexOffset{};
        b8 Indexed{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandDispatch : FCommandBase
    {
        u32 GroupCountX{};
        u32 GroupCountY{};
        u32 GroupCountZ{};
        FDispatchType Type{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class FBindingSyncType : u8
    {
        Transient,
        Persistent,
    };

    struct FCommandSyncBinding : FCommandBase
    {
        u32 Index{};
        FBindingSyncType Type{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandItem
    {
        union
        {
            FCommandType Type{};

            FCommandLabel Label;
            FCommandBeginScope BeginScope;
            FCommandEndScope EndScope;

            FCommandPresent Present;

            FCommandBarrier Barrier;

            FCommandClearColor ClearColor;
            FCommandClearDepthStencil ClearDepthStencil;

            FCommandBind Bind;

            FCommandBufferCopy BufferCopy;

            FCommandRender Render;
            FCommandCompute Compute;

            FCommandSyncBinding SyncBinding;

            u8 _pad[32];
        };
    };

    struct FRenderCommandItem
    {
        union
        {
            FCommandType Type{};

            FCommandLabel Label;
            FCommandBeginScope BeginScope;
            FCommandEndScope EndScope;

            FCommandSetPipeline SetPipeline;
            FCommandSetBinding SetBinding;

            FCommandSetViewportScissor SetViewportScissor;
            FCommandSetMeshBuffers SetMeshBuffers;
            FCommandDraw Draw;

            FCommandDispatch Dispatch;

            FCommandSyncBinding SyncBinding;

            u8 _pad[32];
        };
    };

    struct FComputeCommandItem
    {
        union
        {
            FCommandType Type{};

            FCommandLabel Label;
            FCommandBeginScope BeginScope;
            FCommandEndScope EndScope;

            FCommandSetPipeline SetPipeline;
            FCommandSetBinding SetBinding;

            FCommandDispatch Dispatch;

            FCommandSyncBinding SyncBinding;

            u8 _pad[32];
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCommandSubmit
    {
        FCommandItem* Commands{};
        FRenderCommandItem* RenderCommands{};
        FComputeCommandItem* ComputeCommands{};
        FResourceMeta* Resources{};
        FRenderInfo* RenderInfos{};
        FComputeInfo* ComputeInfos{};
        FResolveInfo* ResolveInfos{};
        FRect* Rects{};
        FViewport* Viewports{};
        FMeshBuffers* MeshBuffers{};
        FVertexBufferRange* VertexBufferRanges{};
        FBufferCopyRange* BufferCopyRanges{};
        FBindItem* BindItems{};
        FBarrier* Barriers{};
        Char8* Str8{};
        Char16* Str16{};
        u32 CommandCount{};
        u32 ResourceCount{};
        u32 SyncBindingCount{};
        u32 GrowingResourceBindingCapacity{};
        u32 GrowingSamplerBindingCapacity{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    #ifdef FFI_SRC

    inline FResourceMeta& FResourceRef::Get(const FCommandSubmit& submit) const
    {
        return submit.Resources[ResourceIndex - 1];
    }

    #endif
}
