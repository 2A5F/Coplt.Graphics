#include "../Include/Error.h"

using namespace Coplt;

std::string Coplt::to_string(const cpptrace::stacktrace& trace)
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

LazyException::LazyException(cpptrace::raw_trace&& trace) : trace_holder(std::move(trace))
{
}

std::string LazyException::what() const noexcept
{
    if (what_string.empty())
    {
        const auto trace = to_string(trace_holder.get_resolved_trace());
        what_string = fmt::format("{}\r\n{}", message().c_str(), trace.c_str());
    }
    return what_string;
}

const cpptrace::stacktrace& LazyException::trace() const noexcept
{
    return trace_holder.get_resolved_trace();
}

ExceptionWithMessage::ExceptionWithMessage(std::string&& message_arg, cpptrace::raw_trace&& trace) noexcept
    : LazyException(std::move(trace)), user_message(std::move(message_arg))
{
}

const std::string& ExceptionWithMessage::message() const noexcept
{
    return user_message;
}

RuntimeException::RuntimeException(std::string&& message_arg, cpptrace::raw_trace&& trace) noexcept
    : ExceptionWithMessage(std::move(message_arg), std::move(trace))
{
}

#ifdef _WINDOWS
std::wstring Coplt::to_wstring(const cpptrace::stacktrace& trace)
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

WLazyException::WLazyException(cpptrace::raw_trace&& trace) : trace_holder(std::move(trace))
{
}

std::wstring WLazyException::what() const noexcept
{
    if (what_string.empty())
    {
        const auto trace = to_wstring(trace_holder.get_resolved_trace());
        what_string = fmt::format(L"{}\r\n{}", message().c_str(), trace.c_str());
    }
    return what_string;
}

const cpptrace::stacktrace& WLazyException::trace() const noexcept
{
    return trace_holder.get_resolved_trace();
}

WExceptionWithMessage::WExceptionWithMessage(std::wstring&& message_arg, cpptrace::raw_trace&& trace) noexcept
    : WLazyException(std::move(trace)), user_message(std::move(message_arg))
{
}

const std::wstring& WExceptionWithMessage::message() const noexcept
{
    return user_message;
}

WRuntimeException::WRuntimeException(std::wstring&& message_arg, cpptrace::raw_trace&& trace) noexcept
    : WExceptionWithMessage(std::move(message_arg), std::move(trace))
{
}

ComException::ComException(std::wstring&& message_arg, cpptrace::raw_trace&& trace) noexcept
    : WExceptionWithMessage(std::move(message_arg), std::move(trace))
{
}

#endif
