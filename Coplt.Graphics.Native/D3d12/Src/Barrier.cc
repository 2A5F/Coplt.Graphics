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

bool ID3d12EnhancedBarrierAnalyzer::Group::IsEmpty() const
{
    return m_buffer_barriers.empty() && m_texture_barriers.empty();
}

ID3d12EnhancedBarrierAnalyzer::ResInfo::ResInfo(const u32 ResIndex) : ResIndex(ResIndex)
{
}

void ID3d12EnhancedBarrierAnalyzer::ResInfo::SetNewState(const ResState state)
{
    OldState = State;
    State = state;
}

namespace Coplt::Enhanced
{
    EnhancedBarrierMarshal::EnhancedBarrierMarshal(const D3d12GpuIsolate& isolate)
        : m_isolate_config(isolate.m_config), m_device(isolate.m_device)
    {
    }

    Rc<ID3d12BarrierAnalyzer> EnhancedBarrierMarshal::CreateAnalyzer()
    {
        return new EnhancedBarrierAnalyzer(*this);
    }

    Rc<ID3d12BarrierCombiner> EnhancedBarrierMarshal::CreateCombiner()
    {
        return new EnhancedBarrierCombiner(m_device);
    }

    EnhancedBarrierAnalyzer::EnhancedBarrierAnalyzer(const EnhancedBarrierMarshal& marshal)
        : m_isolate_config(marshal.m_isolate_config), m_device(marshal.m_device)
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

    void EnhancedBarrierAnalyzer::StartAnalyze(NonNull<D3d12GpuRecord> record)
    {
        m_record = record;
        Clear();
        m_use_split_barrier = m_isolate_config->UseSplitBarrier;
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
        m_record = NonNull<D3d12GpuRecord>::Unchecked(nullptr);
    }

    void EnhancedBarrierAnalyzer::OnAddRes()
    {
        m_resources.push_back(ResInfo(m_resources.size()));
    }

