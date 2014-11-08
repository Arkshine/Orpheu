
#include <typeHandlerImplementations/edict_sPtrHandler.h>

void* Edict_sPtrHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);

	return (*value) ? (void*)INDEXENT(*value) : NULL;
}

void Edict_sPtrHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = standardReturn ? ENTINDEX((edict_t*)standardReturn) : 0;
}

void Edict_sPtrHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<long>(amx, param, address);
}

cell Edict_sPtrHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<long>(amx, params, address);
}
