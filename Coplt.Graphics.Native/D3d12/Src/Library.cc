#include "Instance.h"

#include "mimalloc-new-delete.h"

#include "../../Api/FFI/Instance.h"
#include "../../Api/Include/Object.h"

#include "../FFI/Library.h"

using namespace Coplt;

void Coplt_Graphics_D3d12_Create_Instance(FResult* r, const FInstanceCreateOptions& options, FInstance** out)
{
    *r = feb([&]
    {
        *out = new D3d12Instance(options);
    });
}
