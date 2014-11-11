
#include <typeHandler.h>

class ShortHandler : public TypeHandler
{
public:

	void* convertFromAmx(AMX* amx, cell param);
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
	void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);

	int getSize(AMX* amx, cell param)
	{
		return sizeof(short);
	}

	const char* getName() { return "short"; }
};

