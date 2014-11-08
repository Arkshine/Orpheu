
#include <structData.h>

class PhysentStructData : public StructData < physent_s >
{
public:

	PhysentStructData()
	{
		addMember("name", &structure->name, Types[TypeCharArr]);
		addMember("player", &structure->player, Types[TypeLong]);
		addMember("info", &structure->info, Types[TypeLong]);
		addMember("solid", &structure->solid, Types[TypeLong]);
		addMember("skin", &structure->skin, Types[TypeLong]);
		addMember("rendermode", &structure->rendermode, Types[TypeLong]);
		addMember("sequence", &structure->sequence, Types[TypeLong]);
		addMember("movetype", &structure->movetype, Types[TypeLong]);
		addMember("takedamage", &structure->takedamage, Types[TypeLong]);
		addMember("blooddecal", &structure->blooddecal, Types[TypeLong]);
		addMember("team", &structure->team, Types[TypeLong]);
		addMember("classnumber", &structure->classnumber, Types[TypeLong]);
		addMember("iuser1", &structure->iuser1, Types[TypeLong]);
		addMember("iuser2", &structure->iuser2, Types[TypeLong]);
		addMember("iuser3", &structure->iuser3, Types[TypeLong]);
		addMember("iuser4", &structure->iuser4, Types[TypeLong]);
		addMember("frame", &structure->frame, Types[TypeFloat]);
		addMember("fuser1", &structure->fuser1, Types[TypeFloat]);
		addMember("fuser2", &structure->fuser2, Types[TypeFloat]);
		addMember("fuser3", &structure->fuser3, Types[TypeFloat]);
		addMember("fuser4", &structure->fuser4, Types[TypeFloat]);
		addMember("angles", &structure->angles, Types[TypeVector]);
		addMember("origin", &structure->origin, Types[TypeVector]);
		addMember("mins", &structure->mins, Types[TypeVector]);
		addMember("maxs", &structure->maxs, Types[TypeVector]);
		addMember("vuser1", &structure->vuser1, Types[TypeVector]);
		addMember("vuser2", &structure->vuser2, Types[TypeVector]);
		addMember("vuser3", &structure->vuser3, Types[TypeVector]);
		addMember("vuser4", &structure->vuser4, Types[TypeVector]);
	}

	const char *getName()
	{
		return "physent_s *";
	};
};

