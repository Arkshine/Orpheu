
#if defined _orpheu_memory_included
	#endinput
#endif
#define _orpheu_memory_included

#include <orpheu_const>

/*
* 	To use this functionality you must build files that identify memory locations
* This files go on the folder "configs/orpheu/memory" 
* More instructions on Orpheu thread
*/

/**
 * Replaces data in memory in the full extent of a library
 *
 * @param memoryDataName	The name of the block that qualifies memory as defined in a file at "configs/orpheu/memory" 
 * @param count				The number of occurences to replace. 0 = infinite
 * @param any				A pair "to be replaced" - "replacement". Pair members must be of the same type
 * 
 * @return 					The number of replacements made
 * 
 * Example:
 * 
 * 		Replacing the value 16000 by 100000 in a library
 * 
 * 		OrpheuMemoryReplace("name",0,16000,100000) (where name should be related to a block of data that qualifies the memory as holding a long value)
 * 	
 * 		Replacing the string "weapon_smokegrenade","weapon_flashbang" 
 * 
 * 		OrpheuMemoryReplace("name",0,"weapon_smokegrenade","weapon_flashbang") (where name should be related to a block of data that qualifies the memory as holding a string)
 */
native OrpheuMemoryReplace(const memoryDataName[],count,any:...)

/**
 * Replaces data in memory.It works like OrpheuMemoryReplace but starts at a given address	
 *
 * @param address			The address to start at
 * @param memoryDataName	The name of the block that qualifies memory as defined in a file at "configs/orpheu/memory" 
 * @param count				The number of occurence. 0 = infinite
 * @param any				A pair "to be replaced" - "replacement". Pair members must be of the same type
 * 
 * @return 					The number of replacements made
 */
native OrpheuMemoryReplaceAtAddress(address,const memoryDataNameName[],count,any:...)


/**
 * Retrieves data in memory. The block of data that qualifies memory must also identify it by having identifier blocks,
 * Memory can be located given an offset or a signature
 *
 * @param memoryDataName	The name of the block that qualifies memory as defined in a file at "configs/orpheu/memory"  
 * @param any				If the type of the memory location is passed by ref, the variables needed to get the value
 *							You can give an extra argument to retrieve the address where the data retrieved lies
 * 
 * @return 					If the type of the memory location is not passed by ref, its value
 */
native OrpheuMemoryGet(const memoryDataName[],any:...)

/**
 * Retrieves data in memory. It works like OrpheuMemoryGet but starts at a given address
 *
 * @param address			The address to start at
 * @param memoryDataName	The name of the block that qualifies memory as defined in a file at "configs/orpheu/memory"  
 * @param any				If the type of the memory location is passed by ref, the variables needed to get the value
 *							You can give an extra argument to retrieve the address where the data retrieved lies
 *
 * @return 					If the type of the memory location is not passed by ref, its value
 */
native OrpheuMemoryGetAtAddress(address,const memoryDataName[],any:...)

/**
 * Alters data in memory.  The block of data that qualifies memory must also identify it by having identifier blocks.
 * Memory can be located given an offset or a signature
 *
 * @param memoryDataName	The name of the block that qualifies memory as defined in a file at "configs/orpheu/memory"  
 * @param count				The number of occurences to alter. 0 = infinite
 * @param any				The value to use as replacement
 *							You can give an extra argument to retrieve the address where the data retrieved lies
 *
 * @return 					Number of occurences replaced
 */
native OrpheuMemorySet(const memoryDataName[],count,any:...)

/**
 * Alters data in memory.  It works like OrpheuMemorySet but starts at a given address	
 *
 * @param address			The address to start at
 * @param memoryDataName	The name of the block that qualifies memory as defined in a file at "configs/orpheu/memory"  
 * @param count				The number of occurences to alter. 0 = infinite
 * @param any				The value to use as replacement
 *							You can give an extra argument to retrieve the address where the data retrieved lies
 *
 * @return 					Number of occurences replaced
 */
native OrpheuMemorySetAtAddress(address,const memoryDataName[],count,any:...)

