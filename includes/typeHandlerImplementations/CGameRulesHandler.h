
#include <typeHandlerImplementations/longHandler.h>
#include <typeHandler.h>

class CGameRulesHandler : public LongHandler
{
public:

	const char* getName() { return "CGameRules *"; }
};

