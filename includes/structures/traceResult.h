
#include <structData.h>

class TraceResultStructData : public StructData < TraceResult >
{
public:

	TraceResultStructData()
	{
		addMember("fAllSolid", &structure->fAllSolid, Types[TypeLong]);
		addMember("fStartSolid", &structure->fStartSolid, Types[TypeLong]);
		addMember("fInOpen", &structure->fInOpen, Types[TypeLong]);
		addMember("fInWater", &structure->fInWater, Types[TypeLong]);
		addMember("flFraction", &structure->flFraction, Types[TypeFloat]);
		addMember("vecEndPos", &structure->vecEndPos, Types[TypeVector]);
		addMember("flPlaneDist", &structure->flPlaneDist, Types[TypeFloat]);
		addMember("vecPlaneNormal", &structure->vecPlaneNormal, Types[TypeVector]);
		addMember("pHit", &structure->pHit, Types[TypeEdict]);
		addMember("iHitgroup", &structure->iHitgroup, Types[TypeLong]);
	}

	const char *getName()
	{
		return "TraceResult *";
	};
};

