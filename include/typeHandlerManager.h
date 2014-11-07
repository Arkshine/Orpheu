
#ifndef _INCLUDE_TYPE_HANDLER_MANAGER_
#define _INCLUDE_TYPE_HANDLER_MANAGER_

#include <am-vector.h>
#include <sm_stringhashmap.h>
#include <typeHandler.h>

class TypeHandlerManager
{
public:

	TypeHandlerManager(StringHashMap<ke::Vector<char*>*>* typeAliases);
	void registerTypeHandler(const char* label, TypeHandler* handler);
	bool typeHandlerExists(char* label);
	TypeHandler* getTypeHandler(char *label);

private:

	StringHashMap<TypeHandler*>* registeredTypeHandlers;
	StringHashMap<ke::Vector<char*>*>* typeAliases;
};

#endif

