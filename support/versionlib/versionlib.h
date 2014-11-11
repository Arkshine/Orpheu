#ifndef _INCLUDE_ORPHEU_VERSIONLIB_H_
#define _INCLUDE_ORPHEU_VERSIONLIB_H_

#if !defined(ORPHEU_USE_VERSIONLIB)
// These get defined in orpheu_version.h since
// versionlib does not use versionlib.
    #undef ORPHEU_LOCAL_REV
    #undef ORPHEU_CSET
    #undef ORPHEU_VERSION
    #undef ORPHEU_BUILD_TIME
#endif

#ifdef __cplusplus
    #define EXTERN_C extern "C"
#else
    #define EXTERN_C extern
#endif

EXTERN_C const char *ORPHEU_LOCAL_REV;
EXTERN_C const char *ORPHEU_SHA;
EXTERN_C const char *ORPHEU_VERSION;
EXTERN_C const char *ORPHEU_BUILD_TIME;

#endif // _INCLUDE_ORPHEU_VERSIONLIB_H_
