
#ifndef _INCLUDE_FUNCTION_STRUCTURE_
#define _INCLUDE_FUNCTION_STRUCTURE_

#include <typeHandler.h>
#include <function.h>

#undef min
#undef max

#include <am-string.h>
#include <CVector.h>

using namespace std;

struct FunctionStructure
{
	void* address;
	TypeHandler** argumentsHandlers;
	unsigned int argumentsCount;
	TypeHandler* returnHandler;
	bool isMethod;
	ke::AString library;
	ke::AString name;
	unsigned int virtualTableIndex;
	map<void*, Function*> virtualFunctionsCreated;
};

#endif

