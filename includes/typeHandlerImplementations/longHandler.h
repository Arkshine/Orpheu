
#ifndef _INCLUDE_LONG_HANDLER_
#define _INCLUDE_LONG_HANDLER_

#include <typeHandler.h>

class LongHandler : public TypeHandler
{
public:

	virtual void* convertFromAmx(AMX* amx, cell param);
	virtual void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
	virtual void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	virtual cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);

	int getSize(AMX* amx, cell param)
	{
		return sizeof(long);
	}

	virtual const char* getName() { return "long"; }
};

#endif

