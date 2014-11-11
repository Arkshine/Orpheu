
#include <typeHandlerImplementations/charHandler.h>

void* CharHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void*)*value;
}

void CharHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = (cell)(char)standardReturn;
}

void CharHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<char>(amx, param, address);
}

cell CharHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<char>(amx, params, address);
}
