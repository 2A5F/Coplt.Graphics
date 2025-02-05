#pragma once

#include "Logger.h"

#include <fmt/format.h>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

namespace Coplt
{
    template <class Base>
    struct InstanceBase : Base
    {
        Logger m_logger{};

        FResult SetLogger(const FLogger& logger) noexcept override
        {
            m_logger = Logger(logger);
            return FResult::None();
        }
    };
}
