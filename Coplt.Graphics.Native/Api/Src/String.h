#pragma once

#include <string>
#include <string>

#include <mimalloc.h>

#include "../FFI/String.h"
#include "../Include/Object.h"

namespace Coplt
{
    template <class This, class T, Interface I> requires requires(I& obj)
    {
        { obj.m_data } -> std::same_as<const T*&>;
        { obj.m_size } -> std::same_as<size_t&>;
    }
    struct StringT : Object<StringT<This, T, I>, I>
    {
        using StdString = std::basic_string<T>;

    private:
        friend This;

        // 将复制内容
        constexpr explicit StringT(const T* data, const usize size)
        {
            I::m_data = data;
            I::m_size = size;
        }

    public:
        static This* Create(const T* data)
        {
            return Create(data, std::char_traits<T>::length(data));
        }

        static This* Create(const T* data, const usize size)
        {
            auto mem = static_cast<char*>(mi_malloc_aligned(sizeof(This) + (size + 1) * sizeof(T), alignof(This)));
            auto start = reinterpret_cast<T*>(mem + sizeof(This));
            memcpy(start, data, size * sizeof(T));
            start[size] = 0;
            new(mem) This(start, size);
            return reinterpret_cast<This*>(mem);
        }

        void Free(void* self) noexcept override
        {
            mi_free_aligned(self, alignof(This));
        }

        constexpr const T* data() const
        {
            return I::m_data;
        }

        constexpr size_t size() const
        {
            return I::m_size;
        }

        constexpr StdString string() const
        {
            return StdString(data(), size());
        }
    };

    template <class This, class T, class U, Interface I> requires requires(I& obj)
    {
        { obj.m_data } -> std::same_as<const U*&>;
        { obj.m_size } -> std::same_as<size_t&>;
    }
    struct StringForward : Object<StringForward<This, T, U, I>, I>
    {
        using StdString = std::basic_string<T>;

        StdString const m_string;

        constexpr explicit StringForward(StdString&& string) : m_string(std::move(string))
        {
            I::m_data = reinterpret_cast<const U*>(m_string.data());
            I::m_size = m_string.size();
        }

        // 将复制内容
        constexpr explicit StringForward(const T* data, const usize size) : m_string(data, size)
        {
            I::m_data = reinterpret_cast<const U*>(m_string.data());
            I::m_size = m_string.size();
        }

        constexpr const T* data() const
        {
            return reinterpret_cast<const T*>(I::m_data);
        }

        constexpr size_t size() const
        {
            return I::m_size;
        }

        constexpr const T* c_str() const
        {
            return data();
        }

        constexpr const StdString& string() const
        {
            return m_string;
        }
    };

    struct String8 final : StringT<String8, char8_t, FString8>
    {
    private:
        friend StringT;

        explicit String8(const char8_t* data, const usize size) : StringT(data, size)
        {
        }

    public:
        static String8* Create(const char8_t* data)
        {
            return StringT::Create(data);
        }

        static String8* Create(const char* data)
        {
            return Create(data, std::char_traits<char>::length(data));
        }

        static String8* Create(const char8_t* data, const usize size)
        {
            return StringT::Create(data, size);
        }

        static String8* Create(const char* data, const usize size)
        {
            return StringT::Create(reinterpret_cast<const char8_t*>(data), size);
        }

        constexpr const char* c_str() const
        {
            return reinterpret_cast<const char*>(m_data);
        }

        constexpr std::string to_string() const
        {
            return std::string(c_str(), m_size);
        }

        FStr8or16 GetStr() const noexcept override
        {
            return FStr8or16(c_str(), size());
        }
    };

    struct String16 final : StringT<String16, char16_t, FString16>
    {
    private:
        friend StringT;

        explicit String16(const char16_t* data, const usize size) : StringT(data, size)
        {
        }

    public:
        static String16* Create(const Char16* data)
        {
            return StringT::Create(data);
        }

        static String16* Create(const Char16* data, const usize size)
        {
            return StringT::Create(data, size);
        }

#if _WINDOWS
        static String16* Create(const wchar_t* data)
        {
            return Create(data, std::char_traits<wchar_t>::length(data));
        }

        static String16* Create(const wchar_t* data, const usize size)
        {
            return StringT::Create(reinterpret_cast<const char16_t*>(data), size);
        }

        constexpr const wchar_t* c_str() const
        {
            return reinterpret_cast<const wchar_t*>(m_data);
        }

        constexpr std::wstring to_string() const
        {
            return std::wstring(c_str(), m_size);
        }
#endif

        FStr8or16 GetStr() const noexcept override
        {
            return FStr8or16(data(), size());
        }
    };

    struct CString final : StringForward<CString, char, char8_t, FString8>
    {
        constexpr explicit CString(StdString&& string) : StringForward(std::move(string))
        {
        }

        // 将复制内容
        constexpr explicit CString(const char* data, const usize size) : StringForward(data, size)
        {
        }

        FStr8or16 GetStr() const noexcept override
        {
            return FStr8or16(data(), size());
        }
    };

#if _WINDOWS
    struct WString final : StringForward<WString, wchar_t, char16_t, FString16>
    {
        constexpr explicit WString(StdString&& string) : StringForward(std::move(string))
        {
        }

        // 将复制内容
        constexpr explicit WString(const wchar_t* data, const usize size) : StringForward(data, size)
        {
        }

        FStr8or16 GetStr() const noexcept override
        {
            return FStr8or16(data(), size());
        }
    };
#endif
}
