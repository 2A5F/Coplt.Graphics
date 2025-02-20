#pragma once

#include "Object.h"
#include "Result.h"
#include "List.h"

namespace Coplt
{
    struct FUploadBufferBlock
    {
        volatile u8* mapped_ptr{};
        u64 cur_offset{};
        u64 size{};
    };

    COPLT_INTERFACE_DEFINE(FFrameContext, "3a22de68-8377-4e57-a167-3efdb97c102a", FUnknown)
    {
        FList<FUploadBufferBlock> m_upload_buffer{};

        virtual FResult GrowUploadBuffer(u64 min_required_size) noexcept = 0;
    };
}
