#pragma once

#include "Blob.h"
#include "Output.h"
#include "Pipeline.h"
#include "States.h"

namespace Coplt
{
    enum class FCommandType : u32
    {
        None,
        // 手动资源过渡
        Transition,
        // 仅内部使用
        Present,
        ClearColor,
        ClearDepthStencil,
        SetRenderTargets,
        SetViewportScissor,
        SetPipeline,
        SetMeshBuffers,
        Draw,
        Dispatch,
        BufferCopy,
    };

    enum class FCommandFlags : u32
    {
        None = 0,
        // 不要自动计算资源过渡
        DontTransition = 1 << 0,
    };

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
            FGpuOutput* Output;
            FGpuBuffer* Buffer;
            // todo other
        };

        FResourceState CurrentState{};
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
            return ResourceIndex == COPLT_U32_MAX;
        }
#endif
    };

    struct FRect
    {
        u32 left;
        u32 top;
        u32 right;
        u32 bottom;
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

    struct FCommandTransition
    {
        FResourceRef Resource{};
        FResourceState SrcState{};
        FResourceState DstState{};
    };

    struct FCommandPresent
    {
        FResourceRef Image{};
    };

    struct FCommandClearColor
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 内的索引
        u32 RectIndex{};
        FResourceRef Image{};
        f32 Color[4]{};
    };

    COPLT_ENUM_FLAGS(FDepthStencilClearFlags, u8)
    {
        None = 0,
        Depth = 1,
        Stencil = 2,
    };

    struct FCommandClearDepthStencil
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 内的索引
        u32 RectIndex{};
        FResourceRef Image{};
        f32 Depth{};
        u8 Stencil{};
        FDepthStencilClearFlags Clear{};
    };

    struct FCommandSetRenderTargets
    {
        // 可选
        FResourceRef Dsv{};
        u32 NumRtv{};
        FResourceRef Rtv[8]{};

        // 有多少个 Viewport
        u32 ViewportCount{};
        // Payload 内的索引
        u32 ViewportIndex{};
        // 有多少个 Rect
        u32 ScissorRectCount{};
        // Payload 内的索引
        u32 ScissorRectIndex{};
    };

    struct FCommandSetViewportScissor
    {
        // 有多少个 Viewport
        u32 ViewportCount{};
        // Payload 内的索引
        u32 ViewportIndex{};
        // 有多少个 Rect
        u32 ScissorRectCount{};
        // Payload 内的索引
        u32 ScissorRectIndex{};
    };

    struct FCommandSetPipeline
    {
        FShaderPipeline* Pipeline{};
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

    struct FCommandSetMeshBuffers
    {
        FMeshLayout* MeshLayout{};
        FGraphicsFormat IndexFormat{};
        // 可选
        FBufferRange IndexBuffer{};
        // 0 .. 31
        u32 VertexStartSlot{};
        // 0 .. 31
        u32 VertexBufferCount{};
        // Payload 内的索引，类型为 FVertexBufferRange
        u32 VertexBuffersIndex{};
    };

    struct FCommandDraw
    {
        // 可选
        FShaderPipeline* Pipeline{};
        u32 VertexOrIndexCount{};
        u32 InstanceCount{};
        u32 FirstVertexOrIndex{};
        u32 FirstInstance{};
        // 仅 Indexed 使用
        u32 VertexOffset{};
        b8 Indexed{};
    };

    enum class FDispatchType : u8
    {
        Auto,
        Compute,
        Mesh,
    };

    struct FCommandDispatch
    {
        // 可选
        FShaderPipeline* Pipeline{};
        u32 GroupCountX{};
        u32 GroupCountY{};
        u32 GroupCountZ{};
        FDispatchType Type{};
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

    struct FCommandBufferCopy
    {
        // Size 为 u64::max 时复制整个
        u64 Size{};
        u64 DstOffset{};
        u64 SrcOffset{};
        FBufferRef Dst{};
        FBufferRef Src{};
        FBufferRefType DstType{};
        FBufferRefType SrcType{};
    };

    struct FCommandItem
    {
        FCommandType Type{};
        FCommandFlags Flags{};

        union
        {
            FCommandTransition Transition;
            FCommandPresent Present;
            FCommandClearColor ClearColor;
            FCommandClearDepthStencil ClearDepthStencil;
            FCommandSetRenderTargets SetRenderTargets;
            FCommandSetViewportScissor SetViewportScissor;
            FCommandSetPipeline SetPipeline;
            FCommandSetMeshBuffers SetMeshBuffers;
            FCommandDraw Draw;
            FCommandDispatch Dispatch;
            FCommandBufferCopy BufferCopy;

            u8 _pad[56]{};
        };
    };

    struct FCommandSubmit
    {
        // 有多少命令
        u32 CommandCount{};
        // 有多少资源
        u32 ResourceCount{};
        // 需要保证对象指针生命周期 >= FCommandSubmit 的生命周期，提交操作可能会修改此内存，提交后内存将无效
        FCommandItem* Commands{};
        // 需要保证对象指针生命周期 >= FCommandSubmit 的生命周期，提交操作可能会修改此内存，提交后内存将无效
        FResourceMeta* Resources{};
        // 需要保证对象指针生命周期 >= FCommandSubmit 的生命周期，提交操作可能会修改此内存，提交后内存将无效
        u8* Payload{};
    };

#ifdef FFI_SRC

    inline FResourceMeta& FResourceRef::Get(const FCommandSubmit& submit) const
    {
        return submit.Resources[ResourceIndex];
    }

#endif
}
