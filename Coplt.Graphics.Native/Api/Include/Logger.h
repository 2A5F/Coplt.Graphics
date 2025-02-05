#pragma once
#include "../FFI/Logger.h"

namespace Coplt
{
    struct Logger
    {
        FLogger m_inner{};

        Logger() = default;

        explicit Logger(const FLogger& inner) : m_inner(inner)
        {
        }

        ~Logger()
        {
            if (m_inner.pf_drop == nullptr) return;
            m_inner.pf_drop(m_inner.p_object);
            m_inner = {};
        }

        Logger(Logger&& other) noexcept
        {
            if (m_inner.pf_drop != nullptr)
            {
                m_inner.pf_drop(m_inner.p_object);
            }
            m_inner = other.m_inner;
            other.m_inner = {};
        }

        Logger& operator=(Logger&& other) noexcept
        {
            if (m_inner.pf_drop != nullptr)
            {
                m_inner.pf_drop(m_inner.p_object);
            }
            m_inner = other.m_inner;
            other.m_inner = {};
            return *this;
        }

        Logger(Logger& other) = delete;

        Logger& operator=(Logger& other) = delete;

        bool IsEnable(const FLogLevel level, const FLogType type = FLogType::Common) const
        {
            if (m_inner.pf_enable == nullptr) return true;
            return m_inner.pf_enable(m_inner.p_object, level, type);
        }

        void Log(const FLogLevel level, const FLogType type, const FMessage& msg) const
        {
            if (!IsEnable(level, type)) return;
            if (m_inner.pf_logger == nullptr) return;
            m_inner.pf_logger(m_inner.p_object, level, type, msg);
        }

        void Log(const FLogLevel level, const char* str, const size_t size) const
        {
            Log(level, FLogType::Common, str, size);
        }

        void Log(const FLogLevel level, const FLogType type, const char* str, const size_t size) const
        {
            const FMessage msg{{.str = str}, .len = static_cast<i32>(size), .type = FMessageType::Slice8};
            Log(level, type, msg);
        }

        void Log(const FLogLevel level, std::string& string) const
        {
            Log(level, FLogType::Common, string);
        }

        void Log(const FLogLevel level, const FLogType type, std::string& string) const
        {
            const FMessage msg{
                {.str = string.data()}, .len = static_cast<i32>(string.size()), .type = FMessageType::Slice8
            };
            Log(level, type, msg);
        }

#ifdef _WINDOWS

        void LogW(const FLogLevel level, std::wstring& string) const
        {
            LogW(level, FLogType::Common, string);
        }

        void LogW(const FLogLevel level, const FLogType type, std::wstring& string) const
        {
            const FMessage msg{
                {.wstr = string.data()}, .len = static_cast<i32>(string.size()), .type = FMessageType::Slice16
            };
            Log(level, type, msg);
        }

#endif

        template <usize N>
        void Log(const FLogLevel level, const char (&str)[N]) const
        {
            Log<N>(level, FLogType::Common, str);
        }

        template <usize N>
        void Log(const FLogLevel level, const FLogType type, const char (&str)[N]) const
        {
            const FMessage msg{
                {.str = str}, .len = static_cast<i32>(N), .type = FMessageType::Slice8
            };
            Log(level, type, msg);
        }

        template <usize N>
        void Log(const FLogLevel level, const char16_t (&str)[N]) const
        {
            Log<N>(level, FLogType::Common, str);
        }

        template <usize N>
        void Log(const FLogLevel level, const FLogType type, const char16_t (&str)[N]) const
        {
            const FMessage msg{
                {.str16 = str}, .len = static_cast<i32>(N), .type = FMessageType::Slice16
            };
            Log(level, type, msg);
        }

#ifdef _WINDOWS

        template <usize N>
        void LogW(const FLogLevel level, const wchar_t (&str)[N]) const
        {
            Log<N>(level, FLogType::Common, str);
        }

        template <usize N>
        void LogW(const FLogLevel level, const FLogType type, const wchar_t (&str)[N]) const
        {
            const FMessage msg{
                {.wstr = str}, .len = static_cast<i32>(N), .type = FMessageType::Slice16
            };
            Log(level, type, msg);
        }
#endif
    };
}
