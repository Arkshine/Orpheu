
#include <typeHandlerImplementations/longHandler.h>

void* LongHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void*)*value;
}

void LongHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = (cell)standardReturn;
}

void LongHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<long>(amx, param, address);
}

cell LongHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<long>(amx, params, address);
}
