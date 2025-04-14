#pragma once

#include "List.h"
#include "Structs.h"
#include "Output.h"
#include "Binding.h"
#include "Pipeline.h"

namespace Coplt
{
    enum class FCmdType : u32
    {
        None = 0,
        End = 1,

        Label,
        BeginScope,
        EndScope,

        PreparePresent,

        ClearColor,
        ClearDepthStencil,

        BufferCopy,
        BufferImageCopy,

        Render,
        Compute,

        // Render / Compute
        SetPipeline,
        SetBinding,
        SetConsts,
        SetDynSize,
        SetDynItem,

        // Render
        SetViewportScissor,
        SetMeshBuffers,
        Draw,

        // Render / Compute
        Dispatch,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class FCmdResType : u8
    {
        Image,
        Buffer,
        Output,
    };

    #ifdef FFI_SRC
    inline bool IsBuffer(const FCmdResType Type)
    {
        switch (Type)
        {
        case FCmdResType::Image:
            return false;
        case FCmdResType::Buffer:
            return true;
        case FCmdResType::Output:
            return false;
        }
        return false;
    }
    #endif

    struct FCmdRes
    {
        union
        {
            FGpuOutput2* Output{};
            FGpuBuffer* Buffer;
            FGpuImage* Image;
        };

        FCmdResType Type{};

        #ifdef FFI_SRC
        FGpuObject* GetObjectPtr() const
        {
            switch (Type)
            {
            case FCmdResType::Image:
                return Image;
            case FCmdResType::Buffer:
                return Buffer;
            case FCmdResType::Output:
                return Output;
            }
            return nullptr;
        }
        #endif
    };

    struct FResIndex
    {
        u32 Value{};

        #ifdef FFI_SRC
        explicit FResIndex(const u32 Value) : Value(Value)
        {
        }

        operator u32() const { return Value; }
        #endif
    };

    struct FCmdResRef
    {
        u32 IndexPlusOne{};

        #ifdef FFI_SRC
        explicit operator bool() const { return IndexPlusOne != 0; }

        FResIndex ResIndex() const { return FResIndex{IndexPlusOne - 1}; }
        #endif
    };

    struct FResolveInfo2
    {
        FCmdResRef Src{};
        FCmdResRef Dst{};
        FGraphicsFormat Format{};
        FResolveMode Mode{};
    };

    struct FRenderInfo2
    {
        // 可选
        FCmdResRef Dsv{};
        u32 NumRtv{};
        FCmdResRef Rtv[8]{};
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

    union FBufferRef2
    {
        FCmdResRef Buffer;
        FUploadLoc Upload;
    };

