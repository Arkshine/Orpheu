
#if defined(ORPHEU_EXPORTS)
	#undef ORPHEU_USE_VERSIONLIB
#endif

#include <orpheu_version.h>
#include <versionlib.h>

const char *ORPHEU_LOCAL_REV = ORPHEU_BUILD_LOCAL_REV;
const char *ORPHEU_SHA = ORPHEU_BUILD_CSET;
const char *ORPHEU_VERSION = ORPHEU_VERSION_STRING;
const char *ORPHEU_BUILD_TIME = __DATE__ " " __TIME__;
