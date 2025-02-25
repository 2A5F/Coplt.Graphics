#pragma once

#include <fmt/format.h>
#include <fmt/xchar.h>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

#include "Logger.h"
#include "../FFI/Result.h"
#include "../Src/String.h"

#if _WINDOWS
#include <AtlBase.h>
#endif

#include "Concepts.h"

namespace Coplt
{
    inline std::string to_string(const cpptrace::stacktrace& trace)
    {
        const auto trace8 = trace.to_string(false);
        std::stringstream ss(trace8);
        std::string tmp;
        std::ostringstream stream{};
        while (std::getline(ss, tmp, '\n'))
        {
            stream << "    ";
            stream << tmp;
            stream << "\r\n";
        }
        stream << "    --- End of native exception stack trace ---";
        return std::move(stream).str();
    }

    struct Exception
    {
        virtual ~Exception() = default;
        virtual std::string what() const noexcept = 0;
        virtual const std::string& message() const = 0;
        virtual const cpptrace::stacktrace& trace() const = 0;
    };

    class LazyException : public Exception
    {
        mutable cpptrace::detail::lazy_trace_holder trace_holder;
        mutable std::string what_string;

    public:
        explicit LazyException(
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) : trace_holder(std::move(trace))
        {
        }

        std::string what() const noexcept override
        {
            if (what_string.empty())
            {
                const auto trace = to_string(trace_holder.get_resolved_trace());
                what_string = fmt::format("{}\r\n{}", message().c_str(), trace.c_str());
            }
            return what_string;
        }

        const cpptrace::stacktrace& trace() const noexcept override
        {
            return trace_holder.get_resolved_trace();
        }
    };

    class ExceptionWithMessage : public LazyException
    {
        mutable std::string user_message;

    public:
        explicit ExceptionWithMessage(
            std::string&& message_arg,
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept : LazyException(std::move(trace)), user_message(std::move(message_arg))
        {
        }

        const std::string& message() const noexcept override
        {
            return user_message;
        }
    };

