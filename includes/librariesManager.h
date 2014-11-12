
#ifndef _INCLUDE_LIBRARIES_MANAGER_
#define _INCLUDE_LIBRARIES_MANAGER_

#include <signatureEntry.h>

struct LibraryInfo
{
	void* handle;
	void* baseAddress;
	long length;
};

namespace LibrariesManager
{
	bool addLibrary(const char *libraryName, void *addressContained);
	bool hasLibrary(const char *libraryName);
	void* findFunction(const char* libraryName, const char* functionName);
	void* findFunction(const char* libraryName, unsigned char* signature, SignatureEntryType* signatureData, unsigned int);
	bool libraryContainsAddress(const char* libraryName, long address);
	long getAddressOffset(long address, const char* libraryName);
	long getAddressWithOffset(long offset, const char* libraryName);
	long getLibraryAddress(const char* libraryName);
	LibraryInfo* getLibrary(const char* libraryName);
	void* findMemory(const char* libraryName, unsigned char* signature, SignatureEntryType* signatureData, unsigned int length, long start);
}

#endif

