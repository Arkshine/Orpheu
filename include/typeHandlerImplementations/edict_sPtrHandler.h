
#include <typeHandler.h>

class Edict_sPtrHandler : public TypeHandler
{
public:

	void* convertFromAmx(AMX* amx, cell param);
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
	void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);

	int getSize(AMX* amx, cell param)
	{
		return sizeof(long);
	}

	const char* getName() { return "edict_s *"; }
};

