
#if defined _orpheu_advanced_included
	#endinput
#endif
#define _orpheu_advanced_included

#include <orpheu_const>

/**
 *  Gets the bytes found at an address
 *
 * @param address			The direct address in memory
 * @param bytes				An array to hold the bytes
 * @param count				The number of bytes to get
 */
native OrpheuGetBytesAtAddress(address,bytes[],count)

/**
 *  Gets the address in memory of a function given a handler to it
 *
 * @param function			A handler to the function
 *
 * @return 					The address o the function
 */
native OrpheuGetFunctionAddress(OrpheuFunction:function)

/**
 *  Gets a handle to a struct given an address in memory
 *
 * @param structType		The type of the struct
 * @param address			The address where the struct is in
 *
 * @return 					A handler to the struct
 */
native OrpheuStruct:OrpheuGetStructFromAddress(OrpheuStructType:structType,address)

/**
 *  Gets the handler to a function given its address and the name that you give it in the file where you define the function
 *
 * @param address			The address where the function is
 * @param libFunctionName	The name of the function as it is in the file where the function is defined
 * @param className			The name of the class if the function belongs to one
 *
 * @return 					A handler to the function
 */
native OrpheuFunction:OrpheuCreateFunction(address,const libFunctionName[],const classname[]="")

/**
 *  Gets the offset of the adress where the function is located to the base address of its library
 *
 * @param function			A handler to the function
 *
 * @return 					The offset
 */
native OrpheuGetFunctionOffset(OrpheuFunction:function)

/**
 *  Gets the adress of a library
 *
 * @param libraryName		The name of the library. For the mod library (cstrike,czero,etc) use "mod"
 *
 * @return 					The address
 */
native OrpheuGetLibraryAddress(const libraryName[])

/**
 *  This native should retrieve the address of functions called from an address
 *  For example by passing the address of the function X, and 1 as n, it should retrieve the address of the first
 * function called in X. It isn't confirmed to work on every condition and it should be use for experiments only
 * to help retrieving functions on Windows
 *
 * @param address			The address to start the search at
 * @param number			The number of the the call starting at 1 from the address
 *
 * @return 					The address of the function called
 */
native OrpheuGetNextCallAtAddress(address,number)
