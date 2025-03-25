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

        PreparePresent,

        ClearColor,
        ClearDepthStencil,
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

        bool IsImage() const
        {
            switch (Type)
            {
            case FCmdResType::Image:
                return true;
            case FCmdResType::Buffer:
                return false;
            case FCmdResType::Output:
                return true;
            }
            return false;
        }
        #endif
    };

    struct FCmdResRef
    {
        u32 IndexPlusOne{};

        #ifdef FFI_SRC
        operator bool() const { return IndexPlusOne == 0; }

        u32 ResIndex() const { return IndexPlusOne - 1; }

        FCmdRes& Get(FList<FCmdRes>& list) const
        {
            const auto index = ResIndex();
            if (index > list.m_len)
                COPLT_THROW("Index out of range");
            return list[index];
        }
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

            u8 _pad[32];
        };
    };
}
