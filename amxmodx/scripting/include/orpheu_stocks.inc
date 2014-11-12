
#if defined _orpheu_stocks_included
	#endinput
#endif
#define _orpheu_stocks_included

#include <orpheu_const>
#include <orpheu>
#include <orpheu_advanced>


/**
 *  Retrieves an engine function handler by having its name as a member of the struct that hold 
 * engine functions and the name that you give it in the file where you define the function
 *  The name must be the same as the one in the file where the function is defined
 *
 * @param memberName		The name of the member of the struct that holds the address of the function
 * 							Example: pfnPrecacheModel
 * 							The struct representation can be seen in hlsdk at multiplayer/engine/eiface.h with the name "enginefuncs_s"
 * 
 * @param libFunctionName	The name of the function as it is in the file where the function is defined
 *
 * @return 					A handler to the function
 */
stock OrpheuFunction:OrpheuGetEngineFunction(const memberName[],const libFunctionName[])
{
	static OrpheuStruct:engineFunctions
	
	if(!engineFunctions)
	{
		engineFunctions = OrpheuGetEngineFunctionsStruct()
	}
	
	return OrpheuCreateFunction( OrpheuGetStructMember(engineFunctions,memberName),libFunctionName )
}

/**
 *  Retrieves a dll function handler by having its name as a member of the struct that hold 
 * dll functions and the name that you give it in the file where you define the function
 *  The name must be the same as the one in the file where the function is defined
 *
 * @param memberName		The name of the member of the struct that holds the address of the function
 * 							Example: pfnGameInit
 * 							The struct representation can be seen in hlsdk at multiplayer/engine/eiface.h with the name "DLL_FUNCTIONS"
 * 
 * @param libFunctionName	The name of the function as it is in the file where the function is defined
 *
 * @return 					A handler to the function
 */
stock OrpheuFunction:OrpheuGetDLLFunction(const memberName[],const libFunctionName[])
{
	static OrpheuStruct:OrpheuDLLFunctions
	
	if(!OrpheuDLLFunctions)
	{
		OrpheuDLLFunctions = OrpheuGetDLLFunctionsStruct()
	}
	
	return OrpheuCreateFunction( OrpheuGetStructMember(OrpheuDLLFunctions,memberName),libFunctionName )
}

stock OrpheuHook:OrpheuRegisterHookFromClass(const entityClassName[],const libFunctionName[],const libClassName[],const hookFunctionName[],OrpheuHookPhase:phase = OrpheuHookPre)
{
	return OrpheuRegisterHook(OrpheuGetFunctionFromClass(entityClassName,libFunctionName,libClassName),hookFunctionName,phase)
}

stock OrpheuHook:OrpheuRegisterHookFromEntity(id,const libFunctionName[],const libClassName[],const hookFunctionName[],OrpheuHookPhase:phase = OrpheuHookPre)
{
	return OrpheuRegisterHook(OrpheuGetFunctionFromEntity(id,libFunctionName,libClassName),hookFunctionName,phase)
}

stock OrpheuHook:OrpheuRegisterHookFromObject(object,const libFunctionName[],const libClassName[],const hookFunctionName[],OrpheuHookPhase:phase = OrpheuHookPre)
{
	return OrpheuRegisterHook(OrpheuGetFunctionFromObject(object,libFunctionName,libClassName),hookFunctionName,phase)
}
