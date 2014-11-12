
#if defined _orpheu_const_included
	#endinput
#endif
#define _orpheu_const_included

#if AMXX_VERSION_NUM >= 175
 #pragma reqlib orpheu
 #if !defined AMXMODX_NOAUTOLOAD
  #pragma loadlib orpheu
 #endif
#else
 #pragma library orpheu
#endif

const OrpheuFunction:OrpheuInvalidFunction = OrpheuFunction:0

enum OrpheuHookReturn
{
	OrpheuIgnored,
	OrpheuOverride,
	OrpheuSupercede
}

enum OrpheuHookPhase
{
	OrpheuHookPre,
	OrpheuHookPost
}

enum OrpheuStructType
{
	OrpheuStructMovevars,
	OrpheuStructUsercmd,
	OrpheuStructMonsterEvent,
	OrpheuStructDLL_FUNCTIONS,
	OrpheuStructPlayerMove,
	OrpheuStructEngineFuncs,
	OrpheuStructTraceResult,
	OrpheuStructPhysent,
	OrpheuStructPmplane,
	OrpheuStructPmtrace,
	OrpheuStructWeaponData,
	OrpheuStructAmmoInfo,
	OrpheuStructItemInfo,
	OrpheuStructTask,
	OrpheuStructSchedule,
	OrpheuStructKeyValueData
}
