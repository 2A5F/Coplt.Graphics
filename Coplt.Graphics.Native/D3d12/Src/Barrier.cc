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

    void EnhancedBarrierRecorder::BarrierGroup::Push(D3D12_BUFFER_BARRIER barrier)
    {
        m_buffer_barriers.push_back(barrier);
    }

    void EnhancedBarrierRecorder::BarrierGroup::Push(D3D12_TEXTURE_BARRIER barrier)
    {
        m_texture_barriers.push_back(barrier);
    }

    EnhancedBarrierRecorder::ResInfo::ResInfo(const NonNull<const FCmdRes> Res) : Res(Res)
    {
    }

    void EnhancedBarrierRecorder::ResInfo::SetNewState(const ResState state)
    {
        OldState = State;
        State = state;
    }

    EnhancedBarrierRecorder::EnhancedBarrierRecorder(const Rc<D3d12GpuDevice>& device, const Rc<ID3d12RecordStorage>& storage)
        : m_device(device), m_storage(storage)
    {
    }

    std::span<const IOResState> EnhancedBarrierRecorder::Inputs() const
    {
        return m_inputs;
    }

    std::span<const IOResState> EnhancedBarrierRecorder::Outputs() const
    {
        return m_outputs;
    }

    void EnhancedBarrierRecorder::Clear()
    {
        m_resources.clear();
        m_inputs.clear();
        m_outputs.clear();
        m_buffer_groups.clear();
    }

    void EnhancedBarrierRecorder::StartRecord(const std::span<FCmdRes> resources)
    {
        Clear();
        m_resources.reserve(resources.size());
        for (usize i = 0; i < resources.size(); ++i)
        {
            const auto& res = resources[i];
            m_resources.push_back(ResInfo(&res));
        }
        m_buffer_groups.push_back({.ListIndex = 0});
        m_buffer_groups.push_back({.ListIndex = 1});
    }

    void EnhancedBarrierRecorder::EndRecord()
    {
        for (u32 i = 0; i < m_resources.size(); ++i)
        {
            auto& info = m_resources[i];
            if (info.InfoState == InfoState::Unused) continue;
            if (info.InfoState == InfoState::Input)
            {
                m_inputs.push_back(IOResState{.ResIndex = i, .CurrentListIndex = info.CurrentListIndex, .State = info.State});
            }
            else
            {
                CreateBarrier(info);
            }
            m_outputs.push_back(IOResState{.ResIndex = i, .CurrentListIndex = info.CurrentListIndex, .State = info.State});
        }
        if (m_last_cmd_count == 0)
        {
            m_storage->DropLast();
        }
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
            info.LastUseListIndex = 0;
            info.CurrentListIndex = m_storage->CurListIndex();
            return;
        }
        else if (info.InfoState == InfoState::Input)
        {
            if (info.State.IsCompatible(new_state, true))
            {
                info.State = info.State.Merge(new_state);
                goto End;
            }
            info.InfoState = InfoState::Used;
            m_inputs.push_back(IOResState{.ResIndex = ResIndex, .CurrentListIndex = info.CurrentListIndex, .State = info.State});
            if (info.CurrentListIndex == m_storage->CurListIndex()) Split();
            goto EndState;
        }
        else
        {
            if (info.State.IsCompatible(new_state, true))
            {
                info.State = info.State.Merge(new_state);
                goto End;
            }
        }
        if (info.CurrentListIndex == m_storage->CurListIndex()) Split();
        CreateBarrier(info);
    EndState:
        info.SetNewState(new_state);
        info.LastUseListIndex = info.CurrentListIndex;
    End:
        info.CurrentListIndex = m_storage->CurListIndex();
    }

    void EnhancedBarrierRecorder::OnCmd()
    {
        m_last_cmd_count++;
    }

    void EnhancedBarrierRecorder::Split()
    {
        m_storage->Split();
        m_buffer_groups.push_back({.ListIndex = m_buffer_groups.size() - 1});
        m_last_cmd_count = 0;
    }

    void EnhancedBarrierRecorder::CreateBarrier(ResInfo& info)
    {
        const auto& res = *info.Res;
        const auto BeforeListIndex = info.LastUseListIndex;
        const auto AfterListIndex = info.CurrentListIndex - 1;
        const auto SyncBefore = GetBarrierSync(info.OldState.Access, info.OldState.Usage);
        const auto SyncAfter = GetBarrierSync(info.State.Access, info.State.Usage);
        const auto AccessBefore = GetBarrierAccess(info.OldState.Access);
        const auto AccessAfter = GetBarrierAccess(info.State.Access);
        const auto push = [&](auto& barrier)
        {
            if (BeforeListIndex >= AfterListIndex)
            {
                barrier.SyncAfter = SyncAfter;
                m_buffer_groups[BeforeListIndex].Push(barrier);
            }
            else
            {
                m_buffer_groups[BeforeListIndex].Push(barrier);

                barrier.SyncBefore = D3D12_BARRIER_SYNC_SPLIT;
                barrier.SyncAfter = SyncAfter;
                m_buffer_groups[AfterListIndex].Push(barrier);
            }
        };
        if (res.IsImage())
        {
            const auto img_data = GetImageData(res);
            D3D12_TEXTURE_BARRIER barrier{
                .SyncBefore = SyncBefore,
                .SyncAfter = D3D12_BARRIER_SYNC_SPLIT,
                .AccessBefore = AccessBefore,
                .AccessAfter = AccessAfter,
                .LayoutBefore = GetBarrierLayout(info.OldState.Layout),
                .LayoutAfter = GetBarrierLayout(info.State.Layout),
                .pResource = GetResource(res),
                .Subresources = {
                    .IndexOrFirstMipLevel = 0,
                    .NumMipLevels = img_data->m_mip_levels,
                    .FirstArraySlice = 0,
                    .NumArraySlices = img_data->m_depth_or_length,
                    .FirstPlane = 0,
                    .NumPlanes = img_data->m_planes,
                },
                .Flags = D3D12_TEXTURE_BARRIER_FLAG_NONE,
            };
            push(barrier);
        }
        else
        {
            const auto buffer_data = GetBufferData(res);
            D3D12_BUFFER_BARRIER barrier{
                .SyncBefore = SyncBefore,
                .SyncAfter = D3D12_BARRIER_SYNC_SPLIT,
                .AccessBefore = AccessBefore,
                .AccessAfter = AccessAfter,
                .pResource = GetResource(res),
                .Offset = 0,
                .Size = buffer_data->m_size,
            };
            push(barrier);
        }
    }

    void EnhancedBarrierRecorder::SubmitBarriers()
    {
        for (auto& group : m_buffer_groups)
        {
            D3D12_BARRIER_GROUP groups[2];
            u32 group_count = 0;
            if (group.m_texture_barriers.size() > 0)
            {
                groups[group_count++] = D3D12_BARRIER_GROUP{
                    .Type = D3D12_BARRIER_TYPE_TEXTURE,
                    .NumBarriers = static_cast<u32>(group.m_texture_barriers.size()),
                    .pTextureBarriers = group.m_texture_barriers.data(),
                };
            }
            if (group.m_buffer_barriers.size() > 0)
            {
                groups[group_count++] = D3D12_BARRIER_GROUP{
                    .Type = D3D12_BARRIER_TYPE_BUFFER,
                    .NumBarriers = static_cast<u32>(group.m_buffer_barriers.size()),
                    .pBufferBarriers = group.m_buffer_barriers.data(),
                };
            }
            if (group_count > 0)
            {
                m_storage->Lists()[group.ListIndex]->Barrier(group_count, groups);
                group.m_texture_barriers.clear();
                group.m_buffer_barriers.clear();
            }
        }
    }

    EnhancedBarrierCombiner::EnhancedBarrierCombiner(const Rc<D3d12GpuDevice>& device)
    {
        m_device = device;
    }

    void EnhancedBarrierCombiner::EndSubmit()
    {
        m_buffer_barriers.clear();
        m_texture_barriers.clear();
        Base::EndSubmit();
    }

    void EnhancedBarrierCombiner::Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records)
    {
        for (const auto& record : records)
        {
            const NonNull data = record->Data();
            const auto& barrier_recorder = record->BarrierRecorder();
            const auto& storage = record->Storage();

            #pragma region 生成输入屏障

            for (const auto& input : barrier_recorder->Inputs())
            {
                const auto& res = data->Resources[input.ResIndex];
                bool exist;
                auto& sr = m_submit_resources.GetOrAdd(res.GetObjectPtr()->ObjectId(), exist, [&](auto& p)
                {
                    p.put(ResInfo{
                        .Res = res,
                        .State = GetState(res),
                    });
                });
                if (!exist)
                {
                    if (res.IsImage())
                    {
                        const auto img_data = GetImageData(res);
                        D3D12_TEXTURE_BARRIER barrier{
                            .SyncBefore = D3D12_BARRIER_SYNC_NONE,
                            .SyncAfter = GetBarrierSync(input.State.Access, input.State.Usage),
                            // .SyncAfter = D3D12_BARRIER_SYNC_SPLIT, // todo 拆分屏障
                            .AccessBefore = D3D12_BARRIER_ACCESS_NO_ACCESS,
                            .AccessAfter = GetBarrierAccess(input.State.Access),
                            .LayoutBefore = GetBarrierLayout(sr.State->Layout),
                            .LayoutAfter = GetBarrierLayout(input.State.Layout),
                            .pResource = GetResource(res),
                            .Subresources = {
                                .IndexOrFirstMipLevel = 0,
                                .NumMipLevels = img_data->m_mip_levels,
                                .FirstArraySlice = 0,
                                .NumArraySlices = img_data->m_depth_or_length,
                                .FirstPlane = 0,
                                .NumPlanes = img_data->m_planes,
                            },
                            .Flags = D3D12_TEXTURE_BARRIER_FLAG_NONE,
                        };
                        m_texture_barriers.push_back(barrier);
                    }
                    else
                    {
                        const auto buffer_data = GetBufferData(res);
                        D3D12_BUFFER_BARRIER barrier{
                            .SyncBefore = D3D12_BARRIER_SYNC_NONE,
                            .SyncAfter = GetBarrierSync(input.State.Access, input.State.Usage),
                            // .SyncAfter = D3D12_BARRIER_SYNC_SPLIT, // todo 拆分屏障
                            .AccessBefore = D3D12_BARRIER_ACCESS_NO_ACCESS,
                            .AccessAfter = GetBarrierAccess(input.State.Access),
                            .pResource = GetResource(res),
                            .Offset = 0,
                            .Size = buffer_data->m_size,
                        };
                        m_buffer_barriers.push_back(barrier);
                        //todo
                    }
                }
                // todo
            }

            #pragma endregion

            #pragma region 插入输入屏障

            if (m_buffer_barriers.size() > 0 || m_texture_barriers.size() > 0)
            {
                D3D12_BARRIER_GROUP groups[2];
                u32 group_count = 0;
                if (m_texture_barriers.size() > 0)
                {
                    groups[group_count++] = D3D12_BARRIER_GROUP{
                        .Type = D3D12_BARRIER_TYPE_TEXTURE,
                        .NumBarriers = m_texture_barriers.size(),
                        .pTextureBarriers = m_texture_barriers.data(),
                    };
                }
                if (m_buffer_barriers.size() > 0)
                {
                    groups[group_count++] = D3D12_BARRIER_GROUP{
                        .Type = D3D12_BARRIER_TYPE_BUFFER,
                        .NumBarriers = m_buffer_barriers.size(),
                        .pBufferBarriers = m_buffer_barriers.data(),
                    };
                }
                // 第一个列表是保留的用于屏障的
                storage->Lists()[0]->Barrier(group_count, groups);
                m_buffer_barriers.clear();
                m_texture_barriers.clear();
            }

            #pragma endregion

            #pragma region 提交中间屏障

            barrier_recorder->SubmitBarriers();

            #pragma endregion

            #pragma region 完成录制 记录 list 范围

            {
                ListRange range{};
                // todo fence
                range.ListIndex = m_submit_lists.size();
                const auto lists = storage->Lists();
                range.ListCount = lists.size();
                range.Queue = FGpuQueueType::Direct; // todo 选择队列
                for (const auto& list : lists)
                {
                    m_submit_lists.push_back(list->ToCommandList());
                }
                m_list_ranges.push_back(range);
            }

            #pragma endregion

            #pragma region 记录输出状态

            for (const auto& output : barrier_recorder->Outputs())
            {
                const auto& res = data->Resources[output.ResIndex];
                const auto state = GetState(res);
                state->Layout = output.State.Layout;
                m_submit_resources.AddOrReplace(
                    res.GetObjectPtr()->ObjectId(), ResInfo{
                        .Res = res,
                        .State = state,
                    }
                );
            }

            #pragma endregion
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
    m_queue_deps.clear();
    m_submit_resources.Clear();
    m_used_queues = FGpuQueueFlags::None;
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
            const auto& queue = isolate->GetQueue(range.Queue);
            for (u32 i = 0; i < range.DepCount; ++i)
            {
                const auto& dep = m_queue_deps[i + range.DepIndex];
                const auto& dep_queue = isolate->GetQueue(dep);
                queue->Wait(*dep_queue, dep_queue->Signal());
            }
            queue->m_queue->ExecuteCommandLists(range.ListCount, m_submit_lists.data() + range.ListIndex);
        }

        #pragma endregion

        #pragma region 在主队列中等待其他队列

        {
            if (HasFlags(m_used_queues, FGpuQueueFlags::Compute))
            {
                const auto& dep_queue = isolate->GetQueue(FGpuQueueType::Compute);
                main_queue->Wait(*dep_queue, dep_queue->Signal());
            }
            if (HasFlags(m_used_queues, FGpuQueueFlags::Copy))
            {
                const auto& dep_queue = isolate->GetQueue(FGpuQueueType::Copy);
                main_queue->Wait(*dep_queue, dep_queue->Signal());
            }
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
