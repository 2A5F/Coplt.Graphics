#pragma once

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

#define COPLT_U32_MAX 4294967295

#ifdef FFI_SRC
#define COPLT_ENUM_FLAGS(Name, Type) enum class Name : Type;\
inline bool operator==(const Name a, const Type b)\
{\
    return static_cast<Type>(a) == b;\
}\
inline bool operator!=(const Name a, const Type b)\
{\
    return static_cast<Type>(a) != b;\
}\
inline Name operator~(const Name value)\
{\
    return static_cast<Name>(~static_cast<Type>(value));\
}\
inline Name operator&(const Name a, const Name b)\
{\
    return static_cast<Name>(static_cast<Type>(a) & static_cast<Type>(b));\
}\
inline Name operator|(const Name a, const Name b)\
{\
    return static_cast<Name>(static_cast<Type>(a) | static_cast<Type>(b));\
}\
inline Name operator^(const Name a, const Name b)\
{\
    return static_cast<Name>(static_cast<Type>(a) ^ static_cast<Type>(b));\
}\
inline Name operator&=(Name& a, const Name b)\
{\
    return a = a & b;\
}\
inline Name operator|=(Name& a, const Name b)\
{\
    return a = a | b;\
}\
inline Name operator^=(Name& a, const Name b)\
{\
    return a = a ^ b;\
}\
inline bool HasFlags(Name a, Name b)\
{\
    return (a & b) == b;\
}\
inline bool HasAnyFlags(Name a, Name b)\
{\
    return (a & b) != 0;\
}\
enum class Name : Type
#else
#define COPLT_ENUM_FLAGS(Name, Type) enum class Name : Type
#endif

#include "./Types.h"
