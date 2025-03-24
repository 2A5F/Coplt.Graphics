#pragma once

#include "Device.h"
#include "RecordStorage.h"
#include "../../Api/Include/Object.h"
#include "../../Api/FFI/Cmd.h"
#include "../Include/ResState.h"

namespace Coplt
{
    struct I3d12BarrierRecorder;

    COPLT_INTERFACE_DEFINE(I3d12BarrierAnalyzer, "792f273c-376d-4027-af99-0d14d3d71302", FUnknown)
    {
        virtual Rc<I3d12BarrierRecorder> CreateRecorder(const Rc<ID3d12RecordStorage>& storage) = 0;
    };

    COPLT_INTERFACE_DEFINE(I3d12BarrierRecorder, "115741ea-2138-4c92-9f4f-94f91d254587", FUnknown)
    {
        virtual void Clear() = 0;
        virtual void StartRecord(std::span<FCmdRes> resources) = 0;
        virtual void EndRecord() = 0;

        virtual void OnUse(FCmdResRef ResRef, ResAccess Access, ResUsage Usage, ResLayout Layout) = 0;
    };

    namespace Enhanced
    {
        struct EnhancedBarrierAnalyzer final : Object<EnhancedBarrierAnalyzer, I3d12BarrierAnalyzer>
        {
            Rc<D3d12GpuDevice> m_device;

            explicit EnhancedBarrierAnalyzer(const Rc<D3d12GpuDevice>& device);

            Rc<I3d12BarrierRecorder> CreateRecorder(const Rc<ID3d12RecordStorage>& storage) override;
        };

        struct EnhancedBarrierRecorder final : Object<EnhancedBarrierRecorder, I3d12BarrierRecorder>
        {
            enum class InfoState : u8
            {
                Unused,
                Input,
                Used,
            };

            struct ResInfo
            {
                ResState State{};
                const FCmdResType ResType{};
                InfoState InfoState{};

                explicit ResInfo(FCmdResType ResType);
            };

            Rc<D3d12GpuDevice> m_device{};
            Rc<ID3d12RecordStorage> m_storage{};
            std::vector<ResInfo> m_resources{};

            explicit EnhancedBarrierRecorder(const Rc<D3d12GpuDevice>& device, const Rc<ID3d12RecordStorage>& storage);

            void Clear() override;
            void StartRecord(std::span<FCmdRes> resources) override;
            void EndRecord() override;
            void OnUse(FCmdResRef ResRef, ResAccess Access, ResUsage Usage, ResLayout Layout) override;
        };
    }
}
