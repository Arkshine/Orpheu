
#ifndef _INCLUDE_FUNCTION_STRUCTURES_MANAGER_
#define _INCLUDE_FUNCTION_STRUCTURES_MANAGER_

#include <sm_trie_tpl.h>
#include <CVector.h>

#include <ctime>
#include <functionStructure.h>
#include <function.h>

class FunctionStructuresManager
{
private:

	KTrie<time_t> functionStructureNameToTimestamp;
	KTrie<unsigned int> functionStructureNameToFunctionStructureID;
	CVector<FunctionStructure*> functionStructures;

public:

	time_t getTimestamp(char* functionStructureName);

	FunctionStructure* getFunctionStructure(char* functionStructureName);
	void addFunctionStructure(FunctionStructure* functionStructure, time_t timestamp);
	unsigned short int makeFunction(FunctionStructure* functionStructure, void* address);
};

#endif

