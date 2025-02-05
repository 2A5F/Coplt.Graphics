#pragma once

#include <fmt/format.h>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

#include "Logger.h"
#include "../FFI/Result.h"
#include "../Src/String.h"

namespace Coplt
{
    template <class F, class R, class... Args>
    concept Fn = std::invocable<F, Args...> && requires(F f)
    {
        { f() } -> std::convertible_to<R>;
    };

    template <Fn<FResult> F>
    FResult feb(F f) noexcept
    {
        FResult r;
        CPPTRACE_TRY
            {
                CPPTRACE_TRY
                    {
                        try
                        {
                            r = f();
                        }
                        catch (cpptrace::exception& e)
                        {
                            const auto msg = std::string(e.what());
                            r = FResult::Error(new String8(msg.data(), msg.size()));
                        }
                    }
                CPPTRACE_CATCH(std::exception& ex)
                {
                    const auto msg = fmt::format("{}\n{}", ex.what(), cpptrace::from_current_exception().to_string());
                    r = FResult::Error(new String8(msg.data(), msg.size()));
                }
            }
        CPPTRACE_CATCH(...)
        {
            const auto msg = fmt::format(
                "Unknown failure occurred. Possible memory corruption\n{}",
                cpptrace::from_current_exception().to_string()
            );
            r = FResult::Error(new String8(msg.data(), msg.size()));
        }
        return r;
    }

    template <Fn<void> F>
    FResult feb(F f) noexcept
    {
        FResult r = FResult::None();
        CPPTRACE_TRY
            {
                CPPTRACE_TRY
                    {
                        try
                        {
                            f();
                        }
                        catch (cpptrace::exception& e)
                        {
                            const auto msg = std::string(e.what());
                            r = FResult::Error(new String8(msg.data(), msg.size()));
                        }
                    }
                CPPTRACE_CATCH(std::exception& ex)
                {
                    const auto msg = fmt::format("{}\n{}", ex.what(), cpptrace::from_current_exception().to_string());
                    r = FResult::Error(new String8(msg.data(), msg.size()));
                }
            }
        CPPTRACE_CATCH(...)
        {
            const auto msg = fmt::format(
                "Unknown failure occurred. Possible memory corruption\n{}",
                cpptrace::from_current_exception().to_string()
            );
            r = FResult::Error(new String8(msg.data(), msg.size()));
        }
        return r;
    }

    template <class R, Fn<R> F>
    R feb(Logger& logger, R r, F f) noexcept
    {
        CPPTRACE_TRY
            {
                CPPTRACE_TRY
                    {
                        try
                        {
                            r = f();
                        }
                        catch (cpptrace::exception& e)
                        {
                            const auto msg = std::string(e.what());
                            logger.Log(FLogLevel::Error, msg.data(), msg.size());
                        }
                    }
                CPPTRACE_CATCH(std::exception& ex)
                {
                    const auto msg = fmt::format("{}\n{}", ex.what(), cpptrace::from_current_exception().to_string());
                    logger.Log(FLogLevel::Error, msg.data(), msg.size());
                }
            }
        CPPTRACE_CATCH(...)
        {
            const auto msg = fmt::format(
                "Unknown failure occurred. Possible memory corruption\n{}",
                cpptrace::from_current_exception().to_string()
            );
            logger.Log(FLogLevel::Error, msg.data(), msg.size());
        }
        return r;
    }
}
