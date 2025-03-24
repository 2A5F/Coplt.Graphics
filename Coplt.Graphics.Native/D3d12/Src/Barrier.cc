#include "Barrier.h"

using namespace Coplt;

namespace Coplt::Enhanced
{
    EnhancedBarrierAnalyzer::EnhancedBarrierAnalyzer(const Rc<D3d12GpuDevice>& device)
        : m_device(device)
    {
    }

    Rc<I3d12BarrierRecorder> EnhancedBarrierAnalyzer::CreateRecorder(const Rc<ID3d12RecordStorage>& storage)
    {
        return new EnhancedBarrierRecorder(m_device, storage);
    }

    EnhancedBarrierRecorder::ResInfo::ResInfo(const FCmdResType ResType) : ResType(ResType)
    {
    }

    EnhancedBarrierRecorder::EnhancedBarrierRecorder(const Rc<D3d12GpuDevice>& device, const Rc<ID3d12RecordStorage>& storage)
        : m_device(device), m_storage(storage)
    {
    }

    void EnhancedBarrierRecorder::Clear()
    {
        m_resources.clear();
    }

    void EnhancedBarrierRecorder::StartRecord(const std::span<FCmdRes> resources)
    {
        Clear();
        m_resources.reserve(resources.size());
        for (usize i = 0; i < resources.size(); ++i)
        {
            const auto& res = resources[i];
            m_resources.push_back(ResInfo(res.Type));
        }
    }

    void EnhancedBarrierRecorder::EndRecord()
    {
    }

    void EnhancedBarrierRecorder::OnUse(const FCmdResRef ResRef, const ResAccess Access, const ResUsage Usage, const ResLayout Layout)
    {
        const auto ResIndex = ResRef.ResIndex();
        auto& info = m_resources[ResIndex];
        if (info.InfoState == InfoState::Unused)
        {
            info.InfoState = InfoState::Input;
            info.State = ResState{
                .Access = Access,
                .Usage = Usage,
                .Layout = Layout,
            };
            return;
        }
        else if (info.InfoState == InfoState::Input)
        {
            // todo
        }
        else
        {
            // todo
        }
    }
}
