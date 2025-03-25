#include "Barrier.h"

#include "Record.h"

using namespace Coplt;

namespace Coplt::Enhanced
{
    EnhancedBarrierAnalyzer::EnhancedBarrierAnalyzer(const Rc<D3d12GpuDevice>& device)
        : m_device(device)
    {
    }

    Rc<ID3d12BarrierRecorder> EnhancedBarrierAnalyzer::CreateRecorder(const Rc<ID3d12RecordStorage>& storage)
    {
        return new EnhancedBarrierRecorder(m_device, storage);
    }

    Rc<ID3d12BarrierCombiner> EnhancedBarrierAnalyzer::CreateCombiner()
    {
        return new EnhancedBarrierCombiner(m_device);
    }

    EnhancedBarrierRecorder::ResInfo::ResInfo(const FCmdResType ResType) : ResType(ResType)
    {
    }

    EnhancedBarrierRecorder::EnhancedBarrierRecorder(const Rc<D3d12GpuDevice>& device, const Rc<ID3d12RecordStorage>& storage)
        : m_device(device), m_storage(storage)
    {
    }

    std::span<const InputResState> EnhancedBarrierRecorder::Inputs() const
    {
        return m_inputs;
    }

    void EnhancedBarrierRecorder::Clear()
    {
        m_resources.clear();
        m_inputs.clear();
        m_groups.clear();
        m_begin_buffer_barriers.clear();
        m_begin_texture_barriers.clear();
        m_end_buffer_barriers.clear();
        m_end_texture_barriers.clear();
        m_current_begin_barrier_group_index = 0;
        m_current_end_barrier_group_index = 0;
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
        m_groups.push_back({});
    }

    void EnhancedBarrierRecorder::EndRecord()
    {
    }

    void EnhancedBarrierRecorder::OnUse(const FCmdResRef ResRef, const ResAccess Access, const ResUsage Usage, const ResLayout Layout)
    {
        const auto ResIndex = ResRef.ResIndex();
        auto& info = m_resources[ResIndex];
        const auto new_state = ResState(Access, Usage, Layout);
        if (info.InfoState == InfoState::Unused)
        {
            info.InfoState = InfoState::Input;
            info.State = new_state;
            return;
        }
        else if (info.InfoState == InfoState::Input)
        {
            if (info.State.IsCompatible(new_state, true))
            {
                info.State = info.State.Merge(new_state);
                return;
            }
            const auto list_index = m_storage->Split();
            m_inputs.push_back(InputResState{.ResIndex = ResIndex, .ListIndex = list_index, .State = info.State});
            info.State = new_state;
            info.InfoState = InfoState::Used;
            // todo
            return;
        }
        else
        {
            if (info.State.IsCompatible(new_state, true))
            {
                info.State = info.State.Merge(new_state);
                return;
            }
            // todo
        }
    }

    EnhancedBarrierCombiner::EnhancedBarrierCombiner(const Rc<D3d12GpuDevice>& device)
    {
        m_device = device;
    }

    void EnhancedBarrierCombiner::Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records)
    {
        for (const auto& record : records)
        {
            const NonNull data = record->Data();
            const auto& barrier_recorder = record->BarrierRecorder();
            for (const auto& input : barrier_recorder->Inputs())
            {
                const auto& res = data->Resources[input.ResIndex];
                bool exist;
                auto& sr = m_submit_resources.GetOrAdd(res.GetObjectPtr()->ObjectId(), exist, [&](auto& p)
                {
                    p.put(ResInfo{
                        .Res = res,
                    });
                });
                // todo
            }
        }
    }
}

void AD3d12BarrierCombiner::StartSubmit()
{
}

void AD3d12BarrierCombiner::EndSubmit()
{
    m_submit_lists.clear();
    m_list_ranges.clear();
    m_fence_info.clear();
    m_used_queues.clear();
    m_submit_resources.Clear();
}

bool AD3d12BarrierCombiner::Submit(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records)
{
    StartSubmit();
    Process(isolate, records);
    for (const auto& record : records)
    {
        const auto& storage = record->Storage();
        storage->BeforeSubmit();
    }
    const auto has_submit = m_list_ranges.size() > 0;
    if (has_submit)
    {
        const auto& main_queue = isolate->m_main_queue;

        #pragma region 提交命令列表

        for (const auto& range : m_list_ranges)
        {
            const auto& queue = isolate->GetQueue(range.QueueAt);
            for (u32 i = 0; i < range.FenceCount; ++i)
            {
                const auto& at = m_fence_info[i + range.FenceIndex];
                const auto& dep_queue = isolate->GetQueue(at);
                queue->Wait(*dep_queue, dep_queue->Signal());
            }
            queue->m_queue->ExecuteCommandLists(range.ListCount, m_submit_lists.data() + range.ListIndex);
        }

        #pragma endregion

        #pragma region 在主队列中等待其他队列

        for (auto& at : m_used_queues)
        {
            if (at.IsMain()) continue;
            const auto& dep_queue = isolate->GetQueue(at);
            main_queue->Wait(*dep_queue, dep_queue->Signal());
        }

        #pragma endregion

        #pragma region 所有录制都在主队列中等待执行结束

        const auto fence_value = main_queue->Signal();
        for (const auto& record : records)
        {
            record->RegisterWaitPoint(QueueWaitPoint(main_queue, fence_value));
        }

        #pragma endregion
    }
    for (const auto& record : records)
    {
        const auto& storage = record->Storage();
        storage->AfterSubmit();
    }
    EndSubmit();
    return has_submit;
}
