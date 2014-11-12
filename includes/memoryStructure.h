
#ifndef _INCLUDE_MEMORY_STRUCTURE_
#define _INCLUDE_MEMORY_STRUCTURE_

#include <typeHandler.h>
#include <signatureEntry.h>

#undef min
#undef max

#include <am-string.h>
#include <am-vector.h>

enum MemoryStructureType
{
	MemTypeOffset,
	MemTypeSignature,
	MemTypeSearch
};

struct MemoryStructure
{
	ke::AString library;
	ke::AString name;
	MemoryStructureType type;
	long offset;
	byte* signature;
	SignatureEntryType* signatureEntryData;
	unsigned int signatureLength;
	TypeHandler* typeHandler;
	int memoryProtection;
	int displacement;
	void* basicAddress;
};

#endif

