#pragma once

#include <directx/d3d12.h>

#include "../../Api/FFI/Queue.h"

namespace Coplt
{
    template <Fn<void, D3D12_CLEAR_VALUE&> F>
    inline D3D12_RENDER_PASS_BEGINNING_ACCESS ToDx(const FLoadOp load, F SetClearValue)
    {
        D3D12_RENDER_PASS_BEGINNING_ACCESS acc{};
        switch (load)
        {
        case FLoadOp::Load:
            acc.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
            break;
        case FLoadOp::Clear:
            acc.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
            SetClearValue(acc.Clear.ClearValue);
            break;
        case FLoadOp::Discard:
            acc.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
            break;
        case FLoadOp::NoAccess:
            acc.Type = D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_NO_ACCESS;
            break;
        }
        return acc;
    }

    template <Fn<void, D3D12_RENDER_PASS_ENDING_ACCESS_RESOLVE_PARAMETERS&> F>
    D3D12_RENDER_PASS_ENDING_ACCESS ToDx(const FStoreOp store, F SetResolve)
    {
        D3D12_RENDER_PASS_ENDING_ACCESS acc{};
        switch (store)
        {
        case FStoreOp::Store:
            acc.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
            break;
        case FStoreOp::Discard:
            acc.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
            break;
        case FStoreOp::Resolve:
            acc.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_RESOLVE;
            SetResolve(acc.Resolve);
            break;
        case FStoreOp::NoAccess:
            acc.Type = D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_NO_ACCESS;
            break;
        }
        return acc;
    }
}
