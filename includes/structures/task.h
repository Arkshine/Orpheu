
#include <structData.h>
#include <structuresDump/schedule.h>

class Task : public StructData < Task_t >
{
public:

	Task()
	{
		addMember("iTask", &structure->iTask, Types[TypeLong]);
		addMember("flData", &structure->flData, Types[TypeFloat]);
	}

	const char *getName()
	{
		return "Task_t *";
	};
};

