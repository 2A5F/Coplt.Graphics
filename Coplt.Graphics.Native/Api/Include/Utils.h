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
}
