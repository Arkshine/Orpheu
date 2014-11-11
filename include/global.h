
#ifndef _INCLUDE_GLOBAL_
#define _INCLUDE_GLOBAL_

#include <orpheu.h>
#include <librariesManager.h>

#include <functionManager.h>
#include <typeHandlerManager.h>
#include <configManager.h>
#include <structHandler.h>
#include <structHandlerDirect.h>
#include <structManager.h>
#include <functionStructuresManager.h>
#include <functionVirtualManager.h>
#include <memoryStructureManager.h>

enum TypesID
{
	TypeBool,
	TypeByte,
	TypeLong,
	TypeCBaseEntity,
	TypeString,
	TypeEdict,
	TypeFloat,
	TypeVectorPointer,
	TypeVectorPointerByRef,
	TypeCMBaseMonsterEntity,
	TypeChar,
	TypeShort,
	TypeEntvars,
	TypeVector,
	TypeCGameRules,
	TypeCharArr
};

enum StructuresTypes
{
	StructMovevars,
	StructUsercmd,
	StructMonsterEvent,
	StructDLL_FUNCTIONS,
	StructPlayermove,
	StructEnginefuncs,
	StructTraceResult,
	StructPhysent,
	StructPmplane,
	StructPmtrace,
	StructWeaponData,
	StructAmmoInfo,
	StructItemInfo,
	StructTask,
	StructSchedule,
	StructKeyValueData,
	StructCvar
};

namespace Global
{
	extern TypeHandlerManager* TypeHandlerManagerObj;
	extern ConfigManager* ConfigManagerObj;
	extern FunctionManager* FunctionManagerObj;
	extern StructManager* StructManagerObj;
	extern FunctionStructuresManager* FunctionStructuresManagerObj;
	extern FunctionVirtualManager* FunctionVirtualManagerObj;
	extern MemoryStructManager* MemoryStructManagerObj;
	extern Folders OrpheuFolders;
	extern TypeHandler* Types[];
	extern StructHandler* Structures[];
	extern StructHandlerDirect* StructuresDirect[];
	extern unsigned int TypesCount;
	extern unsigned int StructuresCount;
	extern ke::AString Modname;
	extern int pev;
	extern StringHashMap<char*>* LibrariesCvarToName;
	extern Function* FunctionBeingHooked;
};

#endif
