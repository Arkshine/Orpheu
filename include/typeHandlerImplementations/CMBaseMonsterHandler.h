
#include <typeHandlerImplementations/CBaseEntityHandler.h>

class CMBaseMonsterHandler : public CBaseEntityHandler
{
public:

	const char* getName() { return "CMBaseMonster *"; }
	void* convertFromAmx(AMX* amx, cell param);
	void convertToAmx(cell& value, long standardReturn, ConvertMode convertMode);
};

