
#include <typeHandlerImplementations/entvarsHandler.h>

void* EntvarsHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void *)&INDEXENT(*value)->v;
}

void EntvarsHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	value = 0;

	entvars_t * pev = (entvars_t *)standardReturn;

	if (pev)
		if (pev->pContainingEntity)
			value = ENTINDEX(pev->pContainingEntity);
}
