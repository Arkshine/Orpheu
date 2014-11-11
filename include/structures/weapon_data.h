
#include <structData.h>
#include <weaponinfo.h>

class WeaponData : public StructData < weapon_data_s >
{
public:

	WeaponData()
	{
		addMember("m_iId", &structure->m_iId, Types[TypeLong]);
		addMember("m_iClip", &structure->m_iClip, Types[TypeLong]);
		addMember("m_fInReload", &structure->m_fInReload, Types[TypeLong]);
		addMember("m_fInSpecialReload", &structure->m_fInSpecialReload, Types[TypeLong]);
		addMember("m_fInZoom", &structure->m_fInZoom, Types[TypeLong]);
		addMember("m_iWeaponState", &structure->m_iWeaponState, Types[TypeLong]);
		addMember("iuser1", &structure->iuser1, Types[TypeLong]);
		addMember("iuser2", &structure->iuser2, Types[TypeLong]);
		addMember("iuser3", &structure->iuser3, Types[TypeLong]);
		addMember("iuser4", &structure->iuser4, Types[TypeLong]);
		addMember("m_flNextReload", &structure->m_flNextReload, Types[TypeFloat]);
		addMember("m_flPumpTime", &structure->m_flPumpTime, Types[TypeFloat]);
		addMember("m_fReloadTime", &structure->m_fReloadTime, Types[TypeFloat]);
		addMember("m_fAimedDamage", &structure->m_fAimedDamage, Types[TypeFloat]);
		addMember("m_fNextAimBonus", &structure->m_fNextAimBonus, Types[TypeFloat]);
		addMember("fuser1", &structure->fuser1, Types[TypeFloat]);
		addMember("fuser2", &structure->fuser2, Types[TypeFloat]);
		addMember("fuser3", &structure->fuser3, Types[TypeFloat]);
		addMember("fuser4", &structure->fuser4, Types[TypeFloat]);
		addMember("m_flNextPrimaryAttack", &structure->m_flNextPrimaryAttack, Types[TypeFloat]);
		addMember("m_flNextSecondaryAttack", &structure->m_flNextSecondaryAttack, Types[TypeFloat]);
		addMember("m_flTimeWeaponIdle", &structure->m_flTimeWeaponIdle, Types[TypeFloat]);
	}

	const char *getName()
	{
		return "weapon_data_s *";
	};
};

