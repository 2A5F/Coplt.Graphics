#include "mimalloc-new-delete.h"

#include "../Include/Object.h"

using namespace Coplt;

size_t UniqueObject::Release() noexcept
{
    delete this;
    return 0;
}
