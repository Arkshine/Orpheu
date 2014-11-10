
#ifndef _INCLUDE_FUNCTION_STRUCTURE_
#define _INCLUDE_FUNCTION_STRUCTURE_

#include <typeHandler.h>
#include <function.h>

#undef min
#undef max 

#include <string>
#include <am-vector.h>

using namespace std;

struct FunctionStructure
{
	void* address;
	TypeHandler** argumentsHandlers;
	unsigned int argumentsCount;
	TypeHandler* returnHandler;
	bool isMethod;
	string library;
	string name;
	unsigned int virtualTableIndex;
	map<void*,Function*> virtualFunctionsCreated;
};

#endif

