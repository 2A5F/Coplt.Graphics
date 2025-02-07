#pragma once

#include "Logger.h"
#include "../FFI/Instance.h"

#include <fmt/format.h>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

#include "mimalloc.h"

#include "../Src/Allocator.h"

namespace Coplt
{
    template <class Base>
    struct InstanceBase : Base
    {
        Rc<Allocator> m_allocator{};
        Logger m_logger{};

        explicit InstanceBase()
        {
            m_allocator = new Allocator();
            Base::m_allocator = m_allocator.get();
        }

        FResult SetLogger(const FLogger& logger) noexcept override
        {
            m_logger = Coplt::Logger(logger);
            return FResult::None();
        }

        const Logger& Logger() const noexcept
        {
            return m_logger;
        }
    };
}
