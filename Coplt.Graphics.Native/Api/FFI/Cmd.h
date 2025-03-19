#pragma once

#include "Structs.h"
#include "Output.h"

namespace Coplt
{
    enum class FCmdType : u32
    {
        None = 0,

        Label,
        BeginScope,
        EndScope,

        ClearColor,
        ClearDepthStencil,
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    enum class FCmdResType
    {
        Image,
        Buffer,
        Output,
    };

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
        FUnknown* GetObjectPtr() const
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

    struct FCmdResRef
    {
        u32 IndexPlusOne{};

        #ifdef FFI_SRC
        operator bool() const { return IndexPlusOne == 0; }

        FCmdRes& Get(FList<FCmdRes>& list) const { return list[IndexPlusOne - 1]; }
        #endif
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

    struct FCmdClearColor : FCmdBase
    {
        // 有多少个 Rect
        u32 RectCount{};
        // Payload 中的 Index, 类型为 Rect
        u32 RectIndex{};
        FCmdResRef Image{};
        f32 Color[4]{};
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

            FCmdClearColor ClearColor;

            u8 _pad[32];
        };
    };
}
