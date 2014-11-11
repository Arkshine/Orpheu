
#include <typeHandlerImplementations/CMBaseMonsterHandler.h>
#include <cbase.h>

void* CMBaseMonsterHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);

	CheckEntity(amx, *value);

	entvars_t* entvars = &INDEXENT(*value)->v;

	return (void*)entvars->euser4;
}
void CMBaseMonsterHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	CBaseEntity* entity = (CBaseEntity*)standardReturn;
	value = (entity) ? entity->entindex() : 0;
}
