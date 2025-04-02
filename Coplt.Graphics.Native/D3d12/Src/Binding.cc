#include "Binding.h"

#include "Layout.h"
#include "../../Api/Include/Error.h"
#include "Queue.h"
#include "../../Api/Include/AllocObjectId.h"
#include "../Include/GraphicsFormat.h"

using namespace Coplt;

D3d12ShaderBinding::D3d12ShaderBinding(
    Rc<D3d12GpuDevice>&& device, const FShaderBindingCreateOptions& options
) : m_device(std::move(device))
{
    m_dx_device = m_device->m_device;
}

FResult D3d12ShaderBinding::SetName(const FStr8or16& name) noexcept
{
    return FResult::None();
}
