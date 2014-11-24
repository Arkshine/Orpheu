
#ifndef _INCLUDE_ORPHEU_
#define _INCLUDE_ORPHEU_

//#define _ERRORS_ONLY_

#include <amxxmodule.h>
#include <am-string.h>

#if defined __linux__
#include <link.h>
#else
#include <Psapi.h>
#endif

#undef close

struct Folders
{
	ke::AString config;
	ke::AString libraries;
	ke::AString functions;
	ke::AString typeAliases;
	ke::AString virtualFunctions;
	ke::AString mods;
	ke::AString memory;
};

class Orpheu
{
public:

	static cell AMX_NATIVE_CALL Call(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL CallSuper(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunction(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL TryGetFunction(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL UnregisterHook(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL RegisterHook(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetReturn(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL SetReturn(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetReturnStatus(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL SetParam(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL SetParamStructMember(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetParamStructMember(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL CreateStruct(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetStructFromAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL SetStructMember(AMX*amx, cell* params);
	static cell AMX_NATIVE_CALL GetStructMember(AMX*amx, cell* params);
	static cell AMX_NATIVE_CALL GetStructFromParam(AMX*amx, cell* params);
	static cell AMX_NATIVE_CALL CloneStructFromParam(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL CreateFunction(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetEngineFunctionsStruct(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetDLLFunctionsStruct(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetBytesAtAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunctionAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetNextCallAtAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunctionOffset(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunctionFromClass(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunctionFromEntity(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunctionFromMonster(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetFunctionFromObject(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL GetLibraryAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL MemoryReplace(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL MemoryReplaceAtAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL MemorySet(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL MemorySetAtAddress(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL MemoryGet(AMX* amx, cell* params);
	static cell AMX_NATIVE_CALL MemoryGetAtAddress(AMX* amx, cell* params);
};

extern "C" void __cxa_pure_virtual();

#endif

