
#include <functionStructuresManager.h>

#include <global.h>

time_t FunctionStructuresManager::getTimestamp(const char* functionName)
{
	time_t timestamp = 0;
	functionStructureNameToTimestamp.retrieve(functionName, &timestamp);

	return timestamp ? timestamp : 0;
}

void FunctionStructuresManager::addFunctionStructure(FunctionStructure* functionStructure, time_t timestamp)
{
	const char* functionName = functionStructure->name.chars();

	unsigned int id;

	if (functionStructureNameToFunctionStructureID.retrieve(functionName, &id))
	{
		//delete functionsStructures->at(id);
		functionStructures.at(id) = functionStructure;

		Global::FunctionManagerObj->tryToRemove(functionName);
	}
	else
	{
		id = functionStructures.length();
		functionStructures.append(functionStructure);
	}

	functionStructureNameToFunctionStructureID.replace(functionName, id);
	functionStructureNameToTimestamp.replace(functionName, timestamp);
}

unsigned short int FunctionStructuresManager::makeFunction(FunctionStructure* functionStructure, void* address)
{
	Function* function = new Function(address, functionStructure->argumentsHandlers, functionStructure->argumentsCount, functionStructure->returnHandler, functionStructure->library, functionStructure->isMethod);

	return Global::FunctionManagerObj->addFunction(functionStructure->name.chars(), function, 0);
}

FunctionStructure* FunctionStructuresManager::getFunctionStructure(const char* functionName)
{
	unsigned int id;

	if (functionStructureNameToFunctionStructureID.retrieve(functionName, &id))
	{
		return functionStructures.at(id);
	}

	return NULL;
}