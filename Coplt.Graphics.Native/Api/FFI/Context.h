#pragma once

#include "Object.h"
#include "Result.h"
#include "List.h"
#include "GpuObject.h"

namespace Coplt
{
    struct FUploadBufferBlock
    {
        volatile u8* mapped_ptr{};
        u64 cur_offset{};
        u64 size{};
    };

    COPLT_INTERFACE_DEFINE(FFrameContext2, "fa9c7561-ab0b-4311-bb4c-c6bef93f86fe", FGpuObject)
    {
    };

    COPLT_INTERFACE_DEFINE(FRecordContext, "15b4821d-0648-4b78-9c21-c6a5b6a5ed75", FGpuObject)
    {
        FList<FUploadBufferBlock> m_upload_buffer{};

        virtual FResult GrowUploadBuffer(u64 min_required_size) noexcept = 0;
    };
}
