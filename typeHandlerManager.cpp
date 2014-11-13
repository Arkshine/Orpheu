
#include "typeHandlerManager.h"

TypeHandlerManager::TypeHandlerManager(StringHashMap<ke::Vector<char*>*>* typeAliases)
{
	registeredTypeHandlers = new StringHashMap<TypeHandler*>();
	this->typeAliases = typeAliases;
}

void TypeHandlerManager::registerTypeHandler(const char* label, TypeHandler* handler)
{
	registeredTypeHandlers->insert(label, handler);

	ke::Vector<char*>* aliases;

	if (typeAliases->retrieve(label, &aliases))
	{
		for (size_t i = 0; i < aliases->length(); ++i)
		{
			registeredTypeHandlers->insert(aliases->at(i), handler);
		}
	}
}

bool TypeHandlerManager::typeHandlerExists(const char* label)
{
	return registeredTypeHandlers->retrieve(label);
}

TypeHandler* TypeHandlerManager::getTypeHandler(const char* label)
{
	TypeHandler* handler;

	if (registeredTypeHandlers->retrieve(label, &handler))
	{
		return handler;
	}

	return NULL;
}
