
#include <structData.h>
#include <pm_defs.h>

class PlayerMove : public StructData < playermove_t >
{
public:

	PlayerMove()
	{
		addMember("player_index", &structure->player_index, Types[TypeLong]);
		addMember("movetype", &structure->movetype, Types[TypeLong]);
		addMember("frametime", &structure->frametime, Types[TypeFloat]);
		addMember("time", &structure->time, Types[TypeFloat]);
		addMember("origin", &structure->origin, Types[TypeVector]);
		addMember("PM_StuckTouch", &structure->PM_StuckTouch, Types[TypeLong]);
		addMember("PM_PlaySound", &structure->PM_PlaySound, Types[TypeLong]);
		addMember("PM_Info_ValueForKey", &structure->PM_Info_ValueForKey, Types[TypeLong]);
		addMember("cmd", &structure->cmd, StructuresDirect[StructUsercmd]);
		addMember("sztexturename", &structure->sztexturename, Types[TypeCharArr]);
		addMember("chtexturetype", &structure->chtexturetype, Types[TypeChar]);
		addMember("movevars", &structure->movevars, Structures[StructMovevars]);
		addMember("player_index", &structure->player_index, Types[TypeLong]);
		addMember("server", &structure->server, Types[TypeLong]);
		addMember("multiplayer", &structure->multiplayer, Types[TypeLong]);
		addMember("time", &structure->time, Types[TypeFloat]);
		addMember("frametime", &structure->frametime, Types[TypeFloat]);
		addMember("forward", &structure->forward, Types[TypeVector]);
		addMember("right", &structure->right, Types[TypeVector]);
		addMember("up", &structure->up, Types[TypeVector]);
		addMember("origin", &structure->origin, Types[TypeVector]);
		addMember("angles", &structure->angles, Types[TypeVector]);
		addMember("oldangles", &structure->oldangles, Types[TypeVector]);
		addMember("velocity", &structure->velocity, Types[TypeVector]);
		addMember("movedir", &structure->movedir, Types[TypeVector]);
		addMember("basevelocity", &structure->basevelocity, Types[TypeVector]);
		addMember("view_ofs", &structure->view_ofs, Types[TypeVector]);
		addMember("flDuckTime", &structure->flDuckTime, Types[TypeFloat]);
		addMember("bInDuck", &structure->bInDuck, Types[TypeLong]);
		addMember("flTimeStepSound", &structure->flTimeStepSound, Types[TypeLong]);
		addMember("iStepLeft", &structure->iStepLeft, Types[TypeLong]);
		addMember("flFallVelocity", &structure->flFallVelocity, Types[TypeFloat]);
		addMember("punchangle", &structure->punchangle, Types[TypeVector]);
		addMember("flSwimTime", &structure->flSwimTime, Types[TypeLong]);
		addMember("flNextPrimaryAttack", &structure->flNextPrimaryAttack, Types[TypeFloat]);
		addMember("effects", &structure->effects, Types[TypeLong]);
		addMember("flags", &structure->flags, Types[TypeLong]);
		addMember("usehull", &structure->usehull, Types[TypeLong]);
		addMember("gravity", &structure->gravity, Types[TypeFloat]);
		addMember("friction", &structure->friction, Types[TypeFloat]);
		addMember("oldbuttons", &structure->oldbuttons, Types[TypeLong]);
		addMember("waterjumptime", &structure->waterjumptime, Types[TypeFloat]);
		addMember("dead", &structure->dead, Types[TypeLong]);
		addMember("deadflag", &structure->deadflag, Types[TypeLong]);
		addMember("spectator", &structure->spectator, Types[TypeLong]);
		addMember("movetype", &structure->movetype, Types[TypeLong]);
		addMember("onground", &structure->onground, Types[TypeLong]);
		addMember("waterlevel", &structure->waterlevel, Types[TypeLong]);
		addMember("watertype", &structure->watertype, Types[TypeLong]);
		addMember("oldwaterlevel", &structure->oldwaterlevel, Types[TypeLong]);
		addMember("sztexturename", &structure->sztexturename, Types[TypeCharArr]);
		addMember("chtexturetype", &structure->chtexturetype, Types[TypeChar]);
		addMember("maxspeed", &structure->maxspeed, Types[TypeFloat]);
		addMember("clientmaxspeed", &structure->clientmaxspeed, Types[TypeFloat]);
		addMember("iuser1", &structure->iuser1, Types[TypeLong]);
		addMember("iuser2", &structure->iuser2, Types[TypeLong]);
		addMember("iuser3", &structure->iuser3, Types[TypeLong]);
		addMember("iuser4", &structure->iuser4, Types[TypeLong]);
		addMember("fuser1", &structure->fuser1, Types[TypeFloat]);
		addMember("fuser2", &structure->fuser2, Types[TypeFloat]);
		addMember("fuser3", &structure->fuser3, Types[TypeFloat]);
		addMember("fuser4", &structure->fuser4, Types[TypeFloat]);
		addMember("vuser1", &structure->vuser1, Types[TypeVector]);
		addMember("vuser2", &structure->vuser2, Types[TypeVector]);
		addMember("vuser3", &structure->vuser3, Types[TypeVector]);
		addMember("vuser4", &structure->vuser4, Types[TypeVector]);
		addMember("numphysent", &structure->numphysent, Types[TypeLong]);
		addMember("nummoveent", &structure->nummoveent, Types[TypeLong]);
		addMember("numvisent", &structure->numvisent, Types[TypeLong]);
		addMember("cmd", &structure->cmd, StructuresDirect[StructUsercmd]);
		addMember("numtouch", &structure->numtouch, Types[TypeLong]);
		addMember("physinfo", &structure->physinfo, Types[TypeCharArr]);
		addMember("movevars", &structure->movevars, Structures[StructMovevars]);
		addMember("runfuncs", &structure->runfuncs, Types[TypeLong]);
		addMember("PM_Info_ValueForKey", &structure->PM_Info_ValueForKey, Types[TypeLong]);
		addMember("PM_Particle", &structure->PM_Particle, Types[TypeLong]);
		addMember("PM_TestPlayerPosition", &structure->PM_TestPlayerPosition, Types[TypeLong]);
		addMember("Con_NPrintf", &structure->Con_NPrintf, Types[TypeLong]);
		addMember("Con_DPrintf", &structure->Con_DPrintf, Types[TypeLong]);
		addMember("Con_Printf", &structure->Con_Printf, Types[TypeLong]);
		addMember("Sys_FloatTime", &structure->Sys_FloatTime, Types[TypeLong]);
		addMember("PM_StuckTouch", &structure->PM_StuckTouch, Types[TypeLong]);
		addMember("PM_PointContents", &structure->PM_PointContents, Types[TypeLong]);
		addMember("PM_TruePointContents", &structure->PM_TruePointContents, Types[TypeLong]);
		addMember("PM_HullPointContents", &structure->PM_HullPointContents, Types[TypeLong]);
		addMember("PM_PlayerTrace", &structure->PM_PlayerTrace, Types[TypeLong]);
		addMember("PM_TraceLine", &structure->PM_TraceLine, Types[TypeLong]);
		addMember("RandomLong", &structure->RandomLong, Types[TypeLong]);
		addMember("RandomFloat", &structure->RandomFloat, Types[TypeLong]);
		addMember("PM_GetModelType", &structure->PM_GetModelType, Types[TypeLong]);
		addMember("PM_GetModelBounds", &structure->PM_GetModelBounds, Types[TypeLong]);
		addMember("PM_HullForBsp", &structure->PM_HullForBsp, Types[TypeLong]);
		addMember("PM_TraceModel", &structure->PM_TraceModel, Types[TypeLong]);
		addMember("COM_FileSize", &structure->COM_FileSize, Types[TypeLong]);
		addMember("COM_LoadFile", &structure->COM_LoadFile, Types[TypeLong]);
		addMember("COM_FreeFile", &structure->COM_FreeFile, Types[TypeLong]);
		addMember("memfgets", &structure->memfgets, Types[TypeLong]);
		addMember("PM_PlaySound", &structure->PM_PlaySound, Types[TypeLong]);
		addMember("PM_TraceTexture", &structure->PM_TraceTexture, Types[TypeLong]);
		addMember("PM_PlaybackEventFull", &structure->PM_PlaybackEventFull, Types[TypeLong]);
		addMember("PM_PlayerTraceEx", &structure->PM_PlayerTraceEx, Types[TypeLong]);
		addMember("PM_TestPlayerPositionEx", &structure->PM_TestPlayerPositionEx, Types[TypeLong]);
		addMember("PM_TraceLineEx", &structure->PM_TraceLineEx, Types[TypeLong]);
	}

	const char *getName()
	{
		return "playermove_s *";
	};
};