    void EnhancedBarrierAnalyzer::OnUse(const FResIndex ResIndex, const ResAccess Access, const ResUsage Usage, const ResLayout Layout)
    {
        auto& info = m_resources[ResIndex];
        const auto new_state = ResState(Access, Usage, Layout);
        const auto need_split = m_use_split_barrier && m_last_cmd_count > 0;
        if (info.InfoState == InfoState::Unused)
        {
            info.InfoState = InfoState::Input;
            info.State = new_state;
            if (need_split) Split();
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
            if (info.CurGroup == CurGroupIndex() || need_split) Split();
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
        if (info.CurGroup == CurGroupIndex() || need_split) Split();
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

    void EnhancedBarrierAnalyzer::OnUse(const FResIndex ResIndex, const View& View, const Layout::GroupItemInfo& info)
    {
        auto access = ResAccess::Common;
        auto usage = ResUsage::Common;
        auto layout = ResLayout::Undefined;
        for (const auto stage : IterStage(info.Stages))
        {
            switch (stage)
            {
            case FShaderStage::Compute:
                usage |= ResUsage::Compute;
                break;
            case FShaderStage::Pixel:
                usage |= ResUsage::Pixel;
                break;
            case FShaderStage::Vertex:
            case FShaderStage::Mesh:
            case FShaderStage::Task:
                usage |= ResUsage::VertexOrMesh;
                break;
            default: ;
            }
        }
        switch (info.View)
        {
        case FShaderLayoutItemView::Cbv:
            access = ResAccess::ConstantBufferRead;
            break;
        case FShaderLayoutItemView::Srv:
            access = ResAccess::ShaderResourceRead;
            if (!IsBuffer(info.Type)) layout = ResLayout::ShaderResource;
            break;
        case FShaderLayoutItemView::Uav:
            switch (info.UavAccess)
            {
            case FResourceAccess::ReadOnly:
                access = ResAccess::UnorderedAccessRead;
                break;
            case FResourceAccess::WriteOnly:
                access = ResAccess::UnorderedAccessWrite;
                break;
            case FResourceAccess::ReadWrite:
            case FResourceAccess::Unknown:
            default:
                access = ResAccess::UnorderedAccessRead | ResAccess::UnorderedAccessWrite;
                break;
            }
            if (!IsBuffer(info.Type)) layout = ResLayout::UnorderedAccess;
            break;
        case FShaderLayoutItemView::Sampler:
        case FShaderLayoutItemView::Constants:
        case FShaderLayoutItemView::StaticSampler:
            return;
        }
        OnUse(ResIndex, access, usage, layout);
    }

    void EnhancedBarrierAnalyzer::UpdateUse(const FResIndex ResIndex)
    {
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
        const auto& res = m_record->m_resource_infos[info.ResIndex];
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
            else if (m_use_split_barrier)
            {
                m_groups[BeforeListIndex].Push(barrier);

                barrier.SyncBefore = D3D12_BARRIER_SYNC_SPLIT;
                barrier.SyncAfter = SyncAfter;
                m_groups[AfterListIndex].Push(barrier);
            }
            else
            {
                barrier.SyncAfter = SyncAfter;
                m_groups[BeforeListIndex].Push(barrier);
            }
        };
        if (res.IsImage())
        {
            const auto img_data = res.GetImageData();
            D3D12_TEXTURE_BARRIER barrier{
                .SyncBefore = SyncBefore,
                .SyncAfter = D3D12_BARRIER_SYNC_SPLIT,
                .AccessBefore = AccessBefore,
                .AccessAfter = AccessAfter,
                .LayoutBefore = GetBarrierLayout(info.OldState.Layout),
                .LayoutAfter = GetBarrierLayout(info.State.Layout),
                .pResource = res.GetResource(),
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
            const auto buffer_data = res.GetBufferData();
            D3D12_BUFFER_BARRIER barrier{
                .SyncBefore = SyncBefore,
                .SyncAfter = D3D12_BARRIER_SYNC_SPLIT,
                .AccessBefore = AccessBefore,
                .AccessAfter = AccessAfter,
                .pResource = res.GetResource(),
                .Offset = 0,
                .Size = buffer_data->m_size,
            };
            push(barrier);
        }
    }

    void EnhancedBarrierAnalyzer::Interpret(const D3d12RentedCommandList& list, ID3d12GpuRecord& record)
    {
        record.BeforeInterpret(list);
        for (auto& group : m_groups)
        {
            SubmitBarrier(list, group);
            record.Interpret(list, group.m_cmd_index, group.m_cmd_count);
        }
        record.AfterInterpret(list);
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
        m_tmp_group.Clear();
    }

    void EnhancedBarrierCombiner::Process(NonNull<D3d12GpuIsolate> isolate, std::span<Rc<ID3d12GpuRecord>> records)
    {
        for (const auto& record : records)
        {
            const auto resource_infos = record->ResourceInfos();
            const NonNull barrier_analyzer = record->BarrierAnalyzer()->QueryInterface<ID3d12EnhancedBarrierAnalyzer>();
            const auto& context = record->Context();

            const auto& result_list = record->ResultList();

            #pragma region 生成输入屏障

            for (const auto& input : barrier_analyzer->Inputs())
            {
                const auto& res = resource_infos[input.ResIndex];
                bool exist;
                auto& sr = m_submit_resources.GetOrAdd(res.Resource->ObjectId(), exist, [&](auto& p)
                {
                    p.put(ResInfo{
                        .Record = record,
                        .ResIndex = input.ResIndex,
                        .State = res.GetState(),
                    });
                });
                if (!exist)
                {
                    if (sr.State->Layout != input.State.Layout)
                    {
                        auto& group = result_list ? m_tmp_group : barrier_analyzer->Groups()[0];
                        if (res.IsImage())
                        {
                            const auto img_data = res.GetImageData();
                            const auto SyncAfter = GetBarrierSync(input.State.Access, input.State.Usage);
                            D3D12_TEXTURE_BARRIER barrier{
                                .SyncBefore = D3D12_BARRIER_SYNC_NONE,
                                .SyncAfter = SyncAfter,
                                .AccessBefore = D3D12_BARRIER_ACCESS_NO_ACCESS,
                                .AccessAfter = GetBarrierAccess(input.State.Access),
                                .LayoutBefore = GetBarrierLayout(sr.State->Layout),
                                .LayoutAfter = GetBarrierLayout(input.State.Layout),
                                .pResource = res.GetResource(),
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
                            group.Push(barrier);
                        }
                        else
                        {
                            const auto buffer_data = res.GetBufferData();
                            const auto SyncAfter = GetBarrierSync(input.State.Access, input.State.Usage);
                            D3D12_BUFFER_BARRIER barrier{
                                .SyncBefore = D3D12_BARRIER_SYNC_NONE,
                                .SyncAfter = D3D12_BARRIER_SYNC_SPLIT,
                                .AccessBefore = D3D12_BARRIER_ACCESS_NO_ACCESS,
                                .AccessAfter = GetBarrierAccess(input.State.Access),
                                .pResource = res.GetResource(),
                                .Offset = 0,
                                .Size = buffer_data->m_size,
                            };
                            barrier.SyncAfter = SyncAfter;
                            group.Push(barrier);
                        }
                    }
                }
                // todo
            }

            #pragma endregion

            #pragma region 完成录制 记录 list 节点

            {
                ListNode node{};
                // todo fence
                node.Queue = FGpuQueueType::Direct; // todo 选择队列
                node.ListIndex = m_lists.size();
                node.ListCount++;

                if (!m_tmp_group.IsEmpty())
                {
                    auto allocator = context->m_cmd_alloc_pool->RentCommandAllocator(GetType(record->Data()->Mode));
                    auto list = allocator.RentCommandList();
                    EnhancedBarrierAnalyzer::SubmitBarrier(list, m_tmp_group);
                    list.Close();
                    m_lists.push_back(list->ToCommandList());
                    node.ListCount++;
                    m_rented_lists.push_back(std::move(list));
                    context->m_recycled_command_allocators.push_back(std::move(allocator));
                }

                if (result_list)
                {
                    m_lists.push_back(result_list->ToCommandList());
                }
                else
                {
                    auto allocator = context->m_cmd_alloc_pool->RentCommandAllocator(GetType(record->Data()->Mode));
                    auto list = allocator.RentCommandList();
                    barrier_analyzer->Interpret(list, *record);
                    list.Close();
                    m_lists.push_back(list->ToCommandList());
                    m_rented_lists.push_back(std::move(list));
                    context->m_recycled_command_allocators.push_back(std::move(allocator));
                }

                m_list_node.push_back(node);
            }

            #pragma endregion

            #pragma region 记录输出状态

            for (const auto& output : barrier_analyzer->Outputs())
            {
                const auto& res = resource_infos[output.ResIndex];
                const auto state = res.GetState();
                state->Layout = output.State.Layout;
                m_submit_resources.AddOrReplace(
                    res.Resource->ObjectId(), ResInfo{
                        .Record = record,
                        .ResIndex = output.ResIndex,
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
    m_lists.clear();
    m_rented_lists.clear();
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
            queue->m_queue->ExecuteCommandLists(node.ListCount, m_lists.data() + node.ListIndex);
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
    for (const auto& record : records)
    {
        record->AfterSubmit();
    }
    return has_submit;
}
