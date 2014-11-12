
#ifndef _INCLUDE_TYPE_HANDLER_MANAGER_
#define _INCLUDE_TYPE_HANDLER_MANAGER_

#include <sm_stringhashmap.h>
#include <am-vector.h>

#include <typeHandler.h>

class TypeHandlerManager
{
public:

	TypeHandlerManager(StringHashMap<ke::Vector<char*>*>* typeAliases);
	void registerTypeHandler(const char* label, TypeHandler* handler);
	bool typeHandlerExists(const char* label);
	TypeHandler* getTypeHandler(const char *label);

private:

	StringHashMap<TypeHandler*>* registeredTypeHandlers;
	StringHashMap<ke::Vector<char*>*>* typeAliases;
};

#endif

