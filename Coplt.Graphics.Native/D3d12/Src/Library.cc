#include "Instance.h"

#include "mimalloc-new-delete.h"

#include "../../Api/FFI/Instance.h"
#include "../../Api/Include/Object.h"

#include "../FFI/Library.h"

using namespace Coplt;

FInstance* Coplt_Graphics_D3d12_Create_Instance()
{
    return new D3d12Instance();
}
