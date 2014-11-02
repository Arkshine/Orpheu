
#include <functionStructuresManager.h>

#include <global.h>

time_t FunctionStructuresManager::getTimestamp(char* functionName)
{
	time_t* timestampPointer = functionStructureNameToTimestamp.retrieve(functionName);
	time_t timestamp = timestampPointer ? *timestampPointer : 0;
	return timestamp;
}

void FunctionStructuresManager::addFunctionStructure(FunctionStructure* functionStructure, time_t timestamp)
{
	char* functionName = (char*)functionStructure->name.chars();

	unsigned int* idPointer = functionStructureNameToFunctionStructureID.retrieve(functionName);

	unsigned int id;

	if (idPointer)
	{
		id = *idPointer;
		//delete functionsStructures->at(id);
		functionStructures.at(id) = functionStructure;

		Global::FunctionManagerObj->tryToRemove(functionName);
	}
	else
	{
		id = functionStructures.size();
		functionStructures.push_back(functionStructure);
	}

	functionStructureNameToFunctionStructureID.replace(functionName, id);
	functionStructureNameToTimestamp.replace(functionName, timestamp);
}

unsigned short int FunctionStructuresManager::makeFunction(FunctionStructure* functionStructure, void* address)
{
	Function* function = new Function(address, functionStructure->argumentsHandlers, functionStructure->argumentsCount, functionStructure->returnHandler, functionStructure->library, functionStructure->isMethod);

	return Global::FunctionManagerObj->addFunction((char*)functionStructure->name.chars(), function, 0);
}

FunctionStructure* FunctionStructuresManager::getFunctionStructure(char* functionName)
{
	unsigned int* idPointer = functionStructureNameToFunctionStructureID.retrieve(functionName);

	if (idPointer)
	{
		unsigned int id = *idPointer;
		return functionStructures.at(id);
	}

	return NULL;
}