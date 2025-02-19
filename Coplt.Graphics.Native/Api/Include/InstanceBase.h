#pragma once

#include "Logger.h"
#include "../FFI/Instance.h"

#include <fmt/format.h>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

#include "Error.h"
#include "mimalloc.h"

#include "../Src/Allocator.h"
#include "../Src/String.h"

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

        FResult CreateString8(const Char8* data, const usize size, FString8** out) noexcept override
        {
            return feb([&]
            {
                *out = String8::Create(data, size);
            });
        }

        FResult CreateString16(const Char16* data, const usize size, FString16** out) noexcept override
        {
            return feb([&]
            {
                *out = String16::Create(data, size);
            });
        }

        FResult CreateBlob(const usize size, FBlob** out) noexcept override
        {
            return feb([&]
            {
                *out = FBlob::Create(size);
            });
        }
    };
}
