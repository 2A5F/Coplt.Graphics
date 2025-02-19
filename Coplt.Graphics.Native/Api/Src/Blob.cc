#include "../FFI/Blob.h"

#include <mimalloc.h>

#include "../Include/Object.h"

using namespace Coplt;

size_t FBlob::Release() noexcept
{
    const size_t r = m_strong.fetch_sub(1, std::memory_order_release);
    if (r != 1) return r;
    this->~FBlob();
    mi_free(this);
    return r;
}

size_t FBlob::AddRef() noexcept
{
    return m_strong.fetch_add(1, std::memory_order_relaxed);
}

void* FBlob::QueryInterface(const Guid& id) noexcept
{
    if (id == FUnknown::GUID) return static_cast<FUnknown*>(this);
    if (id == GUID) return this;
    return nullptr;
}

FBlob::FBlob(const size_t size) noexcept : m_size(size)
{
}

FBlob* FBlob::Create(const size_t size) noexcept
{
    auto mem = static_cast<char*>(mi_malloc_aligned(sizeof(FBlob) + size, alignof(FBlob)));
    return new (mem) FBlob(size);
}

FBlob* FBlob::Create(const u8* data, const size_t size) noexcept
{
    Rc r = Create(size);
    memcpy(r->data(), data, size);
    return r.leak();
}

u8* FBlob::data()
{
    return reinterpret_cast<u8*>(&this->m_size + 1);
}

const u8* FBlob::data() const
{
    return reinterpret_cast<const u8*>(&this->m_size + 1);
}

usize FBlob::size() const
{
    return m_size;
}

u8* FBlob::begin()
{
    return data();
}

u8* FBlob::end()
{
    return data() + size();
}

const u8* FBlob::begin() const
{
    return data();
}

const u8* FBlob::end() const
{
    return data() + size();
}
