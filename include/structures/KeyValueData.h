
#include <structData.h>
#include <eiface.h>

class KeyValueData_StructData : public StructData < KeyValueData_s >
{
public:

	KeyValueData_StructData()
	{
		addMember("szClassName", &structure->szClassName, Types[TypeString]);
		addMember("szKeyName", &structure->szKeyName, Types[TypeString]);
		addMember("szValue", &structure->szValue, Types[TypeString]);
		addMember("fHandled", &structure->fHandled, Types[TypeLong]);
	}

	const char *getName()
	{
		return "KeyValueData_s *";
	};
};

