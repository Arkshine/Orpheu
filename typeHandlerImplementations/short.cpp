
#include <typeHandlerImplementations/shortHandler.h>

void* ShortHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void*)*value;
}

void ShortHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = (cell)(short)standardReturn;
}

void ShortHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<short>(amx, param, address);
}

cell ShortHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<short>(amx, params, address);
}
