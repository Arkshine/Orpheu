
#include <typeHandlerImplementations/CBaseEntityHandler.h>

#if defined __linux__
#include <librariesManager.h>
#else
#include <cbase.h>
#endif

void* CBaseEntityHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);

	CheckEntity(amx, *value);

	edict_t* edict = INDEXENT(*value);

	return (void*)edict ? edict->pvPrivateData : NULL;
}

void CBaseEntityHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = PrivateToIndex((const void *)standardReturn);
}

void CBaseEntityHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<long>(amx, param, address);
}

cell CBaseEntityHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<long>(amx, params, address);
}
