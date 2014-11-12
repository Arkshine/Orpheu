#include <structData.h>
#include <cvardef.h>

class cvarStructData : public StructData < cvar_s >
{
public:

	cvarStructData()
	{
		addMember("name", &structure->name, Types[TypeString]);
		addMember("string", &structure->string, Types[TypeString]);
		addMember("flags", &structure->flags, Types[TypeLong]);
		addMember("value", &structure->value, Types[TypeFloat]);
		addMember("next", &structure->next, Structures[StructCvar]);
	}

	const char *getName()
	{
		return "cvar_s *";
	};
};
