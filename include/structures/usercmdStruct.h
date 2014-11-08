
#include <structData.h>
#include <usercmd.h>

class usercmdStructData : public StructData < usercmd_s >
{
public:

	usercmdStructData()
	{
		addMember("lerp_msec", &structure->lerp_msec, Types[TypeShort]);
		addMember("msec", &structure->msec, Types[TypeByte]);
		addMember("viewangles", &structure->viewangles, Types[TypeVector]);
		addMember("forwardmove", &structure->forwardmove, Types[TypeFloat]);
		addMember("sidemove", &structure->sidemove, Types[TypeFloat]);
		addMember("upmove", &structure->upmove, Types[TypeFloat]);
		addMember("lightlevel", &structure->lightlevel, Types[TypeByte]);
		addMember("buttons", &structure->buttons, Types[TypeShort]);
		addMember("impulse", &structure->impulse, Types[TypeByte]);
		addMember("weaponselect", &structure->weaponselect, Types[TypeByte]);
		addMember("impact_index", &structure->impact_index, Types[TypeLong]);
		addMember("impact_position", &structure->impact_position, Types[TypeVector]);
	}

	const char *getName()
	{
		return "usercmd_s *";
	};
};

