
#ifndef _INCLUDE_CHAR_PTR_HANDLER_
#define _INCLUDE_CHAR_PTR_HANDLER_

#include <typeHandler.h>

class CharPtrHandler : public TypeHandler
{
private:
	int len;

public:

	void* convertFromAmx(AMX* amx, cell param);
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
	virtual void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	virtual cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);

	const char* getName() { return "char *"; }
	ForwardParam getParamType(){ return FP_ARRAY; }
	int getIfByRefParamsCount(){ return 2; }
	void convertToAmxByRef(AMX* amx, cell* params, long standardReturn);
	bool compareTo(AMX* amx, cell* params, void* address);

	int getSize(AMX* amx, cell param)
	{
		return strlen(g_fn_GetAmxString(amx, param, 0, NULL)) + 1;
	}

	virtual bool isPointer()
	{
		return true;
	}
};

#endif

