#pragma once

#ifdef FFI_SRC
#include <atomic>
#endif

#include "Object.h"

namespace Coplt
{
    struct COPLT_UUID_MARK("c2a7f23d-4cce-4eae-a1e0-8ffac4836870") COPLT_NOVTABLE FBlob COPLT_FINAL : FUnknown
    {
#ifdef FFI_SRC
        COPLT_INTERFACE_GUID("c2a7f23d-4cce-4eae-a1e0-8ffac4836870");

        mutable std::atomic_size_t m_strong{1};
#else
        size_t m_strong{1};
#endif
        size_t m_size{};

#ifdef FFI_SRC
        size_t Release() noexcept override;
        size_t AddRef() noexcept override;

        void* QueryInterface(const Guid& id) noexcept override;

    private:
        explicit FBlob(size_t size) noexcept;

    public:
        static FBlob* Create(size_t size) noexcept;
        static FBlob* Create(const u8* data, size_t size) noexcept;

        u8* data();
        const u8* data() const;
        usize size() const;

        u8* begin();
        u8* end();

        const u8* begin() const;
        const u8* end() const;
#endif
    };
}
