
#ifndef _INCLUDE_ENTVARS_HANDLER_
#define _INCLUDE_ENTVARS_HANDLER_

#include <typeHandler.h>
#include <typeHandlerImplementations/longHandler.h>

class EntvarsHandler : public LongHandler
{
public:

	void* convertFromAmx(AMX* amx, cell param);
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);

	int getSize(AMX* amx, cell param)
	{
		return sizeof(long);
	}

	virtual const char* getName() { return "entvars_s *"; }
};

#endif

