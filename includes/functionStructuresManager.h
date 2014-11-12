
#ifndef _INCLUDE_FUNCTION_STRUCTURES_MANAGER_
#define _INCLUDE_FUNCTION_STRUCTURES_MANAGER_

#include <sm_stringhashmap.h>
#include <am-vector.h>

#include <ctime>
#include <functionStructure.h>
#include <function.h>

class FunctionStructuresManager
{
private:

	StringHashMap<time_t> functionStructureNameToTimestamp;
	StringHashMap<unsigned int> functionStructureNameToFunctionStructureID;
	ke::Vector<FunctionStructure*> functionStructures;

public:

	time_t getTimestamp(const char* functionStructureName);

	FunctionStructure* getFunctionStructure(const char* functionStructureName);
	void addFunctionStructure(FunctionStructure* functionStructure, time_t timestamp);
	unsigned short int makeFunction(FunctionStructure* functionStructure, void* address);
};

#endif

