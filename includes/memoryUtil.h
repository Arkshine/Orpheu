
#ifndef _INCLUDE_MEMORY_
#define _INCLUDE_MEMORY_

#include <amxxmodule.h>

#ifdef __linux__
// Code from jim yang
#include <sys/mman.h>

const int PAGE_EXECUTE_READWRITE = PROT_READ | PROT_WRITE | PROT_EXEC;
const int PAGE_READWRITE = PROT_READ | PROT_WRITE;
const int PageSize = 4096;

inline void* Align(void* address)
{
	return (void*)((long)address & ~(PageSize - 1));
}
#endif

namespace Memory
{
	int ChangeMemoryProtection(void* function, unsigned int size, unsigned long newProtection);
	int ChangeMemoryProtection(void* address, unsigned int size, unsigned long newProtection, unsigned long & oldProtection);
}

#endif
