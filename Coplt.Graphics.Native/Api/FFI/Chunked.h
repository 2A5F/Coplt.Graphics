#pragma once
#include <ranges>

#include "List.h"

#ifdef FFI_SRC
#include <bit>
#endif

namespace Coplt
{
    template <typename T>
        COPLT_REQUIRES COPLT_POD(T)
    struct FChunkedMemory COPLT_FINAL
    {
        T* m_ptr{};
    };

    template <typename T>
        COPLT_REQUIRES COPLT_POD(T)
    struct FChunkedAllocator COPLT_FINAL
    {
        FList<FChunkedMemory<T>> m_list{};
        usize m_cur{};
        usize m_chunk_size{};

        #ifdef FFI_SRC

        FChunkedAllocator() = default;

        explicit FChunkedAllocator(FAllocator* allocator) : m_list(allocator), m_chunk_size(128)
        {
        }

        explicit FChunkedAllocator(FAllocator* allocator, const usize chunk_size) : m_list(allocator), m_chunk_size(chunk_size)
        {
            if (!std::has_single_bit(chunk_size)) throw std::invalid_argument("Chunk size must be a power of 2");
        }

        void Swap(FChunkedAllocator& other)
        {
            std::swap(m_list, other.m_list);
            std::swap(m_cur, other.m_cur);
            std::swap(m_chunk_size, other.m_chunk_size);
        }

        FChunkedAllocator(FChunkedAllocator&& other) noexcept
            : m_list(std::exchange(other.m_list, {})),
              m_cur(std::exchange(other.m_cur, 0)),
              m_chunk_size(std::exchange(other.m_chunk_size, 0))
        {
        }

        FChunkedAllocator& operator=(FChunkedAllocator&& other) noexcept
        {
            if (&m_list == &other.m_list) return *this;
            FChunkedAllocator(std::forward<FChunkedAllocator>(other)).Swap(*this);
            return *this;
        }

        FChunkedAllocator(FChunkedAllocator& other) = delete;

        FChunkedAllocator& operator=(FChunkedAllocator& other) = delete;

        ~FChunkedAllocator()
        {
            if (m_list.m_allocator && m_list.m_ptr)
            {
                for (usize i = 0; i < m_list.m_len; ++i)
                {
                    auto chunk = m_list[i];
                    m_list.m_allocator->MemoryFree(chunk.m_ptr, alignof(T));
                }
            }
        }

        FChunkedMemory<T> Alloc()
        {
            if (m_cur >= m_list.m_len)
            {
                if (m_list.m_allocator == nullptr) throw std::bad_alloc();
                FChunkedMemory chunk{m_list.m_allocator->MemoryAlloc(m_chunk_size, alignof(T))};
                m_list.Add(chunk);
                return chunk;
            }
            return m_list[m_cur++];
        }

        void Reset()
        {
            m_cur = 0;
        }

        #endif
    };

    // 持有分块内存的 list，分块大小以 16 为起点，每次扩容翻倍
    // 可以随机访问，但是比较慢，适合录制命令这种只写和只顺序遍历的场景
    // 对比普通 list 只能减少扩容复制的开销，用处不是很大
    template <typename T>
        COPLT_REQUIRES COPLT_POD(T)
    struct FGrowthChunkedList COPLT_FINAL
    {
        FList<FChunkedMemory<T>> m_chunks{};
        usize m_len{};
        usize m_cap{};
        usize m_cur_chunk_size{};

        #ifdef FFI_SRC

        constexpr static usize InitChunkSize = 16;
        constexpr static usize ChunkOffset = std::countr_zero(InitChunkSize);

        FGrowthChunkedList() = default;

        explicit FGrowthChunkedList(FAllocator* allocator) : m_chunks(allocator)
        {
        }

        void Swap(FGrowthChunkedList& other)
        {
            std::swap(m_chunks, other.m_chunks);
            std::swap(m_len, other.m_len);
            std::swap(m_cap, other.m_cap);
            std::swap(m_cur_chunk_size, other.m_cur_chunk_size);
        }

        FGrowthChunkedList(FGrowthChunkedList&& other) noexcept
            : m_chunks(std::exchange(other.m_chunks, nullptr)),
              m_len(other.m_len), m_cap(other.m_cap), m_cur_chunk_size(other.m_cur_chunk_size)
        {
        }

        FGrowthChunkedList& operator=(FGrowthChunkedList&& other) noexcept
        {
            if (&m_chunks == &other.m_chunks) return *this;
            FGrowthChunkedList(std::forward<FGrowthChunkedList>(other)).Swap(*this);
            return *this;
        }

        FGrowthChunkedList(FGrowthChunkedList& other) = delete;

        FGrowthChunkedList& operator=(FGrowthChunkedList& other) = delete;

        ~FGrowthChunkedList()
        {
            if (m_chunks.m_allocator && m_chunks.m_ptr)
            {
                for (usize i = 0; i < m_chunks.m_len; ++i)
                {
                    m_chunks.m_allocator.MemoryFree(m_chunks[i].m_ptr, alignof(T));
                }
            }
        }

        T& operator[](const usize index)
        {
            if (index >= m_len) throw std::out_of_range("FGrowthChunkedList::operator[]");
            const auto chunk_size = std::max(std::bit_ceil(index + 1), InitChunkSize);
            const auto chunk_index = std::countr_zero(chunk_size >> ChunkOffset);
            const auto index_in_chunk = index - (chunk_size - InitChunkSize);
            return m_chunks[chunk_index][index_in_chunk];
        }

        usize Size() const
        {
            return m_len;
        }

        // todo 优化用，暂时用不上先不写了

        #endif
    };
}
