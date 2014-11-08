
#include <structData.h>
#include <structuresDump/monsterevent.h>

class MonsterEvent : public StructData < MonsterEvent_t >
{
public:

	MonsterEvent()
	{
		addMember("event", &structure->event, Types[TypeLong]);
		addMember("options", &structure->options, Types[TypeString]);
	}

	const char *getName()
	{
		return "MonsterEvent_t *";
	};
};

