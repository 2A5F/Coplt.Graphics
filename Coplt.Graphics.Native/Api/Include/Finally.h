#pragma once

#include "Concepts.h"

namespace Coplt
{
    template <Fn<void> F>
    class Finally
    {
        F f;

    public:
        explicit Finally(F&& f) : f(std::forward<F>(f))
        {
        }

        explicit Finally(const F& f) : f(f)
        {
        }

        ~Finally() noexcept(false)
        {
            f();
        }
    };
}
