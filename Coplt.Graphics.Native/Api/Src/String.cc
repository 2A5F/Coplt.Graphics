#include "String.h"

using namespace Coplt;

FString* FStr8or16::ToString() const
{
    switch (type)
    {
    case FStrType::Str16:
        return String16::Create(str16, len);
    case FStrType::Str8:
        return String8::Create(str8, len);
    }
    return nullptr;
}
