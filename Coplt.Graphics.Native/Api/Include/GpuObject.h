#pragma once

#include "AllocObjectId.h"
#include "Object.h"

namespace Coplt
{
    template <class This, class Base, Interface... Interfaces>
    struct GpuObject : Object<This, Base, Interfaces...>
    {
        u64 const m_object_id{};

        GpuObject() : m_object_id(AllocObjectId())
        {
        }

        u64 ObjectId() const noexcept override { return m_object_id; }
    };
}
