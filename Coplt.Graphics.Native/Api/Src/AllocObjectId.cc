#include "../Include/AllocObjectId.h"

using namespace Coplt;

namespace
{
    std::atomic<u64> s_id_inc{0};
}

u64 Coplt::AllocObjectId()
{
    return ++s_id_inc;
}
