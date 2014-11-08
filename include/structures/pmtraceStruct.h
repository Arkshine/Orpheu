
#include <structData.h>
#include <pmtrace.h>

class PmtraceStructData : public StructData < pmtrace_t >
{
public:

	PmtraceStructData()
	{
		addMember("fraction", &structure->fraction, Types[TypeFloat]);
		addMember("inwater", &structure->inwater, Types[TypeLong]);
		addMember("inopen", &structure->inopen, Types[TypeLong]);
		addMember("startsolid", &structure->startsolid, Types[TypeLong]);
		addMember("allsolid", &structure->allsolid, Types[TypeLong]);
		addMember("ent", &structure->ent, Types[TypeLong]);
		addMember("hitgroup", &structure->hitgroup, Types[TypeLong]);
		addMember("plane", &structure->plane, StructuresDirect[StructPmplane]);
		addMember("endpos", &structure->endpos, Types[TypeVector]);
		addMember("deltavelocity", &structure->deltavelocity, Types[TypeVector]);
	}

	const char *getName()
	{
		return "pmtrace_s *";
	};
};

