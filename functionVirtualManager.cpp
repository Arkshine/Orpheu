
#include <functionVirtualManager.h>

#include <global.h>

time_t FunctionVirtualManager::getTimestamp(char* functionName)
{
	time_t* timestampPointer = functionVirtualNameToTimestamp.retrieve(functionName);
	time_t timestamp = timestampPointer ? *timestampPointer : 0;
	return timestamp;
}

void FunctionVirtualManager::add(FunctionStructure* functionStructure, time_t timestamp)
{
	char* functionName = (char*)functionStructure->name.chars();

	unsigned int* idPointer = functionVirtualNameToFunctionStructureID.retrieve(functionName);

	unsigned int id;

	if (idPointer)
	{
		id = *idPointer;
		//delete functionsStructures->at(id);
		functionStructures.at(id) = functionStructure;
	}
	else
	{
		id = functionStructures.size();
		functionStructures.push_back(functionStructure);
	}

	functionVirtualNameToFunctionStructureID.replace(functionName, id);
	functionVirtualNameToTimestamp.replace(functionName, timestamp);
}

unsigned short int FunctionVirtualManager::makeFunction(FunctionStructure* functionStructure, long object)
{
	TypeHandler* class_ = functionStructure->argumentsHandlers[0];

	if (class_->hasVirtualTableOffset())
	{
		long offset = class_->getVirtualTableOffset();

		void **vtable = *((void***)(((char*)object) + offset));
		int **ivtable = (int**)vtable;

		void* address = (void*)ivtable[functionStructure->virtualTableIndex];

		map<void*, Function*>::iterator iterator = functionStructure->virtualFunctionsCreated.find(address);

		Function* function;

		if (iterator == functionStructure->virtualFunctionsCreated.end())
		{
			function = new Function(address, functionStructure->argumentsHandlers, functionStructure->argumentsCount, functionStructure->returnHandler, functionStructure->library, functionStructure->isMethod);
			Global::FunctionManagerObj->addFunction("", function, 0);

			functionStructure->virtualFunctionsCreated[address] = function;
		}
		else
		{
			function = (*iterator).second;
		}

		return function->getID();
	}

	return 0;
}

FunctionStructure* FunctionVirtualManager::get(char* functionName)
{
	unsigned int* idPointer = functionVirtualNameToFunctionStructureID.retrieve(functionName);

	if (idPointer)
	{
		unsigned int id = *idPointer;
		return functionStructures.at(id);
	}

	return NULL;
}