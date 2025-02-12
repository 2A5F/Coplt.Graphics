#pragma once

#include <concepts>

namespace Coplt
{
    template <class F, class R, class... Args>
    concept Fn = std::invocable<F, Args...> && requires(F&& f, Args&&... args)
    {
        { f(std::forward<Args>(args)...) } -> std::convertible_to<R>;
    };

    template <class F, class T>
    concept Hash = Fn<F, size_t, const T&> || Fn<F, size_t, T>;

    template <class F, class A, class B = A>
    concept Eq = Fn<F, bool, const A&, const B&>;
}
