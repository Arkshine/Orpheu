
#ifndef _INCLUDE_MEMORY_STRUCTURE_
#define _INCLUDE_MEMORY_STRUCTURE_

#include <typeHandler.h>
#include <signatureEntry.h>

#undef min
#undef max 

#include <string>
#include <CVector.h>

using namespace std;

enum MemoryStructureType
{
	MemTypeOffset,
	MemTypeSignature,
	MemTypeSearch
};

struct MemoryStructure
{
	string library;
	string name;
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