    enum class FBufferRefType2 : u8
    {
        // GpuBuffer 对象资源引用
        Buffer = 0,
        // 当前帧上下文中第几个上传缓冲区
        Upload,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdBase
    {
        FCmdType Type{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdLabel : FCmdBase
    {
        u32 StringLength{};
        // Blob 中的 index
        usize StringIndex{};
        FStrType StrType{};
        u8 Color[3]{};
        b8 HasColor{};
    };

    struct FCmdBeginScope : FCmdBase
    {
        u32 StringLength{};
        // Blob 中的 index
        usize StringIndex{};
        FStrType StrType{};
        u8 Color[3]{};
        b8 HasColor{};
    };

    struct FCmdEndScope : FCmdBase
    {
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdPreparePresent : FCmdBase
    {
        FCmdResRef Output{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdClearColor : FCmdBase
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 中的索引, 类型为 FRect
        u32 RectIndex{};
        FCmdResRef Image{};
        f32 Color[4]{};
    };

    struct FCmdClearDepthStencil : FCmdBase
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 内的索引, 类型为 FRect
        u32 RectIndex{};
        FCmdResRef Image{};
        f32 Depth{};
        u8 Stencil{};
        FDepthStencilClearFlags Clear{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdBufferCopy : FCmdBase
    {
        // 类型为 FBufferCopyRange
        u32 RangeIndex{};
        FBufferRef2 Dst{};
        FBufferRef2 Src{};
        FBufferRefType2 DstType{};
        FBufferRefType2 SrcType{};
    };

    struct FCmdBufferImageCopy : FCmdBase
    {
        // 类型为 FBufferImageCopyRange
        u32 RangeIndex{};
        FCmdResRef Image;
        FBufferRef2 Buffer{};
        FBufferRefType2 BufferType{};
        // false 为 Buffer To Image
        b8 ImageToBuffer{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdRender : FCmdBase
    {
        // 类型为 FRenderInfo2
        u32 InfoIndex{};
        u32 CommandCount{};
    };

    struct FCmdCompute : FCmdBase
    {
        u32 CommandCount{};
    }; ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdSetPipeline : FCmdBase
    {
        FShaderPipeline* Pipeline{};
    };

    struct FCmdBinding
    {
        FShaderBinding* Binding{};
    };

    struct FBindingChange
    {
        // Payload 中的索引，类型为 FCmdBinding
        u32 Binding{};
    };

    struct FCmdSetBinding : FCmdBase
    {
        // Payload 中的索引，类型为 FCmdBinding
        u32 Binding{};
        // 第几个 Set
        u32 Index{};
    };

    struct FCmdSetConsts : FCmdBase
    {
        // 绑定中的第几个组
        u32 Group{};
        u32 Slot{};
        // Payload 中的索引，类型为 u32
        u32 ValueIndex{};
        u32 Count{};
        u32 Offset{};
    };

    // 设置动态绑定的动态数组大小，调用后之前的动态数组内容将被丢弃
    struct FCmdSetDynSize : FCmdBase
    {
        // 绑定中的第几个组
        u32 Group{};
        // 动态数组大小
        u32 Size{};
    };

    struct FCmdSetDynItem : FCmdBase
    {
        // 绑定中的第几个组
        u32 Group{};
        // Payload 中的索引，类型为 FSetBindItem
        u32 ItemIndex{};
        u32 Count{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdSetViewportScissor : FCmdBase
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

    struct FBufferRange2
    {
        FCmdResRef Buffer{};
        u32 Offset{};
        u32 Size{};
    };

    struct FVertexBufferRange2 : FBufferRange2
    {
        u32 Index{};
    };

    struct FMeshBuffers2
    {
        FMeshLayout* MeshLayout{};
        // 可选
        FBufferRange2 IndexBuffer{};
        // 0 .. 31
        u32 VertexBufferCount{};
        // Payload 内的索引，类型为 FVertexBufferRange2
        u32 VertexBuffersIndex{};
    };

    struct FCmdSetMeshBuffers : FCmdBase
    {
        FGraphicsFormat IndexFormat{};
        // 0 .. 31
        u32 VertexStartSlot{};
        // 类型为 FMeshBuffers2
        u32 PayloadIndex{};
    };

    struct FCmdDraw : FCmdBase
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

    struct FCmdDispatch : FCmdBase
    {
        u32 GroupCountX{};
        u32 GroupCountY{};
        u32 GroupCountZ{};
        FDispatchType Type{};
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    struct FCmdItem
    {
        union
        {
            FCmdType Type{};

            FCmdLabel Label;
            FCmdBeginScope BeginScope;
            FCmdEndScope EndScope;

            FCmdPreparePresent PreparePresent;

            FCmdClearColor ClearColor;
            FCmdClearDepthStencil ClearDepthStencil;

            FCmdBufferCopy BufferCopy;
            FCmdBufferImageCopy BufferImageCopy;

            FCmdRender Render;
            FCmdCompute Compute;

            FCmdSetPipeline SetPipeline;
            FCmdSetBinding SetBinding;
            FCmdSetConsts SetConsts;
            FCmdSetDynSize SetDynSize;
            FCmdSetDynItem SetDynItem;

            FCmdSetViewportScissor SetViewportScissor;
            FCmdSetMeshBuffers SetMeshBuffers;
            FCmdDraw Draw;

            FCmdDispatch Dispatch;

            u8 _pad[32];
        };
    };
}
