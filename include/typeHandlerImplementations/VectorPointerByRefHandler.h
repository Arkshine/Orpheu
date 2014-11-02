
#include <typeHandlerImplementations/VectorPointerHandler.h>

class VectorPointerByRefHandler : public VectorPointerHandler
{
	const char* getName() { return "Vector *&"; }
	bool isPassedByRef(){ return true; }
};

