
#include <typeHandlerImplementations/VectorPointerHandler.h>

void* VectorPointerHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* amxVector = MF_GetAmxAddr(amx, param);
	vec3_t* vector = allocateMemory<vec3_t>();

	vector->x = amx_ctof(amxVector[0]);
	vector->y = amx_ctof(amxVector[1]);
	vector->z = amx_ctof(amxVector[2]);

	return (void *)vector;
}

void VectorPointerHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	cell *amxVector = allocateMemory<cell>(3);

	if (standardReturn)
	{
		vec3_t* vector = (vec3_t*)standardReturn;

		amxVector[0] = amx_ftoc(vector->x);
		amxVector[1] = amx_ftoc(vector->y);
		amxVector[2] = amx_ftoc(vector->z);
	}
	else
	{
		amxVector[0] = amx_ftoc(0.0);
		amxVector[1] = amx_ftoc(0.0);
		amxVector[2] = amx_ftoc(0.0);
	}

	value = MF_PrepareCellArray(amxVector, 3);
}

void VectorPointerHandler::convertToAmxByRef(AMX* amx, cell* params, long standardReturn)
{
	cell* amxVector = MF_GetAmxAddr(amx, params[0]);
	if (standardReturn)
	{
		vec3_t* vector = (vec3_t*)standardReturn;

		amxVector[0] = amx_ftoc(vector->x);
		amxVector[1] = amx_ftoc(vector->y);
		amxVector[2] = amx_ftoc(vector->z);
	}
	else
	{
		amxVector[0] = amx_ftoc(0.0);
		amxVector[1] = amx_ftoc(0.0);
		amxVector[2] = amx_ftoc(0.0);
	}
}

void VectorPointerHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	cell* amxVector = MF_GetAmxAddr(amx, param);
	vec3_t* vector = (vec3_t*)address;

	vector->x = amx_ctof(amxVector[0]);
	vector->y = amx_ctof(amxVector[1]);
	vector->z = amx_ctof(amxVector[2]);
}

cell VectorPointerHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	convertToAmxByRef(amx, params, (long)address);
	return 0;
}
