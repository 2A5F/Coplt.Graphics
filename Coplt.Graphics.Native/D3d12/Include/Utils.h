#pragma once

#include <wrl/client.h>
#include "../ThirdParty/DirectXTK12/Src/PlatformHelpers.h"
#include "../../Api/Include/Error.h"
#include <directx/d3d12.h>

namespace Coplt
{
    using Microsoft::WRL::ComPtr;

    inline void ThrowIfFailed(const HRESULT hr)
    {
        if (FAILED(hr))
        {
            LPWSTR msg = nullptr;
            FormatMessageW(
                FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<LPWSTR>(&msg), 0, nullptr
            );
            if (nullptr != msg)
            {
                auto str = std::wstring(msg);
                LocalFree(msg);
                throw ComException(std::move(str));
            }
            throw DirectX::com_exception(hr);
        }
    }

    struct CheckHR final
    {
    };

    constexpr inline static CheckHR chr;

    inline HRESULT operator |(CheckHR, const HRESULT r)
    {
        ThrowIfFailed(r);
        return r;
    }

    inline HRESULT operator |(const HRESULT r, CheckHR)
    {
        ThrowIfFailed(r);
        return r;
    }

    template <class T>
    struct SetName_t final
    {
        T str;
    };

    inline auto SetNameEx(const Str8or16& str)
    {
        return SetName_t{&str};
    }

    template <class T>
    HRESULT operator >>(const ComPtr<ID3D12Object>& obj, const SetName_t<T> arg)
    {
        return obj.Get() >> arg;
    }

    inline HRESULT operator >>(ID3D12Object* obj, const SetName_t<const Str8or16*> arg)
    {
        if (arg.str->has16())
        {
            return obj->SetPrivateData(WKPDID_D3DDebugObjectNameW, static_cast<UINT>(arg.str->len * 2), arg.str->str16);
        }
        else if (arg.str->has8())
        {
            return obj->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(arg.str->len), arg.str->str8);
        }
        return S_OK;
    }
}
