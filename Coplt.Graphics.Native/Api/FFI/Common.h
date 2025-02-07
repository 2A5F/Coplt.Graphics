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

#include "./Types.h"
