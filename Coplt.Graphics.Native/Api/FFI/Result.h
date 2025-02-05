#pragma once

#include "String.h"

namespace Coplt
{
    enum class FResultCode : u8
    {
        Success,
        Error,
    };

    struct FResult
    {
        FMessage msg{};
        FResultCode code{};

#ifdef FFI_SRC
        explicit FResult() : msg{.msg = {.str = nullptr}}, code(FResultCode::Success)
        {
        }

        explicit FResult(
            const FMessageUnion msg, const i32 size, const FMessageType msg_type, const FResultCode code
        ) : msg({.msg = msg, .len = size, .type = msg_type}), code(code)
        {
        }

        constexpr static FResult None()
        {
            return FResult();
        }

        constexpr static FResult Error(const char* str)
        {
            return Error(FResultCode::Error, str);
        }

#ifdef _WINDOWS

        constexpr static FResult ErrorW(const wchar_t* str)
        {
            return ErrorW(FResultCode::Error, str);
        }

#endif

        template <usize N>
        constexpr static FResult Error(const char (&str)[N])
        {
            return Error<N>(FResultCode::Error, str);
        }

        template <usize N>
        constexpr static FResult Error(const char16_t (&str)[N])
        {
            return Error<N>(FResultCode::Error, str);
        }

#ifdef _WINDOWS

        template <usize N>
        constexpr static FResult ErrorW(const wchar_t (&str)[N])
        {
            return ErrorW<N>(FResultCode::Error, str);
        }

#endif

        constexpr static FResult Error(FString8* str)
        {
            return Error(FResultCode::Error, str);
        }

        constexpr static FResult Error(FString16* str)
        {
            return Error(FResultCode::Error, str);
        }

        constexpr static FResult Error(const FResultCode code, const char* str)
        {
            return FResult({.str = str}, 0, FMessageType::CStr8, code);
        }

#ifdef _WINDOWS

        constexpr static FResult ErrorW(const FResultCode code, const wchar_t* str)
        {
            return FResult({.wstr = str}, 0, FMessageType::CStr16, code);
        }

#endif

        template <usize N>
        constexpr static FResult Error(const FResultCode code, const char (&str)[N])
        {
            return FResult({.str = str}, N, FMessageType::Slice8, code);
        }

        template <usize N>
        constexpr static FResult Error(const FResultCode code, const char16_t (&str)[N])
        {
            return FResult({.str16 = str}, N, FMessageType::Slice16, code);
        }

#ifdef _WINDOWS

        template <usize N>
        constexpr static FResult ErrorW(const FResultCode code, const wchar_t (&str)[N])
        {
            return FResult({.wstr = str}, N, FMessageType::Slice16, code);
        }

#endif

        constexpr static FResult Error(const FResultCode code, FString8* str)
        {
            return FResult({.string8 = str}, 0, FMessageType::String8, code);
        }

        constexpr static FResult Error(const FResultCode code, FString16* str)
        {
            return FResult({.string16 = str}, 0, FMessageType::String16, code);
        }
#endif
    };

#ifdef FFI_SRC
#endif
}
