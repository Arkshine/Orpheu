
#include <functionManager.h>

FunctionManager::FunctionManager()
{
	functionNameToTimestamp = new KTrie<time_t>;
	functionNameToFunctionID = new KTrie<unsigned short int>;
	functions = new CVector<Function*>;
	functions->push_back(NULL);
	currentHookID = 1;
}

time_t FunctionManager::getTimestamp(const char* functionName)
{
	time_t* timestampPointer = functionNameToTimestamp->retrieve(functionName);
	time_t timestamp = timestampPointer ? *timestampPointer : 0;
	return timestamp;
}

unsigned short int FunctionManager::addFunction(const char* functionName,Function* function,time_t timestamp)
{
	unsigned short int* idPointer;

	unsigned short int id;

	if(functionName[0] && (idPointer = functionNameToFunctionID->retrieve(functionName)))
	{
		id = *idPointer;
		//delete functions->at(id);
		functions->at(id) = function;
	}
	else
	{
		id = functions->size();
		functions->push_back(function);
	}

	function->setID(id);
	
	functionNameToFunctionID->replace(functionName,id);
	functionNameToTimestamp->replace(functionName,timestamp);

	return id;
}

Function* FunctionManager::getFunction(unsigned short int functionID)
{
	if(functionID >= 1 && functionID < (unsigned short int)functions->size())
	{
		return functions->at(functionID);
	}

	return NULL;
}

unsigned short int FunctionManager::getFunctionID(const char* functionName)
{
	unsigned short int* idPointer = functionNameToFunctionID->retrieve(functionName);

	if(idPointer)
	{
		return *idPointer;
	}

	return 0;
}

long FunctionManager::addHook(AMX* amx,const char* functionName,Function* function,OrpheuHookPhase phase)
{
	HookReferenceData* hookReferenceData = new HookReferenceData;

	hookReferenceData->function = function;
	hookReferenceData->phase = phase;
	hookReferenceData->hookFunctionPhaseID = function->addHook(amx,functionName,hookReferenceData->phase);

	this->hookReferences[currentHookID] = hookReferenceData;

	return currentHookID++;
}

bool FunctionManager::removeHook(long hookID)
{
	map<long,HookReferenceData*>::iterator iterator = this->hookReferences.find(hookID);

	if(iterator != this->hookReferences.end())
	{
		pair<long,HookReferenceData*> hookPair = *iterator;

		HookReferenceData* hookReferenceData = hookPair.second;

		hookReferenceData->function->removeHook(hookReferenceData->phase,hookReferenceData->hookFunctionPhaseID);
		
		delete hookReferenceData;
	
		this->hookReferences.erase(iterator);

		return true;
	}

	return false;
}

void FunctionManager::removeAllHooks()
{
	for(map<long,HookReferenceData*>::iterator iterator = this->hookReferences.begin(); iterator != this->hookReferences.end(); iterator++)
	{
		pair<long,HookReferenceData*> hookPair = *iterator;

		HookReferenceData* hookReferenceData = hookPair.second;

		hookReferenceData->function->removeHook(hookReferenceData->phase,hookReferenceData->hookFunctionPhaseID);

		delete hookReferenceData;
	}

	hookReferences.clear();

	currentHookID = 1;
}

void FunctionManager::tryToRemove(const char* functionName)
{
	unsigned short int* idPointer = functionNameToFunctionID->retrieve(functionName);

	if(idPointer)
	{
		functionNameToFunctionID->remove(functionName);
		functionNameToTimestamp->remove(functionName);
		this->functions->erase(this->functions->iterAt(*idPointer));
	}
}
