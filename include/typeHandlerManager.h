
#ifndef _INCLUDE_TYPE_HANDLER_MANAGER_
#define _INCLUDE_TYPE_HANDLER_MANAGER_

#include <sm_trie_tpl.h>
#include <am-vector.h>

#include <typeHandler.h>

class TypeHandlerManager
{
public:

	TypeHandlerManager(KTrie<ke::Vector<char*>*>* typeAliases);
	void registerTypeHandler(const char* label, TypeHandler* handler);
	bool typeHandlerExists(char* label);
	TypeHandler* getTypeHandler(char *label);

private:

	KTrie<TypeHandler*>* registeredTypeHandlers;
	KTrie<ke::Vector<char*>*>* typeAliases;
};

#endif

