
#include <functionManager.h>

FunctionManager::FunctionManager()
{
	hookReferences.init();
	functions = new ke::Vector < Function* > ;
	functions->append(NULL);
	currentHookID = 1;
}

time_t FunctionManager::getTimestamp(const char* functionName)
{
	time_t timestampPointer;
	functionNameToTimestamp.retrieve(functionName, &timestampPointer);
	time_t timestamp = timestampPointer ? timestampPointer : 0;
	return timestamp;
}

unsigned short int FunctionManager::addFunction(const char* functionName, Function* function, time_t timestamp)
{
	unsigned short int id;

	if (functionName[0] && functionNameToFunctionID.retrieve(functionName, &id))
	{
		//delete functions->at(id);
		functions->at(id) = function;
	}
	else
	{
		id = functions->length();
		functions->append(function);
	}

	function->setID(id);

	functionNameToFunctionID.replace(functionName, id);
	functionNameToTimestamp.replace(functionName, timestamp);

	return id;
}

Function* FunctionManager::getFunction(unsigned short int functionID)
{
	if (functionID >= 1 && functionID < (unsigned short int)functions->length())
	{
		return functions->at(functionID);
	}

	return NULL;
}

unsigned short int FunctionManager::getFunctionID(const char* functionName)
{
	unsigned short int id;
	
	if (functionNameToFunctionID.retrieve(functionName, &id))
	{
		return id;
	}

	return 0;
}

long FunctionManager::addHook(AMX* amx, const char* functionName, Function* function, OrpheuHookPhase phase)
{
	HookReferenceData* hookReferenceData = new HookReferenceData;

	hookReferenceData->function = function;
	hookReferenceData->phase = phase;
	hookReferenceData->hookFunctionPhaseID = function->addHook(amx, functionName, hookReferenceData->phase);

	HookTableMap::Insert i = this->hookReferences.findForAdd(currentHookID);
	if (!i.found())
	{
		if (this->hookReferences.add(i))
		{
			i->key = currentHookID;
		}
	}
	i->value = hookReferenceData;

	return currentHookID++;
}

bool FunctionManager::removeHook(long hookID)
{
	HookTableMap::Result r = this->hookReferences.find(hookID);

	if (r.found())
	{
		HookReferenceData* hookReferenceData = r->value;

		hookReferenceData->function->removeHook(hookReferenceData->phase, hookReferenceData->hookFunctionPhaseID);

		delete hookReferenceData;

		this->hookReferences.remove(r);

		return true;
	}

	return false;
}

void FunctionManager::removeAllHooks()
{
	for (HookTableMap::iterator iter = this->hookReferences.iter(); !iter.empty(); iter.next())
	{
		HookReferenceData* hookReferenceData = iter->value;

		hookReferenceData->function->removeHook(hookReferenceData->phase, hookReferenceData->hookFunctionPhaseID);

		delete hookReferenceData;
	}

	hookReferences.clear();

	currentHookID = 1;
}

void FunctionManager::tryToRemove(const char* functionName)
{
	unsigned short int id;
	
	if (functionNameToFunctionID.retrieve(functionName, &id))
	{
		functionNameToFunctionID.remove(functionName);
		functionNameToTimestamp.remove(functionName);
		this->functions->remove(id);
	}
}