#pragma once

#ifndef COPLT_FINAL
#ifdef FFI_SRC
#define COPLT_FINAL final
#else
#define COPLT_FINAL
#endif
#endif

#ifndef COPLT_REQUIRES
#ifdef FFI_SRC
#define COPLT_REQUIRES requires
#else
#define COPLT_REQUIRES
#endif
#endif

#ifndef COPLT_POD
#ifdef FFI_SRC
#define COPLT_POD(T) std::is_trivially_copyable_v<T>
#else
#define COPLT_POD(T)
#endif
#endif

#ifndef COPLT_CDECL
#ifdef _MSC_VER
#define COPLT_CDECL __cdecl
#else
#define COPLT_CDECL __attribute__((cdecl))
#endif
#endif

#ifndef COPLT_FORCE_INLINE
#ifdef _MSC_VER
#define COPLT_FORCE_INLINE __forceinline
#else
#define COPLT_FORCE_INLINE inline __attribute__((__always_inline__))
#endif
#endif

#ifndef COPLT_NO_INLINE
#ifdef _MSC_VER
#define COPLT_NO_INLINE __declspec(noinline)
#else
#define COPLT_NO_INLINE __attribute__((__noinline__))
#endif
#endif

#ifndef COPLT_EXPORT
#ifdef _MSC_VER
#define COPLT_EXPORT __declspec(dllexport)
#else
#define COPLT_EXPORT
#endif
#endif

#ifndef COPLT_IMPORT
#ifdef _MSC_VER
#define COPLT_IMPORT __declspec(dllimport)
#else
#define COPLT_IMPORT
#endif
#endif

#ifndef COPLT_API
#ifdef COPLT_DLL_IMPORT
#define COPLT_API COPLT_IMPORT
#else
#define COPLT_API COPLT_EXPORT
#endif
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
#define COPLT_X64
#elif defined(__aarch64__) || defined(_M_ARM64)
#define COPLT_ARM64
#endif

#define COPLT_U32_MAX 4294967295
#define COPLT_U64_MAX 18446744073709551615

#ifdef FFI_SRC
#define COPLT_ENUM_FLAGS(Name, Type) enum class Name : Type;\
inline constexpr bool operator==(const Name a, const Type b)\
{\
    return static_cast<Type>(a) == b;\
}\
inline constexpr bool operator!=(const Name a, const Type b)\
{\
    return static_cast<Type>(a) != b;\
}\
inline constexpr bool operator&&(const Name a, const Name b)\
{\
return (a != 0) && (b != 0);\
}\
inline constexpr bool operator||(const Name a, const Name b)\
{\
return (a != 0) || (b != 0);\
}\
inline constexpr Name operator~(const Name value)\
{\
    return static_cast<Name>(~static_cast<Type>(value));\
}\
inline constexpr Name operator&(const Name a, const Name b)\
{\
    return static_cast<Name>(static_cast<Type>(a) & static_cast<Type>(b));\
}\
inline constexpr Name operator|(const Name a, const Name b)\
{\
    return static_cast<Name>(static_cast<Type>(a) | static_cast<Type>(b));\
}\
inline constexpr Name operator^(const Name a, const Name b)\
{\
    return static_cast<Name>(static_cast<Type>(a) ^ static_cast<Type>(b));\
}\
inline constexpr Name operator&=(Name& a, const Name b)\
{\
    return a = a & b;\
}\
inline constexpr Name operator|=(Name& a, const Name b)\
{\
    return a = a | b;\
}\
inline constexpr Name operator^=(Name& a, const Name b)\
{\
    return a = a ^ b;\
}\
inline constexpr bool HasFlags(Name a, Name b)\
{\
    return (a & b) == b;\
}\
inline constexpr bool HasAnyFlags(Name a, Name b)\
{\
    return (a & b) != 0;\
}\
inline constexpr bool HasFlagsOnly(Name a, Name b)\
{\
return (a & ~(b)) == 0;\
}\
enum class Name : Type
#else
#define COPLT_ENUM_FLAGS(Name, Type) enum class Name : Type
#endif

#include "./Types.h"
