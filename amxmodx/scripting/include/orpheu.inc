
#if defined _orpheu_included
	#endinput
#endif
#define _orpheu_included

#include <orpheu_const>

/**
 * Retrieves a function based on a function name
 * The name must be the same as the one in the file where the function is defined
 *
 * @param libFunctionName	The name of the function as it is in the file where the function is defined
 * @param className			The name of the class if the function belongs to one
 *
 * @return 					A handler to the function
 */
native OrpheuFunction:OrpheuGetFunction(const libFunctionName[],const className[]="")

/**
 * Hooks a function
 *
 * @param function			A handler to the function
 * @param hookFunctionName	The function name in the plugin that shall be called upon interception of the original function
 * @param phase				The phase of the hook. It can have two values. Pre means "right before the original function is called". Post means "right after the original function is called"
 *
 * @return 					A handler to the hook
 */
native OrpheuHook:OrpheuRegisterHook(OrpheuFunction:function,const hookFunctionName[],OrpheuHookPhase:phase = OrpheuHookPre)

/**
 * Unregisters a hook (stops it)
 *
 * @param hook			A handler to the hook
 */
native OrpheuUnregisterHook(OrpheuHook:hook)

/**
 * Calls a function without triggering its hooks
 * 
 * @param function		A handler to the function
 * @param any			The arguments of the function
 */
native OrpheuCall(OrpheuFunction:function,any:...)

/**
 * Calls a function and triggers its hooks
 * 
 * @param function		A handler to the function
 * @param any			The arguments of the function
 */
native OrpheuCallSuper(OrpheuFunction:function,any:...)

/**
 * Gets the return value of a function (To be used in hooks Post)
 *
 * @param any			In case the value is multi cell (string or vector), an holder to receive them by ref
 * @return				In case the value is uni cell, the value itself
 */
native any:OrpheuGetReturn(any:...)

/**
 * Sets the return value of a function
 *
 * @param any			Depending on the type of the return of the function, a value to be used as the return as the original hooked function
 */
native OrpheuSetReturn(any:...)

/**
 * Sets the value of an argument
 * 
 * @param num 			The number of the argument. The first argument would be the number "1"
 * @param any			Depending on the type of the argument, a value to be replace it to change the behaviour of the hooked function
 */
native OrpheuSetParam(num,any:...)

/**
 * Creates a struct
 * 
 * @param structType 	The type of the struct 
 * 
 * @return				A handler to the struct
 */
native OrpheuStruct:OrpheuCreateStruct(OrpheuStructType:structType)

/**
 * Retrieves the value of a member of a struct given the argument number the struct is and the member name
 * 
 * @param num 			The number of the argument. The first argument would be the number "1"
 * @param memberName 	The name of the member of the struct we want to deal with
 * @param any 			If the member is multi cell, 
 * 
 * @return				In case the value is uni cell, the value itself
 */
native OrpheuGetParamStructMember(num,const memberName[],any:...)

/**
 * Sets the value of member of a struct given the argument number the struct is and the member name
 * 
 * @param num 			The number of the argument. The first argument would be the number "1"
 * @param memberName 	The name of the member of the struct we want to deal with
 * @param any 			The new value
 */
native OrpheuSetParamStructMember(num,const memberName[],any:...)

/**
 * Gets a struct handler for a struct received as an argument 
 * Beware that if the original struct gets destroyed or changed these effects will reflect on your use of it.
 * 
 * @param num 			The number of the argument. The first argument would be the number "1"
 * 
 * @return				A handler to the struct
 */
native OrpheuStruct:OrpheuGetStructFromParam(num)

/**
 * Creates a struct equal to one received as an argument
 * 
 * @param num 			The number of the argument. The first argument would be the number "1"
 * 
 * @return				A handler to the struct
 */
native OrpheuStruct:OrpheuCloneStructFromParam(num)

/**
 * Sets the value of a member of a struct given a struct handler and the member name
 * 
 * @param struct 		A handler to the struct
 * @param memberName 	The name of the member of the struct we want to deal with
 * @param any 			The new value
 */
native OrpheuSetStructMember(OrpheuStruct:struct,const memberName[],any:...)

