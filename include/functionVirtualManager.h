
#ifndef _INCLUDE_FUNCTION_VIRTUAL_MANAGER_
#define _INCLUDE_FUNCTION_VIRTUAL_MANAGER_

#include <sm_trie_tpl.h>
#include <am-vector.h>

#include <ctime> 
#include <functionStructure.h>
#include <function.h>

class FunctionVirtualManager
{
	private:

		KTrie<time_t> functionVirtualNameToTimestamp;
		KTrie<unsigned int> functionVirtualNameToFunctionStructureID;
		ke::Vector<FunctionStructure*> functionStructures;

	public:

		time_t getTimestamp(const char* functionVirtualName);

		FunctionStructure* get(const char* functionVirtualName);
		void add(FunctionStructure* functionStructure,time_t timestamp);
		unsigned short int makeFunction(FunctionStructure* functionStructure,long object);
};

#endif

