
#include <typeHandler.h>
#include <typeHandlerImplementations/charPtrHandler.h>

class CharArrHandler : public CharPtrHandler
{
public:

	void convertFromAmxToStructure(AMX* amx, cell param, void* address);
	cell convertToAmxFromStructure(AMX* amx, cell* params, void* address);
	bool compareTo(AMX* amx, cell* params, void* address);
	const char* getName() { return "string"; }

	bool isPointer()
	{
		return false;
	}
};

