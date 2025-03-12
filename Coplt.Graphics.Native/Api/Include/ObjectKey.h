#pragma once

#include "Ptr.h"
#include "../FFI/GpuObject.h"

namespace Coplt
{
    template <CGpuObject T>
    struct ObjectKey : NonNull<T>
    {
        ObjectKey(NonNull<T> ptr) : NonNull<T>(ptr)
        {
        }

        ObjectKey(Ptr<T> ptr) : NonNull<T>(ptr)
        {
        }

        ObjectKey(T* ptr) : NonNull<T>(ptr)
        {
        }

        usize GetHashCode() const
        {
            return std::hash<usize>{}(reinterpret_cast<usize>(this->get()));
        }

        bool operator==(const ObjectKey& other) const
        {
            if (this->get() != other.get()) return false;
            return (*this)->ObjectId() == other->ObjectId();
        }
    };
}
