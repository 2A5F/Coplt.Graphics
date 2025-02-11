#include "../Include/HashMap.h"

using namespace Coplt;
using namespace _HashMap_internal;

namespace
{
    constexpr i32 HashPrime = 101;

    constexpr i32 MaxPrimeArrayLength = 0x7FFFFFC3;

    constexpr i32 Primes[] = {
        3, 7, 11, 17, 23, 29, 37, 47, 59, 71, 89, 107, 131, 163, 197, 239, 293, 353, 431, 521, 631, 761, 919,
        1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
        17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
        187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
        1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369
    };
}

bool HashHelpers::IsPrime(const i32 candidate)
{
    if ((candidate & 1) != 0)
    {
        const i32 limit = static_cast<i32>(std::sqrt(candidate));
        for (i32 divisor = 3; divisor <= limit; divisor += 2)
        {
            if ((candidate % divisor) == 0)
                return false;
        }
        return true;
    }
    return candidate == 2;
}

i32 HashHelpers::GetPrime(const i32 min)
{
    if (min < 0) throw std::invalid_argument("min must be non-negative");

    for (i32 i = 0; i < std::size(Primes); ++i)
    {
        if (const auto prime = Primes[i]; prime > min)
            return prime;
    }

    for (i32 i = (min | 1); i < std::numeric_limits<i32>::max(); i += 2)
    {
        if (IsPrime(i) && ((i - 1) % HashPrime != 0))
            return i;
    }

    return min;
}

i32 HashHelpers::ExpandPrime(const i32 old_size)
{
    const auto new_size = 2 * old_size;

    if (static_cast<u32>(new_size) > MaxPrimeArrayLength && MaxPrimeArrayLength > old_size)
    {
        return MaxPrimeArrayLength;
    }

    return GetPrime(new_size);
}

u64 HashHelpers::GetFastModMultiplier(const u32 divisor)
{
    return std::numeric_limits<u64>::max() / divisor + 1;
}

u32 HashHelpers::FastMod(const usize value, const u32 divisor, const u64 multiplier)
{
    const auto highbits = static_cast<u32>(((((multiplier * value) >> 32) + 1) * divisor) >> 32);
    return highbits;
}
