#include "Barrier.h"

#include "Record.h"

using namespace Coplt;

void ID3d12EnhancedBarrierAnalyzer::Group::Push(D3D12_BUFFER_BARRIER barrier)
{
    m_buffer_barriers.push_back(barrier);
}

void ID3d12EnhancedBarrierAnalyzer::Group::Push(D3D12_TEXTURE_BARRIER barrier)
{
    m_texture_barriers.push_back(barrier);
}

void ID3d12EnhancedBarrierAnalyzer::Group::Clear()
{
    m_buffer_barriers.clear();
    m_texture_barriers.clear();
}

ID3d12EnhancedBarrierAnalyzer::ResInfo::ResInfo(const NonNull<const FCmdRes> Res) : Res(Res)
{
}

void ID3d12EnhancedBarrierAnalyzer::ResInfo::SetNewState(const ResState state)
{
    OldState = State;
    State = state;
}

namespace Coplt::Enhanced
{
    EnhancedBarrierMarshal::EnhancedBarrierMarshal(const Rc<D3d12GpuDevice>& device)
        : m_device(device)
    {
    }

    Rc<ID3d12BarrierAnalyzer> EnhancedBarrierMarshal::CreateAnalyzer()
    {
        return new EnhancedBarrierAnalyzer(m_device);
    }

    Rc<ID3d12BarrierCombiner> EnhancedBarrierMarshal::CreateCombiner()
    {
        return new EnhancedBarrierCombiner(m_device);
    }

    EnhancedBarrierAnalyzer::EnhancedBarrierAnalyzer(const Rc<D3d12GpuDevice>& device)
        : m_device(device)
    {
    }

    std::span<const IOResState> EnhancedBarrierAnalyzer::Inputs() const
    {
        return m_inputs;
    }

    std::span<const IOResState> EnhancedBarrierAnalyzer::Outputs() const
    {
        return m_outputs;
    }

    std::span<EnhancedBarrierAnalyzer::Group> EnhancedBarrierAnalyzer::Groups()
    {
        return m_groups;
    }

    ID3d12EnhancedBarrierAnalyzer::Group& EnhancedBarrierAnalyzer::CurGroup()
    {
        return m_groups.back();
    }

    const ID3d12EnhancedBarrierAnalyzer::Group& EnhancedBarrierAnalyzer::CurGroup() const
    {
        return m_groups.back();
    }

    u32 EnhancedBarrierAnalyzer::CurGroupIndex() const
    {
        return m_groups.size() - 1;
    }

    void EnhancedBarrierAnalyzer::Clear()
    {
        m_resources.clear();
        m_inputs.clear();
        m_outputs.clear();
        m_groups.clear();
    }

    void EnhancedBarrierAnalyzer::StartAnalyze(const std::span<FCmdRes> resources)
    {
        Clear();
        m_resources.reserve(resources.size());
        for (usize i = 0; i < resources.size(); ++i)
        {
            const auto& res = resources[i];
            m_resources.push_back(ResInfo(&res));
        }
        m_groups.push_back({});
    }

    void EnhancedBarrierAnalyzer::EndAnalyze()
    {
        for (u32 i = 0; i < m_resources.size(); ++i)
        {
            auto& info = m_resources[i];
            if (info.InfoState == InfoState::Unused) continue;
            if (info.InfoState == InfoState::Input)
            {
                m_inputs.push_back(IOResState{.ResIndex = i, .LstGroup = info.LstGroup, .State = info.State});
            }
            else
            {
                CreateBarrier(info);
            }
            m_outputs.push_back(IOResState{.ResIndex = i, .LstGroup = info.LstGroup, .State = info.State});
        }
    }

