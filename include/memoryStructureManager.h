
#ifndef _INCLUDE_MEMORY_STRUCT_MANAGER_
#define _INCLUDE_MEMORY_STRUCT_MANAGER_

#include <sm_trie_tpl.h>
#include <CVector.h>

#include <ctime>
#include <memoryStructure.h>

class MemoryStructManager
{
private:

	KTrie<unsigned int> memoryStructureNameToMemoryStructureID;
	CVector<MemoryStructure*> memoryStructures;

public:
	//FunctionStructure* get(char* memoryStructureName);
	void add(MemoryStructure* memoryStructure);
	MemoryStructure* get(char* name);
};

#endif

