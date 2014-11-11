
#ifndef _INCLUDE_FUNCTION_STRUCTURE_
#define _INCLUDE_FUNCTION_STRUCTURE_

#include <typeHandler.h>
#include <function.h>

#undef min
#undef max

#include <am-string.h>
#include <am-vector.h>

struct FunctionStructure
{
	FunctionStructure()
	{
		virtualFunctionsCreated.init();
	}

	void* address;
	TypeHandler** argumentsHandlers;
	unsigned int argumentsCount;
	TypeHandler* returnHandler;
	bool isMethod;
	ke::AString library;
	ke::AString name;
	unsigned int virtualTableIndex;
	typedef ke::HashMap< void*, Function*, ke::PointerPolicy<void> > VFuncTableMap;
	VFuncTableMap virtualFunctionsCreated;
};

#endif

