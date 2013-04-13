
#ifndef _INCLUDE_TYPE_HANDLER_MANAGER_
#define _INCLUDE_TYPE_HANDLER_MANAGER_

#include <sm_trie_tpl.h>
#include <CVector.h>

#include <typeHandler.h>

class TypeHandlerManager
{
	public:

		TypeHandlerManager(KTrie<CVector<char*>*>* typeAliases);
		void registerTypeHandler(const char* label,TypeHandler* handler);
		bool typeHandlerExists(char* label);
		TypeHandler* getTypeHandler(char *label);
	
	private:
	
		KTrie<TypeHandler*>* registeredTypeHandlers;
		KTrie<CVector<char*>*>* typeAliases;
};

#endif

