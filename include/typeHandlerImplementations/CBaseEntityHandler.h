
#ifndef _INCLUDE_CBASE_ENTITY_HANDLER_
#define _INCLUDE_CBASE_ENTITY_HANDLER_

#include <typeHandler.h>
#include <global.h>

class CBaseEntityHandler : public TypeHandler
{
	virtual void* convertFromAmx(AMX* amx, cell param);
	virtual void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
	void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);

	int getSize(AMX* amx, cell param)
	{
		return sizeof(long);
	}

	const char* getName() { return "CBaseEntity *"; }
};

inline void CheckEntity(AMX* amx, cell id)
{
	if (id < 1 || id > gpGlobals->maxEntities)
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Entity \"%d\" out of range", id);
	}
	else if (FNullEnt(INDEXENT(id)))
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Entity \"%d\" invalid", id);
	}
	else if (id <= gpGlobals->maxClients)
	{
		if (!MF_IsPlayerIngame(id))
		{
			MF_LogError(amx, AMX_ERR_NATIVE, "Player \"%d\" not in game", id);
		}
	}
}

// Borrowed from HamSandwich
inline int PrivateToIndex(const void *pdata)
{
	if (pdata == NULL)
	{
		return -1;
	}
	char *ptr=(char*)pdata;

	ptr += Global::pev;

	entvars_t *pev=*(entvars_t **)ptr;

	if (pev == NULL)
	{
		return -1;
	}

	if (pev->pContainingEntity == NULL)
	{
		return -1;
	}

	return ENTINDEX(pev->pContainingEntity);
};

#endif

