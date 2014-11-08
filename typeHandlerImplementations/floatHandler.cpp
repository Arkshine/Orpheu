
#include <typeHandlerImplementations/floatHandler.h>

void* FloatHandler::convertFromAmx(AMX* amx, cell param)
{
	cell* value = allocateMemory<cell>();
	*value = *MF_GetAmxAddr(amx, param);
	return (void*)(*value);
}

void FloatHandler::convertToAmx(cell& value, long standardReturn, ConvertMode convertMode)
{
	static float returnFloat;

	switch (convertMode)
	{
	case Pass:
	{
		value = standardReturn;
		break;
	}
	case Call:
	{
#if defined WIN32
		__asm fstp returnFloat;
#else
		asm("fstpl %0":"=m"(returnFloat));
#endif
		value = amx_ftoc(returnFloat);
		break;
	}
	case CallHooked:
	{
#if defined WIN32
		__asm fst returnFloat;
#else
		asm("fstl %0":"=m"(returnFloat));
#endif
		value = amx_ftoc(returnFloat);
	}
	}
}

void FloatHandler::convertFromAmxToStructure(AMX* amx, cell param, void* address)
{
	*(reinterpret_cast<float*>(address)) = amx_ctof((long)convertFromAmx(amx, param));
}

cell FloatHandler::convertToAmxFromStructure(AMX* amx, cell* params, void* address)
{
	return amx_ftoc(*((float*)(address)));
}
