#include "String.h"

using namespace Coplt;

FString* Str8or16::ToString() const
{
    if (has16()) return String16::Create(str16, len);
    if (has8()) return String8::Create(str8, len);
    return nullptr;
}
