#pragma once

#include <bit>
#include <cstdint>
#include <stdexcept>

#ifdef FFI_SRC
#include <type_traits>
#include <functional>
#include <glm/glm.hpp>
#include <shared_mutex>

#ifdef COPLT_X64
#include <emmintrin.h>
#endif

#endif

namespace Coplt
{
    using b8 = int8_t;
    using b32 = int32_t;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;
    using usize = size_t;

    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    #ifdef FFI_SRC
    using f32 = glm::f32;
    using f64 = glm::f64;

    using u128 = __uint128_t;
    using i128 = __int128_t;

    using float2 = glm::f32vec2;
    using float3 = glm::f32vec3;
    using float4 = glm::f32vec4;

    using int2 = glm::ivec2;
    using int3 = glm::ivec3;
    using int4 = glm::ivec4;

    using uint2 = glm::uvec2;
    using uint3 = glm::uvec3;
    using uint4 = glm::uvec4;

    using float3x3 = glm::f32mat3x3;
    using float3x4 = glm::f32mat3x4;
    using float4x3 = glm::f32mat4x3;
    using float4x4 = glm::f32mat4x4;

    using quaternion = glm::f32quat;

    using Char8 = char8_t;
    using Char16 = char16_t;

    using RwLock = std::shared_mutex;
    using WriteGuard = std::unique_lock<RwLock>;
    using ReadGuard = std::shared_lock<RwLock>;

    template <class T>
    using Ref = std::reference_wrapper<T>;

    #else

    using f32 = float;
    using f64 = double;

    struct alignas(16) u128 { u64 a; u64 b; };
    struct alignas(16) i128 { i64 a; i64 b; };

    struct alignas(8)  float2 { f32 x; f32 y; };
    struct alignas(16) float3 { f32 x; f32 y; f32 z; };
    struct alignas(16) float4 { f32 x; f32 y; f32 z; f32 w; };

    struct alignas(8)  int2 { i32 x; i32 y; };
    struct alignas(16) int3 { i32 x; i32 y; i32 z; };
    struct alignas(16) int4 { i32 x; i32 y; i32 z; i32 w; };

    struct alignas(8)  uint2 { u32 x; u32 y; };
    struct alignas(16) uint3 { u32 x; u32 y; u32 z; };
    struct alignas(16) uint4 { u32 x; u32 y; u32 z; u32 w; };

    struct alignas(16) float3x3 { float3 c0; float3 c1; float3 c2; };
    struct alignas(16) float3x4 { float3 c0; float3 c1; float3 c2; float3 c3; };
    struct alignas(16) float4x3 { float4 c0; float4 c1; float4 c2; };
    struct alignas(16) float4x4 { float4 c0; float4 c1; float4 c2; float4 c3; };

    struct alignas(16) quaternion { float4 value; };

    using Char8 = u8;
    using Char16 = u16;

    #endif

    struct Guid
    {
        u32 _a{};
        u16 _b{};
        u16 _c{};
        u8 _d{};
        u8 _e{};
        u8 _f{};
        u8 _g{};
        u8 _h{};
        u8 _i{};
        u8 _j{};
        u8 _k{};

        #ifdef FFI_SRC
        constexpr explicit Guid() = default;

        constexpr explicit Guid(
            const u32 _a,
            const u16 _b,
            const u16 _c,
            const u8 _d,
            const u8 _e,
            const u8 _f,
            const u8 _g,
            const u8 _h,
            const u8 _i,
            const u8 _j,
            const u8 _k
        ) : _a(_a), _b(_b), _c(_c), _d(_d), _e(_e), _f(_f), _g(_g), _h(_h), _i(_i), _j(_j), _k(_k)
        {
        }

        constexpr explicit Guid(const u8 bytes[16]) : Guid(
            bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24,
            bytes[4] | bytes[5] << 8,
            bytes[6] | bytes[7] << 8,
            bytes[8],
            bytes[9],
            bytes[10],
            bytes[11],
            bytes[12],
            bytes[13],
            bytes[14],
            bytes[15]
        )
        {
        }

        constexpr explicit Guid(u32 a, u16 b, u16 c, const u8 bytes[8]) : Guid(
            a, b, c,
            bytes[0],
            bytes[1],
            bytes[2],
            bytes[3],
            bytes[4],
            bytes[5],
            bytes[6],
            bytes[7]
        )
        {
        }

    private:
        constexpr static u8 CharToHexLookup[] = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 15
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 31
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 47
            0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 63
            0xFF, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 79
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 95
            0xFF, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 111
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 127
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 143
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 159
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 175
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 191
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 207
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 223
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // 239
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF // 255
        };

