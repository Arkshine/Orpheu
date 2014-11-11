
#include <typeHandler.h>

class AddressInStructHandler : public TypeHandler
{
	public:

		void* convertFromAmx(AMX* amx,cell param){return NULL;}
		void convertToAmx(cell& value,long standardReturn,ConvertMode convertMode){}
		void convertFromAmxToStructure(AMX* amx,cell param,void* address){}
		cell convertToAmxFromStructure(AMX* amx,cell* params,void* address){return (cell) address;}
		const char* getName() { return ""; }
};

