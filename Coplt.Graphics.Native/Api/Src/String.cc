#include "String.h"

using namespace Coplt;

FString* Coplt::ToString(const FStr8or16& str)
{
    switch (str.type)
    {
    case FStrType::Str16:
        return String16::Create(str.str16, str.len);
    case FStrType::Str8:
        return String8::Create(str.str8, str.len);
    }
    return nullptr;
}

String8* String8::Create(const char8_t* data)
{
    return StringT::Create(data);
}

String8* String8::Create(const char* data)
{
    return Create(data, std::char_traits<char>::length(data));
}

String8* String8::Create(const char8_t* data, const usize size)
{
    return StringT::Create(data, size);
}

String8* String8::Create(const char* data, const usize size)
{
    return StringT::Create(reinterpret_cast<const char8_t*>(data), size);
}

FStr8or16 String8::GetStr() const noexcept
{
    return FStr8or16(c_str(), size());
}

String8* Coplt::CreateString8(const char* data, const usize size)
{
    return String8::Create(data, size);
}

String16::String16(const char16_t* data, const usize size) : StringT(data, size)
{
}

String16* String16::Create(const Char16* data)
{
    return StringT::Create(data);
}

String16* String16::Create(const Char16* data, const usize size)
{
    return StringT::Create(data, size);
}

#ifdef _WINDOWS
String16* String16::Create(const wchar_t* data)
{
    return Create(data, std::char_traits<wchar_t>::length(data));
}

String16* String16::Create(const wchar_t* data, const usize size)
{
    return StringT::Create(reinterpret_cast<const char16_t*>(data), size);
}
#endif

FStr8or16 String16::GetStr() const noexcept
{
    return FStr8or16(data(), size());
}

String16* Coplt::CreateString16(const wchar_t* data, const usize size)
{
    return String16::Create(data, size);
}

FStr8or16 CString::GetStr() const noexcept
{
    return FStr8or16(data(), size());
}

CString* Coplt::CreateCString(std::string&& string)
{
    return new CString(std::forward<std::string>(string));
}

CString* Coplt::CreateCString(const char* data, const usize size)
{
    return new CString(data, size);
}

#ifdef _WINDOWS
FStr8or16 WString::GetStr() const noexcept
{
    return FStr8or16(data(), size());
}
#endif
