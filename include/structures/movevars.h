
#include <structData.h>
#include <pm_movevars.h>

class movevarsStructData : public StructData < movevars_s >
{
public:

	movevarsStructData()
	{
		addMember("gravity", &structure->gravity, Types[TypeFloat]);
		addMember("stopspeed", &structure->stopspeed, Types[TypeFloat]);
		addMember("maxspeed", &structure->maxspeed, Types[TypeFloat]);
		addMember("spectatormaxspeed", &structure->spectatormaxspeed, Types[TypeFloat]);
		addMember("accelerate", &structure->accelerate, Types[TypeFloat]);
		addMember("airaccelerate", &structure->airaccelerate, Types[TypeFloat]);
		addMember("wateraccelerate", &structure->wateraccelerate, Types[TypeFloat]);
		addMember("friction", &structure->friction, Types[TypeFloat]);
		addMember("edgefriction", &structure->edgefriction, Types[TypeFloat]);
		addMember("waterfriction", &structure->waterfriction, Types[TypeFloat]);
		addMember("entgravity", &structure->entgravity, Types[TypeFloat]);
		addMember("bounce", &structure->bounce, Types[TypeFloat]);
		addMember("stepsize", &structure->stepsize, Types[TypeFloat]);
		addMember("maxvelocity", &structure->maxvelocity, Types[TypeFloat]);
		addMember("zmax", &structure->zmax, Types[TypeFloat]);
		addMember("waveHeight", &structure->waveHeight, Types[TypeFloat]);
		addMember("footsteps", &structure->footsteps, Types[TypeLong]);
		addMember("skyName", &structure->skyName, Types[TypeCharArr]);
		addMember("rollangle", &structure->rollangle, Types[TypeFloat]);
		addMember("rollspeed", &structure->rollspeed, Types[TypeFloat]);
		addMember("skycolor_r", &structure->skycolor_r, Types[TypeFloat]);
		addMember("skycolor_g", &structure->skycolor_g, Types[TypeFloat]);
		addMember("skycolor_b", &structure->skycolor_b, Types[TypeFloat]);
		addMember("skyvec_x", &structure->skyvec_x, Types[TypeFloat]);
		addMember("skyvec_y", &structure->skyvec_y, Types[TypeFloat]);
		addMember("skyvec_z", &structure->skyvec_z, Types[TypeFloat]);
	}

	const char *getName()
	{
		return "movevars_s *";
	};
};

