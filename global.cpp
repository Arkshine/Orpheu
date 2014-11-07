
#include <global.h>

#include <typeHandlerImplementations/charHandler.h>
#include <typeHandlerImplementations/shortHandler.h>
#include <typeHandlerImplementations/boolHandler.h>
#include <typeHandlerImplementations/byteHandler.h>
#include <typeHandlerImplementations/longHandler.h>
#include <typeHandlerImplementations/CBaseEntityHandler.h>
#include <typeHandlerImplementations/charPtrHandler.h>
#include <typeHandlerImplementations/edict_sPtrHandler.h>
#include <typeHandlerImplementations/floatHandler.h>
#include <typeHandlerImplementations/VectorHandler.h>
#include <typeHandlerImplementations/VectorPointerHandler.h>
#include <typeHandlerImplementations/VectorPointerByRefHandler.h>
#include <typeHandlerImplementations/CMBaseMonsterHandler.h>
#include <typeHandlerImplementations/entvarsHandler.h>
#include <typeHandlerImplementations/CGameRulesHandler.h>

#include <typeHandlerImplementations/charArrHandler.h>

#include <structures/MonsterEvent.h>
#include <structures/DLL_FUNCTIONS.h>
#include <structures/playermove.h>
#include <structures/enginefuncs.h>
#include <structures/usercmdStruct.h>
#include <structures/traceResult.h>
#include <structures/movevars.h>
#include <structures/physent.h>
#include <structures/pmplane.h>
#include <structures/pmtraceStruct.h>
#include <structures/weapon_data.h>
#include <structures/ammoInfo.h>
#include <structures/itemInfo.h>
#include <structures/task.h>
#include <structures/schedule.h>
#include <structures/KeyValueData.h>
#include <structures/cvar.h>

namespace Global
{
	TypeHandlerManager* TypeHandlerManagerObj = NULL;
	ConfigManager* ConfigManagerObj = NULL;
	FunctionManager* FunctionManagerObj = NULL;
	StructManager* StructManagerObj = new StructManager();
	FunctionStructuresManager* FunctionStructuresManagerObj = new FunctionStructuresManager();
	FunctionVirtualManager* FunctionVirtualManagerObj = new FunctionVirtualManager();
	MemoryStructManager* MemoryStructManagerObj = new MemoryStructManager();

	Folders OrpheuFolders =
	{
		ke::AString("orpheu"),
		ke::AString("libraries"),
		ke::AString("functions"),
		ke::AString("types"),
		ke::AString("virtualFunctions"),
		ke::AString("mods"),
		ke::AString("memory")
	};

	TypeHandler* Types[] =
	{
		new BoolHandler(),
		new ByteHandler(),
		new LongHandler(),
		new CBaseEntityHandler(),
		new CharPtrHandler(),
		new Edict_sPtrHandler(),
		new FloatHandler(),
		new VectorPointerHandler(),
		new VectorPointerByRefHandler(),
		new CMBaseMonsterHandler(),
		new CharHandler(),
		new ShortHandler(),
		new EntvarsHandler(),
		new VectorHandler(),
		new CGameRulesHandler(),
		new CharArrHandler()
	};

	StructHandlerDirect* StructuresDirect[] =
	{
		new StructHandlerDirect((new movevarsStructData())->getStructInfo()),
		new StructHandlerDirect((new usercmdStructData())->getStructInfo()),
		new StructHandlerDirect((new MonsterEvent())->getStructInfo()),
		new StructHandlerDirect((new DLL_FUNCTIONS_StructData())->getStructInfo()),
		new StructHandlerDirect((new PlayerMove())->getStructInfo()),
		new StructHandlerDirect((new enginefuncsStructData())->getStructInfo()),
		new StructHandlerDirect((new TraceResultStructData())->getStructInfo()),
		new StructHandlerDirect((new PhysentStructData())->getStructInfo()),
		new StructHandlerDirect((new PmplaneStructData())->getStructInfo()),
		new StructHandlerDirect((new PmtraceStructData())->getStructInfo()),
		new StructHandlerDirect((new WeaponData())->getStructInfo()),
		new StructHandlerDirect((new AmmoInfo())->getStructInfo()),
		new StructHandlerDirect((new ItemInfo())->getStructInfo()),
		new StructHandlerDirect((new Task())->getStructInfo()),
		new StructHandlerDirect((new Schedule())->getStructInfo()),
		new StructHandlerDirect((new KeyValueData_StructData())->getStructInfo()),
		new StructHandlerDirect((new cvarStructData())->getStructInfo())
	};

	StructHandler* Structures[] =
	{
		new StructHandler((new movevarsStructData())->getStructInfo()),
		new StructHandler((new usercmdStructData())->getStructInfo()),
		new StructHandler((new MonsterEvent())->getStructInfo()),
		new StructHandler((new DLL_FUNCTIONS_StructData())->getStructInfo()),
		new StructHandler((new PlayerMove())->getStructInfo()),
		new StructHandler((new enginefuncsStructData())->getStructInfo()),
		new StructHandler((new TraceResultStructData())->getStructInfo()),
		new StructHandler((new PhysentStructData())->getStructInfo()),
		new StructHandler((new PmplaneStructData())->getStructInfo()),
		new StructHandler((new PmtraceStructData())->getStructInfo()),
		new StructHandler((new WeaponData())->getStructInfo()),
		new StructHandler((new AmmoInfo())->getStructInfo()),
		new StructHandler((new ItemInfo())->getStructInfo()),
		new StructHandler((new Task())->getStructInfo()),
		new StructHandler((new Schedule())->getStructInfo()),
		new StructHandler((new KeyValueData_StructData())->getStructInfo()),
		new StructHandler((new cvarStructData())->getStructInfo())
	};

	unsigned int TypesCount = sizeof(Types) / sizeof(TypeHandler*);
	unsigned int StructuresCount = sizeof(Structures) / sizeof(StructHandler*);

	ke::AString Modname;

	int pev = 0;

	StringHashMap<char*>* LibrariesCvarToName = NULL;
	Function* FunctionBeingHooked = NULL;
}
