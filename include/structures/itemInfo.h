
#include <structData.h>

typedef struct
{
	int  iSlot;
	int  iPosition;
	const char  *pszAmmo1;    // ammo 1 type
	int  iMaxAmmo1;      // max ammo 1
	const char  *pszAmmo2;    // ammo 2 type
	int  iMaxAmmo2;      // max ammo 2
	const char  *pszName;
	int  iMaxClip;
	int  iId;
	int  iFlags;
	int  iWeight;// this value used to determine this weapon's importance in autoselection.
} ItemInfo_;

class ItemInfo : public StructData < ItemInfo_ >
{
public:

	ItemInfo()
	{
		addMember("pszAmmo1", &structure->pszAmmo1, Types[TypeString]);
		addMember("pszAmmo2", &structure->pszAmmo2, Types[TypeString]);
		addMember("pszName", &structure->pszName, Types[TypeString]);
		addMember("iSlot", &structure->iSlot, Types[TypeLong]);
		addMember("iPosition", &structure->iPosition, Types[TypeLong]);
		addMember("iMaxAmmo1", &structure->iMaxAmmo1, Types[TypeLong]);
		addMember("iMaxAmmo2", &structure->iMaxAmmo2, Types[TypeLong]);
		addMember("iMaxClip", &structure->iMaxClip, Types[TypeLong]);
		addMember("iId", &structure->iId, Types[TypeLong]);
		addMember("iFlags", &structure->iFlags, Types[TypeLong]);
		addMember("iWeight", &structure->iWeight, Types[TypeLong]);
	}

	const char *getName()
	{
		return "ItemInfo *";
	};
};

