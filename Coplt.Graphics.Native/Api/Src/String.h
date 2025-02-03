#pragma once

#include <string>

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
        constexpr ~StringT() override
        {
            delete[] I::m_data;
        }

        // 将复制内容
        constexpr explicit StringT(const T* data, const usize size)
        {
            auto p_data = new T[size + 1];
            I::m_data = p_data;
            I::m_size = size;
            memcpy(p_data, data, size * sizeof(T));
            p_data[size] = 0;
        }

        constexpr const T* data() const
        {
            return I::m_data;
        }

        constexpr size_t size() const
        {
            return I::m_size;
        }
    };

    struct String8 final : StringT<String8, char8_t, FString8>
    {
        // 将复制内容
        constexpr explicit String8(const char8_t* m_data, const usize m_size) : StringT(m_data, m_size)
        {
        }

        constexpr const char* c_str() const
        {
            return reinterpret_cast<const char*>(m_data);
        }

        constexpr std::string to_string() const
        {
            return std::string(c_str(), m_size);
        }
    };

    struct String16 final : StringT<String16, char16_t, FString16>
    {
        // 将复制内容
        constexpr explicit String16(const char16_t* m_data, const usize m_size) : StringT(m_data, m_size)
        {
        }

#if _WINDOWS
        constexpr const wchar_t* c_str() const
        {
            return reinterpret_cast<const wchar_t*>(m_data);
        }

        constexpr std::wstring to_string() const
        {
            return std::wstring(c_str(), m_size);
        }
#endif
    };
}