    class RuntimeException : public ExceptionWithMessage
    {
    public:
        explicit RuntimeException(
            std::string&& message_arg,
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept
            : ExceptionWithMessage(std::move(message_arg), std::move(trace))
        {
        }

        template <usize N>
        explicit RuntimeException(
            const char (&message_arg)[N],
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept
            : RuntimeException(std::string(message_arg, N), std::move(trace))
        {
        }
    };

#if _WINDOWS
    inline std::wstring to_wstring(const cpptrace::stacktrace& trace)
    {
        const auto trace8 = trace.to_string(false);
        std::stringstream ss(trace8);
        std::string tmp;
        std::wostringstream stream{};
        while (std::getline(ss, tmp, '\n'))
        {
            stream << L"    ";
            stream << to_wstring(tmp.c_str());
            stream << L"\r\n";
        }
        stream << L"    --- End of native exception stack trace ---";
        return std::move(stream).str();
    }

    struct WException
    {
        virtual ~WException() = default;
        virtual std::wstring what() const noexcept = 0;
        virtual const std::wstring& message() const = 0;
        virtual const cpptrace::stacktrace& trace() const = 0;
    };

    class WLazyException : public WException
    {
        mutable cpptrace::detail::lazy_trace_holder trace_holder;
        mutable std::wstring what_string;

    public:
        explicit WLazyException(
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) : trace_holder(std::move(trace))
        {
        }

        std::wstring what() const noexcept override
        {
            if (what_string.empty())
            {
                const auto trace = to_wstring(trace_holder.get_resolved_trace());
                what_string = fmt::format(L"{}\r\n{}", message().c_str(), trace.c_str());
            }
            return what_string;
        }

        const cpptrace::stacktrace& trace() const noexcept override
        {
            return trace_holder.get_resolved_trace();
        }
    };

    class WExceptionWithMessage : public WLazyException
    {
        mutable std::wstring user_message;

    public:
        explicit WExceptionWithMessage(
            std::wstring&& message_arg,
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept : WLazyException(std::move(trace)), user_message(std::move(message_arg))
        {
        }

        const std::wstring& message() const noexcept override
        {
            return user_message;
        }
    };

    class WRuntimeException : public WExceptionWithMessage
    {
    public:
        explicit WRuntimeException(
            std::wstring&& message_arg,
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept
            : WExceptionWithMessage(std::move(message_arg), std::move(trace))
        {
        }

        template <usize N>
        explicit WRuntimeException(
            const wchar_t (&message_arg)[N],
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept
            : WRuntimeException(std::wstring(message_arg, N), std::move(trace))
        {
        }
    };

    class ComException : public WExceptionWithMessage
    {
    public:
        explicit ComException(
            std::wstring&& message_arg,
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept
            : WExceptionWithMessage(std::move(message_arg), std::move(trace))
        {
        }

        template <usize N>
        explicit ComException(
            const wchar_t (&message_arg)[N],
            cpptrace::raw_trace&& trace = cpptrace::detail::get_raw_trace_and_absorb()
        ) noexcept
            : ComException(std::wstring(message_arg, N), std::move(trace))
        {
        }
    };

#endif

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
                        catch (FResult& e)
                        {
                            r = e;
                        }
                        catch (cpptrace::exception& e)
                        {
                            auto msg = std::string(e.what());
                            r = FResult::Error(new CString(std::move(msg)));
                        }
                        catch (Exception& e)
                        {
                            const auto& msg = e.what();
                            r = FResult::Error(String8::Create(msg.data(), msg.size()));
                        }
#ifdef _WINDOWS
                        catch (WException& e)
                        {
                            const auto& msg = e.what();
                            r = FResult::Error(String16::Create(msg.data(), msg.size()));
                        }
#endif
                    }
                CPPTRACE_CATCH(std::exception& ex)
                {
                    const auto trace = to_string(cpptrace::from_current_exception());
                    auto msg = fmt::format("{}\r\n{}", ex.what(), trace.c_str());
                    r = FResult::Error(new CString(std::move(msg)));
                }
            }
        CPPTRACE_CATCH(...)
        {
            const auto trace = to_string(cpptrace::from_current_exception());
            auto msg = fmt::format(
                "Unknown failure occurred. Possible memory corruption\r\n{}", trace.c_str());
            r = FResult::Error(new CString(std::move(msg)));
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
                        catch (FResult& e)
                        {
                            r = e;
                        }
                        catch (cpptrace::exception& e)
                        {
                            auto msg = std::string(e.what());
                            r = FResult::Error(new CString(std::move(msg)));
                        }
                        catch (Exception& e)
                        {
                            const auto& msg = e.what();
                            r = FResult::Error(String8::Create(msg.data(), msg.size()));
                        }
#ifdef _WINDOWS
                        catch (WException& e)
                        {
                            const auto& msg = e.what();
                            r = FResult::Error(String16::Create(msg.data(), msg.size()));
                        }
#endif
                    }
                CPPTRACE_CATCH(std::exception& ex)
                {
                    const auto trace = to_string(cpptrace::from_current_exception());
                    const auto msg = fmt::format("{}\r\n{}", ex.what(), trace.c_str());
                    r = FResult::Error(new CString(msg.data(), msg.size()));
                }
            }
        CPPTRACE_CATCH(...)
        {
            const auto trace = to_string(cpptrace::from_current_exception());
            const auto msg = fmt::format(
                "Unknown failure occurred. Possible memory corruption\r\n{}", trace.c_str());
            r = FResult::Error(new CString(msg.data(), msg.size()));
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
                        catch (FResult& e)
                        {
                            logger.Log(FLogLevel::Error, FLogType::Common, e.msg);
                            e.Drop();
                        }
                        catch (cpptrace::exception& e)
                        {
                            logger.Log(FLogLevel::Error, e.what());
                        }
                        catch (Exception& e)
                        {
                            const auto& msg = e.what();
                            logger.Log(FLogLevel::Error, msg);
                        }
#ifdef _WINDOWS
                        catch (WException& e)
                        {
                            const auto& msg = e.what();
                            logger.LogW(FLogLevel::Error, msg);
                        }
#endif
                    }
                CPPTRACE_CATCH(std::exception& ex)
                {
                    const auto trace = to_string(cpptrace::from_current_exception());
                    const auto msg = fmt::format("{}\r\n{}", ex.what(), trace.c_str());
                    logger.Log(FLogLevel::Error, msg);
                }
            }
        CPPTRACE_CATCH(...)
        {
            const auto trace = to_string(cpptrace::from_current_exception());
            const auto msg = fmt::format("Unknown failure occurred. Possible memory corruption\r\n{}", trace.c_str());
            logger.Log(FLogLevel::Error, msg);
        }
        return r;
    }

#ifdef _WINDOWS
#define COPLT_THROW(msg) throw WRuntimeException(L##msg)
#define COPLT_THROW_FMT(msg, ...) throw WRuntimeException(fmt::format(L##msg, __VA_ARGS__))
#else
#define COPLT_THROW(msg) throw RuntimeException(msg)
#define COPLT_THROW_FMT(msg, ...) throw RuntimeException(fmt::format(msg, __VA_ARGS__))
#endif
}
