
#include <structData.h>

class PmplaneStructData : public StructData < pmplane_t >
{
public:

	PmplaneStructData()
	{
		addMember("normal", &structure->normal, Types[TypeVector]);
		addMember("dist", &structure->dist, Types[TypeFloat]);
	}

	const char *getName()
	{
		return "pmplane_s *";
	};
};

