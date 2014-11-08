
#include <typeHandlerImplementations/boolHandler.h>

void* BoolHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void*)*value;
}

void BoolHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = (cell)(char)standardReturn;
}

void BoolHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<bool>(amx, param, address);
}

cell BoolHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<bool>(amx, params, address);
}
