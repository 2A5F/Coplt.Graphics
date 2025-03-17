#pragma once
#include <utility>

namespace Coplt
{
    template <class T>
    using Box = std::unique_ptr<T>;

    template <class T, class... Args>
    Box<T> box(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    // move or forward
    template <class T>
    std::remove_reference_t<T>&& fove(T&& a)
    {
        return std::move(a);
    }

    // move or forward
    template <class T>
    const T& fove(const T& a)
    {
        return a;
    }

    // move or forward
    template <class T>
    T& fove(T& a)
    {
        return a;
    }

    inline u32 Aligned256(const u32 value)
    {
        return (value + 0xFFu) & ~0xFFu;
    }

    inline u32 Aligned256(const u64 value)
    {
        return (value + 0xFFu) & ~0xFFu;
    }

    inline bool IsAligned256(const u32 value)
    {
        return (value & 0xFFu) == 0;
    }

    inline bool IsAligned256(const u64 value)
    {
        return (value & 0xFFu) == 0;
    }
}
