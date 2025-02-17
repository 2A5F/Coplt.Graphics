#pragma once

#include <directx/d3d12.h>

#include "../Include/Utils.h"

namespace Coplt
{
    struct CmdListPack final
    {
        ComPtr<ID3D12GraphicsCommandList> m_list0{};
        ComPtr<ID3D12GraphicsCommandList1> m_list1{};
        ComPtr<ID3D12GraphicsCommandList2> m_list2{};
        ComPtr<ID3D12GraphicsCommandList3> m_list3{};
        ComPtr<ID3D12GraphicsCommandList4> m_list4{};
        ComPtr<ID3D12GraphicsCommandList5> m_list5{};
        ComPtr<ID3D12GraphicsCommandList6> m_list6{};
        ComPtr<ID3D12GraphicsCommandList7> m_list7{};
        ComPtr<ID3D12GraphicsCommandList8> m_list8{};
        ComPtr<ID3D12GraphicsCommandList9> m_list9{};
        ComPtr<ID3D12GraphicsCommandList10> m_list10{};

        CmdListPack() = default;

        explicit CmdListPack(ComPtr<ID3D12GraphicsCommandList>&& list) noexcept : m_list0(std::move(list))
        {
            m_list0->QueryInterface<ID3D12GraphicsCommandList1>(&m_list1);
            m_list0->QueryInterface<ID3D12GraphicsCommandList2>(&m_list2);
            m_list0->QueryInterface<ID3D12GraphicsCommandList3>(&m_list3);
            m_list0->QueryInterface<ID3D12GraphicsCommandList4>(&m_list4);
            m_list0->QueryInterface<ID3D12GraphicsCommandList5>(&m_list5);
            m_list0->QueryInterface<ID3D12GraphicsCommandList6>(&m_list6);
            m_list0->QueryInterface<ID3D12GraphicsCommandList7>(&m_list7);
            m_list0->QueryInterface<ID3D12GraphicsCommandList8>(&m_list8);
            m_list0->QueryInterface<ID3D12GraphicsCommandList9>(&m_list9);
            m_list0->QueryInterface<ID3D12GraphicsCommandList10>(&m_list10);
        }

        ID3D12GraphicsCommandList* operator->() const noexcept
        {
            return m_list0.Get();
        }

        ID3D12GraphicsCommandList& operator*() const noexcept
        {
            return *m_list0.Get();
        }
    };
}
