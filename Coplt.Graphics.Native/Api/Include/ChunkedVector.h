#pragma once

#include <vector>
#include <bit>

#include "Error.h"
#include "mimalloc.h"

namespace Coplt
{
    // 具有指针稳定性的分块 vector
    template <class T>
    class ChunkedVector
    {
        struct Chunk final
        {
            T* data{};
            size_t size{};

            Chunk() = default;

            explicit Chunk(const size_t cap)
            {
                data = static_cast<T*>(mi_malloc_aligned(sizeof(T) * cap, alignof(T)));
            }

            Chunk(const Chunk&) = delete;
            Chunk& operator=(const Chunk&) = delete;

            Chunk(Chunk&& other) noexcept :
                data(std::exchange(other.data, nullptr)),
                size(std::exchange(other.size, 0))
            {
            }

            Chunk& operator=(Chunk&& other) noexcept
            {
                Chunk(std::forward<Chunk>(other)).swap(*this);
                return *this;
            }

            void swap(Chunk& other) noexcept
            {
                std::swap(data, other.data);
                std::swap(size, other.size);
            }

            ~Chunk()
            {
                if (!data) return;
                clear();
                mi_free(data);
            }

            void clear()
            {
                if constexpr (!std::is_trivially_destructible_v<T>)
                {
                    for (size_t i = 0; i < size; ++i)
                    {
                        data[i].~T();
                    }
                }
                size = 0;
            }
        };

        std::vector<Chunk> m_chunks{};
        size_t const m_chunk_size_mask{};
        size_t const m_chunk_size_shift{};
        size_t m_size{};

    public:
        constexpr static size_t DefaultChunkSize = 128;

        ChunkedVector() : ChunkedVector(DefaultChunkSize)
        {
        }

        explicit ChunkedVector(const size_t chunk_size)
            : m_chunk_size_mask(chunk_size - 1), m_chunk_size_shift(std::countr_zero(chunk_size))
        {
            if (chunk_size < 2) throw std::invalid_argument("Chunk size must be greater than 2");
            if (!std::has_single_bit(chunk_size)) throw std::invalid_argument("Chunk size must be a power of 2");
        }

        size_t size() const
        {
            return m_size;
        }

        T* at(const size_t index)
        {
            if (index >= m_size) throw std::out_of_range("Chunk index out of range");
            auto chunk = index >> m_chunk_size_shift;
            auto sub_index = index & m_chunk_size_mask;
            return m_chunks[chunk].data + sub_index;
        }

        const T* at(const size_t index) const
        {
            if (index >= m_size) throw std::out_of_range("Chunk index out of range");
            auto chunk = index >> m_chunk_size_shift;
            auto sub_index = index & m_chunk_size_mask;
            return m_chunks[chunk].data + sub_index;
        }

        T& operator[](const size_t index)
        {
            return *at(index);
        }

        T& operator[](const size_t index) const
        {
            return *at(index);
        }

        T* add(const T& value)
        {
            auto chunk_index = m_size >> m_chunk_size_shift;
            if (chunk_index >= m_chunks.size()) m_chunks.push_back(Chunk(m_chunk_size_mask + 1));
            auto& chunk = m_chunks[chunk_index];
            auto ptr = chunk.data + chunk.size;
            *ptr = value;
            ++chunk.size;
            ++m_size;
            return ptr;
        }

        T* add(T&& value)
        {
            auto chunk_index = m_size >> m_chunk_size_shift;
            if (chunk_index >= m_chunks.size()) m_chunks.push_back(Chunk(m_chunk_size_mask + 1));
            auto& chunk = m_chunks[chunk_index];
            auto ptr = chunk.data + chunk.size;
            *ptr = std::forward<T>(value);
            ++chunk.size;
            ++m_size;
            return ptr;
        }

        void clear()
        {
            const auto chunk_size = m_chunk_size_mask + 1;
            for (size_t i = 0, c = 0; i < m_chunks.size(), c < m_size; ++i, c += chunk_size)
            {
                auto& chunk = m_chunks[i];
                chunk.clear();
            }
        }
    };
}
