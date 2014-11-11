
#include <typeHandlerImplementations/byteHandler.h>

void* ByteHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void*)*value;
}

void ByteHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = (cell)(byte)standardReturn;
}

void ByteHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	convertFromAmxToStructureStandard<byte>(amx, param, address);
}

cell ByteHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return this->convertToAmxFromStructureStandard<byte>(amx, params, address);
}
