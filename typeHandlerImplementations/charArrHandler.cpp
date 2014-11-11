
#include <typeHandlerImplementations/charArrHandler.h>

void CharArrHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	int len;
	char *str = g_fn_GetAmxString(amx, param, 0, &len);

	strcpy((char*)address, str);
}

cell CharArrHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return g_fn_SetAmxString(amx, params[0], (char*)address, *MF_GetAmxAddr(amx, params[1]));
}

bool CharArrHandler::compareTo(AMX* amx, cell* params, void* address)
{
	char *str = g_fn_GetAmxString(amx, params[0], 0, NULL);
	return !strcmp(str, (char*)address);
}
