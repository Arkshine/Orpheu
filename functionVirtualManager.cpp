
#include <functionVirtualManager.h>

#include <global.h>

time_t FunctionVirtualManager::getTimestamp(const char* functionName)
{
	time_t timestamp;
	functionVirtualNameToTimestamp.retrieve(functionName, &timestamp);

	return timestamp ? timestamp : 0;;
}

void FunctionVirtualManager::add(FunctionStructure* functionStructure,time_t timestamp)
{
	char* functionName = (char*) functionStructure->name.chars();

	unsigned int id;

	if (functionVirtualNameToFunctionStructureID.retrieve(functionName, &id))
	{
		//delete functionsStructures->at(id);
		functionStructures.at(id) = functionStructure;
	}
	else
	{
		id = functionStructures.length();
		functionStructures.append(functionStructure);
	}

	functionVirtualNameToFunctionStructureID.replace(functionName,id);
	functionVirtualNameToTimestamp.replace(functionName,timestamp);
}

unsigned short int FunctionVirtualManager::makeFunction(FunctionStructure* functionStructure,long object)
{
	TypeHandler* class_ = functionStructure->argumentsHandlers[0];

	if(class_->hasVirtualTableOffset())
	{
		long offset = class_->getVirtualTableOffset();

		void **vtable = *((void***)(((char*)object)+offset));
		int **ivtable = (int**) vtable;

		void* address = (void*) ivtable[functionStructure->virtualTableIndex];

		map<void*,Function*>::iterator iterator = functionStructure->virtualFunctionsCreated.find(address);

		Function* function;

		if(iterator == functionStructure->virtualFunctionsCreated.end())
		{
			function = new Function(address,functionStructure->argumentsHandlers,functionStructure->argumentsCount,functionStructure->returnHandler,functionStructure->library,functionStructure->isMethod);
			Global::FunctionManagerObj->addFunction("",function,0);

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

FunctionStructure* FunctionVirtualManager::get(const char* functionName)
{
	unsigned int id;

	if (functionVirtualNameToFunctionStructureID.retrieve(functionName, &id))
	{
		return functionStructures.at(id);
	}

	return NULL;
}
