#pragma once

#include "Context.h"
#include "../../Api/Include/Object.h"
#include "../../Api/FFI/Record.h"
#include "Isolate.h"

namespace Coplt
{
    COPLT_INTERFACE_DEFINE(ID3d12RecordStorage, "ba14d287-5cea-4715-b655-7ad81184fd84", FUnknown)
    {
        virtual void Clear() = 0;

        virtual void StartRecord(FGpuRecordMode Mode) = 0;
        virtual void EndRecord() = 0;
        virtual void BeforeSubmit() = 0;
        virtual void AfterSubmit() = 0;

        // 返回当前 List 的 index
        virtual u32 Split() = 0;
        virtual D3d12RentedCommandList& CurList() = 0;

        virtual std::span<D3d12RentedCommandList> Lists() = 0;
    };

    struct D3d12RecordStorage final : Object<D3d12RecordStorage, ID3d12RecordStorage>
    {
        Rc<D3d12GpuDevice> m_device{};
        Rc<D3d12RecordContext> m_context{};
        D3d12RentedCommandAllocator m_cmd_allocator{};
        D3d12RentedCommandList m_cur_list{};
        std::vector<D3d12RentedCommandList> m_result_lists{};

        explicit D3d12RecordStorage(NonNull<D3d12GpuIsolate> isolate, const Rc<D3d12RecordContext>& context);

        void Clear() override;

        void StartRecord(FGpuRecordMode Mode) override;
        void EndRecord() override;
        void BeforeSubmit() override;
        void AfterSubmit() override;

        u32 Split() override;
        D3d12RentedCommandList& CurList() override;

        std::span<D3d12RentedCommandList> Lists() override;
    };

    D3D12_COMMAND_LIST_TYPE ToListType(FGpuRecordMode value);
}