    void EnhancedBarrierAnalyzer::OnUse(const FCmdResRef ResRef, const ResAccess Access, const ResUsage Usage, const ResLayout Layout)
    {
        const auto ResIndex = ResRef.ResIndex();
        auto& info = m_resources[ResIndex];
        const auto new_state = ResState(Access, Usage, Layout);
        if (info.InfoState == InfoState::Unused)
        {
            info.InfoState = InfoState::Input;
            info.State = new_state;
            if (m_last_cmd_count > 0) Split();
            info.PreGroup = 0;
            info.LstGroup = info.CurGroup = CurGroupIndex();
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
            m_inputs.push_back(IOResState{.ResIndex = ResIndex, .LstGroup = info.LstGroup, .State = info.State});
            if (info.CurGroup == CurGroupIndex() || m_last_cmd_count > 0) Split();
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
        if (info.CurGroup == CurGroupIndex() || m_last_cmd_count > 0) Split();
        CreateBarrier(info);
    EndState:
        info.SetNewState(new_state);
        info.PreGroup = info.CurGroup;
        info.LstGroup = info.CurGroup = CurGroupIndex();
        return;
    End:
        info.CurGroup = CurGroupIndex();
        return;
    }

    void EnhancedBarrierAnalyzer::OnUse(FCmdResRef ResRef)
    {
        const auto ResIndex = ResRef.ResIndex();
        auto& info = m_resources[ResIndex];
        if (info.InfoState == InfoState::Unused)
            COPLT_THROW("The resource is not being used and the usage location cannot be updated");
        info.CurGroup = CurGroupIndex();
    }

    void EnhancedBarrierAnalyzer::OnCmd()
    {
        m_last_cmd_count++;
    }

    void EnhancedBarrierAnalyzer::CmdNext()
    {
        CurGroup().m_cmd_count++;
    }

    void EnhancedBarrierAnalyzer::Split()
    {
        const auto& cur_group = CurGroup();
        m_groups.push_back({.m_cmd_index = cur_group.m_cmd_index + cur_group.m_cmd_count});
        m_last_cmd_count = 0;
    }

    void EnhancedBarrierAnalyzer::CreateBarrier(const ResInfo& info)
    {
        const auto& res = *info.Res;
        const auto BeforeListIndex = info.PreGroup + 1;
        const auto AfterListIndex = info.LstGroup;
        const auto SyncBefore = GetBarrierSync(info.OldState.Access, info.OldState.Usage);
        const auto SyncAfter = GetBarrierSync(info.State.Access, info.State.Usage);
        const auto AccessBefore = GetBarrierAccess(info.OldState.Access);
        const auto AccessAfter = GetBarrierAccess(info.State.Access);
        const auto push = [&](auto& barrier)
        {
            if (BeforeListIndex >= AfterListIndex)
            {
                barrier.SyncAfter = SyncAfter;
                m_groups[AfterListIndex].Push(barrier);
            }
            else
            {
                m_groups[BeforeListIndex].Push(barrier);

                barrier.SyncBefore = D3D12_BARRIER_SYNC_SPLIT;
                barrier.SyncAfter = SyncAfter;
                m_groups[AfterListIndex].Push(barrier);
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

    void EnhancedBarrierAnalyzer::Interpret(const D3d12RentedCommandList& list, ID3d12GpuRecord& record)
    {
        for (auto& group : m_groups)
        {
            SubmitBarrier(list, group);
            record.Interpret(list, group.m_cmd_index, group.m_cmd_count);
        }
    }

    void EnhancedBarrierAnalyzer::SubmitBarrier(const D3d12RentedCommandList& list, Group& group)
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
            list->Barrier(group_count, groups);
            group.Clear();
        }
    }

    EnhancedBarrierCombiner::EnhancedBarrierCombiner(const Rc<D3d12GpuDevice>& device)
    {
        m_device = device;
    }

    void EnhancedBarrierCombiner::EndSubmit()
    {
        Base::EndSubmit();
    }

    void EnhancedBarrierCombiner::Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records)
    {
        for (const auto& record : records)
        {
            const NonNull data = record->Data();
            const NonNull barrier_analyzer = record->BarrierAnalyzer()->QueryInterface<ID3d12EnhancedBarrierAnalyzer>();
            const auto& context = record->Context();

            #pragma region 生成输入屏障

            for (const auto& input : barrier_analyzer->Inputs())
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
                    const auto AfterIndex = input.LstGroup;
                    if (res.IsImage())
                    {
                        const auto img_data = GetImageData(res);
                        const auto SyncAfter = GetBarrierSync(input.State.Access, input.State.Usage);
                        D3D12_TEXTURE_BARRIER barrier{
                            .SyncBefore = D3D12_BARRIER_SYNC_NONE,
                            .SyncAfter = SyncAfter,
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
                        barrier_analyzer->Groups()[0].Push(barrier);
                    }
                    else
                    {
                        const auto buffer_data = GetBufferData(res);
                        const auto SyncAfter = GetBarrierSync(input.State.Access, input.State.Usage);
                        D3D12_BUFFER_BARRIER barrier{
                            .SyncBefore = D3D12_BARRIER_SYNC_NONE,
                            .SyncAfter = D3D12_BARRIER_SYNC_SPLIT,
                            .AccessBefore = D3D12_BARRIER_ACCESS_NO_ACCESS,
                            .AccessAfter = GetBarrierAccess(input.State.Access),
                            .pResource = GetResource(res),
                            .Offset = 0,
                            .Size = buffer_data->m_size,
                        };
                        barrier.SyncAfter = SyncAfter;
                        barrier_analyzer->Groups()[0].Push(barrier);
                        // if (AfterIndex == 0)
                        // {
                        //     barrier.SyncAfter = SyncAfter;
                        //     barrier_recorder->BarrierGroups()[0].Push(barrier);
                        // }
                        // else
                        // {
                        //     barrier_recorder->BarrierGroups()[0].Push(barrier);
                        //     barrier.SyncBefore = D3D12_BARRIER_SYNC_SPLIT;
                        //     barrier.SyncAfter = SyncAfter;
                        //     barrier_recorder->BarrierGroups()[AfterIndex].Push(barrier);
                        // }
                    }
                }
                // todo
            }

            #pragma endregion

            #pragma region 完成录制 记录 list 范围

            {
                auto allocator = context->m_cmd_alloc_pool->RentCommandAllocator(GetType(data->Mode));
                auto list = allocator.RentCommandList();

                barrier_analyzer->Interpret(list, *record);

                ListNode node{};
                // todo fence
                node.List = list->ToCommandList();
                node.Queue = FGpuQueueType::Direct; // todo 选择队列
                m_list_node.push_back(node);

                list.Close();
                context->m_recycled_command_allocators.push_back(std::move(allocator));
            }

            #pragma endregion

            #pragma region 记录输出状态

            for (const auto& output : barrier_analyzer->Outputs())
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
    m_list_node.clear();
    m_queue_deps.clear();
    m_submit_resources.Clear();
    m_used_queues = FGpuQueueFlags::None;
}

bool AD3d12BarrierCombiner::Submit(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records)
{
    StartSubmit();
    Process(isolate, records);
    const auto has_submit = m_list_node.size() > 0;
    if (has_submit)
    {
        const auto& main_queue = isolate->m_main_queue;

        #pragma region 提交命令列表

        for (const auto& node : m_list_node)
        {
            const auto& queue = isolate->GetQueue(node.Queue);
            for (u32 i = 0; i < node.DepCount; ++i)
            {
                const auto& dep = m_queue_deps[i + node.DepIndex];
                const auto& dep_queue = isolate->GetQueue(dep);
                queue->Wait(*dep_queue, dep_queue->Signal());
            }
            queue->m_queue->ExecuteCommandLists(1, &node.List);
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
    EndSubmit();
    return has_submit;
}
