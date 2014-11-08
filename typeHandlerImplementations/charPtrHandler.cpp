
#include <typeHandlerImplementations/charPtrHandler.h>

void* CharPtrHandler::convertFromAmx(AMX* amx, cell param)
{
	char *str = g_fn_GetAmxString(amx, param, 0, &len);

	/*static char msg[1024];
	sprintf( msg, "[CharPtrHandler::convertFromAmx] \"%s\"\n", str );
	SERVER_PRINT( msg );*/

	char *allocString = allocateMemory<char>(len + 1);
	strcpy(allocString, str);

	return (void *)allocString;
}

void CharPtrHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	if (standardReturn)
	{
		value = MF_PrepareCharArray((char*)standardReturn, strlen((char *)standardReturn) + 1);
	}
	else
	{
		value = MF_PrepareCharArray((char *)"", 1);
	}
}

void CharPtrHandler::convertToAmxByRef(AMX* amx, cell* params, long standardReturn)
{
	g_fn_SetAmxString(amx, params[0], (const char *)standardReturn, *MF_GetAmxAddr(amx, params[1]));
}

void CharPtrHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	int len;
	char *str = g_fn_GetAmxString(amx, param, 0, &len);

	/*static char msg[1024];
	sprintf( msg, "[CharPtrHandler::convertFromAmxToStructure] \"%s\"\n", str );
	SERVER_PRINT( msg );*/

	char* allocString = allocateMemory<char>(len + 1);
	strcpy(allocString, str);

	*((const char **)address) = allocString;
}

cell CharPtrHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return g_fn_SetAmxString(amx, params[0], *((const char **)address), *MF_GetAmxAddr(amx, params[1]));
}

bool CharPtrHandler::compareTo(AMX* amx, cell* params, void* address)
{
	char *str = g_fn_GetAmxString(amx, params[0], 0, NULL);
	return !strcmp(str, *((char**)address));
}

