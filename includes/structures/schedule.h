
#include <structData.h>
#include <structuresDump/schedule.h>

class Schedule : public StructData < Schedule_t >
{
public:

	Schedule()
	{
		addMember("pTasklist", &structure->pTasklist, Structures[StructTask]);
		addMember("cTasks", &structure->cTasks, Types[TypeLong]);
		addMember("iInterruptMask", &structure->iInterruptMask, Types[TypeLong]);
		addMember("iSoundMask", &structure->iSoundMask, Types[TypeLong]);
		addMember("pName", &structure->pName, Types[TypeString]);
	}

	const char *getName()
	{
		return "Schedule_t *";
	};
};

