
#ifndef _INCLUDE_MEMORY_STRUCT_MANAGER_
#define _INCLUDE_MEMORY_STRUCT_MANAGER_

#include <sm_stringhashmap.h>
#include <am-vector.h>

#include <ctime>
#include <memoryStructure.h>

class MemoryStructManager
{
private:

	StringHashMap<unsigned int> memoryStructureNameToMemoryStructureID;
	ke::Vector<MemoryStructure*> memoryStructures;

public:
	//FunctionStructure* get(char* memoryStructureName);
	void add(MemoryStructure* memoryStructure);
	MemoryStructure* get(char* name);
};

#endif

