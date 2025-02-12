#pragma once
#include <utility>

namespace Coplt
{
    // move or forward
    template<class T>
    std::remove_reference_t<T>&& fove(T&& a)
    {
        return std::move(a);
    }

    // move or forward
    template<class T>
    const T& fove(const T& a)
    {
        return a;
    }

    // move or forward
    template<class T>
    T& fove(T& a)
    {
        return a;
    }
}
