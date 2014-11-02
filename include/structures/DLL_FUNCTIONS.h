
#include <structData.h>
#include <eiface.h>

class DLL_FUNCTIONS_StructData : public StructData < DLL_FUNCTIONS >
{
public:

	DLL_FUNCTIONS_StructData()
	{
		addMember("pfnGameInit", &structure->pfnGameInit, Types[TypeLong]);
		addMember("pfnSpawn", &structure->pfnSpawn, Types[TypeLong]);
		addMember("pfnThink", &structure->pfnThink, Types[TypeLong]);
		addMember("pfnUse", &structure->pfnUse, Types[TypeLong]);
		addMember("pfnTouch", &structure->pfnTouch, Types[TypeLong]);
		addMember("pfnBlocked", &structure->pfnBlocked, Types[TypeLong]);
		addMember("pfnKeyValue", &structure->pfnKeyValue, Types[TypeLong]);
		addMember("pfnSave", &structure->pfnSave, Types[TypeLong]);
		addMember("pfnRestore", &structure->pfnRestore, Types[TypeLong]);
		addMember("pfnSetAbsBox", &structure->pfnSetAbsBox, Types[TypeLong]);
		addMember("pfnSaveWriteFields", &structure->pfnSaveWriteFields, Types[TypeLong]);
		addMember("pfnSaveReadFields", &structure->pfnSaveReadFields, Types[TypeLong]);
		addMember("pfnSaveGlobalState", &structure->pfnSaveGlobalState, Types[TypeLong]);
		addMember("pfnRestoreGlobalState", &structure->pfnRestoreGlobalState, Types[TypeLong]);
		addMember("pfnResetGlobalState", &structure->pfnResetGlobalState, Types[TypeLong]);
		addMember("pfnClientConnect	", &structure->pfnClientConnect, Types[TypeLong]);
		addMember("pfnClientDisconnect", &structure->pfnClientDisconnect, Types[TypeLong]);
		addMember("pfnClientKill", &structure->pfnClientKill, Types[TypeLong]);
		addMember("pfnClientPutInServer", &structure->pfnClientPutInServer, Types[TypeLong]);
		addMember("pfnClientCommand", &structure->pfnClientCommand, Types[TypeLong]);
		addMember("pfnClientUserInfoChanged", &structure->pfnClientUserInfoChanged, Types[TypeLong]);
		addMember("pfnServerActivate", &structure->pfnServerActivate, Types[TypeLong]);
		addMember("pfnServerDeactivate", &structure->pfnServerDeactivate, Types[TypeLong]);
		addMember("pfnPlayerPreThink", &structure->pfnPlayerPreThink, Types[TypeLong]);
		addMember("pfnPlayerPostThink", &structure->pfnPlayerPostThink, Types[TypeLong]);
		addMember("pfnStartFrame", &structure->pfnStartFrame, Types[TypeLong]);
		addMember("pfnParmsNewLevel", &structure->pfnParmsNewLevel, Types[TypeLong]);
		addMember("pfnParmsChangeLevel", &structure->pfnParmsChangeLevel, Types[TypeLong]);
		addMember("pfnGetGameDescription", &structure->pfnGetGameDescription, Types[TypeLong]);
		addMember("pfnPlayerCustomization", &structure->pfnPlayerCustomization, Types[TypeLong]);
		addMember("pfnSpectatorConnect", &structure->pfnSpectatorConnect, Types[TypeLong]);
		addMember("pfnSpectatorDisconnect", &structure->pfnSpectatorDisconnect, Types[TypeLong]);
		addMember("pfnSpectatorThink", &structure->pfnSpectatorThink, Types[TypeLong]);
		addMember("pfnSys_Error", &structure->pfnSys_Error, Types[TypeLong]);
		addMember("pfnPM_Move", &structure->pfnPM_Move, Types[TypeLong]);
		addMember("pfnPM_Init", &structure->pfnPM_Init, Types[TypeLong]);
		addMember("pfnPM_FindTextureType", &structure->pfnPM_FindTextureType, Types[TypeLong]);
		addMember("pfnSetupVisibility", &structure->pfnSetupVisibility, Types[TypeLong]);
		addMember("pfnUpdateClientData", &structure->pfnUpdateClientData, Types[TypeLong]);
		addMember("pfnAddToFullPack", &structure->pfnAddToFullPack, Types[TypeLong]);
		addMember("pfnCreateBaseline", &structure->pfnCreateBaseline, Types[TypeLong]);
		addMember("pfnRegisterEncoders", &structure->pfnRegisterEncoders, Types[TypeLong]);
		addMember("pfnGetWeaponData", &structure->pfnGetWeaponData, Types[TypeLong]);
		addMember("pfnCmdStart", &structure->pfnCmdStart, Types[TypeLong]);
		addMember("pfnCmdEnd", &structure->pfnCmdEnd, Types[TypeLong]);
		addMember("pfnConnectionlessPacket ", &structure->pfnConnectionlessPacket, Types[TypeLong]);
		addMember("pfnGetHullBounds", &structure->pfnGetHullBounds, Types[TypeLong]);
		addMember("pfnCreateInstancedBaselines", &structure->pfnCreateInstancedBaselines, Types[TypeLong]);
		addMember("pfnInconsistentFile", &structure->pfnInconsistentFile, Types[TypeLong]);
		addMember("pfnAllowLagCompensation", &structure->pfnAllowLagCompensation, Types[TypeLong]);
	}

	const char *getName()
	{
		return "DLL_FUNCTIONS *";
	};
};

