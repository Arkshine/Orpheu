
#ifndef _INCLUDE_VECTOR_HANDLER_
#define _INCLUDE_VECTOR_HANDLER_

#include <typeHandler.h>

class VectorHandler : public TypeHandler
{
public:

	void* convertFromAmx(AMX* amx, cell param);
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
	void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);

	const char* getName() { return "Vector"; }
	ForwardParam getParamType(){ return FP_ARRAY; }
	virtual int getIfByRefParamsCount(){ return 1; }
	void convertToAmxByRef(AMX* amx, cell* params, long standardReturn);

	int getSize(AMX* amx, cell param){ return 0; }
};

#endif