/**
 * Retrieves the value of a member of a struct given a struct handler and the member name
 * 
 * @param struct 		A handler to the struct
 * @param memberName 	The name of the member of the struct we want to deal with
 * @param any 			In case the value is multi cell (string or vector), an holder to receive them by ref
 * 
 * @return				In case the value is uni cell, the value itself
 */
native OrpheuGetStructMember(OrpheuStruct:struct,const memberName[],any:...)

/**
 * Retrieves a handler to a struct that hold the addresses of the engine functions
 * By retrieving addresses from the struct is possible to hook them.
 * A easier way to achieve the same thing is by using the stock OrpheuGetEngineFunction
 
 * @return				A handler to a struct that holds the engine functions
 */
native OrpheuStruct:OrpheuGetEngineFunctionsStruct()

/**
 * Retrieves a handler to a struct that hold the addresses of the dll functions
 * By retrieving addresses from the struct is possible to hook them.
 * A easier way to achieve the same thing is by using the stock OrpheuGetDllFunction
 
 * @return				A handler to a struct that holds the dll functions
 */
native OrpheuStruct:OrpheuGetDLLFunctionsStruct()

/**
 * Retrieves a handler to a function given a classname, the function name and the classname
 * This function is a virtual function (a function defined in abase class and implemented 
 * differently by each extender class)
 * For example: every class that extends CBaseEntity has a Spawn function. That function is defined in CBaseEntity
 * and implemented differently by each class derived from CBaseEntity
 * 
 * @param entityClassName 	A class related to the object that holds the function wanted to be hooked. Example: "player"
 * @param libFunctionName 	The library function name as it is in the file created to define the function
 * @param libClassName	 	The library function name as it is in the file created to define the function
 * @return					A handler to the function
 */
native OrpheuFunction:OrpheuGetFunctionFromClass(const entityClassName[],const libFunctionName[],const libClassName[])

/**
 * Retrieves a handler to a function given the ID of an entity, the function name and the classname
 * This function is a virtual function (a function defined in abase class and implemented 
 * differently by each extender class)
 * For example: every class that extends CBaseEntity has a Spawn function. That function is defined in CBaseEntity
 * and implemented differently by each class derived from CBaseEntity
 * 
 * @param id				The id of the entity
 * @param libFunctionName 	The library function name as it is in the file created to define the function
 * @param libClassName	 	The library function name as it is in the file created to define the function
 * @return					A handler to the function
 */
native OrpheuFunction:OrpheuGetFunctionFromEntity(id,const libFunctionName[],const libClassName[])

/**
 * Retrieves a handler to a function given an object, the function name and the classname
 * This function is a virtual function (a function defined in abase class and implemented 
 * differently by each extender class)
 * For example: every class that extends CBaseEntity has a Spawn function. That function is defined in CBaseEntity
 * and implemented differently by each class derived from CBaseEntity
 * 
 * @param object			An object. More precisely, the address of a C++ object. This should be used for classes that are not entities
 * @param libFunctionName 	The library function name as it is in the file created to define the function
 * @param libClassName	 	The library function name as it is in the file created to define the function
 * @return					A handler to the function
 */
native OrpheuFunction:OrpheuGetFunctionFromObject(object,const libFunctionName[],const libClassName[])

/**
 * Retrieves a handler to a function given the id of a monster of monstermod, the function name and the classname
 * This function is a virtual function (a function defined in abase class and implemented 
 * differently by each extender class)
 * For example: every class that extends CBaseEntity has a Spawn function. That function is defined in CBaseEntity
 * and implemented differently by each class derived from CBaseEntity
 * 
 * This function goes against the spirit of orpheu of hardcoding the less possible but without it would be much
 * more complex to use virtual functions
 * 
 * @param id				The id of a monster from monstermod
 * @param libFunctionName 	The library function name as it is in the file created to define the function
 * @param libClassName	 	The library function name as it is in the file created to define the function
 * @return					A handler to the function
 */
native OrpheuFunction:OrpheuGetFunctionFromMonster(id, const libFunctionName[], const libClassName[])
