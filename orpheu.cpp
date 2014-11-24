

#include <orpheu.h>
#include <global.h>
#include <librariesManager.h>
#include <hooker.h>
#include <function.h>
#include <am-string.h>
#include <eiface.h>
#include <typeHandlerImplementations/CBaseEntityHandler.h>

cell AMX_NATIVE_CALL Orpheu::GetFunction(AMX* amx, cell* params)
{
	int len;
	char *functionName = g_fn_GetAmxString(amx, params[1], 0, &len);
	char *classname = g_fn_GetAmxString(amx, params[2], 1, &len);

	char name[256];

	if (classname[0])
	{
		UTIL_Format(name, sizeof(name) - 1, "%s::%s", classname, functionName);
	}
	else
	{
		UTIL_Format(name, sizeof(name) - 1, "%s", functionName);
	}

	unsigned short int id = Global::FunctionManagerObj->getFunctionID(name);

	if (id)
	{
		return id;
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Function \"%s\" not found", name);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::TryGetFunction(AMX* amx, cell* params)
{
	int len;
	char *functionName = g_fn_GetAmxString(amx, params[1], 0, &len);
	char *classname = g_fn_GetAmxString(amx, params[2], 1, &len);

	char name[256];

	if (classname[0])
	{
		UTIL_Format(name, sizeof(name) - 1, "%s::%s", classname, functionName);
	}
	else
	{
		UTIL_Format(name, sizeof(name) - 1, "%s", functionName);
	}

	return Global::FunctionManagerObj->getFunctionID(name);
}

cell AMX_NATIVE_CALL Orpheu::Call(AMX* amx, cell* params)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	Function* function = Global::FunctionManagerObj->getFunction(params[1]);

	if (function)
	{
		unsigned int expectedParamsCount = 1 + function->getArgumentsCount() + function->getIfReturnByRefParamsCount();

		if (paramsCount == expectedParamsCount)
		{
			function->blockHooks();
			return function->call(amx, &params[2]);
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid function [%d]", params[1]);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::CallSuper(AMX* amx, cell* params)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	Function* function = Global::FunctionManagerObj->getFunction(params[1]);

	if (function)
	{
		unsigned int expectedParamsCount = 1 + function->getArgumentsCount() + function->getIfReturnByRefParamsCount();

		if (paramsCount == expectedParamsCount)
		{
			return function->call(amx, &params[2]);
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid function [%d]", params[1]);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::UnregisterHook(AMX* amx, cell* params)
{
	return Global::FunctionManagerObj->removeHook(params[1]);
}

cell AMX_NATIVE_CALL Orpheu::RegisterHook(AMX* amx, cell* params)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	if ((paramsCount == 2) || (paramsCount == 3))
	{
		Function* function = Global::FunctionManagerObj->getFunction(params[1]);

		if (function)
		{
			int len;
			char *functionName = g_fn_GetAmxString(amx, params[2], 0, &len);

			OrpheuHookPhase phase = (paramsCount == 3) ? (OrpheuHookPhase)params[3] : OrpheuHookPre;

			return Global::FunctionManagerObj->addHook(amx, functionName, function, phase);
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid function [%d]", params[1]);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d or %d]. Got [%d]", 2, 3, paramsCount);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetReturn(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int paramsCount = params[0] / sizeof(cell);

		unsigned int expectedParamsCount = function->getIfReturnByRefParamsCount();

		if (paramsCount == expectedParamsCount)
		{
			if (function->hasReturn())
			{
				if (function->getHookPhase() == OrpheuHookPost)
				{
					return function->getReturnToAmx(amx, &params[1]);
				}
				else
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Cannot retrieve the return of a function in hooks pre");
				}
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Function being hook doesn't have a return defined");
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot get the return value of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::SetReturn(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int paramsCount = params[0] / sizeof(cell);

		unsigned int expectedParamsCount = 1;

		if (paramsCount == expectedParamsCount)
		{
			if (function->hasReturn())
			{
				function->setReturnFromAmx(amx, &params[1]);
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Function being hooked doesn't have a return defined");
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot set the return value of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetReturnStatus(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		return function->getHookReturnStatus();
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot retrieve the return status of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::SetParam(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int paramsCount = params[0] / sizeof(cell);

		unsigned int expectedParamsCount = 2;

		if (paramsCount == expectedParamsCount)
		{
			if (!function->setParamFromAmx(amx, &params[1]))
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter number provided", expectedParamsCount, paramsCount);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot set a parameter of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::SetParamStructMember(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int paramsCount = params[0] / sizeof(cell);

		unsigned int argumentID = params[1] - 1;

		int len;
		char* memberName = g_fn_GetAmxString(amx, params[2], 0, &len);

		TypeHandler* structAsTypeHandler = function->getArgumentHandler(argumentID);

		if (structAsTypeHandler)
		{
			if (structAsTypeHandler->isStructure())
			{
				long structureAddress = function->getArgumentValue(argumentID);

				StructHandler* structHandler = static_cast<StructHandler*>(structAsTypeHandler);

				int memberID = structHandler->getMemberID(memberName);

				if (memberID >= 0)
				{
					//					TypeHandler* member = structHandler->getMemberTypeHandler(memberID);

					unsigned int expectedParamsCount = 2 + 1;

					if (paramsCount == expectedParamsCount)
					{
						structHandler->convertMemberFromAmx(amx, &params[3], memberID, structureAddress);
					}
					else
					{
						MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
					}
				}
				else
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Structure \"%s\" doesn't have a member named \"%s\"", structHandler->getName(), memberName);
				}
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Parameter [%d] is not a structure", argumentID + 1);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter [%d]", argumentID + 1);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot set a parameter of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetParamStructMember(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int paramsCount = params[0] / sizeof(cell);

		unsigned int argumentID = params[1] - 1;

		int len;
		char* memberName = g_fn_GetAmxString(amx, params[2], 0, &len);

		TypeHandler* structAsTypeHandler = function->getArgumentHandler(argumentID);

		if (structAsTypeHandler)
		{
			if (structAsTypeHandler->isStructure())
			{
				long structureAddress = function->getArgumentValue(argumentID);

				StructHandler* structHandler = static_cast<StructHandler*>(structAsTypeHandler);

				int memberID = structHandler->getMemberID(memberName);

				if (memberID >= 0)
				{
					TypeHandler* member = structHandler->getMemberTypeHandler(memberID);

					unsigned int expectedParamsCount = 2 + member->getIfByRefParamsCount();

					if (paramsCount == expectedParamsCount)
					{
						return structHandler->convertMemberToAmx(amx, &params[3], memberID, structureAddress);
					}
					else
					{
						MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
					}
				}
				else
				{
					MF_LogError(amx, AMX_ERR_NATIVE, "Structure \"%s\" doesn't have a member named \"%s\"", structHandler->getName(), memberName);
				}
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Parameter [%d] is not a structure", argumentID + 1);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter [%d]", argumentID + 1);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot get a parameter of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::CreateStruct(AMX* amx, cell* params)
{
	unsigned int id = params[1];

	if (id < Global::StructuresCount)
	{
		StructHandler* structHandler = Global::Structures[id];

		return Global::StructManagerObj->createStructure(structHandler);
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid structure ID [%d]", id);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetStructFromAddress(AMX* amx, cell* params)
{
	unsigned int id = params[1];

	if (id < Global::StructuresCount)
	{
		long address = params[2];
		StructHandler* structHandler = Global::Structures[id];
		Global::StructManagerObj->add(address, structHandler);
		return address;
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid structure ID [%d]", id);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetStructMember(AMX* amx, cell* params)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	long structureAddress = params[1];

	int len;
	char* memberName = g_fn_GetAmxString(amx, params[2], 0, &len);

	StructHandler* structHandler = Global::StructManagerObj->getHandler(structureAddress);

	if (structHandler)
	{
		int memberID = structHandler->getMemberID(memberName);

		if (memberID >= 0)
		{
			TypeHandler* member = structHandler->getMemberTypeHandler(memberID);

			unsigned int expectedParamsCount = 2 + member->getIfByRefParamsCount();

			if (paramsCount == expectedParamsCount)
			{
				return structHandler->convertMemberToAmx(amx, &params[3], memberID, structureAddress);
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Structure \"%s\" doesn't have a member named \"%s\"", structHandler->getName(), memberName);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid structure provided");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::SetStructMember(AMX* amx, cell* params)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	long structureAddress = params[1];

	int len;
	char* memberName = g_fn_GetAmxString(amx, params[2], 0, &len);

	StructHandler* structHandler = Global::StructManagerObj->getHandler(structureAddress);

	if (structHandler)
	{
		int memberID = structHandler->getMemberID(memberName);

		if (memberID >= 0)
		{
			//			TypeHandler* member = structHandler->getMemberTypeHandler(memberID);

			unsigned int expectedParamsCount = 2 + 1;

			if (paramsCount == expectedParamsCount)
			{
				structHandler->convertMemberFromAmx(amx, &params[3], memberID, structureAddress);
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameters count. Expected [%d]. Got [%d]", expectedParamsCount, paramsCount);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Structure \"%s\" doesn't have a member named \"%s\"", structHandler->getName(), memberName);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid structure provided");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetStructFromParam(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int argumentID = params[1] - 1;

		TypeHandler* structAsTypeHandler = function->getArgumentHandler(argumentID);

		if (structAsTypeHandler)
		{
			if (structAsTypeHandler->isStructure())
			{
				long structureAddress = function->getArgumentValue(argumentID);

				StructHandler* structHandler = static_cast<StructHandler*>(structAsTypeHandler);

				Global::StructManagerObj->add(structureAddress, structHandler);

				return structureAddress;
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Parameter [%d] is not a structure", argumentID + 1);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter [%d]", argumentID + 1);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot use a parameter of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::CloneStructFromParam(AMX* amx, cell* params)
{
	Function* function = Global::FunctionBeingHooked;

	if (function)
	{
		unsigned int argumentID = params[1] - 1;

		TypeHandler* structAsTypeHandler = function->getArgumentHandler(argumentID);

		if (structAsTypeHandler)
		{
			if (structAsTypeHandler->isStructure())
			{
				long structureAddress = function->getArgumentValue(argumentID);

				StructHandler* structHandler = static_cast<StructHandler*>(structAsTypeHandler);

				return Global::StructManagerObj->clone(structureAddress, structHandler);
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Parameter [%d] is not a structure", argumentID + 1);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter [%d]", argumentID + 1);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Cannot use a parameter of a hook outside one");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::CreateFunction(AMX* amx, cell* params)
{
	long structureAddress = params[1];

	int len;
	char *functionName = g_fn_GetAmxString(amx, params[2], 0, &len);
	char *classname = g_fn_GetAmxString(amx, params[3], 1, &len);

	char name[256];

	if (classname[0])
	{
		UTIL_Format(name, sizeof(name) - 1, "%s::%s", classname, functionName);
	}
	else
	{
		UTIL_Format(name, sizeof(name) - 1, "%s", functionName);
	}

	unsigned short int functionID = Global::FunctionManagerObj->getFunctionID(name);

	if (functionID)
	{
		return functionID;
	}
	else
	{
		FunctionStructure* functionStructure = Global::FunctionStructuresManagerObj->getFunctionStructure(name);

		if (functionStructure)
		{
			if (LibrariesManager::libraryContainsAddress((char*)functionStructure->library.chars(), structureAddress))
			{
				return Global::FunctionStructuresManagerObj->makeFunction(functionStructure, (void*)structureAddress);
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Address isn't contained in the library defined in the file of the function");
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid function structure \"%s\"", name);
		}
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetEngineFunctionsStruct(AMX* amx, cell* params)
{
	Global::StructManagerObj->add((long)&g_engfuncs, Global::Structures[StructEnginefuncs]);
	return (cell)&g_engfuncs;
}

cell AMX_NATIVE_CALL Orpheu::GetDLLFunctionsStruct(AMX* amx, cell* params)
{
	Global::StructManagerObj->add((long)gpGamedllFuncs->dllapi_table, Global::Structures[StructDLL_FUNCTIONS]);
	return (cell)gpGamedllFuncs->dllapi_table;
}

cell AMX_NATIVE_CALL Orpheu::GetBytesAtAddress(AMX* amx, cell* params)
{
	unsigned char* address = (unsigned char*)params[1];
	cell *bytes = MF_GetAmxAddr(amx, params[2]);

	for (int i=0; i < params[3]; ++i)
	{
		bytes[i] = address[i];
	}

	return 0;
}
cell AMX_NATIVE_CALL Orpheu::GetFunctionAddress(AMX* amx, cell* params)
{
	Function* function = Global::FunctionManagerObj->getFunction(params[1]);

	if (function)
	{
		return function->getAddress();
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetFunctionOffset(AMX* amx, cell* params)
{
	Function* function = Global::FunctionManagerObj->getFunction(params[1]);

	if (function)
	{
		return LibrariesManager::getAddressOffset(function->getAddress(), function->getLibrary());
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetNextCallAtAddress(AMX* amx, cell* params)
{
	/*unsigned char* address = (unsigned char*) params[1];

	int call = params[2];
	int currentCall = 0;

	CDetourDis x(NULL,NULL);

	do
	{
	int len = x.InstructionLen(address);

	currentCall += (address[0] == 0xE8);

	if(currentCall == call)
	{
	long displacement = *((long *)&address[1]);
	return (long)&address[5] + displacement;
	}

	address += len;
	}
	while(true);*/

	return 0;
}

cell GetFunctionFrom(AMX* amx, char* functionName, char* classname, long object)
{
	char name[256];

	if (classname[0])
	{
		UTIL_Format(name, sizeof(name) - 1, "%s::%s", classname, functionName);
	}
	else
	{
		UTIL_Format(name, sizeof(name) - 1, "%s", functionName);
	}

	FunctionStructure* functionStructure = Global::FunctionVirtualManagerObj->get(name);

	if (functionStructure)
	{
		unsigned int functionID = Global::FunctionVirtualManagerObj->makeFunction(functionStructure, object);

		if (!functionID)
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Class doesn't have it's virtual table pointer set");
		}

		return functionID;
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid virtual function \"%s\"", name);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetFunctionFromClass(AMX* amx, cell* params)
{
	char* entityClassname = g_fn_GetAmxString(amx, params[1], 0, NULL);

	edict_t *entity = CREATE_ENTITY();
	CALL_GAME_ENTITY(PLID, entityClassname, &entity->v);

	if (entity->pvPrivateData != NULL)
	{
		char *functionName = g_fn_GetAmxString(amx, params[2], 1, NULL);
		char *classname = g_fn_GetAmxString(amx, params[3], 2, NULL);

		cell function = GetFunctionFrom(amx, functionName, classname, (long)entity->pvPrivateData);

		REMOVE_ENTITY(entity);

		return function;
	}
	else
	{
		REMOVE_ENTITY(entity);
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid class \"%s\"", entityClassname);
		return 0;
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetFunctionFromEntity(AMX* amx, cell* params)
{
	CheckEntity(amx, params[1]);

	edict_t* edict = INDEXENT(params[1]);
	long object =  edict ? (long)edict->pvPrivateData : 0;

	if (object)
	{
		int len;
		char *functionName = g_fn_GetAmxString(amx, params[2], 0, &len);
		char *classname = g_fn_GetAmxString(amx, params[3], 1, &len);

		return GetFunctionFrom(amx, functionName, classname, object);
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid entity");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetFunctionFromMonster(AMX* amx, cell* params)
{
	CheckEntity(amx, params[1]);

	entvars_t* entvars = &INDEXENT(params[1])->v;

	long object =  entvars ? (long)entvars->euser4 : 0;

	if (object)
	{
		int len;
		char *functionName = g_fn_GetAmxString(amx, params[2], 0, &len);
		char *classname = g_fn_GetAmxString(amx, params[3], 1, &len);

		return GetFunctionFrom(amx, functionName, classname, object);
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid monster");
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::GetFunctionFromObject(AMX* amx, cell* params)
{
	int len;
	char *functionName = g_fn_GetAmxString(amx, params[2], 0, &len);
	char *classname = g_fn_GetAmxString(amx, params[3], 1, &len);

	return GetFunctionFrom(amx, functionName, classname, params[1]);
}

cell AMX_NATIVE_CALL Orpheu::GetLibraryAddress(AMX* amx, cell* params)
{
	char *libraryName = g_fn_GetAmxString(amx, params[1], 0, NULL);
	return LibrariesManager::getLibraryAddress(libraryName);
}

cell MemoryReplace_(AMX* amx, cell* params, long address)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	unsigned int expectedParams = 4;

	if ((expectedParams == paramsCount) || ((expectedParams + 1) == paramsCount))
	{
		char *memoryStructureName = g_fn_GetAmxString(amx, params[1], 0, NULL);

		MemoryStructure* memoryStructure = Global::MemoryStructManagerObj->get(memoryStructureName);

		if (memoryStructure)
		{
			TypeHandler* typeHandler = memoryStructure->typeHandler;

			LibraryInfo* libraryInfo = LibrariesManager::getLibrary((char*)memoryStructure->library.chars());

			if (libraryInfo)
			{
				long count = params[2];

				long size = typeHandler->getSize(amx, params[4]);

				long start = 0;
				long end = 0;

				if (address)
				{
					if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), address))
					{
						MF_LogError(amx, AMX_ERR_NATIVE, "Address isn't contained in the library");
						return 0;
					}

					start = address;
					end = start + (long)libraryInfo->length - size - (start - (long)libraryInfo->baseAddress);
				}
				else
				{
					start = (long)libraryInfo->baseAddress;
					end = start + (long)libraryInfo->length - size;
				}

				long replacements = 0;

				long lastAddress = 0;

				for (long i=start; i < end; ++i)
				{
					if (typeHandler->isPointer())
					{
						if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), *((long*)i)))
							continue;
					}

					if (typeHandler->compareTo(amx, &params[3], (void*)i))
					{
						lastAddress = i;

						unsigned long oldProtection;

						if (Memory::ChangeMemoryProtection((void*)i, size, PAGE_EXECUTE_READWRITE, oldProtection))
						{
							typeHandler->convertFromAmxToStructure(amx, params[4], (void*)i);

							// oldProtection = memoryStructure->memoryProtection;

							if (oldProtection == (unsigned long)PAGE_EXECUTE_READWRITE || Memory::ChangeMemoryProtection((void*)i, size, oldProtection))
							{
								if (++replacements == count)
								{
									break;
								}
							}
						}
					}
				}

				if ((expectedParams + 1) == paramsCount)
				{
					*MF_GetAmxAddr(amx, params[5]) = lastAddress;
				}

				return replacements;
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Invalid library \"%s\"", memoryStructure->library.chars());
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid memory structure \"%s\"", memoryStructureName);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Got [%d] Expected [%d] or [%d]", paramsCount, expectedParams, expectedParams + 1);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::MemoryReplace(AMX* amx, cell* params)
{
	return MemoryReplace_(amx, params, 0);
}

cell AMX_NATIVE_CALL Orpheu::MemoryReplaceAtAddress(AMX* amx, cell* params)
{
	long address = params[1];
	params[1] = params[0] - sizeof(cell);

	return MemoryReplace_(amx, &params[1], address);
}

cell MemoryGet_(AMX* amx, cell* params, long address)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	char *memoryStructureName = g_fn_GetAmxString(amx, params[1], 0, NULL);

	MemoryStructure* memoryStructure = Global::MemoryStructManagerObj->get(memoryStructureName);

	if (memoryStructure)
	{
		if (memoryStructure->type != MemTypeSearch)
		{
			unsigned int expectedParams = 1 + memoryStructure->typeHandler->getIfByRefParamsCount();

			if ((expectedParams == paramsCount) || ((expectedParams + 1) == paramsCount))
			{
				LibraryInfo* libraryInfo = LibrariesManager::getLibrary((char*)memoryStructure->library.chars());

				long position = 0;

				if (address)
				{
					if (libraryInfo)
					{
						if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), address))
						{
							MF_LogError(amx, AMX_ERR_NATIVE, "Address isn't contained in the library");
							return 0;
						}
					}

					position = address;
				}
				else
				{
					if (!libraryInfo)
					{
						MF_LogError(amx, AMX_ERR_NATIVE, "Memory structure doesn't have a library defined");
						return 0;
					}

					position = (long)libraryInfo->baseAddress;
				}

				TypeHandler* typeHandler = memoryStructure->typeHandler;

				switch (memoryStructure->type)
				{
				case MemTypeOffset:
				{
					position += memoryStructure->offset;

					if (libraryInfo)
					{
						if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), position))
						{
							MF_LogError(amx, AMX_ERR_NATIVE, "Address with the offset isn't contained in the library");
							return 0;
						}

						if (typeHandler->isPointer())
						{
							if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), *((long*)position)))
							{
								MF_LogError(amx, AMX_ERR_NATIVE, "Address with the offset doesn't contain a valid pointer");
								return 0;
							}
						}
					}

					if ((expectedParams + 1) == paramsCount)
					{
						*MF_GetAmxAddr(amx, params[2 + typeHandler->getIfByRefParamsCount()]) = position;
					}

					return typeHandler->convertToAmxFromStructure(amx, &params[2], (void*)position);

					break;
				}
				case MemTypeSignature:
				{
					if (address)
						position = (long)LibrariesManager::findMemory((char*)memoryStructure->library.chars(), memoryStructure->signature, memoryStructure->signatureEntryData, memoryStructure->signatureLength, position);
					else
						position = (long)memoryStructure->basicAddress;

					if (position)
					{
						position += memoryStructure->displacement;

						if (typeHandler->isPointer())
						{
							if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), *((long*)position)))
							{
								MF_LogError(amx, AMX_ERR_NATIVE, "Address found with signature doesn't contain a valid pointer");
								return 0;
							}
						}

						if ((expectedParams + 1) == paramsCount)
						{
							*MF_GetAmxAddr(amx, params[2 + typeHandler->getIfByRefParamsCount()]) = position;
						}

						return typeHandler->convertToAmxFromStructure(amx, &params[2], (void*)position);
					}
					else
					{
						MF_LogError(amx, AMX_ERR_NATIVE, "Signature not found in memory");
						return 0;
					}
				}
				case MemTypeSearch:
				{
					break;
				}
				}
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Got [%d] Expected [%d] or [%d]", paramsCount, expectedParams, expectedParams + 1);
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "To use this functionality you must provide valid identifiers for the memory\n");
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid memory structure \"%s\"", memoryStructureName);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::MemoryGet(AMX* amx, cell* params)
{
	return MemoryGet_(amx, params, 0);
}

cell AMX_NATIVE_CALL Orpheu::MemoryGetAtAddress(AMX* amx, cell* params)
{
	long address = params[1];
	params[1] = params[0] - sizeof(cell);

	return MemoryGet_(amx, &params[1], address);
}

cell MemorySet_(AMX* amx, cell* params, long address)
{
	unsigned int paramsCount = params[0] / sizeof(cell);

	unsigned int expectedParams = 3;

	if ((expectedParams == paramsCount) || ((expectedParams + 1) == paramsCount))
	{
		char *memoryStructureName = g_fn_GetAmxString(amx, params[1], 0, NULL);

		MemoryStructure* memoryStructure = Global::MemoryStructManagerObj->get(memoryStructureName);

		if (memoryStructure)
		{
			if (memoryStructure->type != MemTypeSearch)
			{
				TypeHandler* typeHandler = memoryStructure->typeHandler;

				LibraryInfo* libraryInfo = LibrariesManager::getLibrary((char*)memoryStructure->library.chars());

				long position = 0;

				if (address)
				{
					if (libraryInfo)
					{
						if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), address))
						{
							MF_LogError(amx, AMX_ERR_NATIVE, "Address isn't contained in the library");
							return 0;
						}
					}

					position = address;
				}
				else
				{
					if (!libraryInfo)
					{
						MF_LogError(amx, AMX_ERR_NATIVE, "Memory structure doesn't have a library defined");
						return 0;
					}

					position = (long)libraryInfo->baseAddress;
				}

				long size = typeHandler->getSize(amx, params[3]);

				switch (memoryStructure->type)
				{
				case MemTypeOffset:
				{
					position += memoryStructure->offset;

					if (libraryInfo)
					{
						if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), position))
						{
							MF_LogError(amx, AMX_ERR_NATIVE, "Address with the offset isn't contained in the library");
							return 0;
						}

						if (typeHandler->isPointer())
						{
							if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), *((long*)position)))
							{
								MF_LogError(amx, AMX_ERR_NATIVE, "Address with the offset doesn't contain a valid pointer");
								return 0;
							}
						}
					}

					unsigned long oldProtection;

					if (Memory::ChangeMemoryProtection((void*)position, size, PAGE_EXECUTE_READWRITE, oldProtection))
					{
						typeHandler->convertFromAmxToStructure(amx, params[3], (void*)position);

						//oldProtection = memoryStructure->memoryProtection;

						if (oldProtection == (unsigned long)PAGE_EXECUTE_READWRITE || Memory::ChangeMemoryProtection((void*)position, size, oldProtection))
						{
							if ((expectedParams + 1) == paramsCount)
							{
								*MF_GetAmxAddr(amx, params[4]) = position;
								return 1;
							}
						}
					}

					break;
				}
				case MemTypeSignature:
				{
					long max = params[2];
					long count = 0;

					if (max == 1)
					{
						if (address)
							position = (long)LibrariesManager::findMemory((char*)memoryStructure->library.chars(), memoryStructure->signature, memoryStructure->signatureEntryData, memoryStructure->signatureLength, position);
						else
							position = (long)memoryStructure->basicAddress;

						if (position)
						{
							position += memoryStructure->displacement;

							if (typeHandler->isPointer())
							{
								if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), *((long*)position)))
								{
									MF_LogError(amx, AMX_ERR_NATIVE, "Address found with signature doesn't contain a valid pointer");
									return 0;
								}
							}

							unsigned long oldProtection;

							if (Memory::ChangeMemoryProtection((void*)position, size, PAGE_EXECUTE_READWRITE, oldProtection))
							{
								typeHandler->convertFromAmxToStructure(amx, params[3], (void*)position);

								//oldProtection = memoryStructure->memoryProtection;

								if (oldProtection == (unsigned long)PAGE_EXECUTE_READWRITE || Memory::ChangeMemoryProtection((void*)position, size, oldProtection))
								{
									count = 1;
								}
							}
						}
					}
					else
					{
						while (max--)
						{
							position = (long)LibrariesManager::findMemory((char*)memoryStructure->library.chars(), memoryStructure->signature, memoryStructure->signatureEntryData, memoryStructure->signatureLength, position);

							if (position)
							{
								position += memoryStructure->displacement;

								if (typeHandler->isPointer())
								{
									if (!LibrariesManager::libraryContainsAddress((char*)memoryStructure->library.chars(), *((long*)position)))
									{
										MF_LogError(amx, AMX_ERR_NATIVE, "Address found with signature doesn't contain a valid pointer");
										return 0;
									}
								}

								unsigned long oldProtection;

								if (Memory::ChangeMemoryProtection((void*)position, size, PAGE_EXECUTE_READWRITE, oldProtection))
								{
									typeHandler->convertFromAmxToStructure(amx, params[3], (void*)position);

									//oldProtection = memoryStructure->memoryProtection;

									if (oldProtection == (unsigned long)PAGE_EXECUTE_READWRITE || Memory::ChangeMemoryProtection((void*)position, size, oldProtection))
									{
										count++;
										position += size;
									}
								}

								count++;
								position += size;
							}
							else
							{
								break;
							}
						}
					}

					if ((expectedParams + 1) == paramsCount)
					{
						*MF_GetAmxAddr(amx, params[4]) = position;
					}

					return count;

					break;
				}
				case MemTypeSearch:
				{
					break;
				}
				}
			}
			else
			{
				MF_LogError(amx, AMX_ERR_NATIVE, "To use this functionality you must provide valid identifiers for the memory\n");
			}
		}
		else
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Invalid memory structure \"%s\"", memoryStructureName);
		}
	}
	else
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid parameter count. Got [%d] Expected [%d] or [%d]", paramsCount, expectedParams, expectedParams + 1);
	}

	return 0;
}

cell AMX_NATIVE_CALL Orpheu::MemorySet(AMX* amx, cell* params)
{
	return MemorySet_(amx, params, 0);
}

cell AMX_NATIVE_CALL Orpheu::MemorySetAtAddress(AMX* amx, cell* params)
{
	long address = params[1];
	params[1] = params[0] - sizeof(cell);

	return MemorySet_(amx, &params[1], address);
}

AMX_NATIVE_INFO OrpheuNatives[] =
{
	{ "OrpheuCall"                    , Orpheu::Call                     },
	{ "OrpheuCallSuper"               , Orpheu::CallSuper                },
	{ "OrpheuGetFunction"             , Orpheu::GetFunction              },
	{ "OrpheuTryGetFunction"          , Orpheu::TryGetFunction           },
	{ "OrpheuRegisterHook"            , Orpheu::RegisterHook             },
	{ "OrpheuUnregisterHook"          , Orpheu::UnregisterHook           },
	{ "OrpheuGetReturn"               , Orpheu::GetReturn                },
	{ "OrpheuSetReturn"               , Orpheu::SetReturn                },
	{ "OrpheuGetReturnStatus"         , Orpheu::GetReturnStatus          },
	{ "OrpheuSetParam"                , Orpheu::SetParam                 },
	{ "OrpheuSetParamStructMember"    , Orpheu::SetParamStructMember     },
	{ "OrpheuGetParamStructMember"    , Orpheu::GetParamStructMember     },
	{ "OrpheuCreateStruct"            , Orpheu::CreateStruct             },
	{ "OrpheuGetStructFromAddress"    , Orpheu::GetStructFromAddress     },
	{ "OrpheuSetStructMember"         , Orpheu::SetStructMember          },
	{ "OrpheuGetStructMember"         , Orpheu::GetStructMember          },
	{ "OrpheuGetStructFromParam"      , Orpheu::GetStructFromParam       },
	{ "OrpheuCloneStructFromParam"    , Orpheu::CloneStructFromParam     },
	{ "OrpheuCreateFunction"          , Orpheu::CreateFunction           },
	{ "OrpheuGetEngineFunctionsStruct", Orpheu::GetEngineFunctionsStruct },
	{ "OrpheuGetDLLFunctionsStruct"   , Orpheu::GetDLLFunctionsStruct    },
	{ "OrpheuGetFunctionOffset"       , Orpheu::GetFunctionOffset        },
	{ "OrpheuGetBytesAtAddress"       , Orpheu::GetBytesAtAddress        },
	{ "OrpheuGetFunctionAddress"      , Orpheu::GetFunctionAddress       },
	{ "OrpheuGetNextCallAtAddress"    , Orpheu::GetNextCallAtAddress     },
	{ "OrpheuGetFunctionFromClass"    , Orpheu::GetFunctionFromClass     },
	{ "OrpheuGetFunctionFromEntity"   , Orpheu::GetFunctionFromEntity    },
	{ "OrpheuGetFunctionFromObject"   , Orpheu::GetFunctionFromObject    },
	{ "OrpheuGetFunctionFromMonster"  , Orpheu::GetFunctionFromMonster   },
	{ "OrpheuGetLibraryAddress"       , Orpheu::GetLibraryAddress        },
	{ "OrpheuMemoryReplace"           , Orpheu::MemoryReplace            },
	{ "OrpheuMemoryReplaceAtAddress"  , Orpheu::MemoryReplaceAtAddress   },
	{ "OrpheuMemorySet"               , Orpheu::MemorySet                },
	{ "OrpheuMemorySetAtAddress"      , Orpheu::MemorySetAtAddress       },
	{ "OrpheuMemoryGet"               , Orpheu::MemoryGet                },
	{ "OrpheuMemoryGetAtAddress"      , Orpheu::MemoryGetAtAddress       },
	{ NULL                            , NULL }
};

#ifdef __linux__
static int addLibraryRecursive(struct dl_phdr_info *info, size_t size, void *data)
{
	char msg[100];
	char process[260];
	char library[32];

	strncpy(process, info->dlpi_name, strlen(info->dlpi_name));

	if (strstr(process, "addons"))
	{
		size_t length = strlen(process);
		size_t i = length;

		while (process[--i] != '.') {}; process[i] = '\0';
		while (process[--i] != '/') {};

		char *pLibrary = strcpy(library, &process[i + 1]);
		char *ptr = strstr(pLibrary, "_i386");

		if (ptr)
		{
			library[ptr - pLibrary] = '\0';
		}

		if (!LibrariesManager::addLibrary(library, (void *)info->dlpi_addr))
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\tERROR adding library %s (0x%p)\n", library, info->dlpi_addr);
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
		}
		else
		{
			UTIL_Format(msg, sizeof(msg) - 1, "\tAdding library %s (0x%p)\n", library, info->dlpi_addr);
			Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
		}
	}

	return 0;
}
#else
void addLibraries() // IM THE KING OF EXAMPLE COPYPASTING!
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;
	char msg[100];

	hProcess = GetCurrentProcess();

	if (hProcess == NULL) // IS NOT POSSIBLE!
		return;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		TCHAR process[MAX_PATH];
		char library[32];

		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); ++i)
		{
			if (GetModuleFileNameEx(hProcess, hMods[i], process, sizeof(process) / sizeof(TCHAR)))
			{
				_MODULEINFO info;
				GetModuleInformation(hProcess, hMods[i], &info, sizeof(info));

				if (strstr(process, "addons"))
				{
					size_t length = strlen(process);
					size_t i = length;

					while (process[--i] != '.') {}; process[i] = '\0';
					while (process[--i] != '\\') {};

					strcpy(library, &process[i + 1]);

					if (!LibrariesManager::addLibrary(library, info.lpBaseOfDll))
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\tERROR adding library %s (0x%p)\n", library, info.lpBaseOfDll);
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
					}
					else
					{
						UTIL_Format(msg, sizeof(msg) - 1, "\tAdding library %s (0x%p)\n", library, info.lpBaseOfDll);
						Global::ConfigManagerObj->ModuleConfig.append(ke::AString(msg));
					}
				}
			}
		}
	}
}
#endif

void print_srvconsole(const char *fmt, ...)
{
	va_list argptr;
	static char string[384];
	va_start(argptr, fmt);
	vsnprintf(string, sizeof(string) - 1, fmt, argptr);
	string[sizeof(string) - 1] = '\0';
	va_end(argptr);

	SERVER_PRINT(string);
}

void CommandOrpheu(void)
{
	const char *cmd = CMD_ARGV(1);

	if (!strcasecmp(cmd, "version"))
	{
		print_srvconsole("\n %s %s\n -\n", Plugin_info.name, ORPHEU_VERSION);
		print_srvconsole(" Support      : %s\n", Plugin_info.url);
		print_srvconsole(" Author       : %s\n", Plugin_info.author);
		print_srvconsole(" Compiled on: : %s\n", ORPHEU_BUILD_TIME);
#if defined(ORPHEU_GENERATED_BUILD)
		print_srvconsole(" Built from   : https://github.com/Arkshine/Orpheu/commit/%s\n", ORPHEU_SHA);
		print_srvconsole(" Build ID     : %s:%s\n", ORPHEU_LOCAL_REV, ORPHEU_SHA);
#endif
		print_srvconsole("\n");
		return;
	}
	else if (!strcasecmp(cmd, "config"))
	{
		for (size_t i = 0; i < Global::ConfigManagerObj->ModuleConfig.length(); ++i)
		{
			print_srvconsole("%s", Global::ConfigManagerObj->ModuleConfig.at(i).chars());
		}

		char file[512];
		char date[32];
		time_t td; time(&td);

		strftime(date, sizeof date - 1, "%m-%d-%Y", localtime(&td));
		MF_BuildPathnameR(file, sizeof file - 1, "%s/orpheu-%s.log", MF_GetLocalInfo("amxx_logs", "addons/amxmodx/logs"), date);

		FILE* h = fopen(file, "w");
		if (h)
		{
			for (size_t i = 0; i < Global::ConfigManagerObj->ModuleConfig.length(); ++i)
			{
				fputs(Global::ConfigManagerObj->ModuleConfig.at(i).chars(), h);
			}
			fclose(h);
		}

		return;
	}

	print_srvconsole("\n Usage: orpheu < command >\n");
	print_srvconsole(" Commands:\n");
	print_srvconsole("   version    - Display some informations about the module and where to get a support.\n");
	print_srvconsole("   config     - Display module status.\n\n");
}

void OnMetaAttach()
{
	REG_SVR_COMMAND("orpheu", CommandOrpheu);

	if (!LibrariesManager::addLibrary("mod", (void*)MNEW_OnFreeEntPrivateData))
		LibrariesManager::addLibrary("mod", (void*)MDLL_Spawn);

	LibrariesManager::addLibrary("engine", (void*)gpGlobals);
	LibrariesManager::addLibrary("metamod", (void*)gpMetaGlobals);

	Global::FunctionManagerObj = new FunctionManager();
	Global::ConfigManagerObj = new ConfigManager();

	Global::ConfigManagerObj->loadBaseData();

	Hooker::hookerCvarRegister = new Hooker::HookerCvarRegister();
}

void OnAmxxAttach()
{
	MF_AddNatives(OrpheuNatives);
}

void OnPluginsLoaded()
{
	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu libraries search started.\n\n"));

#ifdef __linux__
	dl_iterate_phdr(addLibraryRecursive, NULL);
#else
	addLibraries();
#endif

	Global::ConfigManagerObj->ModuleConfig.append(ke::AString("\nOrpheu libraries search ended.\n"));

	Global::ConfigManagerObj->loadFunctions();
	Global::ConfigManagerObj->loadVirtualFunctions();
	Global::ConfigManagerObj->loadMemoryStructures();
}

void OnPluginsUnloaded()
{
	Global::FunctionManagerObj->removeAllHooks();

	for (size_t i = 0; i < Global::TypesCount; ++i)
		Global::Types[i]->freeAllocatedMemory();

	for (size_t i = 0; i < Global::StructuresCount; ++i)
		Global::Structures[i]->freeAllocatedMemory();

	Global::ConfigManagerObj->ModuleConfig.clear();
}

extern "C" void __cxa_pure_virtual() { while (1); }