        constexpr static u8 DecodeByte(const char c1, const char c2, i32& invalid_if_negative)
        {
            const auto upper = CharToHexLookup[c1];
            const auto lower = CharToHexLookup[c2];
            auto result = (upper << 4) | lower;

            result = (c1 | c2) >> 8 == 0 ? result : -1;

            invalid_if_negative |= result;
            return result;
        }

    public:
        constexpr static Guid FromStr(const char* str, const usize len)
        {
            if (len < 36) throw std::invalid_argument("Invalid GUID");

            if (str[8] != '-' || str[13] != '-' || str[18] != '-' || str[23] != '-')
                throw std::invalid_argument("Invalid GUID");

            i32 invalid_if_negative = 0;
            u8 bytes[16]{};
            bytes[0] = DecodeByte(str[6], str[7], invalid_if_negative);
            bytes[1] = DecodeByte(str[4], str[5], invalid_if_negative);
            bytes[2] = DecodeByte(str[2], str[3], invalid_if_negative);
            bytes[3] = DecodeByte(str[0], str[1], invalid_if_negative);
            bytes[4] = DecodeByte(str[11], str[12], invalid_if_negative);
            bytes[5] = DecodeByte(str[9], str[10], invalid_if_negative);
            bytes[6] = DecodeByte(str[16], str[17], invalid_if_negative);
            bytes[7] = DecodeByte(str[14], str[15], invalid_if_negative);
            bytes[8] = DecodeByte(str[19], str[20], invalid_if_negative);
            bytes[9] = DecodeByte(str[21], str[22], invalid_if_negative);
            bytes[10] = DecodeByte(str[24], str[25], invalid_if_negative);
            bytes[11] = DecodeByte(str[26], str[27], invalid_if_negative);
            bytes[12] = DecodeByte(str[28], str[29], invalid_if_negative);
            bytes[13] = DecodeByte(str[30], str[31], invalid_if_negative);
            bytes[14] = DecodeByte(str[32], str[33], invalid_if_negative);
            bytes[15] = DecodeByte(str[34], str[35], invalid_if_negative);

            if (invalid_if_negative < 0) throw std::invalid_argument("Invalid GUID");

            auto r = Guid(bytes);

            if constexpr (std::endian::native == std::endian::little)
            {
                r.ReverseAbcEndianness();
            }

            return r;
        }

        constexpr explicit Guid(const char str[36]) : Guid()
        {
            *this = FromStr(str, 36);
        }

        COPLT_FORCE_INLINE constexpr bool operator==(const Guid& other) const noexcept
        {
            if consteval
            {
                return _a == other._a && _b == other._b && _c == other._c
                    && _d == other._d && _e == other._e && _f == other._f && _g == other._g
                    && _h == other._h && _i == other._i && _j == other._j && _k == other._k;
            }
            else
            {
                #ifdef COPLT_X64
                const __m128i a = _mm_loadu_si128(reinterpret_cast<const __m128i*>(this));
                const __m128i b = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&other));
                const __m128i r = _mm_cmpeq_epi8(a, b);
                const auto mask = _mm_movemask_epi8(r);
                return mask == 0xFFFF;
                #else
                const auto src = reinterpret_cast<const u8*>(this);
                const auto dst = reinterpret_cast<const u8*>(&other);
                for (auto i = 0; i < sizeof(Guid); ++i)
                {
                    if (src[i] != dst[i]) return false;
                }
                return true;
                #endif
            }
        }

        COPLT_FORCE_INLINE constexpr bool operator!=(const Guid& other) const noexcept
        {
            if consteval
            {
                return _a != other._a || _b != other._b || _c != other._c
                    || _d != other._d || _e != other._e || _f != other._f || _g != other._g
                    || _h != other._h || _i != other._i || _j != other._j || _k != other._k;
            }
            else
            {
                return !operator==(other);
            }
        }

    private:
        constexpr static u32 ReverseEndianness(u32 value)
        {
            // bswap in x86; rev in arm
            return std::rotr(value & 0x00FF00FFu, 8) // xx zz
                + std::rotl(value & 0xFF00FF00u, 8); // ww yy
        }

        constexpr static u16 ReverseEndianness(u16 value)
        {
            return (value >> 8) + (value << 8);
        }

    public:
        constexpr void ReverseAbcEndianness()
        {
            _a = ReverseEndianness(_a);
            _b = ReverseEndianness(_b);
            _c = ReverseEndianness(_c);
        }

        constexpr i32 Hash32() const
        {
            const auto r = static_cast<i32>(_a);
            return r ^ _a + 1 ^ _a + 2 ^ _a + 3;
        }
        #endif
    };
}
