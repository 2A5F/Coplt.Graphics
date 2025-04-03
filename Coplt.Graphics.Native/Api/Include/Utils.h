#pragma once
#include <memory>
#include <utility>

#include "../../Api/FFI/Common.h"

namespace Coplt
{
    template <class T>
    using Box = std::unique_ptr<T>;

    template <class T, class... Args>
    Box<T> box(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <class T>
    using SRc = std::shared_ptr<T>;

    template <class T, class... Args>
    SRc<T> src(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
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

    inline void hash_combine(std::size_t& seed)
    {
    }

    template <class T>
    void hash_combine(std::size_t& seed, const T& v)
    {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <class T, class... Rest>
    size_t hash_multi(const T& v, const Rest&... rest)
    {
        auto seed = std::hash<T>{}(v);
        (hash_combine(seed, rest), ...);
        return seed;
    }
}
