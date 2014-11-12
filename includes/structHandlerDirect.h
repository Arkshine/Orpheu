
#ifndef _INCLUDE_STRUCT_HANDLER_DIRECT_
#define _INCLUDE_STRUCT_HANDLER_DIRECT_

#include <structHandler.h>

class StructHandlerDirect : public StructHandler
{
public:
	StructHandlerDirect(StructInfo structInfo) :
		StructHandler(structInfo){}

	void convertFromAmxToStructure(AMX* amx, cell param, void* address)
	{
		memcpy(address, (void*)param, this->getStructSize());
	}
	cell convertToAmxFromStructure(AMX* amx, cell* params, void* address)
	{
		cell value;
		this->convertToAmx(value, (long)address);
		return value;
	}
};

#endif
