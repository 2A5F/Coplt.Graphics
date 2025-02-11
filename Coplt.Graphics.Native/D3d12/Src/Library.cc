#include "Instance.h"

#include "mimalloc-new-delete.h"

#include "../../Api/FFI/Instance.h"
#include "../../Api/Include/Object.h"

#include "../FFI/Library.h"

#include "../../Api/Include/HashMap.h"

using namespace Coplt;

FInstance* Coplt_Graphics_D3d12_Create_Instance()
{
    HashMap<i32, i32> test{};
    for (u32 i = 0; i < 10; i++)
    {
        test.TryAdd(i, i);
    }

    return new D3d12Instance();
}
