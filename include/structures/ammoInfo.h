
#include <structData.h>

typedef struct
{
	const char *pszName;
	int iId;
} AmmoInfo_;

class AmmoInfo : public StructData < AmmoInfo_ >
{
public:

	AmmoInfo()
	{
		addMember("iId", &structure->iId, Types[TypeLong]);
		addMember("pszName", &structure->pszName, Types[TypeString]);
	}

	const char *getName()
	{
		return "AmmoInfo *";
	};
};

