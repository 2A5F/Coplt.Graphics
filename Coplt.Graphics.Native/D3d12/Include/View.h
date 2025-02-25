#pragma once

#include <variant>

#include "../../Api/FFI/Resource.h"
#include "../Src/Buffer.h"

namespace Coplt
{
    struct View
    {
        enum class Type : u8
        {
            None,
            Buffer,
        };

    private:
        union
        {
            Rc<D3d12GpuBuffer> m_buffer;
        };

        Type m_type;

    public:
        Type Type() const { return m_type; }
        Rc<D3d12GpuBuffer>* Buffer() { return m_type == Type::Buffer ? std::addressof(m_buffer) : nullptr; }

        ~View()
        {
            // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
            switch (m_type)
            {
            case Type::None:
                return;
            case Type::Buffer:
                m_buffer.~Rc();
                return;
            }
            std::unreachable();
        }

        View() : m_type(Type::None)
        {
        }

        View(const View& other) : m_type(other.m_type)
        {
            switch (m_type)
            {
            case Type::None:
                break;
            case Type::Buffer:
                new(std::addressof(m_buffer)) Rc(other.m_buffer);
                break;
            }
        }

        View(View&& other) noexcept : m_type(std::exchange(other.m_type, Type::None))
        {
            switch (m_type)
            {
            case Type::None:
                break;
            case Type::Buffer:
                new(std::addressof(m_buffer)) Rc(std::move(other.m_buffer));
                break;
            }
        }

        View& operator=(const View& view) noexcept
        {
            this->~View();
            new(this) View(view);
            return *this;
        }

        View& operator=(View&& view) noexcept
        {
            this->~View();
            new(this) View(std::forward<View>(view));
            return *this;
        }

        View(const FView& view)
        {
            switch (view.Type)
            {
            case FViewType::None:
                m_type = Type::None;
                break;
            case FViewType::Buffer:
                m_type = Type::Buffer;
                if (view.Buffer == nullptr) new(std::addressof(m_buffer)) Rc<D3d12GpuBuffer>();
                const auto db = view.Buffer->QueryInterface<D3d12GpuBuffer>();
                if (db == nullptr)
                    COPLT_THROW("GpuBuffer from different backends.");
                new(std::addressof(m_buffer)) Rc(Rc<D3d12GpuBuffer>::UnsafeClone(db));
                break;
            }
        }

        View& operator=(const FView& view)
        {
            this->~View();
            new(this) View(view);
            return *this;
        }

        View(const Rc<D3d12GpuBuffer>& buffer) : m_type(Type::Buffer)
        {
            new(std::addressof(m_buffer)) Rc(buffer);
        }

        View(Rc<D3d12GpuBuffer>&& buffer) : m_type(Type::Buffer)
        {
            new(std::addressof(m_buffer)) Rc(std::move(buffer));
        }

        View& operator=(const Rc<D3d12GpuBuffer>& buffer)
        {
            this->~View();
            new(this) View(buffer);
            return *this;
        }

        View& operator=(Rc<D3d12GpuBuffer>&& buffer)
        {
            this->~View();
            new(this) View(std::forward<Rc<D3d12GpuBuffer>>(buffer));
            return *this;
        }
    };
}
