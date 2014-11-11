
#ifndef _INCLUDE_FUNCTION_MANAGER_
#define _INCLUDE_FUNCTION_MANAGER_

#include <sm_stringhashmap.h>
#include <am-vector.h>

#include <ctime>
#include <function.h>

typedef struct
{
	Function* function;
	OrpheuHookPhase phase;
	long hookFunctionPhaseID;
} HookReferenceData;

class FunctionManager
{
private:

	typedef ke::HashMap< long, HookReferenceData*, ke::IntegerPolicy<long> > HookRefsTableMap;
	HookRefsTableMap hookReferences;
	StringHashMap<time_t>* functionNameToTimestamp;
	StringHashMap<unsigned short int>* functionNameToFunctionID;
	long currentHookID;
	ke::Vector<Function*>* functions;

public:

	FunctionManager();
	time_t getTimestamp(const char* functionName);
	unsigned short int  addFunction(const char* functionName, Function* function, time_t timestamp);
	Function* getFunction(unsigned short int functionID);
	unsigned short int getFunctionID(const char* functionName);
	long addHook(AMX* amx, const char* functionName, Function* function, OrpheuHookPhase phase);
	bool removeHook(long hookID);
	void removeAllHooks();
	void tryToRemove(const char* functionName);
};

#endif

